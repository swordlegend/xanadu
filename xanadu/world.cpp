//

#include "world.hpp"

#include "map.hpp"
#include "channel.hpp"
#include "guild.hpp"
#include "guild_member.hpp"
#include "messenger.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "hiredmerchant.hpp"
#include "player.hpp"
#include "hiredmerchant_item.hpp"
#include "packetcreator.hpp"
#include "valid_char_data_provider.hpp"
#include "cash_shop_package_data_provider.hpp"
#include "cash_shop_item_data_provider.hpp"
#include "item_data_provider.hpp"
#include "equip_data_provider.hpp"
#include "shop_data_provider.hpp"
#include "skill_data_provider.hpp"
#include "mob_data_provider.hpp"
#include "mob_drops_data_provider.hpp"
#include "pet_data_provider.hpp"
#include "quest_data_provider.hpp"
#include "map_data_provider.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"

#include "Poco\Data\RecordSet.h"

// singleton

World *World::instance_ = nullptr;

World *World::get_instance()
{
	return instance_;
}

// default constructor

World::World(bool is_dedicated_server) :
	is_dedicated_server_(is_dedicated_server),
	is_accepting_connections_(true),
	id_(0),
	pet_ids_(0),
	summon_ids_(0),
	messenger_ids_(0),
	party_ids_(0),
	guild_ids_(0),
	exp_rate_(kExpRate),
	name_(kWorld1Name),
	header_message_(kHeaderMessage), // standard header message ingame
	mysql_session_("MySQL", "user=root;password=xxxx;db=xanadu62;auto-reconnect=true")
{
	instance_ = this;
}

// initializing

void World::initialize()
{
	// create and prepare the wz reader for usage

	wz_reader_ = new WZMain();
	wz_reader_->initialize();

	// load .wz file data

	CashShopItemDataProvider::get_instance()->load_data();
	CashShopPackageDataProvider::get_instance()->load_data();
	MobDataProvider::get_instance()->load_data();
	PetDataProvider::get_instance()->load_data();
	SkillDataProvider::get_instance()->load_data();
	ValidCharDataProvider::get_instance()->load_data();
	QuestDataProvider::get_instance()->load_data();
	ItemDataProvider::get_instance()->load_data();
	EquipDataProvider::get_instance()->load_data();
	MapDataProvider::get_instance()->load_data();

	// done loading wz data

	// now do other world initializing

	// load custom data
	// shops, mob drops and reactor drops

	ShopDataProvider::get_instance()->load_data();
	MobDropsDataProvider::get_instance()->load_data();
	// todo: reactor drops

	// done loading custom data

	// create channels
	for (signed char channel_id = 0; channel_id < kChannelsCount; ++channel_id)
	{
		channels_[channel_id] = new Channel(channel_id);
	}


	// free memory used

	delete wz_reader_;

	// load guilds
	Poco::Data::Statement statement1(mysql_session_);
	statement1 << "SELECT * FROM guilds";
	statement1.execute();

	Poco::Data::RecordSet recordset1(statement1);

	for (unsigned int i = 0; i < recordset1.rowCount(); ++i)
	{
		int guild_id = recordset1["guild_id"];
		std::string guild_name = recordset1["guild_name"].convert<std::string>();

		Guild *guild = new Guild(guild_id, guild_name);

		guild->SetNotice(recordset1["notice"]);
		guild->SetRank1(recordset1["rank1"]);
		guild->SetRank2(recordset1["rank2"]);
		guild->SetRank3(recordset1["rank3"]);
		guild->SetRank4(recordset1["rank4"]);
		guild->SetRank5(recordset1["rank5"]);
		guild->SetLogoBackground(recordset1["logo_bg"]);
		guild->SetLogoBackgroundColor(recordset1["logo_bg_color"]);
		guild->SetLogo(recordset1["logo"]);
		guild->SetLogoColor(recordset1["logo_color"]);

		guilds_[guild_id] = guild;
		guild_ids_ = guild_id;

		recordset1.moveNext();
	}

	// add guild members

	for (auto &it : guilds_)
	{
		auto &guild = it.second;
		int guild_id = guild->get_id();

		Poco::Data::Statement statement2(mysql_session_);
		statement2 << "SELECT id, name, job, level, guild_rank FROM characters WHERE guild_id = " << guild_id;
		statement2.execute();

		Poco::Data::RecordSet recordset2(statement2);

		for (unsigned int i = 0; i < recordset2.rowCount(); ++i)
		{
			GuildMember *member = new GuildMember(recordset2["id"], recordset2["job"], recordset2["level"], recordset2["guild_rank"], 0, recordset2["name"]);
			guild->AddMember(member);

			recordset2.moveNext();
		}
	}

	// reset partys

	Poco::Data::Statement statement13(mysql_session_);
	statement13 << "UPDATE characters SET party_id = 0";
	statement13.execute();

	// retrieve all player names

	Poco::Data::Statement statement14(mysql_session_);
	statement14 << "SELECT id, name FROM characters";
	statement14.execute();

	Poco::Data::RecordSet recordset14(statement14);

	for (unsigned int i = 0; i < recordset14.rowCount(); ++i)
	{
		std::string player_name = recordset14["name"].convert<std::string>();
		int player_id = recordset14["id"];
		add_name_id(player_name, player_id);

		recordset14.moveNext();
	}
}

