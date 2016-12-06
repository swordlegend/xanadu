//

#include "player.hpp"

#include "quest_data_provider.hpp"
#include "playernpc.hpp"
#include "party_member.hpp"
#include "party.hpp"
#include "guild_member.hpp"
#include "guild.hpp"
#include "map.hpp"
#include "messenger.hpp"
#include "inventory.hpp"
#include "hiredmerchant.hpp"
#include "skill_data.hpp"
#include "key.hpp"
#include "skill_data_provider.hpp"
#include "receive_packet_opcodes.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "summon.hpp"
#include "quest.hpp"
#include "quest_data.hpp"
#include "quest_reward_data.hpp"
#include "effect.hpp"
#include "equip_data.hpp"
#include "equip_data_provider.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "pet_data_provider.hpp"
#include "session.hpp"
#include "shop_data_provider.hpp"
#include "packetcreator.hpp"
#include "constants.hpp"
#include "job_constants.hpp"
#include "tools.hpp"
#include "logger.hpp"

#include "Poco\Data\RecordSet.h"

// constructor

Player::Player(Session *session) :
	session_(session),
	id_(0),
	user_name_(""),
	channel_id_(0),
	position_x_(0),
	position_y_(0),
	foothold_(0),
	summon_(nullptr),
	messenger_(nullptr),
	messenger_position_(0),
	chair_(0),
	stance_(0),
	exp_(0),
	hair_(0),
	nx_cash_credit_(0),
	user_id_(0),
	recv_length_(0),
	recv_pos_(0),
	is_gm_(false),
	item_effect_(0),
	shop_(nullptr),
	trade_partner_(nullptr),
	trade_mesos(0),
	trade_locked_(false),
	party_(nullptr),
	party_invite_(0),
	guild_(nullptr),
	guild_rank_(5),
	map_(nullptr),
	merchant_(nullptr),
	in_hide_(false),
	in_cash_shop_(false),
	in_game_(false),
	last_gm_call_ticks_(0),
	npc_(new PlayerNpc()),
	mount_skill_id_(0),
	mount_item_id_(0),
	energy_bar_(0),
	hyperbody_max_hp_(0),
	hyperbody_max_mp_(0),
	storage_mesos_(0),
	face_(0),
	fame_(0),
	character_slots_(0),
	selected_equip_slot_(0),
	donation_points_(0),
	hp_(0),
	max_hp_(0),
	mp_(0),
	max_mp_(0),
	mesos_(0),
	job_(0),
	str_(0),
	dex_(0),
	int_(0),
	luk_(0),
	ap_(0),
	sp_(0),
	spawn_point_(0),
	skin_color_(0),
	gender_(0),
	storage_slots_(0),
	level_(1),
	merchant_storage_mesos_(0),
	crusader_combo_value_(1),
	chalk_board_text_(""),
	name_("")//,
	//summon_timer_(World::get_instance()->get_io_service())
{
}

// destructor

Player::~Player()
{
	if (id_ == 0 && user_name_ == "")
	{
		return;
	}

	World *world = World::get_instance();
	world->remove_online_user(user_name_);
	world->RemovePlayer(id_);

	if (in_game_ && map_)
	{
		map_->remove_player(this);
	}

	if (in_game_ || in_cash_shop_)
	{
		// trading
		mesos_ += trade_mesos;

		for (auto item : trade_items_)
		{
			Inventory *inventory = get_inventory(item->get_inventory_id());
			if (inventory)
			{
				inventory->add_item_find_slot(item);
			}
		}

		if (trade_partner_)
		{
			if (trade_partner_->get_trade_partner() == this)
			{
				trade_partner_->set_trade_partner(nullptr);
				trade_partner_->add_mesos(trade_partner_->get_trade_mesos());

				auto trade_partner_items = trade_partner_->get_trade_items();
				for (auto item : *trade_partner_items)
				{
					Inventory *inventory = trade_partner_->get_inventory(item->get_inventory_id());
					if (inventory)
					{
						inventory->add_item_find_slot(item);
					}
				}
				// send a packet
				PacketCreator packet5;
				packet5.TradeError(0, 3);
				trade_partner_->send_packet(&packet5);
			}
		}

		// make hyperbody adjustments
		max_hp_ -= hyperbody_max_hp_;
		max_mp_ -= hyperbody_max_mp_;
		hp_ = max_hp_;
		mp_ = max_mp_;

		// save the player
		save();

		if (party_)
		{
			party_->remove_member(id_);

			// send a packet
			PacketCreator packet20;
			packet20.UpdateParty(party_);
			party_->send_packet(&packet20);
		}

		if (guild_)
		{
			guild_->RemoveMember(id_);

			// send a packet
			PacketCreator packet21;
			packet21.GuildMemberOnline(guild_->get_id(), id_, false);
			guild_->send_packet(&packet21);
		}

		if (messenger_)
		{
			if (messenger_->get_players()->size() == 1)
			{
				world->remove_messenger(messenger_->get_id());
			}
			else
			{
				messenger_->delete_member(id_);

				// send a packet
				PacketCreator packet22;
				packet22.MessengerRemovePlayer(messenger_position_);
				messenger_->send_packet(&packet22);
			}
		}

		if (merchant_)
		{
			if (merchant_->is_owner(id_))
			{
				merchant_->set_open(true);
			}
			else
			{
				merchant_->remove_visitor(this);
			}
		}

		// delete buffs
		for (auto it : id_buffs_)
		{
			delete_buff(it.second, false);
		}

		// delete buddies
		for (auto it : buddies_)
		{
			Player *buddy = world->GetPlayerById(it.first);
			if (buddy)
			{
				change_buddy_channel(buddy, get_id(), -1);
			}
		}

		// delete inventories
		for (auto it : inventories_)
		{
			Inventory *inventory = it.second;
			delete inventory;
		}
	}

	for (auto it : characters_)
	{
		Character *character = it.second;
		if (character)
		{
			delete character;
		}
	}

	if (npc_)
	{
		delete npc_;
	}
}

void Player::send_packet(PacketCreator *packet)
{
	session_->send_packet(packet);
}

void Player::disconnect()
{
	session_->disconnect();
}

void Player::skip_bytes(int amount)
{
	recv_pos_ += amount;
}

void Player::handle_packet_in_cashshop()
{
	try
	{
		short header = read<short>();
		switch (header)
		{
		case receive_headers::kCHECK_CASH:
			handle_update_cash_shop();
			break;
		case receive_headers::kCASHSHOP_OPERATION:
			handle_cash_shop_action();
			break;
		case receive_headers::kREQUEST_MAP_CHANGE:
			handle_leave_cash_shop();
			break;
		}
	}
	catch (Poco::Exception/* &e*/)
	{
		/*std::string text(e.displayText());
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_cashshop_poco_exceptions.txt");
		logger.write(text.c_str());*/
	}
	catch (std::exception/* &e*/)
	{
		/*std::string text(e.what());
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_cashshop_std_exceptions.txt");
		logger.write(text.c_str());*/
	}
	catch (...)
	{
		/*std::string text("unknown exception");
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_cashshop_unk_exceptions.txt");
		logger.write(text.c_str());*/
	}
}

