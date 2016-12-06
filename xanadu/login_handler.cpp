//

#include "player.hpp"

#include "character.hpp"
#include "world.hpp"
#include "session.hpp"
#include "valid_char_data_provider.hpp"
#include "packetcreator.hpp"
#include "constants.hpp"

#include "Poco\Data\RecordSet.h"

void Player::handle_login_request()
{
	World *world = World::get_instance();
	if (!world->is_accepting_connections())
	{
		return;
	}

	user_name_ = read<std::string>();
	if (user_name_.size() < 4 || user_name_.size() > 13)
	{
		return;
	}

	std::string password = read<std::string>();
	if (password.size() < 4 || password.size() > 13)
	{
		return;
	}

	Poco::Data::Session &mysql_session = World::get_instance()->get_mysql_session();
	Poco::Data::Statement statement1(mysql_session);

	// username check
	statement1 << "SELECT id FROM users WHERE username = '" << user_name_ << "'", Poco::Data::Keywords::into(user_id_);
	if (statement1.execute() == 0)
	{
		// send a packet
		PacketCreator packet1;
		packet1.GetLoginFailed(5);
		send_packet(&packet1);
		return;
	}

	// password check
	Poco::Data::Statement statement2(mysql_session);
	statement2 << "SELECT id FROM users WHERE username = '" << user_name_ << "' AND password = '" << password << "'";
	if (statement2.execute() == 0)
	{
		// send a packet
		PacketCreator packet2;
		packet2.GetLoginFailed(4);
		send_packet(&packet2);
		return;
	}

	// get the user id
	Poco::Data::Statement statement3(mysql_session);
	statement3 << "SELECT id FROM users WHERE username = '" << user_name_ << "' AND password = '" << password << "'", Poco::Data::Keywords::into(user_id_);
	statement3.execute();

	// ban check
	if (world->is_account_banned(user_id_))
	{
		// send a packet
		PacketCreator packet3;
		packet3.GetLoginFailed(3);
		send_packet(&packet3);
		return;
	}

	// online check
	if (world->is_online_user(user_name_))
	{
		// send a packet
		PacketCreator packet4;
		packet4.GetLoginFailed(7);
		send_packet(&packet4);
		return;
	}

	world->add_online_user(user_name_);

	// get character slots
	Poco::Data::Statement statement4(mysql_session);
	statement4 << "SELECT character_slots FROM users WHERE username = '" << user_name_ << "'", Poco::Data::Keywords::into(character_slots_);
	statement4.execute();

	// get the character ids from the user account
	std::vector<int> ids;

	Poco::Data::Statement statement5(mysql_session);
	statement5 << "SELECT id FROM characters WHERE user_id = " << user_id_, Poco::Data::Keywords::into(ids);
	statement5.execute();

	// load characters
	for (int id : ids)
	{
		Character *character = new Character(id);
		Poco::Data::Statement statement6(mysql_session);
		statement6 << "SELECT name, level, job, str, dex, intt, luk, fame, gender, skin, face, hair FROM characters WHERE id = " << id;
		statement6.execute();

		Poco::Data::RecordSet rs1(statement6);
		character->set_name(rs1["name"]);
		character->set_level(rs1["level"]);
		character->set_job(rs1["job"]);
		character->set_str(rs1["str"]);
		character->set_dex(rs1["dex"]);
		character->set_int(rs1["intt"]);
		character->set_luk(rs1["luk"]);
		character->set_fame(rs1["fame"]);
		character->set_gender(rs1["gender"]);
		character->set_skin(rs1["skin"]);
		character->set_face(rs1["face"]);
		character->set_hair(rs1["hair"]);

		// load character equips
		Poco::Data::Statement statement7(mysql_session);
		statement7 << "SELECT equip_id, pos FROM equips WHERE player_id = " << id << " AND pos < 0";
		size_t rows = statement7.execute();

		Poco::Data::RecordSet recordset2(statement7);

		for (; rows > 0; --rows)
		{
			int equip_id = recordset2["equip_id"];
			int pos = recordset2["pos"];
			character->add_equip(equip_id, pos);

			recordset2.moveNext();
		}

		characters_[id] = character;
	}

	// send a packet
	PacketCreator packet5;
	packet5.GetAuthSuccessRequest(user_id_, user_name_);
	send_packet(&packet5);
}

void Player::handle_pin_operation()
{
	// send a packet
	PacketCreator packet1;
	packet1.LoginProcess();
	send_packet(&packet1);
}

void Player::handle_channel_selection()
{
	signed char world_id = read<signed char>();
	channel_id_ = read<unsigned char>();

	if (channel_id_ < kChannelsCount)
	{
		// temporary store the userid combined with the channel id
		// this workaround is needed, as there are no split servers
		// loginserver(s), worldserver(s) and channelservers
		// so when logging into the character, by temporary storing
		// this info, it will know which channel was choosen
		World *world = World::get_instance();
		world->store_channel_id_for_user_id(channel_id_, user_id_);

		// send a packet
		PacketCreator packet1;
		packet1.ShowCharacters(&characters_, character_slots_);
		send_packet(&packet1);
	}
}