void World::ShutdownServer()
{
	// stop accepting new connections
	is_accepting_connections_ = false;

	// disconnect players
	for (auto &it : online_players_)
	{
		Player *player = it.second;
		player->disconnect();
	}

	// clear guilds
	Poco::Data::Statement statement1(mysql_session_);
	statement1 << "DELETE FROM guilds";
	statement1.execute();

	// save guilds
	for (auto &it : guilds_)
	{
		Guild *guild = it.second;

		Poco::Data::Statement statement2(mysql_session_);

		statement2 << "INSERT INTO guilds(guild_id, guild_name, notice, rank1, rank2, rank3, rank4, rank5, logo_bg, logo_bg_color, logo, logo_color) VALUES("
			<< guild->get_id() << ",'"
			<< guild->get_name() << "','"
			<< guild->GetNotice() << "','"
			<< guild->GetRank1() << "','"
			<< guild->GetRank2() << "','"
			<< guild->GetRank3() << "','"
			<< guild->GetRank4() << "','"
			<< guild->GetRank5() << "',"
			<< static_cast<int>(guild->get_logo_background()) << ","
			<< static_cast<int>(guild->get_logo_background_color()) << ","
			<< static_cast<int>(guild->get_logo()) << ","
			<< static_cast<int>(guild->get_logo_color())
			<< ")";

		statement2.execute();
	}

	// save hired merchants into owners fredrick merchant storage
	std::vector<int> hired_merchant_map_ids;

	hired_merchant_map_ids.push_back(910000001);
	hired_merchant_map_ids.push_back(910000002);
	hired_merchant_map_ids.push_back(910000003);
	hired_merchant_map_ids.push_back(910000004);
	hired_merchant_map_ids.push_back(910000005);
	hired_merchant_map_ids.push_back(910000006);
	hired_merchant_map_ids.push_back(910000007);
	hired_merchant_map_ids.push_back(910000008);
	hired_merchant_map_ids.push_back(910000009);
	hired_merchant_map_ids.push_back(910000010);
	hired_merchant_map_ids.push_back(910000011);
	hired_merchant_map_ids.push_back(910000012);
	hired_merchant_map_ids.push_back(910000013);
	hired_merchant_map_ids.push_back(910000014);
	hired_merchant_map_ids.push_back(910000015);
	hired_merchant_map_ids.push_back(910000016);
	hired_merchant_map_ids.push_back(910000017);
	hired_merchant_map_ids.push_back(910000018);
	hired_merchant_map_ids.push_back(910000019);
	hired_merchant_map_ids.push_back(910000020);
	hired_merchant_map_ids.push_back(910000021);
	hired_merchant_map_ids.push_back(910000022);

	for (auto &it : channels_)
	{
		Channel *channel = it.second;

		for (int map_id : hired_merchant_map_ids)
		{
			// skip map check if it wasn't used and therefore has no merchants
			if (!channel->has_map_loaded(map_id))
			{
				continue;
			}

			Map *map = channel->get_map(map_id);
			auto merchants = map->get_hired_merchants();

			for (auto &it : *merchants)
			{
				auto merchant = it.second;
				int owner_player_id = merchant->get_owner_id();
				auto merchant_items = merchant->get_items();

				// save player goods from his store in the database
				// add merchant mesos to fredrick

				Poco::Data::Statement statement3(mysql_session_);
				statement3 << "UPDATE characters SET merchant_storage_mesos = '" << merchant->get_merchant_mesos()
					<< "' WHERE id = " << owner_player_id;
				statement3.execute();

				// merchant storage equips
				Poco::Data::Statement statement4(mysql_session_);
				statement4 << "DELETE FROM merchant_storage_equips WHERE player_id = " << owner_player_id;
				statement4.execute();

				Poco::Data::Statement statement5(mysql_session_);
				bool firstrun = true;

				for (auto it : *merchant_items)
				{
					auto merchant_item = it.second;
					if (merchant_item->get_bundles() == 0)
					{
						continue;
					}
					std::shared_ptr<Item> equip = merchant_item->get_item();
					if (equip->get_inventory_id() != kInventoryConstantsTypesEquip)
					{
						continue;
					}
					if (firstrun)
					{
						statement5 << "INSERT INTO merchant_storage_equips (equip_id, player_id, pos, slots, used_scrolls, str, dex, iint, luk, hp, mp, weapon_attack, magic_attack, weapon_def, magic_def, accuracy, avoid, hand, speed, jump) VALUES";
						firstrun = false;
					}
					else
					{
						statement5 << ",";
					}
					statement5 << "("
						<< equip->get_item_id() << ","
						<< owner_player_id << ","
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
					statement5.execute();
				}

				// merchant storage items
				Poco::Data::Statement statement6(mysql_session_);
				statement6 << "DELETE FROM merchant_storage_items WHERE player_id = " << owner_player_id;
				statement6.execute();

				Poco::Data::Statement statement7(mysql_session_);
				firstrun = true;
				for (auto it : *merchant_items)
				{
					auto merchant_item = it.second;
					if (merchant_item->get_bundles() == 0)
					{
						continue;
					}
					std::shared_ptr<Item> item = merchant_item->get_item();
					item->set_amount(merchant_item->get_quantity());

					if (item->get_inventory_id() == kInventoryConstantsTypesEquip || item->get_inventory_id() == kInventoryConstantsTypesCash)
					{
						continue;
					}

					if (firstrun)
					{
						statement7 << "INSERT INTO merchant_storage_items (item_id, player_id, pos, amount) VALUES";
						firstrun = false;
					}
					else
					{
						statement7 << ",";
					}
					statement7 << "("
						<< item->get_item_id() << ","
						<< owner_player_id << ","
						<< static_cast<int>(item->get_slot()) << ","
						<< static_cast<int>(item->get_amount())
						<< ")";
				}
				if (!firstrun)
				{
					statement7.execute();
				}
			}
		}
	}
}