void Player::handle_packet_in_game()
{
	try
	{
		short header = read<short>();
		switch (header)
		{
		case receive_headers::kDISTRIBUTE_SP:
			handle_add_skill_point();
			break;
		case receive_headers::kDISTRIBUTE_AP:
			handle_add_stat();
			break;
		case receive_headers::kREQUEST_CHARACTER_INFO:
			handle_get_player_info();
			break;
		case receive_headers::kMOVE_PLAYER:
			handle_player_movement();
			break;
		case receive_headers::kGIVE_FAME:
			handle_faming();
			break;
		case receive_headers::kFREDRICK_OPERATION:
			handle_merchant_storage_request();
			break;
		case receive_headers::kCHANGE_MAP_SPECIAL:
			handle_use_scripted_portal();
			break;
		case receive_headers::kGENERAL_CHAT:
			handle_use_chat();
			break;
		case receive_headers::kWHISPER:
			handle_chat_command();
			break;
		case receive_headers::kREQUEST_MAP_CHANGE:
			handle_change_map();
			break;
		case receive_headers::kREQUEST_HEAL_OVER_TIME:
			handle_hp_mp_recovering();
			break;
		case receive_headers::kMESO_DROP:
			handle_player_drop_mesos();
			break;
		case receive_headers::kREQUEST_ITEM_PICKUP:
			handle_player_loot_drop();
			break;
		case receive_headers::kQUEST_ACTION:
			handle_quest_action();
			break;
		case receive_headers::kCHANGE_KEYMAP:
			handle_key_map_changes();
			break;
		case receive_headers::kFACE_EXPRESSION:
			handle_face_expression();
			break;
		case receive_headers::kMOVE_ITEM:
			handle_move_item();
			break;
		case receive_headers::kUSE_CASH_ITEM:
			handle_use_cash_item();
			break;
		case receive_headers::kUSE_SKILL_BOOK:
			handle_use_skill_book();
			break;
		case receive_headers::kBUDDY_LIST:
			handle_buddy_list_action();
			break;
		case receive_headers::kHANDLE_NPC:
			handle_request_npc();
			break;
		case receive_headers::kNPC_CHAT:
			handle_npc_chat();
			break;
		case receive_headers::kNPC_SHOP:
			handle_npc_shop();
			break;
		case receive_headers::kSTORAGE:
			handle_storage_reqest();
			break;
		case receive_headers::kCLOSE_CHALKBOARD:
			handle_close_chalkboard();
			break;
		case receive_headers::kMESSENGER:
			handle_messenger_action();
			break;
		case receive_headers::kUSE_CHAIR:
			handle_player_use_chair();
			break;
		case receive_headers::kCANCEL_CHAIR:
			handle_player_cancel_chair();
			break;
		case receive_headers::kPARTY_OPERATION:
			handle_party_action();
			break;
		case receive_headers::kHIRED_MERCHANT_REQUEST:
			handle_hired_merchant_request();
			break;
		case receive_headers::kCHANGE_CHANNEL:
			handle_change_channel();
			break;
		case receive_headers::kREQUEST_TAKE_DAMAGE:
			handle_player_hit();
			break;
		case receive_headers::kREQUEST_CLOSE_RANGE_ATTACK:
			handle_use_close_range_attack();
			break;
		case receive_headers::kRANGED_ATTACK:
			handle_use_ranged_attack();
			break;
		case receive_headers::kMAGIC_ATTACK:
			handle_use_magic_attack();
			break;
		case receive_headers::kENERGY_ATTACK:
			handle_use_energy_attack();
			break;
		case receive_headers::kMOVE_LIFE:
			handle_mob_movement();
			break;
		case receive_headers::kITEM_SORT:
			handle_inventory_sort();
			break;
		case receive_headers::kMULTI_CHAT:
			handle_use_group_chat();
			break;
		case receive_headers::kPLAYER_INTERACTION:
			handle_item_transportation();
			break;
		case receive_headers::kPARTY_REQUEST_ANSWER:
			handle_party_invitation();
			break;
		case receive_headers::kUSE_SKILL:
			handle_use_skill();
			break;
		case receive_headers::kUSE_ITEMEFFECT:
			handle_use_item_effect();
			break;
		case receive_headers::kUSE_ITEM:
			handle_use_item();
			break;
		case receive_headers::kUSE_RETURN_SCROLL:
			handle_use_return_scroll();
			break;
		case receive_headers::kUSE_SCROLL:
			handle_use_scroll();
			break;
		case receive_headers::kREQUEST_CANCEL_BUFF:
			handle_cancel_skill_buff();
			break;
		case receive_headers::kCANCEL_ITEM_BUFF:
			handle_cancel_item_buff();
			break;
		case receive_headers::kSPAWN_PET:
			handle_use_pet();
			break;
		case receive_headers::kMOVE_PET:
			handle_pet_movement();
			break;
		case receive_headers::kPET_COMMAND:
			handle_pet_command();
			break;
		case receive_headers::kPET_CHAT:
			handle_pet_chat();
			break;
		case receive_headers::kPET_LOOT:
			handle_pet_loot();
			break;
		case receive_headers::kSUMMON_MOVE:
			handle_summon_movement();
			break;
		case receive_headers::kDAMAGE_MOB_SUMMON:
			handle_damage_mob_summon();
			break;
		case receive_headers::kSUMMON_DAMAGE:
			handle_puppet_damage();
			break;
		case receive_headers::kGUILD_OPERATION:
			handle_guild_action();
			break;
		case receive_headers::kENTER_CASHSHOP:
			handle_cash_shop_enter();
			break;
		case receive_headers::kSKILL_EFFECT:
			handle_use_special_skill();
			break;
		case receive_headers::kHIT_REACTOR:
			handle_hit_reactor();
			break;
		case receive_headers::kENTER_MTS:
			// warp the player to the free market
			set_map(910000000);
			break;
		}
	}
	catch (Poco::Exception/* &e*/)
	{
		/*std::string text(e.displayText());
			text.append("\n header of the packet: ");
			text.append(std::to_string(header) + "\n");
			text.append("player_name: " + name_);

			Logger logger("handle_packet_in_game_poco_exceptions.txt");
			logger.write(text.c_str());*/
	}
	catch (std::exception/* &e*/)
	{
		/*std::string text(e.what());
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_game_std_exceptions.txt");
		logger.write(text.c_str());*/
	}
	catch (...)
	{
		/*std::string text("unknown exception");
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_game_unk_exceptions.txt");
		logger.write(text.c_str());*/
	}
}

void Player::handle_packet_in_login()
{
	try
	{
		short header = read<short>();
		switch (header)
		{
		case receive_headers::kLoginRequest:
			handle_login_request();
			break;
		case receive_headers::kHANDLE_LOGIN:
			handle_pin_operation();
			break;
		case receive_headers::kCHANNEL_SELECT:
			handle_channel_selection();
			break;
		case receive_headers::kWORLD_SELECT:
			handle_world_selection();
			break;
		case receive_headers::kSHOW_WORLD:
		case receive_headers::kBACK_TO_WORLD:
			handle_world_list_request();
			break;
		case receive_headers::kCHAR_SELECT:
			handle_connect_game();
			break;
		case receive_headers::kPLAYER_LOGGEDIN:
			player_connect();
			break;
		case receive_headers::kNAME_CHECK:
			handle_character_creation_name_check();
			break;
		case receive_headers::kCREATE_CHARACTER:
			handle_create_character();
			break;
		case receive_headers::kDELETE_CHARACTER:
			handle_delete_character();
			break;
		case receive_headers::kLOGIN_BACK:
			handle_relog_request();
			break;
		}
	}
	catch (Poco::Exception/* &e*/)
	{
		/*std::string text(e.displayText());
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_login_poco_exceptions.txt");
		logger.write(text.c_str());*/
	}
	catch (std::exception/* &e*/)
	{
		/*std::string text(e.what());
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_login_std_exceptions.txt");
		logger.write(text.c_str());*/
	}
	catch (...)
	{
		/*std::string text("unknown exception");
		text.append("\n header of the packet: ");
		text.append(std::to_string(header) + "\n");
		text.append("player_name: " + name_);

		Logger logger("handle_packet_in_login_unk_exceptions.txt");
		logger.write(text.c_str());*/
	}
}

void Player::handle_packet(unsigned short bytes_amount)
{
	recv_pos_ = 0;
	recv_length_ = bytes_amount;

	if (in_cash_shop_)
	{
		handle_packet_in_cashshop();
	}
	else if (in_game_)
	{
		handle_packet_in_game();
	}
	else
	{
		handle_packet_in_login();
	}
}