void Player::handle_world_selection()
{
	// send a packet
	PacketCreator packet1;
	packet1.ShowChannels();
	send_packet(&packet1);
}

void Player::handle_world_list_request()
{
	// send a packet
	PacketCreator packet1;
	packet1.ShowWorld();
	send_packet(&packet1);

	// send a packet
	PacketCreator packet2;
	packet2.EndWorlds();
	send_packet(&packet2);
}

void Player::handle_relog_request()
{
	// send a packet
	PacketCreator packet1;
	packet1.RelogResponse();
	send_packet(&packet1);
}

void Player::handle_connect_game()
{
	int player_id = read<int>();

	// send a packet
	PacketCreator packet1;
	packet1.ConnectToChannel(player_id);
	send_packet(&packet1);
}

void Player::handle_character_creation_name_check()
{
	std::string name = read<std::string>();
	if (name.length() < 4 || name.length() > 12)
	{
		return;
	}
	bool name_used = World::get_instance()->is_name_used(name);
	
	// send a packet
	PacketCreator packet1;
	packet1.CheckName(name, name_used);
	send_packet(&packet1);
}

void Player::handle_create_character()
{
	std::string name = read<std::string>();
	if (name.length() < 4 || name.length() > 12)
	{
		return;
	}
	if (World::get_instance()->is_name_used(name))
	{
		return;
	}
	int face = read<int>();
	int hair = read<int>();
	int hair_color = read<int>();
	hair += hair_color;
	int skin_color = read<int>();

	int top_or_overall = read<int>();
	if (!ValidCharDataProvider::get_instance()->is_valid_item(top_or_overall, ValidItemType::Top))
	{
		return;
	}

	int bottom = read<int>();
	if (!ValidCharDataProvider::get_instance()->is_valid_item(bottom, ValidItemType::Bottom))
	{
		return;
	}

	int shoes = read<int>();
	if (!ValidCharDataProvider::get_instance()->is_valid_item(shoes, ValidItemType::Shoes))
	{
		return;
	}

	int weapon = read<int>();
	if (!ValidCharDataProvider::get_instance()->is_valid_item(weapon, ValidItemType::Weapon))
	{
		return;
	}

	signed char gender = read<signed char>();
	if (gender != kGenderConstantsMale && gender != kGenderConstantsFemale)
	{
		return;
	}

	int str = read<signed char>();
	int dex = read<signed char>();
	int intt = read<signed char>();
	int luk = read<signed char>();
	if (str + dex + intt + luk != 25 || str < 4 || dex < 4 || intt < 4 || luk < 4)
	{
		return;
	}

	short job_id = 0; // 0 is the beginner job
	unsigned char level = 1; // 1 is the standard starting level

	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = World::get_instance()->get_mysql_session();

	Poco::Data::Statement statement1(mysql_session);

	statement1 << "INSERT INTO characters(name, user_id, job, str, dex, intt, luk, face, hair, skin, gender) VALUES("
		<< "'" << name << "',"
		<< user_id_ << ","
		<< job_id << ","
		<< str << ","
		<< dex << ","
		<< intt << ","
		<< luk << ","
		<< face << ","
		<< hair << ","
		<< skin_color << ","
		<< static_cast<int>(gender) << ")";

	statement1.execute();

	int id = 0;
	Poco::Data::Statement statement2(mysql_session);
	statement2 << "SELECT LAST_INSERT_ID()", Poco::Data::Keywords::into(id), Poco::Data::Keywords::now;

	if (id == 0)
	{
		return;
	}

	// keymap

	Poco::Data::Statement statement3(mysql_session);

	statement3 << "INSERT INTO keymap(player_id, pos, type, action) VALUES(" << id << ", 18,4,0)" <<
		",(" << id << ", 65,6,106)" <<
		",(" << id << ", 2,4,10)" <<
		",(" << id << ", 23,4,1)" <<
		",(" << id << ", 3,4,12)" <<
		",(" << id << ", 4,4,13)" <<
		",(" << id << ", 5,4,18)" <<
		",(" << id << ", 6,4,24)" <<
		",(" << id << ", 16,4,8)" <<
		",(" << id << ", 17,4,5)" <<
		",(" << id << ", 19,4,4)" <<
		",(" << id << ", 25,4,19)" <<
		",(" << id << ", 26,4,14)" <<
		",(" << id << ", 27,4,15)" <<
		",(" << id << ", 31,4,2)" <<
		",(" << id << ", 34,4,17)" <<
		",(" << id << ", 35,4,11)" <<
		",(" << id << ", 37,4,3)" <<
		",(" << id << ", 38,4,20)" <<
		",(" << id << ", 40,4,16)" <<
		",(" << id << ", 43,4,9)" <<
		",(" << id << ", 44,5,50)" <<
		",(" << id << ", 45,5,51)" <<
		",(" << id << ", 46,4,6)" <<
		",(" << id << ", 50,4,7)" <<
		",(" << id << ", 56,5,53)" <<
		",(" << id << ", 59,6,100)" <<
		",(" << id << ", 60,6,101)" <<
		",(" << id << ", 61,6,102)" <<
		",(" << id << ", 62,6,103)" <<
		",(" << id << ", 63,6,104)" <<
		",(" << id << ", 64,6,105)" <<
		",(" << id << ", 57,5,54)" <<
		",(" << id << ", 48,4,22)" <<
		",(" << id << ", 29,5,52)" <<
		",(" << id << ", 7,4,21)" <<
		",(" << id << ", 24,4,25)" <<
		",(" << id << ", 33,4,26)" <<
		",(" << id << ", 41,4,23)";

	statement3.execute();

	// equips

	// top/overall
	Poco::Data::Statement statement4(mysql_session);
	statement4 << "INSERT INTO equips(equip_id, player_id, weapon_attack, weapon_def, slots, pos) VALUES(" << top_or_overall << "," << id << "," << "0, 3, 7," << -kCreateCharacterEquipSlotsTop << ")", Poco::Data::Keywords::now;

	// bottom
	Poco::Data::Statement statement5(mysql_session);
	statement5 << "INSERT INTO equips(equip_id, player_id, weapon_attack, weapon_def, slots, pos) VALUES(" << bottom << "," << id << "," << "0, 2, 7," << -kCreateCharacterEquipSlotsBottom << ")", Poco::Data::Keywords::now;

	// shoes
	Poco::Data::Statement statement6(mysql_session);
	statement6 << "INSERT INTO equips(equip_id, player_id, weapon_attack, weapon_def, slots, pos) VALUES(" << shoes << "," << id << "," << "0, 2, 5," << -kCreateCharacterEquipSlotsShoes << ")", Poco::Data::Keywords::now;

	// weapon
	Poco::Data::Statement statement7(mysql_session);
	statement7 << "INSERT INTO equips(equip_id, player_id, weapon_attack, weapon_def, slots, pos) VALUES(" << weapon << "," << id << "," << "15, 0, 7," << -kCreateCharacterEquipSlotsWeapon << ")", Poco::Data::Keywords::now;

	Character *character = new Character(id);

	character->set_name(name);
	character->set_level(level);
	character->set_job(job_id);
	character->set_str(str);
	character->set_dex(dex);
	character->set_int(intt);
	character->set_luk(luk);
	character->set_fame(0);
	character->set_gender(gender);
	character->set_skin(static_cast<signed char>(skin_color));
	character->set_face(face);
	character->set_hair(hair);

	character->add_equip(top_or_overall, -kCreateCharacterEquipSlotsTop);
	character->add_equip(bottom, -kCreateCharacterEquipSlotsBottom);
	character->add_equip(shoes, -kCreateCharacterEquipSlotsShoes);
	character->add_equip(weapon, -kCreateCharacterEquipSlotsWeapon);

	characters_[id] = character;
	World::get_instance()->add_name_id(name, id);

	// send a packet
	PacketCreator packet1;
	packet1.AddCharacter(character);
	send_packet(&packet1);
}