bool World::is_accepting_connections()
{
	return is_accepting_connections_;
}

bool World::is_dedicated_server()
{
	return is_dedicated_server_;
}

void World::set_is_accepting_connections(bool value)
{
	is_accepting_connections_ = value;
}

Poco::Data::Session &World::get_mysql_session()
{
	return mysql_session_;
}

asio::io_service &World::get_io_service()
{
	return io_service_;
}

// unique ids

int World::get_summon_id()
{
	return ++summon_ids_;
}

int World::get_messenger_id()
{
	return ++messenger_ids_;
}

int World::get_party_id()
{
	return ++party_ids_;
}

int World::get_guild_id()
{
	return ++guild_ids_;
}

int World::get_pet_id()
{
	return ++pet_ids_;
}

// Properties

std::string World::get_name()
{
	return name_;
}

int World::get_id()
{
	return id_;
}

// Scrolling yellow message

void World::set_header_message(std::string header_message)
{
	header_message_ = header_message;
}

std::string World::get_header_message()
{
	return header_message_;
}

// channels

Channel *World::GetChannel(int id)
{
	if (channels_.find(id) != channels_.end())
	{
		return channels_[id];
	}

	return nullptr;
}

// players

Player *World::GetPlayerByName(std::string name)
{
	// transform the argument-passed string into lowercase for comparison
	std::transform(name.begin(), name.end(), name.begin(), tolower);

	for (auto it : online_players_)
	{
		Player *player = it.second;

		// get the target player's name for comparison with the argument-passed one
		std::string player_name = player->get_name();

		// transform the target player's name into lowercase for comparison
		std::transform(player_name.begin(), player_name.end(), player_name.begin(), tolower);

		if (player_name == name)
		{
			return player;
		}
	}

	return nullptr;
}

Player *World::GetPlayerById(int id)
{
	if (online_players_.find(id) != online_players_.end())
	{
		return online_players_[id];
	}

	return nullptr;
}

void World::send_packet(PacketCreator *packet)
{
	for (auto it : online_players_)
	{
		Player *player = it.second;
		player->send_packet(packet);
	}
}

int World::get_player_id_from_name_(std::string player_name)
{
	if (player_name_id_.find(player_name) == player_name_id_.end())
	{
		return 0;
	}
	return player_name_id_[player_name];
}

std::string World::get_player_name_from_id(int player_id)
{
	if (player_id_name_.find(player_id) == player_id_name_.end())
	{
		return "";
	}
	return player_id_name_[player_id];
}