void Player::player_connect()
{
	id_ = read<int>();

	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	// get userid
	Poco::Data::Statement statement1(mysql_session);
	statement1 << "SELECT user_id FROM characters WHERE id = " << id_, Poco::Data::Keywords::into(user_id_);

	if (statement1.execute() != 1)
	{
		session_->disconnect();
		return;
	}

	// get the stored channel id

	channel_id_ = world->get_channel_id_for_user_id(user_id_);

	// in this case, the number 100 is used to signalize the error
	// unusual, but due to it being unsigned and the channel id's starting
	// with 0, this way is needed
	if (channel_id_ == 100)
	{
		session_->disconnect();
		return;
	}

	// erase channel id it from storing
	world->erase_channel_id_for_user_id(user_id_);

	// get username
	Poco::Data::Statement statement2(mysql_session);
	statement2 << "SELECT username FROM users WHERE id = " << user_id_, Poco::Data::Keywords::into(user_name_);

	if (statement2.execute() != 1)
	{
		session_->disconnect();
		return;
	}

	world->add_online_user(user_name_);

	// load character data
	Poco::Data::Statement statement3(mysql_session);
	statement3 << "SELECT * FROM characters WHERE id = " << id_;
	if (statement3.execute() != 1)
	{
		session_->disconnect();
		return;
	}

	Poco::Data::RecordSet rs1(statement3);
	name_ = rs1["name"].convert<std::string>();
	level_ = rs1["level"];
	job_ = rs1["job"];
	str_ = rs1["str"];
	dex_ = rs1["dex"];
	int_ = rs1["intt"];
	luk_ = rs1["luk"];
	hp_ = rs1["hp"];
	max_hp_ = rs1["max_hp"];
	mp_ = rs1["mp"];
	max_mp_ = rs1["max_mp"];
	ap_ = rs1["ap"];
	sp_ = rs1["sp"];
	exp_ = rs1["exp"];
	fame_ = rs1["fame"];

	Channel *channel = world->GetChannel(channel_id_);
	if (!channel)
	{
		session_->disconnect();
		return;
	}

	map_ = channel->get_map(rs1["map"]);

	if (!map_)
	{
		session_->disconnect();
		return;
	}

	int forced_return = map_->get_forced_return();
	if (forced_return != 999999999)
	{
		Map *map = world->GetChannel(channel_id_)->get_map(forced_return);

		if (map)
		{
			map_ = map;
		}
	}

	spawn_point_ = rs1["pos"];
	gender_ = rs1["gender"];
	skin_color_ = rs1["skin"];
	face_ = rs1["face"];
	hair_ = rs1["hair"];
	mesos_ = rs1["mesos"];

	int party_id = rs1["party_id"];

	if (party_id != 0)
	{
		party_ = world->get_party(party_id);

		if (!party_ || !party_->get_member(id_))
		{
			party_ = nullptr;
		}
		else
		{
			party_->add_member(this);
		}
	}

	guild_rank_ = rs1["guild_rank"];

	int guild_id = rs1["guild_id"];

	if (guild_id != 0)
	{
		guild_ = world->get_guild(guild_id);

		if (!guild_ || !guild_->GetMember(id_))
		{
			guild_ = nullptr;
			guild_rank_ = 5;
		}
		else
		{
			guild_->AddMember(this);
		}
	}

	signed char equip_slots = rs1["equip_slots"];
	signed char use_slots = rs1["use_slots"];
	signed char setup_slots = rs1["setup_slots"];
	signed char etc_slots = rs1["etc_slots"];
	merchant_storage_mesos_ = rs1["merchant_storage_mesos"];

	// initialize inventories
	inventories_[0] = new Inventory(this, 0, 24); // Equipped
	inventories_[1] = new Inventory(this, 1, equip_slots); // Equip
	inventories_[2] = new Inventory(this, 2, use_slots); // Use
	inventories_[3] = new Inventory(this, 3, setup_slots); // Setup
	inventories_[4] = new Inventory(this, 4, etc_slots); // Etc
	inventories_[5] = new Inventory(this, 5, 24); // Cash

	// account data
	Poco::Data::Statement statement4(mysql_session);
	statement4 << "SELECT gm, character_slots, donation_points, storage_mesos, nxcash_credit FROM users WHERE id = " << user_id_;
	statement4.execute();

	Poco::Data::RecordSet rs2(statement4);

	is_gm_ = rs2["gm"];
	character_slots_ = rs2["character_slots"];
	donation_points_ = rs2["donation_points"];
	storage_mesos_ = rs2["storage_mesos"];
	nx_cash_credit_ = rs2["nxcash_credit"];
	storage_slots_ = 24;

	// storage equips
	Poco::Data::Statement statement5(mysql_session);
	statement5 << "SELECT * FROM storage_equips WHERE user_id = " << user_id_;
	size_t cols = statement5.execute();

	Poco::Data::RecordSet rs3(statement5);

	for (; cols > 0; --cols)
	{
		int item_id = rs3["equip_id"];

		std::shared_ptr<Item> equip(new Item(item_id));

		equip->set_slot(rs3["pos"]);
		equip->set_free_slots(rs3["slots"]);
		equip->set_used_scrolls(rs3["used_scrolls"]);
		equip->set_str(rs3["str"]);
		equip->set_dex(rs3["dex"]);
		equip->set_int(rs3["iint"]);
		equip->set_luk(rs3["luk"]);
		equip->set_hp(rs3["hp"]);
		equip->set_mp(rs3["mp"]);
		equip->set_weapon_attack(rs3["weapon_attack"]);
		equip->set_magic_attack(rs3["magic_attack"]);
		equip->set_weapon_defense(rs3["weapon_def"]);
		equip->set_magic_defense(rs3["magic_def"]);
		equip->set_acc(rs3["accuracy"]);
		equip->set_avoid(rs3["avoid"]);
		equip->set_hand(rs3["hand"]);
		equip->set_speed(rs3["speed"]);
		equip->set_jump(rs3["jump"]);

		EquipData *data = EquipDataProvider::get_instance()->get_item_data(equip->get_item_id());

		if (data)
		{
			signed char i;

			for (i = 0; i < storage_items_.size(); ++i)
			{
				if (storage_items_[i]->get_inventory_id() > equip->get_inventory_id())
				{
					break;
				}
			}

			storage_items_.insert(storage_items_.begin() + i, equip);
		}

		rs3.moveNext();
	}

	// storage items
	Poco::Data::Statement statement6(mysql_session);
	statement6 << "SELECT item_id, pos, amount FROM storage_items WHERE user_id = " << user_id_;
	cols = statement6.execute();

	Poco::Data::RecordSet rs4(statement6);

	for (; cols > 0; --cols)
	{
		int item_id = rs4["item_id"];
		short amount = rs4["amount"];
		signed char slot = rs4["pos"];

		std::shared_ptr<Item> item(new Item(item_id));
		item->set_amount(amount);
		item->set_slot(slot);

		ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

		if (data)
		{
			signed char i;
			for (i = 0; i < storage_items_.size(); ++i)
			{
				if (storage_items_[i]->get_inventory_id() > item->get_inventory_id())
				{
					break;
				}
			}
			storage_items_.insert(storage_items_.begin() + i, item);
		}

		rs4.moveNext();
	}

	// merchant storage items
	Poco::Data::Statement statement7(mysql_session);
	statement7 << "SELECT item_id, pos, amount FROM merchant_storage_items WHERE player_id = " << id_;
	cols = statement7.execute();

	Poco::Data::RecordSet rs5(statement7);

	for (; cols > 0; --cols)
	{
		int item_id = rs5["item_id"];
		short amount = rs5["amount"];
		signed char slot = rs5["pos"];

		std::shared_ptr<Item> item(new Item(item_id));
		item->set_amount(amount);
		item->set_slot(slot);

		ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

		if (data)
		{
			merchant_storage_items_[static_cast<signed char>(merchant_storage_items_.size())] = item;
		}

		rs5.moveNext();
	}

	// merchant storage equips
	Poco::Data::Statement statement8(mysql_session);
	statement8 << "SELECT * FROM merchant_storage_equips WHERE player_id = " << id_;
	cols = statement8.execute();

	Poco::Data::RecordSet rs6(statement8);

	for (; cols > 0; --cols)
	{
		int item_id = rs6["equip_id"];
		signed char slot = rs6["pos"];

		std::shared_ptr<Item> equip(new Item(item_id));
		equip->set_slot(slot);

		equip->set_free_slots(rs6["slots"]);
		equip->set_used_scrolls(rs6["used_scrolls"]);
		equip->set_str(rs6["str"]);
		equip->set_dex(rs6["dex"]);
		equip->set_int(rs6["iint"]);
		equip->set_luk(rs6["luk"]);
		equip->set_hp(rs6["hp"]);
		equip->set_mp(rs6["mp"]);
		equip->set_weapon_attack(rs6["weapon_attack"]);
		equip->set_magic_attack(rs6["magic_attack"]);
		equip->set_weapon_defense(rs6["weapon_def"]);
		equip->set_magic_defense(rs6["magic_def"]);
		equip->set_acc(rs6["accuracy"]);
		equip->set_avoid(rs6["avoid"]);
		equip->set_hand(rs6["hand"]);
		equip->set_speed(rs6["speed"]);
		equip->set_jump(rs6["jump"]);

		EquipData *data = EquipDataProvider::get_instance()->get_item_data(equip->get_item_id());

		if (data)
		{
			merchant_storage_items_[static_cast<signed char>(merchant_storage_items_.size())] = equip;
		}

		rs6.moveNext();
	}

	// equips
	Poco::Data::Statement statement9(mysql_session);
	statement9 << "SELECT * FROM equips WHERE player_id = " << id_;
	cols = statement9.execute();

	Poco::Data::RecordSet rs7(statement9);

	for (; cols > 0; --cols)
	{
		int item_id = rs7["equip_id"];
		signed char slot = rs7["pos"];

		std::shared_ptr<Item> equip(new Item(item_id));
		equip->set_slot(slot);

		equip->set_free_slots(rs7["slots"]);
		equip->set_used_scrolls(rs7["used_scrolls"]);
		equip->set_str(rs7["str"]);
		equip->set_dex(rs7["dex"]);
		equip->set_int(rs7["iint"]);
		equip->set_luk(rs7["luk"]);
		equip->set_hp(rs7["hp"]);
		equip->set_mp(rs7["mp"]);
		equip->set_weapon_attack(rs7["weapon_attack"]);
		equip->set_magic_attack(rs7["magic_attack"]);
		equip->set_weapon_defense(rs7["weapon_def"]);
		equip->set_magic_defense(rs7["magic_def"]);
		equip->set_acc(rs7["accuracy"]);
		equip->set_avoid(rs7["avoid"]);
		equip->set_hand(rs7["hand"]);
		equip->set_speed(rs7["speed"]);
		equip->set_jump(rs7["jump"]);

		signed char inventory_id = (equip->get_slot() > 0 ? kInventoryConstantsTypesEquip : kInventoryConstantsTypesEquipped);
		Inventory *inventory = get_inventory(inventory_id);
		EquipData *data = EquipDataProvider::get_instance()->get_item_data(equip->get_item_id());

		if (inventory && data)
		{
			inventory->add_item_no_find_slot(equip, false);
		}

		rs7.moveNext();
	}

	// items
	Poco::Data::Statement statement10(mysql_session);
	statement10 << "SELECT item_id, pos, amount FROM items WHERE player_id = " << id_;
	cols = statement10.execute();

	Poco::Data::RecordSet rs8(statement10);

	for (; cols > 0; --cols)
	{
		int item_id = rs8["item_id"];
		short amount = rs8["amount"];
		signed char slot = rs8["pos"];

		std::shared_ptr<Item> item(new Item(item_id));
		item->set_amount(amount);
		item->set_slot(slot);

		Inventory *inventory = get_inventory(item->get_inventory_id());
		ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

		if (inventory && data)
		{
			inventory->add_item_no_find_slot(item, false);
		}

		rs8.moveNext();
	}

	// pets
	Poco::Data::Statement statement11(mysql_session);
	statement11 << "SELECT pet_id, pos, pet_slot, level, closeness FROM pets WHERE player_id = " << id_;
	cols = statement11.execute();

	Poco::Data::RecordSet rs9(statement11);

	for (; cols > 0; --cols)
	{
		int pet_id = rs9["pet_id"];

		if (bool check = PetDataProvider::get_instance()->get_data_by_id(pet_id) != nullptr)
			//if (inventory_constants::is_pet(pet_id))
		{
			std::shared_ptr<Item> pet(new Item(pet_id));
			pet->set_pet_level(rs9["level"]);
			pet->set_slot(rs9["pos"]);
			pet->set_pet_slot(rs9["pet_slot"]);
			pet->set_closeness(rs9["closeness"]);

			get_inventory(kInventoryConstantsTypesCash)->add_item_no_find_slot(pet, false);

			if (pet->get_pet_slot() >= 0 && pets_.size() < 3)
			{
				pet->set_pet_slot(static_cast<signed char>(pets_.size()));
				pets_.push_back(pet);
			}
		}

		rs9.moveNext();
	}

	// is this needed in version 62?
	/*{
		//
		// - HACK - //
		// load quests that have "auto_start" to prevent client sending these
		//

		initialize_player_quests(7707, true, 0, 0);
		initialize_player_quests(24012, true, 0, 0);
		initialize_player_quests(24013, true, 0, 0);
		initialize_player_quests(29900, true, 0, 0);
		initialize_player_quests(29901, true, 0, 0);
		initialize_player_quests(29902, true, 0, 0);
		initialize_player_quests(29903, true, 0, 0);
		initialize_player_quests(29906, true, 0, 0);
		initialize_player_quests(29907, true, 0, 0);
		initialize_player_quests(29908, true, 0, 0);
		initialize_player_quests(29909, true, 0, 0);
		initialize_player_quests(29941, true, 0, 0);
		initialize_player_quests(29942, true, 0, 0);
		initialize_player_quests(29943, true, 0, 0);
		initialize_player_quests(29944, true, 0, 0);
		initialize_player_quests(29945, true, 0, 0);
		initialize_player_quests(29946, true, 0, 0);
	}*/

	// quests
	Poco::Data::Statement statement12(mysql_session);
	statement12 << "SELECT * FROM quests WHERE player_id = " << id_;
	cols = statement12.execute();

	Poco::Data::RecordSet rs10(statement12);

	for (; cols > 0; --cols)
	{
		int quest_id = rs10["quest_id"];
		bool is_completed = rs10["is_complete"];
		int mob_id = rs10["killed_mob"];
		int amount = rs10["amount"];

		QuestData *data = QuestDataProvider::get_instance()->get_quest_data(quest_id);

		if (data)
		{
			initialize_player_quests(quest_id, is_completed, mob_id, amount);
		}

		rs10.moveNext();
	}

	// add the monster riding skill to the player

	Skill monster_riding_skill;
	int monster_riding_skill_id = 1004;
	monster_riding_skill.level_ = 1;
	monster_riding_skill.master_level_ = 0;
	skills_[monster_riding_skill_id] = monster_riding_skill;

	// load skills
	Poco::Data::Statement statement13(mysql_session);
	statement13 << "SELECT skill_id, level, master_level FROM skills WHERE player_id = " << id_;
	cols = statement13.execute();

	Poco::Data::RecordSet rs11(statement13);

	for (; cols > 0; --cols)
	{
		int skill_id = rs11["skill_id"];
		int skill_level = rs11["level"];
		int master_level = rs11["master_level"];

		SkillData *data = SkillDataProvider::get_instance()->get_skill_data(skill_id);

		if (data)
		{
			Skill skill;
			skill.level_ = skill_level;
			skill.master_level_ = master_level;

			skills_[skill_id] = skill;
		}

		rs11.moveNext();
	}

	// load keymap
	{
		for (int pos = kMinKeymapPos; pos < kMaxKeymapPos; ++pos)
		{
			keys_[pos] = Key();

			Key &key = keys_[pos];
			key.type = 0;
			key.action = 0;
		}

		Poco::Data::Statement statement14(mysql_session);
		statement14 << "SELECT pos, type, action FROM keymap WHERE player_id = " << id_;
		cols = statement14.execute();

		Poco::Data::RecordSet rs12(statement14);

		for (; cols > 0; --cols)
		{
			int pos = rs12["pos"];
			signed char type = rs12["type"];
			int action = rs12["action"];

			keys_[pos] = Key();

			Key &key = keys_[pos];
			key.type = type;
			key.action = action;

			rs12.moveNext();
		}
	}

	// load buddylist
	Poco::Data::Statement statement15(mysql_session);
	statement15 << "SELECT buddy_id FROM buddy_lists WHERE player_id = " << id_;
	cols = statement15.execute();

	Poco::Data::RecordSet rs13(statement15);

	for (; cols > 0; --cols)
	{
		int buddy_id = rs13["buddy_id"];

		Player *buddy = world->GetPlayerById(buddy_id);

		if (buddy && buddy->get_buddy(id_))
		{
			add_buddy(buddy->get_id(), buddy->get_name(), buddy->get_channel_id());
			change_buddy_channel(buddy, id_, channel_id_);
		}
		else
		{
			std::string buddy_name = world->get_player_name_from_id(buddy_id);

			if (buddy_name != "")
			{
				add_buddy(buddy_id, buddy_name, -1);
			}
		}

		rs13.moveNext();
	}

	// add the player to the world
	world->AddPlayer(this);

	in_game_ = true;

	// send the connection data
	PacketCreator packet1;
	packet1.change_map(this, true);
	send_packet(&packet1);

	// guild
	if (guild_)
	{
		// send a packet
		PacketCreator packet22;
		packet22.GuildInfo(guild_);
		send_packet(&packet22);
		// send a packet
		PacketCreator packet23;
		packet23.GuildMemberOnline(guild_->get_id(), id_);
		guild_->send_packet(&packet23, this);
	}

	// add the player to the map
	map_->add_player(this);

	// display keymap
	// send a packet
	PacketCreator packet11;
	packet11.ShowKeymap(this);
	send_packet(&packet11);

	// display buddylist
	// send a packet
	PacketCreator packet12;
	packet12.BuddyList(this);
	send_packet(&packet12);

	// display header message
	// send a packet
	PacketCreator packet13;
	packet13.ShowMessage(world->get_header_message(), 4);
	send_packet(&packet13);
}