void Player::handle_delete_character()
{
	int date = read<int>();
	int player_id = read<int>();

	// remove character data from mysql

	World *world = World::get_instance();
	Poco::Data::Session &mysql_session = world->get_mysql_session();

	Poco::Data::Statement statement1(mysql_session);
	statement1 << "DELETE FROM buddy_lists WHERE player_id = " << player_id;
	statement1.execute();

	Poco::Data::Statement statement2(mysql_session);
	statement2 << "DELETE FROM characters WHERE id = " << player_id;
	statement2.execute();

	Poco::Data::Statement statement3(mysql_session);
	statement3 << "DELETE FROM equips WHERE player_id = " << player_id;
	statement3.execute();

	Poco::Data::Statement statement4(mysql_session);
	statement4 << "DELETE FROM items WHERE player_id = " << player_id;
	statement4.execute();

	Poco::Data::Statement statement5(mysql_session);
	statement5 << "DELETE FROM skills WHERE player_id = " << player_id;
	statement5.execute();

	Poco::Data::Statement statement6(mysql_session);
	statement6 << "DELETE FROM keymap WHERE player_id = " << player_id;
	statement6.execute();

	Poco::Data::Statement statement7(mysql_session);
	statement7 << "DELETE FROM pets WHERE player_id = " << player_id;
	statement7.execute();

	Poco::Data::Statement statement8(mysql_session);
	statement8 << "DELETE FROM quests WHERE player_id = " << player_id;
	statement8.execute();

	Poco::Data::Statement statement9(mysql_session);
	statement9 << "DELETE FROM merchant_storage_items WHERE player_id = " << player_id;
	statement9.execute();

	Poco::Data::Statement statement10(mysql_session);
	statement10 << "DELETE FROM merchant_storage_equips WHERE player_id = " << player_id;
	statement10.execute();

	// end of mysql code

	if (characters_.find(player_id) == characters_.end())
	{
		return;
	}

	Character *character = characters_[player_id];

	std::string player_name = character->get_name();
	world->remove_name(player_name, player_id);
	world->remove_fame(player_id);

	characters_.erase(player_id);
	delete character;

	// send a packet
	PacketCreator packet1;
	packet1.RemoveCharacter(player_id);
	send_packet(&packet1);
}