void World::ban_account(std::string player_name)
{
	int user_id;

	// retrieve the target players userid
	Poco::Data::Statement statement1(mysql_session_);
	statement1 << "SELECT user_id FROM characters WHERE name = '" << player_name << "'", Poco::Data::Keywords::into(user_id);
	if (statement1.execute() == 0)
	{
		return;
	}

	// set the account to banned
	Poco::Data::Statement statement2(mysql_session_);
	statement2 << "UPDATE users SET banned = 1 WHERE id = " << user_id;
	statement2.execute();
}

void World::unban_account(std::string player_name)
{
	int user_id;

	// retrieve the target players userid
	Poco::Data::Statement statement1(mysql_session_);
	statement1 << "SELECT user_id FROM characters WHERE name = '" << player_name << "'", Poco::Data::Keywords::into(user_id);
	if (statement1.execute() == 0)
	{
		return;
	}

	// set the account to banned
	Poco::Data::Statement statement2(mysql_session_);
	statement2 << "UPDATE users SET banned = 0 WHERE id = " << user_id;
	statement2.execute();
}

bool World::is_account_banned(int user_id)
{
	bool banned = false;

	Poco::Data::Statement statement1(mysql_session_);
	statement1 << "SELECT banned FROM users WHERE id = " << user_id, Poco::Data::Keywords::into(banned);
	statement1.execute();

	return banned;
}

void World::add_name_id(std::string player_name, int player_id)
{
	player_name_id_[player_name] = player_id;
	player_id_name_[player_id] = player_name;
}

void World::remove_name(std::string player_name, int player_id)
{
	auto iterator1 = player_name_id_.find(player_name);
	if (iterator1 != player_name_id_.end())
	{
		player_name_id_.erase(iterator1);
	}

	auto iterator2 = player_id_name_.find(player_id);
	if (iterator2 != player_id_name_.end())
	{
		player_id_name_.erase(iterator2);
	}
}

bool World::is_name_used(std::string player_name)
{
	if (player_name_id_.find(player_name) != player_name_id_.end())
	{
		return true;
	}

	return false;
}

void World::store_channel_id_for_user_id(unsigned char channel_id, int user_id)
{
	user_id_channel_id_[user_id] = channel_id;
}

void World::erase_channel_id_for_user_id(int user_id)
{
	auto iterator = user_id_channel_id_.find(user_id);
	if (iterator != user_id_channel_id_.end())
	{
		user_id_channel_id_.erase(iterator);
	}
}

unsigned char World::get_channel_id_for_user_id(int user_id)
{
	auto iterator = user_id_channel_id_.find(user_id);
	if (iterator != user_id_channel_id_.end())
	{
		return iterator->second;
	}
	
	// special number to signalize error in this case
	return 100;
}

void World::add_fame(int player_id)
{
	player_fame_ticks_[player_id] = GetTickCount64();
}

void World::remove_fame(int player_id)
{
	auto iterator = player_fame_ticks_.find(player_id);
	if (iterator != player_fame_ticks_.end())
	{
		player_fame_ticks_.erase(iterator);
	}
}

bool World::can_fame(int player_id)
{
	if (player_fame_ticks_.find(player_id) == player_fame_ticks_.end())
	{
		return true;
	}

	if ((GetTickCount64() - player_fame_ticks_[player_id]) > (24 * 60 * 60 * 1000))
	{
		return true;
	}

	return false;
}

bool World::is_online_user(std::string user_name)
{
	if (online_users_.find(user_name) != online_users_.end())
	{
		return true;
	}

	return false;
}

void World::add_online_user(std::string user_name)
{
	online_users_[user_name] = 0;
}

void World::remove_online_user(std::string user_name)
{
	auto iterator = online_users_.find(user_name);
	if (iterator != online_users_.end())
	{
		online_users_.erase(iterator);
	}
}

void World::AddPlayer(Player *player)
{
	online_players_[player->get_id()] = player;
}

void World::RemovePlayer(int player_id)
{
	auto iterator = online_players_.find(player_id);
	if (iterator != online_players_.end())
	{
		online_players_.erase(iterator);
	}
}

std::unordered_map<int, Player *> *World::get_players()
{
	return &online_players_;
}

// Messengers

void World::remove_messenger(int id)
{
	auto iterator = messengers_.find(id);
	if (iterator != messengers_.end())
	{
		Messenger *messenger = iterator->second;
		delete messenger;

		messengers_.erase(iterator);
	}
}

void World::create_messenger(Player *player)
{
	int messenger_id = get_messenger_id();
	messengers_[messenger_id] = new Messenger(messenger_id, player);
}