bool Player::add_mesos(int amount)
{
	if (amount < 0)
	{
		if (-amount > mesos_)
		{
			return false;
		}
	}
	else
	{
		long long mesos_test = mesos_;
		long long amount_test = amount;
		mesos_test += amount_test;

		if (mesos_test > INT_MAX) // prevent players having more than INT_MAX (~2,1 billion usually) mesos
		{
			return false;
		}
	}

	mesos_ += amount;

	// packet
	PacketCreator packet85;
	packet85.UpdateStatInt(kCharacterStatsMesos, mesos_);
	send_packet(&packet85);

	return true;
}

void Player::set_level(unsigned char newlevel)
{
	level_ = newlevel;

	// packet
	PacketCreator packet1;
	packet1.UpdateLevel(newlevel);
	send_packet(&packet1);

	// show the levelup effect to the map

	// send a packet
	PacketCreator packet2;
	packet2.ShowForeignEffect(id_, 0);
	map_->send_packet(&packet2, this);

	// update the party
	if (party_)
	{
		PartyMember *member = party_->get_member(id_);
		if (member)
		{
			member->set_level(newlevel);
			// send a packet
			PacketCreator packet;
			packet.UpdateParty(party_);
			party_->send_packet(&packet);
		}
	}
	// update the guild
	if (guild_)
	{
		GuildMember *member = guild_->GetMember(id_);
		if (member)
		{
			member->set_level(newlevel);
			// send a packet
			PacketCreator packet22;
			packet22.GuildInfo(guild_);
			guild_->send_packet(&packet22);
		}
	}
}

void Player::set_job(short newjob)
{
	// player starts with level 10 -> mage starts at level 8 normally
	if (newjob == job_ids::kMagician)
	{
		add_sp(6);
	}

	// reset the ap for beginner
	if (is_beginner_job(job_))
	{
		short temp = (str_ - 4 + dex_ - 4 + int_ - 4 + luk_ - 4);
		set_ap(ap_ + temp);
		set_str(4);
		set_dex(4);
		set_luk(4);
		set_int(4);
	}

	// update the job
	job_ = newjob;

	// packet
	PacketCreator packet3;
	packet3.UpdateStatShort(kCharacterStatsJob, job_);
	send_packet(&packet3);

	// show the job change effect to the map
	// send a packet
	PacketCreator packet5;
	packet5.ShowForeignEffect(id_, 8);
	map_->send_packet(&packet5, this);

	if (party_)
	{
		PartyMember *member = party_->get_member(id_);
		if (member)
		{
			member->set_job(job_);
			// send a packet
			PacketCreator packet;
			packet.UpdateParty(get_party());
			party_->send_packet(&packet);
		}
	}

	if (guild_)
	{
		GuildMember *member = guild_->GetMember(id_);
		if (member)
		{
			member->set_job(job_);
			// send a packet
			PacketCreator packet30;
			packet30.GuildInfo(guild_);
			guild_->send_packet(&packet30);
		}
	}

	// give sp
	if (!is_beginner_job(newjob))
	{
		add_sp(1);
	}

	// increase hp and mp

	set_max_hp(get_max_hp() + 300);
	set_hp(get_max_hp());

	set_max_mp(get_max_mp() + 300);
	set_mp(get_max_mp());

	// give skills for 4th jobs
	// skill books are only available for a few skills
	// mastery books are for mastery level 20 and 30

	switch (job_)
	{
	case job_ids::kHero:
	case job_ids::kPaladin:
	case job_ids::kDarkKnight:
	case job_ids::kFpArchMage:
	case job_ids::kIlArchMage:
	case job_ids::kBishop:
	case job_ids::kBowmaster:
	case job_ids::kMarksman:
	case job_ids::kNightLord:
	case job_ids::kShadower:
	case job_ids::kBuccaneer:
	case job_ids::kCorsair:
	{
		auto skills_data = SkillDataProvider::get_instance()->get_data();
		for (auto &it : *skills_data)
		{
			int skill_id = it.first;

			if (tools::get_job_id_from_skill_id(skill_id) == job_)
			{
				Skill skill;
				skill.level_ = 0;
				skill.master_level_ = 10;
				skills_[skill_id] = skill;
			}
		}
		// send a packet
		PacketCreator packet10;
		packet10.UpdateSkills(this);
		send_packet(&packet10);
	}
	}
}

void Player::set_str(short str)
{
	str_ = str;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsStr, str);
	send_packet(&packet1);
}

void Player::set_dex(short dex)
{
	dex_ = dex;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsDex, dex);
	send_packet(&packet1);
}

void Player::set_int(short intt)
{
	int_ = intt;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsInt, int_);
	send_packet(&packet1);
}

void Player::set_luk(short luk)
{
	luk_ = luk;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsLuk, luk);
	send_packet(&packet1);
}

int Player::get_mesos()
{
	return mesos_;
}

unsigned char Player::get_level()
{
	return level_;
}

short Player::get_job()
{
	return job_;
}

short Player::get_str()
{
	return str_;
}

short Player::get_dex()
{
	return dex_;
}

short Player::get_int()
{
	return int_;
}

short Player::get_luk()
{
	return luk_;
}

short Player::get_hp()
{
	return hp_;
}

short Player::get_mp()
{
	return mp_;
}

void Player::add_hp(short hp)
{
	int test = (hp + hp_);

	if (test > SHRT_MAX)
	{
		set_hp(SHRT_MAX);
	}
	else
	{
		set_hp(hp_ + hp);
	}
}

void Player::add_mp(short mp)
{
	int test = (mp + mp_);

	if (test > SHRT_MAX)
	{
		set_mp(SHRT_MAX);
	}
	else
	{
		set_mp(mp_ + mp);
	}
}

short Player::get_max_hp()
{
	return max_hp_;
}

short Player::get_max_mp()
{
	return max_mp_;
}

void Player::set_hyperbody_max_hp(short value)
{
	if (hyperbody_max_hp_ != 0 || value == 0)
	{
		set_max_hp(get_max_hp() - hyperbody_max_hp_);
		set_hp(max_hp_);
		hyperbody_max_hp_ = 0;
	}

	set_max_hp(get_max_hp() + value);
	hyperbody_max_hp_ = value;
}

short Player::get_hyperbody_max_hp()
{
	return hyperbody_max_hp_;
}

void Player::set_hyperbody_max_mp(short value)
{
	if (hyperbody_max_mp_ != 0 || value == 0)
	{
		set_max_mp(get_max_mp() - hyperbody_max_mp_);
		set_mp(max_mp_);
		hyperbody_max_mp_ = 0;
	}

	set_max_mp(get_max_mp() + value);
	hyperbody_max_mp_ = value;
}

short Player::get_hyperbody_max_mp()
{
	return hyperbody_max_mp_;
}

short Player::get_ap()
{
	return ap_;
}

short Player::get_sp()
{
	return sp_;
}

int Player::get_fame()
{
	return fame_;
}

int Player::get_exp()
{
	return exp_;
}

void Player::add_nx_cash_credit(int nx_cash_credit)
{
	nx_cash_credit_ += nx_cash_credit;
}

void Player::set_hp(short shp)
{
	if (shp < 0)
	{
		shp = 0;
	}

	else if (shp > max_hp_)
	{
		shp = max_hp_;
	}

	hp_ = shp;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsHp, hp_);
	send_packet(&packet1);

	if (party_)
	{
		PacketCreator packet;
		packet.UpdatePartyHp(this);
		party_->send_packet(&packet);
	}
}

void Player::set_mp(short smp)
{
	if (smp < 0)
	{
		smp = 0;
	}

	else if (smp > max_mp_)
	{
		smp = max_mp_;
	}

	mp_ = smp;

	PacketCreator packet;
	packet.UpdateStatShort(kCharacterStatsMp, mp_);
	send_packet(&packet);
}

void Player::set_max_hp(short max_hp)
{
	// does this check even make sense?
	int test = max_hp + max_hp_;

	if (test > SHRT_MAX)
	{
		max_hp = SHRT_MAX;
	}

	else if (max_hp < 1)
	{
		max_hp = 1;
	}

	max_hp_ = max_hp;

	PacketCreator packet;
	packet.UpdateStatShort(kCharacterStatsMaxHp, max_hp_);
	send_packet(&packet);

	if (party_)
	{
		PacketCreator packet;
		packet.UpdatePartyHp(this);
		party_->send_packet(&packet);
	}
}

void Player::set_max_mp(short max_mp)
{
	// does this check even make sense?
	int test = max_mp + max_mp_;

	if (max_mp > SHRT_MAX)
	{
		max_mp = SHRT_MAX;
	}

	else if (max_mp < 1)
	{
		max_mp = 1;
	}

	max_mp_ = max_mp;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsMaxMp, max_mp_);
	send_packet(&packet1);
}

void Player::set_sp(short sp)
{
	sp_ = sp;

	// packet
	PacketCreator packet1;
	packet1.UpdateSp(this, sp);
	send_packet(&packet1);
}

void Player::add_sp(short sp)
{
	set_sp(get_sp() + sp);
}

void Player::set_ap(short ap)
{
	ap_ = ap;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsAp, ap);
	send_packet(&packet1);
}

void Player::set_fame(int fame)
{
	fame_ = fame;

	// packet
	PacketCreator packet1;
	packet1.UpdateStatInt(kCharacterStatsFame, fame);
	send_packet(&packet1);
}

void Player::set_exp(int new_exp)
{
	// stores the highest level that the job of the player allows
	unsigned char max_level = 200;

	int needed_exp_for_lvlup = kCharacterStatsExpTable[level_ - 1];

	// loop as long as the received exp is higher than the exp needed for the current player's level
	while (new_exp >= needed_exp_for_lvlup)
	{
		// exit the loop and set the exp to 0 when the highest level is reached
		if (level_ >= max_level)
		{
			new_exp = 0;
			break;
		}

		// decrease the received exp based on how much the current level requires for a level up
		new_exp -= needed_exp_for_lvlup;

		// call the function that processes a level up
		level_up();

		// update the value
		needed_exp_for_lvlup = kCharacterStatsExpTable[level_ - 1];
	}

	// set the player's exp to the rest of the received exp
	exp_ = new_exp;

	// update the player's client by sending a packet

	// packet
	PacketCreator packet5;
	packet5.UpdateStatInt(kCharacterStatsExp, exp_);
	send_packet(&packet5);
}

void Player::level_up()
{
	// increase the level
	set_level(level_ + 1);

	if (level_ == 200 && get_is_gm() == false)
	{
		PacketCreator packet1;
		packet1.ShowMessage("[Congrats] " + name_ + " has reached Level 200! Congratulate " + name_ + " on such an amazing achievement!", 6);
		World::get_instance()->send_packet(&packet1);
	}

	set_ap(ap_ + 5);

	// add sp if the player isn't a beginner

	if (!is_beginner_job(job_))
	{
		add_sp(3);
	}

	// default values
	// used for GameMasters

	int max_hp_gain = 100;
	int max_mp_gain = 100;

	// Beginner
	if (is_beginner_job(job_))
	{
		max_hp_gain = tools::random_int(12, 16);
		max_mp_gain = tools::random_int(10, 12);
	}
	// Warrrior
	else if (job_ >= 100 && job_ <= 132)
	{
		max_hp_gain = tools::random_int(73, 80);
		max_mp_gain = tools::random_int(13, 18);
	}
	// Magician
	else if (job_ >= 200 && job_ <= 232)
	{
		max_hp_gain = tools::random_int(10, 14);
		max_mp_gain = tools::random_int(50, 54);
	}
	// Bowman, Thief
	else if ((job_ >= 300 && job_ <= 322) || (job_ >= 400 && job_ <= 422))
	{
		max_hp_gain = tools::random_int(20, 24);
		max_mp_gain = tools::random_int(14, 16);
	}
	// Pirate
	else if (job_ >= 500 && job_ <= 522)
	{
		max_hp_gain = tools::random_int(37, 41);
		max_mp_gain = tools::random_int(18, 22);
	}

	set_max_hp(max_hp_ + max_hp_gain);
	set_hp(max_hp_);

	set_max_mp(max_mp_ + max_mp_gain);
	set_mp(max_mp_);
}

void Player::set_map(int mapid)
{
	World *world = World::get_instance();

	Channel *channel = world->GetChannel(channel_id_);
	if (!channel)
	{
		return;
	}

	Map *new_map = channel->get_map(mapid);
	if (new_map)
	{
		change_map(new_map);
	}
}

void Player::change_map(Map *new_map, signed char new_spawn_point)
{
	map_->remove_player(this);
	map_ = new_map;
	spawn_point_ = new_spawn_point;

	// send a packet
	PacketCreator packet;
	packet.change_map(this, false);
	send_packet(&packet);

	map_->add_player(this);
}

std::shared_ptr<Item> Player::get_pet(int id)
{
	for (size_t i = 0; i < pets_.size(); ++i)
	{
		if (pets_[i]->get_unique_id() == id)
		{
			return pets_[i];
		}
	}
	return nullptr;
}

void Player::set_skin_color(signed char id)
{
	skin_color_ = id;
	// send a packet
	PacketCreator packet1;
	packet1.UpdateStatShort(kCharacterStatsSkin, skin_color_);
	send_packet(&packet1);
	// send a packet
	PacketCreator packet2;
	packet2.UpdatePlayer(this);
	map_->send_packet(&packet2, this);
}

void Player::set_face(int id)
{
	face_ = id;
	// send a packet
	PacketCreator packet1;
	packet1.UpdateStatInt(kCharacterStatsFace, face_);
	send_packet(&packet1);
	// send a packet
	PacketCreator packet2;
	packet2.UpdatePlayer(this);
	map_->send_packet(&packet2, this);
}