void World::join_messenger(Player *player, int messenger_id)
{
	auto iterator = messengers_.find(messenger_id);
	if (iterator == messengers_.end())
	{
		return;
	}

	Messenger *messenger = iterator->second;

	if (!messenger)
	{
		// packet
		PacketCreator packet1;
		packet1.ShowMessage("The messenger has already been closed.", 5);
		player->send_packet(&packet1);

		return;
	}

	auto players = messenger->get_players();

	if (players->size() > 2)
	{
		// packet
		PacketCreator packet2;
		packet2.ShowMessage("The messenger is full.", 5);
		player->send_packet(&packet2);

		return;
	}

	player->set_messenger(messenger);

	bool pos0 = false;
	bool pos1 = false;

	for (auto &it : *players)
	{
		Player *target = it.second;

		switch (target->get_messenger_position())
		{
		case 0:
			pos0 = true;
			break;
		case 1:
			pos1 = true;
			break;
		}
	}

	if (!pos0)
	{
		player->set_messenger_position(0);
	}
	else if (!pos1)
	{
		player->set_messenger_position(1);
	}
	else
	{
		player->set_messenger_position(2);
	}

	for (auto &it : *players)
	{
		Player *target = it.second;
		// send a packet
		PacketCreator packet30;
		packet30.MessengerAddPlayer(target);
		player->send_packet(&packet30);
		// send a packet
		PacketCreator packet31;
		packet31.MessengerAddPlayer(player);
		target->send_packet(&packet31);
	}

	messenger->add_member(player);

	// send a packet
	PacketCreator packet32;
	packet32.MessengerJoin(player->get_messenger_position());
	player->send_packet(&packet32);
}

// guilds

Guild *World::get_guild(int id)
{
	auto iterator = guilds_.find(id);
	if (iterator != guilds_.end())
	{
		return iterator->second;
	}

	return nullptr;
}

void World::create_guild(Player *player, std::string guild_name)
{
	int guild_id = get_guild_id();
	Guild *guild = new Guild(guild_id, guild_name);
	guilds_[guild->get_id()] = guild;
	guild->AddMember(player);
	player->set_guild(guild);

	// send a packet
	PacketCreator packet1;
	packet1.GuildInfo(guild);
	player->send_packet(&packet1);
}

void World::join_guild(Player *player, int guild_id)
{
	Guild *guild = get_guild(guild_id);
	if (!guild)
	{
		return;
	}

	player->set_guild(guild);
	guild->AddMember(player);

	GuildMember *member = guild->GetMember(player->get_id());
	if (member)
	{
		// send a packet
		PacketCreator packet2;
		packet2.GuildInfo(guild);
		player->send_packet(&packet2);

		// send a packet
		PacketCreator packet3;
		packet3.AddGuildPlayer(guild->get_id(), member);
		guild->send_packet(&packet3);
	}
}

void World::expel_guild_member(Guild *guild, int target_player_id, std::string target_player_name)
{
	GuildMember *member = guild->GetMember(target_player_id);
	if (!member)
	{
		return;
	}

	// send a packet
	PacketCreator packet22;
	packet22.GuildPlayerLeave(guild->get_id(), target_player_id, target_player_name, true);
	guild->send_packet(&packet22);

	Player *expelled = GetPlayerById(target_player_id);
	if (expelled)
	{
		expelled->set_guild(nullptr);
		expelled->set_guild_rank(5);
	}

	guild->DeleteMember(target_player_id);
}

void World::delete_guild(int guild_id)
{
	Guild *guild = get_guild(guild_id);
	if (!guild)
	{
		return;
	}

	// send a packet
	PacketCreator packet22;
	packet22.DisbandGuild(guild_id);
	guild->send_packet(&packet22);

	delete guild;
	guilds_.erase(guild_id);
}

bool World::guild_name_used(std::string guild_name)
{
	for (auto &it : guilds_)
	{
		Guild *guild = it.second;
		if (guild->get_name() == guild_name)
		{
			return true;
		}
	}
	return false;
}

// parties

Party *World::get_party(int id)
{
	auto iterator = parties_.find(id);
	if (iterator != parties_.end())
	{
		return iterator->second;
	}
	return nullptr;
}

void World::create_party(Player *player)
{
	int party_id = get_party_id();
	parties_[party_id] = new Party(party_id, player);
}

void World::delete_party(int party_id)
{
	auto iterator = parties_.find(party_id);
	if (iterator != parties_.end())
	{
		Party *party = iterator->second;
		delete party;

		parties_.erase(iterator);
	}
}