void Player::set_hair(int id)
{
	hair_ = id;
	// send a packet
	PacketCreator packet1;
	packet1.UpdateStatInt(kCharacterStatsHair, hair_);
	send_packet(&packet1);
	// send a packet
	PacketCreator packet2;
	packet2.UpdatePlayer(this);
	map_->send_packet(&packet2, this);
}

short Player::get_item_amount(int itemid)
{
	short amount = 0;

	Inventory *inventory = get_inventory(tools::get_inventory_id_from_item_id(itemid));
	if (!inventory)
	{
		return amount;
	}

	amount = inventory->get_item_amount(itemid);
	return amount;
}

bool Player::give_item(int item_id, short amount)
{
	Inventory *inventory = get_inventory(tools::get_inventory_id_from_item_id(item_id));
	if (!inventory)
	{
		return false;
	}

	if (!inventory->give_item(item_id, amount))
	{
		return false;
	}

	// send a packet
	PacketCreator packet;
	packet.ItemGainChat(item_id, amount);
	send_packet(&packet);

	return true;
}

void Player::remove_item(int item_id, short amount)
{
	Inventory *inventory = get_inventory(tools::get_inventory_id_from_item_id(item_id));
	if (!inventory)
	{
		return;
	}

	inventory->remove_item(item_id, amount);

	// send a packet
	PacketCreator packet;
	packet.ItemGainChat(item_id, -amount);
	send_packet(&packet);
}

void Player::save_account()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	Poco::Data::Statement statement1(mysql_session);

	statement1 << "UPDATE users SET character_slots = '" << character_slots_
		<< "', donation_points = '" << donation_points_
		<< "', storage_mesos = '" << storage_mesos_
		<< "', nxcash_credit = '" << nx_cash_credit_
		<< "' WHERE id = " << user_id_;

	statement1.execute();
}

void Player::save_character()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	Poco::Data::Statement statement1(mysql_session);

	statement1 << "UPDATE characters SET level = '" << static_cast<int>(level_)
		<< "', job = '" << static_cast<int>(job_)
		<< "', str = '" << static_cast<int>(str_)
		<< "', dex = '" << static_cast<int>(dex_)
		<< "', intt = '" << static_cast<int>(int_)
		<< "', luk = '" << static_cast<int>(luk_)
		<< "', hp = '" << static_cast<int>(hp_)
		<< "', max_hp = '" << static_cast<int>(max_hp_)
		<< "', mp = '" << static_cast<int>(mp_)
		<< "', max_mp = '" << static_cast<int>(max_mp_)
		<< "', ap = '" << static_cast<int>(ap_)
		<< "', sp = '" << static_cast<int>(sp_)
		<< "', exp = '" << static_cast<int>(exp_)
		<< "', fame = '" << static_cast<int>(fame_)
		<< "', map = '" << map_->get_id()
		<< "', pos = '" << static_cast<int>(map_->get_closest_spawn_point(position_x_, position_y_))
		<< "', gender = '" << static_cast<int>(gender_)
		<< "', skin = '" << static_cast<int>(skin_color_)
		<< "', face = '" << static_cast<int>(face_)
		<< "', hair = '" << static_cast<int>(hair_)
		<< "', mesos = '" << mesos_
		<< "', party_id = '" << static_cast<int>((party_) ? party_->get_id() : 0)
		<< "', guild_id = '" << static_cast<int>((guild_) ? guild_->get_id() : 0)
		<< "', guild_rank = '" << static_cast<int>(guild_rank_)
		<< "', equip_slots = '" << static_cast<int>(get_inventory(kInventoryConstantsTypesEquip)->get_slots())
		<< "', use_slots = '" << static_cast<int>(get_inventory(kInventoryConstantsTypesUse)->get_slots())
		<< "', setup_slots = '" << static_cast<int>(get_inventory(kInventoryConstantsTypesSetup)->get_slots())
		<< "', etc_slots = '" << static_cast<int>(get_inventory(kInventoryConstantsTypesEtc)->get_slots())
		<< "', merchant_storage_mesos= '" << merchant_storage_mesos_
		<< "' WHERE id = " << id_;

	statement1.execute();
}

void Player::save_skills()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	// skills
	Poco::Data::Statement statement1(mysql_session);
	statement1 << "DELETE FROM skills WHERE player_id = " << id_;
	statement1.execute();

	if (skills_.empty())
	{
		return;
	}

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;

	for (auto &it : skills_)
	{
		if (firstrun)
		{
			statement2 << "INSERT INTO skills (player_id, skill_id, level, master_level) VALUES (";
			firstrun = false;
		}
		else
		{
			statement2 << ",(";
		}
		statement2 << id_ << "," << it.first << "," << static_cast<short>(it.second.level_) << "," << static_cast<short>(it.second.master_level_) << ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}

}

void Player::save_equips()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	// equips
	Poco::Data::Statement statement1(mysql_session);
	statement1 << "DELETE FROM equips WHERE player_id = " << id_;
	statement1.execute();

	for (int i = kInventoryConstantsTypesEquipped; i <= kInventoryConstantsTypesEquip; ++i)
	{
		Poco::Data::Statement statement2(mysql_session);
		bool firstrun = true;
		auto items = get_inventory(i)->get_items();

		for (auto &it : *items)
		{
			std::shared_ptr<Item> equip = it.second;
			if (firstrun)
			{
				statement2 << "INSERT INTO equips (equip_id, player_id, pos, slots, used_scrolls, str, dex, iint, luk, hp, mp, weapon_attack, magic_attack, weapon_def, magic_def, accuracy, avoid, hand, speed, jump) VALUES";
				firstrun = false;
			}
			else
			{
				statement2 << ",";
			}
			statement2 << "("
				<< equip->get_item_id() << ","
				<< id_ << ","
				<< static_cast<int>(equip->get_slot()) << ","
				<< static_cast<int>(equip->get_free_slots()) << ","
				<< static_cast<int>(equip->get_used_scrolls()) << ","
				<< static_cast<int>(equip->get_str()) << ","
				<< static_cast<int>(equip->get_dex()) << ","
				<< static_cast<int>(equip->get_int()) << ","
				<< static_cast<int>(equip->get_luk()) << ","
				<< static_cast<int>(equip->get_hp()) << ","
				<< static_cast<int>(equip->get_mp()) << ","
				<< static_cast<int>(equip->get_weapon_attack()) << ","
				<< static_cast<int>(equip->get_magic_attack()) << ","
				<< static_cast<int>(equip->get_weapon_defense()) << ","
				<< static_cast<int>(equip->get_magic_defense()) << ","
				<< static_cast<int>(equip->get_acc()) << ","
				<< static_cast<int>(equip->get_avoid()) << ","
				<< static_cast<int>(equip->get_hand()) << ","
				<< static_cast<int>(equip->get_speed()) << ","
				<< static_cast<int>(equip->get_jump())
				<< ")";
		}
		if (!firstrun)
		{
			statement2.execute();
		}
	}
}

void Player::save_items()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM items WHERE player_id = " << id_;
	statement1.execute();

	for (int i = kInventoryConstantsTypesUse; i <= kInventoryConstantsTypesCash; ++i)
	{
		Poco::Data::Statement statement2(mysql_session);
		bool firstrun = true;
		auto items = get_inventory(i)->get_items();

		for (auto &it : *items)
		{
			std::shared_ptr<Item> item = it.second;
			if (!item->is_pet())
			{
				if (firstrun)
				{
					statement2 << "INSERT INTO items (item_id, player_id, pos, amount) VALUES";
					firstrun = false;
				}
				else
				{
					statement2 << ",";
				}
				statement2 << "(" << item->get_item_id() << "," << id_ << "," << static_cast<int>(item->get_slot()) << "," << static_cast<int>(item->get_amount()) << ")";
			}
		}

		if (!firstrun)
		{
			statement2.execute();
		}
	}
}

void Player::save_pets()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM pets WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	auto items = get_inventory(kInventoryConstantsTypesCash)->get_items();

	for (auto &it : *items)
	{
		std::shared_ptr<Item> pet = it.second;
		if (pet->is_pet())
		{
			if (firstrun)
			{
				statement2 << "INSERT INTO pets (pet_id, player_id, pos, pet_slot, level, closeness) VALUES";
				firstrun = false;
			}
			else
			{
				statement2 << ",";
			}
			statement2 << "(" << pet->get_item_id() << "," << id_ << "," << static_cast<int>(pet->get_slot()) << "," << static_cast<int>(pet->get_pet_slot()) << "," << static_cast<int>(pet->get_pet_level()) << "," << static_cast<int>(pet->get_pet_closeness()) << ")";
		}
	}

	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_keymap()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM keymap WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (int pos = kMinKeymapPos; pos < kMaxKeymapPos; ++pos)
	{
		Key &key = keys_[pos];
		if (firstrun)
		{
			statement2 << "INSERT INTO keymap (player_id, pos, type, action) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "(" << id_ << "," << pos << "," << static_cast<int>(key.type) << "," << key.action << ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_quests()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM quests WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto &it : completed_quests_)
	{
		Quest *quest = it.second.get();
		if (firstrun)
		{
			statement2 << "INSERT INTO quests (player_id, quest_id, is_complete) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "(" << id_ << ", " << quest->get_id() << ", 1" << ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
	Poco::Data::Statement statement3(mysql_session);
	firstrun = true;
	for (auto &it : quests_in_progress_)
	{
		Quest *quest = it.second.get();
		if (quest)
		{
			if (firstrun)
			{
				statement3 << "INSERT INTO quests (player_id, quest_id, is_complete, killed_mob, amount) VALUES";
				firstrun = false;
			}
			else
			{
				statement3 << ",";
			}
			auto killed_mobs = quest->get_killed_mobs2();
			if (killed_mobs->size() == 0)
			{
				statement3 << "(" << id_ << ", " << quest->get_id() << ", 0, 0, 0)";
			}
			for (auto &it2 : *killed_mobs)
			{
				int mob_id = it2.first;
				int count = it2.second;
				statement3 << "(" << id_ << ", " << quest->get_id() << ", 0, " << mob_id << ", " << count << ")";
				break; // todo: allow entering multiple killed mobs
			}
		}
	}
	if (!firstrun)
	{
		statement3.execute();
	}
}

void Player::save_buddylist()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM buddy_lists WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto &it : buddies_)
	{
		Buddy *buddy = it.second.get();
		if (firstrun)
		{
			statement2 << "INSERT INTO buddy_lists (player_id, buddy_id) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "(" << id_ << "," << buddy->get_player_id() << ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_storage_equips()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM storage_equips WHERE user_id = " << user_id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto equip : storage_items_)
	{
		if (equip->get_inventory_id() != kInventoryConstantsTypesEquip)
		{
			continue;
		}
		if (firstrun)
		{
			statement2 << "INSERT INTO storage_equips (equip_id, user_id, pos, slots, used_scrolls, str, dex, iint, luk, hp, mp, weapon_attack, magic_attack, weapon_def, magic_def, accuracy, avoid, hand, speed, jump) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "("
			<< equip->get_item_id() << ","
			<< user_id_ << ","
			<< static_cast<int>(equip->get_slot()) << ","
			<< static_cast<int>(equip->get_free_slots()) << ","
			<< static_cast<int>(equip->get_used_scrolls()) << ","
			<< static_cast<int>(equip->get_str()) << ","
			<< static_cast<int>(equip->get_dex()) << ","
			<< static_cast<int>(equip->get_int()) << ","
			<< static_cast<int>(equip->get_luk()) << ","
			<< static_cast<int>(equip->get_hp()) << ","
			<< static_cast<int>(equip->get_mp()) << ","
			<< static_cast<int>(equip->get_weapon_attack()) << ","
			<< static_cast<int>(equip->get_magic_attack()) << ","
			<< static_cast<int>(equip->get_weapon_defense()) << ","
			<< static_cast<int>(equip->get_magic_defense()) << ","
			<< static_cast<int>(equip->get_acc()) << ","
			<< static_cast<int>(equip->get_avoid()) << ","
			<< static_cast<int>(equip->get_hand()) << ","
			<< static_cast<int>(equip->get_speed()) << ","
			<< static_cast<int>(equip->get_jump())
			<< ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_storage_items()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM storage_items WHERE user_id = " << user_id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto item : storage_items_)
	{
		if (item->get_inventory_id() == kInventoryConstantsTypesEquip || kInventoryConstantsTypesCash)
		{
			continue;
		}
		if (firstrun)
		{
			statement2 << "INSERT INTO storage_items (item_id, user_id, pos, amount) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "("
			<< item->get_item_id() << ","
			<< user_id_ << ","
			<< static_cast<int>(item->get_slot()) << ","
			<< static_cast<int>(item->get_amount())
			<< ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_merchant_storage_equips()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM merchant_storage_equips WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto it : merchant_storage_items_)
	{
		std::shared_ptr<Item> equip = it.second;
		if (equip->get_inventory_id() != kInventoryConstantsTypesEquip)
		{
			continue;
		}
		if (firstrun)
		{
			statement2 << "INSERT INTO merchant_storage_equips (equip_id, player_id, pos, slots, used_scrolls, str, dex, iint, luk, hp, mp, weapon_attack, magic_attack, weapon_def, magic_def, accuracy, avoid, hand, speed, jump) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "("
			<< equip->get_item_id() << ","
			<< id_ << ","
			<< static_cast<int>(equip->get_slot()) << ","
			<< static_cast<int>(equip->get_free_slots()) << ","
			<< static_cast<int>(equip->get_used_scrolls()) << ","
			<< static_cast<int>(equip->get_str()) << ","
			<< static_cast<int>(equip->get_dex()) << ","
			<< static_cast<int>(equip->get_int()) << ","
			<< static_cast<int>(equip->get_luk()) << ","
			<< static_cast<int>(equip->get_hp()) << ","
			<< static_cast<int>(equip->get_mp()) << ","
			<< static_cast<int>(equip->get_weapon_attack()) << ","
			<< static_cast<int>(equip->get_magic_attack()) << ","
			<< static_cast<int>(equip->get_weapon_defense()) << ","
			<< static_cast<int>(equip->get_magic_defense()) << ","
			<< static_cast<int>(equip->get_acc()) << ","
			<< static_cast<int>(equip->get_avoid()) << ","
			<< static_cast<int>(equip->get_hand()) << ","
			<< static_cast<int>(equip->get_speed()) << ","
			<< static_cast<int>(equip->get_jump())
			<< ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save_merchant_storage_items()
{
	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	statement1 << "DELETE FROM merchant_storage_items WHERE player_id = " << id_;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	bool firstrun = true;
	for (auto it : merchant_storage_items_)
	{
		std::shared_ptr<Item> item = it.second;
		if (item->get_inventory_id() == kInventoryConstantsTypesEquip || item->get_inventory_id() == kInventoryConstantsTypesCash)
		{
			continue;
		}
		if (firstrun)
		{
			statement2 << "INSERT INTO merchant_storage_items (item_id, player_id, pos, amount) VALUES";
			firstrun = false;
		}
		else
		{
			statement2 << ",";
		}
		statement2 << "("
			<< item->get_item_id() << ","
			<< id_ << ","
			<< static_cast<int>(item->get_slot()) << ","
			<< static_cast<int>(item->get_amount())
			<< ")";
	}
	if (!firstrun)
	{
		statement2.execute();
	}
}

void Player::save()
{
	save_account();
	save_character();
	save_skills();
	save_equips();
	save_items();
	save_pets();
	save_keymap();
	save_quests();
	save_buddylist();
	save_storage_equips();
	save_storage_items();
	save_merchant_storage_equips();
	save_merchant_storage_items();
}

void Player::hide()
{
	if (in_hide_ == true)
	{
		// packet
		PacketCreator packet1;
		packet1.ShowMessage("You are already hidden.", 1);
		send_packet(&packet1);

		return;
	}

	// packet
	PacketCreator packet2;
	packet2.ShowMessage("You are now hidden.", 1);
	send_packet(&packet2);

	in_hide_ = true;

	auto players = map_->get_players();
	for (size_t i = 0; i < players->size(); ++i)
	{
		Player *player = players->at(i);
		if (player && player->get_is_gm() == false)
		{
			// send a packet
			PacketCreator packet11;
			packet11.RemovePlayer(this);
			player->send_packet(&packet11);
		}
	}
}

void Player::unhide()
{
	if (in_hide_ == false)
	{
		// packet
		PacketCreator packet1;
		packet1.ShowMessage("You are already visible.", 1);
		send_packet(&packet1);

		return;
	}

	// packet
	PacketCreator packet2;
	packet2.ShowMessage("You are now visible.", 1);
	send_packet(&packet2);

	in_hide_ = false;

	auto players = map_->get_players();

	for (size_t i = 0; i < players->size(); ++i)
	{
		Player *player = players->at(i);

		if (player && player->get_is_gm() == false)
		{
			PacketCreator packet10;
			packet10.ShowPlayer(this);
			player->send_packet(&packet10);
		}
	}
}

void Player::apply_attack_skill_costs(int skill_id, signed char skill_level)
{
	Effect *effect = Effect::get_skill_effect(skill_id, skill_level);

	if (effect)
	{
		effect->use_attack_skill(this);
	}
}

signed char Player::get_equip_slots()
{
	return get_inventory(kInventoryConstantsTypesEquip)->get_slots();
}

signed char Player::get_use_slots()
{
	return get_inventory(kInventoryConstantsTypesUse)->get_slots();
}

signed char Player::get_setup_slots()
{
	return get_inventory(kInventoryConstantsTypesSetup)->get_slots();
}

signed char Player::get_etc_slots()
{
	return get_inventory(kInventoryConstantsTypesEtc)->get_slots();
}

signed char Player::get_cash_slots()
{
	return get_inventory(kInventoryConstantsTypesCash)->get_slots();
}

std::string Player::get_name()
{
	return name_;
}

bool Player::is_married()
{
	return false;
}

signed char Player::get_buddy_list_capacity()
{
	return 100;
}

signed char Player::get_gender()
{
	return gender_;
}

signed char Player::get_skin_color()
{
	return skin_color_;
}

int Player::get_face()
{
	return face_;
}

int Player::get_hair()
{
	return hair_;
}

Map* Player::get_map()
{
	return map_;
}

signed char Player::get_spawn_point()
{
	return spawn_point_;
}

int Player::get_chair()
{
	return chair_;
}

std::string Player::get_chalk_board()
{
	return chalk_board_text_;
}

int Player::get_item_effect()
{
	return item_effect_;
}

void Player::set_party(Party* party)
{
	party_ = party;
}

Party* Player::get_party()
{
	return party_;
}

void Player::set_party_invitation(int invite)
{
	party_invite_ = invite;
}

void Player::set_guild(Guild* guild)
{
	guild_ = guild;
}

Guild* Player::get_guild()
{
	return guild_;
}

bool Player::get_has_guild()
{
	return (guild_ != nullptr);
}

void Player::set_guild_rank(int rank)
{
	guild_rank_ = rank;
}

int Player::get_guild_rank()
{
	return guild_rank_;
}

bool Player::get_is_in_cash_shop()
{
	return in_cash_shop_;
}

bool Player::get_is_gm()
{
	return (is_gm_ > 0);
}

Inventory* Player::get_inventory(signed char id)
{
	if (inventories_.find(id) != inventories_.end())
	{
		return inventories_[id];
	}
	return nullptr;
}

std::shared_ptr<HiredMerchant> Player::get_hired_merchant()
{
	return merchant_;
}

void Player::set_hired_merchant(std::shared_ptr<HiredMerchant> merchant)
{
	merchant_ = merchant;
}

std::vector<std::shared_ptr<Item>> *Player::get_pets()
{
	return &pets_;
}

signed char Player::get_messenger_position()
{
	return messenger_position_;
}

void Player::set_messenger_position(signed char position)
{
	messenger_position_ = position;
}

Messenger* Player::get_messenger()
{
	return messenger_;
}

void Player::set_messenger(Messenger* messenger)
{
	messenger_ = messenger;
}

Key& Player::get_key(int pos)
{
	return keys_[pos];
}

std::unordered_map<int, Skill>* Player::get_skills()
{
	return &skills_;
}

unsigned char Player::get_channel_id()
{
	return channel_id_;
}

void Player::set_stance(signed char stance)
{
	stance_ = stance;
}

signed char Player::get_stance()
{
	return stance_;
}

short Player::get_position_x()
{
	return position_x_;
}

short Player::get_position_y()
{
	return position_y_;
}

short Player::get_foothold()
{
	return foothold_;
}

int Player::get_id()
{
	return id_;
}

bool Player::is_hidden()
{
	return in_hide_;
}

std::unordered_map<timer::Id, std::shared_ptr<Timer>> *Player::get_timers()
{
	return &timers_;
}

int Player::get_mount_skill_id()
{
	return mount_skill_id_;
}

void Player::set_mount_skill_id(int mount_skill_id)
{
	mount_skill_id_ = mount_skill_id;
}

int Player::get_mount_item_id()
{
	return mount_item_id_;
}

void Player::set_mount_item_id(int mount_item_id)
{
	mount_item_id_ = mount_item_id;
}

void Player::add_donation_points(int amount)
{
	donation_points_ += amount;
}

std::unordered_map<signed char, std::shared_ptr<Item>>* Player::get_merchant_storage_items()
{
	return &merchant_storage_items_;
}

void Player::add_merchant_storage_mesos(int amount)
{
	merchant_storage_mesos_ += amount;
}

signed char Player::get_crusader_combo_value()
{
	return crusader_combo_value_;
}

void Player::set_crusader_combo_value(signed char value)
{
	crusader_combo_value_ = value;
}

void Player::send_npc(int npc_id)
{
	if (npc_->id_ == npc_id && npc_->state_ == 0)
	{
		return;
	}
	npc_->id_ = npc_id;
	npc_->set_selected(-1);
	npc_->set_state(0);

	send_npc_final();
}

void Player::send_npc_final()
{
	switch (npc_->id_)
	{
		// custom Free Market npcs
	case 9100117: // Gachapon
		handle_gachapon_npc();
		break;
	case 9001000: // Donator Npc
		handle_donator_npc();
		break;
	case 9001001: // Maple Leaf Exchanger
		handle_maple_leaf_exchanger_npc();
		break;

		// job advancer npcs
	case 1032001: // Mage job advancer
		handle_mage_job_advancer();
		break;
	case 1052001: // Thief job advancer
		handle_thief_job_advancer();
		break;
	case 1012100: // Bowman job advancer
		handle_bowman_job_advancer();
		break;
	case 1022000: // Warrior job advancer
		handle_warrior_job_advancer();
		break;
	case 1072008: // Pirate job advancer
		handle_pirate_job_advancer();
		break;

		// Storage npcs
	case 9030000: // Fredrick
	{
		// packet
		PacketCreator packet20;
		packet20.GetFredrickStorage(merchant_storage_items_, merchant_storage_mesos_);
		send_packet(&packet20);

		break;
	}
	case 1002005: // Mr. Kim
	case 1012009: // Mr. Lee
	case 1022005: // Mr. Wang
	case 1032006: // Mr. Park
	case 1033000: // Hermes
	case 1061008: // Mr. Oh
	case 1091004: // Dondlass
	case 1100000: // Kirium
	case 1200000: // Pusla
	case 1400000: // Cristophe
	case 2010006: // Trina
	case 2020004: // Mr. Mohammed
	case 2041008: // Seppy
	case 9030100: // Scrooge
	{
		// packet
		PacketCreator packet22;
		packet22.GetStorage(npc_->id_, storage_slots_, storage_items_, storage_mesos_);
		send_packet(&packet22);

		break;
	}

	// PQ Npcs

	case 1012112: // Tory, warps into Henesys PQ
		handle_hpq_tory_npc();
		break;
	case 1012113: // Tommy, warps out of Henesys PQ
		handle_hpq_tommy_npc();
		break;
	case 1012114: // Growlie, warps to shortcut map after finishing HPQ main stage
		handle_hpq_growlie_npc();
		break;
	case 9020000: // Lakelis, brings party into Kerning PQ
		handle_kpq_lakelis_npc();
		break;
	case 9020001: // Cloto, KPQ Stage 1, 2, 3, 4, Last Stage
		handle_kpq_cloto_npc();
		break;
	case 9020002: // Nella, Exit for KPQ
		handle_kpq_nella_npc();
		break;
	case 2040034: // Red Sign, brings party into Ludibrium PQ
		handle_lpq_red_sign_npc();
		break;
	case 2013000: // Wonky the fairy, brings party into Orbis PQ
		handle_npc_opq_wonky();
		break;
	case 2094000: // Guon, brings party into Pirate PQ
		handle_npc_ppq_guon();
		break;
	case 2094002: // Guon2, handles pirate pq inside
		handle_npc_ppq_guon2();
		break;
	case 9103000: // Pietri, at Ludibrium Maze PQ room 16
		handle_npc_lmpq_pietri();
		break;
	case 9103001: // Rolly, brings party into Ludi Maze PQ
		handle_npc_lmpq_rolly();
		break;
	case 9103002: // Rolly2, warps out of Ludi Maze PQ
		handle_npc_lmpq_rolly2();
		break;
	case 9103003: // Rolly3, warps out of Ludi Maze PQ and gives rewards
		handle_npc_lmpq_rolly3();
		break;
	case 2042000: // Spiegelmann, warps into Carnival PQ
		handle_npc_cpq_spiegelmann();
		break;

		// boss npcs
	case 2041025: // Machine Apparatus, Origin of Clocktower(220080001)
		handle_papulatus_npc();
		break;
	case 2030010: // Amon @ Last Mission: Zakum's Altar
		handle_amon_zakum_npc();
		break;
	case 2030013: // Adobis @ El Nath: Entrance to Zakum Altar
		handle_adobis_zakum_npc();
		break;
	case 2081005: // Horntail entrance
		handle_horntail_entrance_npc();
		break;

		// misc npcs
	case 1012000: // Regular Cab in Victoria
		handle_regular_cab_npc();
		break;
	case 9201056: // NLC Taxi
		handle_nlc_taxi_npc();
		break;
	case 2012014: // Orbis Magic Spot
		handle_orbis_magic_spot_npc();
		break;
	case 2012015: // EL Nath Magic Spot
		handle_el_nath_magic_spot_npc();
		break;
	case 2040024: // First eos rock
		handle_first_eos_rock_npc();
		break;
	case 2040025: // Second eos rock
		handle_second_eos_rock_npc();
		break;
	case 2040026: // Third eos rock
		handle_third_eos_rock_npc();
		break;
	case 2040027: // Fourth eos rock
		handle_fourth_eos_rock_npc();
		break;
	case 2030000: // Jeff - El Nath : Ice Valley II (211040200)
		handle_jeff_npc();
		break;
	case 2010007: // Heracle guild creator
		handle_heracle_guild_creator_npc();
		break;
	case 2010008: // Lea guild emblem creator
		handle_lea_guild_emblem_npc();
		break;
	case 9900000: // KIN
		handle_kin_npc();
		break;
	case 9900001: // NimaKIN
		handle_nimakin_npc();
		break;

		// maple camp npcs
	case 2100: // Sera
		set_map(100000000);
		break;

		// ship, subway and plane npcs
	case 1032008: // Ellinia Station Ticket Npc
		set_map(200000100);
		break;
	case 2012001: // Orbis Station<To Ellinia>
		set_map(101000300);
		break;
	case 2012013: // Orbis Station<Ludibrium>
		set_map(220000100);
		break;
	case 2041000: // Ludubrium Station<Orbis>
		set_map(200000100);
		break;
	case 2012006: // Orbis Platform Usher
		handle_orbis_platform_usher();
		break;
	case 9270038: // Changi Airport Shalon
		set_map(103000000);
		break;
	case 9270041: // Kerning City Irene
		set_map(540010000);
		break;
	case 1052007: // The Ticket Gate (Kerning)
		set_map(600010001);
		break;
	case 2102000: // Asesson (Ariant)
		set_map(200000100);
		break;
	case 2082001: // Tommie (Leafre)
		set_map(200000100);
		break;

		// misc npcs

	case 9000020: // Spinel
		set_map(800000000);
		break;
	case 9201022: // Thomas Swift
		set_map(680000000);
		break;

	default:
	{
		int shop_id = npc_->id_;
		ShopData *npc_shop = ShopDataProvider::get_instance()->get_shop_data(shop_id);

		if (npc_shop)
		{
			shop_ = npc_shop;

			// packet
			PacketCreator packet22;
			packet22.ShowNpcShop(shop_);
			send_packet(&packet22);
		}
	}
	}
}
