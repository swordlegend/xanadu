//

#include "player.hpp"

#include "packetcreator.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "map.hpp"
#include "item.hpp"
#include "mob_data_provider.hpp"
#include "shop_data_provider.hpp"
#include "inventory.hpp"
#include "skill_data_provider.hpp"
#include "skill_data.hpp"
#include "job_constants.hpp"
#include "tools.hpp"
#include "constants.hpp"

void Player::handle_chat_command()
{
	signed char type = read<signed char>();
	std::string name = read<std::string>();

	Player *receiver_player = World::get_instance()->GetPlayerByName(name);

	switch (type)
	{
	case 5: // find
	{
		if (!receiver_player || receiver_player->get_is_gm() && !get_is_gm())
		{
			// packet
			PacketCreator packet10;
			packet10.FindPlayerReply(name, false);
			send_packet(&packet10);

			return;
		}
		if (receiver_player->get_channel_id() == channel_id_)
		{
			if (receiver_player->get_is_in_cash_shop())
			{
				// packet
				PacketCreator packet11;
				packet11.FindPlayerCashShop(receiver_player->get_name());
				send_packet(&packet11);
			}
			else
			{
				// packet
				PacketCreator packet12;
				packet12.FindPlayerMap(receiver_player->get_name(), receiver_player->get_map()->get_id());
				send_packet(&packet12);
			}
		}
		else
		{
			// packet
			PacketCreator packet13;
			packet13.FindPlayerChannel(receiver_player->get_name(), receiver_player->get_channel_id());
			send_packet(&packet13);
		}
		break;
	}
	case 6: // whisper
	{
		if (receiver_player)
		{
			std::string message = read<std::string>();

			if (message.empty())
			{
				return;
			}
			if (message.length() > 70)
			{
				return;
			}

			// send a packet
			PacketCreator packet14;
			packet14.WhisperPlayer(this, message);
			receiver_player->send_packet(&packet14);

			// send a packet
			PacketCreator packet15;
			packet15.FindPlayerReply(receiver_player->get_name(), true);
			send_packet(&packet15);
		}
		else
		{
			// packet
			PacketCreator packet16;
			packet16.FindPlayerReply(name, false);
			send_packet(&packet16);
		}
		break;
	}
	}
}

void Player::handle_use_chat()
{
	World *world = World::get_instance(); // this pointer is often used in this function so declare it right away

	std::string message = read<std::string>();

	if (message.empty())
	{
		return;
	}

	if (message.length() > 70)
	{
		return;
	}

	bool bubble_only = read<bool>(); // Skill macros only display chat bubble

	if (get_is_gm() && message[0] == '!')
	{
		// GM Commands

		std::string command = message.substr(1, message.find(" ") - 1);

		if (command == "warp")
		{
			int map_id = stoi(message.substr(message.find(" ") + 1));
			set_map(map_id);
		}

		else if (command == "warpto")
		{
			Player *target_player = world->GetPlayerByName(message.substr(message.find(" ") + 1));
			if (target_player)
			{
				change_map(target_player->get_map());
			}
		}

		else if (command == "warphere")
		{
			Player *target_player = world->GetPlayerByName(message.substr(message.find(" ") + 1));
			if (target_player)
			{
				target_player->change_map(map_);
			}
		}

		else if (command == "pos")
		{
			// packet
			PacketCreator packet51;
			packet51.ShowMessage("x: " + std::to_string(position_x_) + " y: " + std::to_string(position_y_) + " foothold: " + std::to_string(foothold_) + " map: " + std::to_string(map_->get_id()), 5);
			send_packet(&packet51);
		}

		// for development purposes only
		else if (command == "posa")
		{
			// door skill test
			PacketCreator packet2;
			packet2.SpawnDoor(1, true, position_x_, position_y_);
			send_packet(&packet2);
		}

		// for development purposes only
		else if (command == "posb")
		{
			// packet
			PacketCreator packet52;
			packet52.ShowMessage("x: " + std::to_string(position_x_) + " y: " + std::to_string(position_y_) + " foothold: " + std::to_string(foothold_) + " map: " + std::to_string(map_->get_id()), 5);
			send_packet(&packet52);

			// door skill test
			PacketCreator packet2;
			packet2.RemoveDoor(1, true);
			send_packet(&packet2);
		}

		// for development purposes only
		else if (command == "posc")
		{
			// packet
			PacketCreator packet53;
			packet53.ShowMessage("x: " + std::to_string(position_x_) + " y: " + std::to_string(position_y_) + " foothold: " + std::to_string(foothold_) + " map: " + std::to_string(map_->get_id()), 5);
			send_packet(&packet53);

			// poison mist skill test
			PacketCreator packet2;
			int object_id = 1;
			int skill_id = 2111003;
			signed char skill_level = 10;
			int mist_position_width = 300;
			int mist_position_height = 300;
			packet2.SpawnMist(object_id, id_, skill_id, position_x_, position_y_, mist_position_width, mist_position_height, skill_level);
			send_packet(&packet2);
		}

		// for development purposes only
		else if (command == "posd")
		{
			// packet
			PacketCreator packet54;
			packet54.ShowMessage("x: " + std::to_string(position_x_) + " y: " + std::to_string(position_y_) + " foothold: " + std::to_string(foothold_) + " map: " + std::to_string(map_->get_id()), 5);
			send_packet(&packet54);

			// poison mist skill test
			PacketCreator packet2;
			int object_id = 1;
			packet2.RemoveMist(object_id);
			send_packet(&packet2);
		}

		// for development purposes only
		else if (command == "pose")
		{
			// packet
			PacketCreator packet5;
			packet5.obtain_cp();
			send_packet(&packet5);
		}

		// for development purposes only
		else if (command == "posf")
		{
			// packet
			PacketCreator packet6;
			packet6.obtain_party_cp(0);
			send_packet(&packet6);
		}

		// for development purposes only
		else if (command == "posg")
		{
			// packet
			PacketCreator packet7;
			packet7.carnival_pq_message(5);
			send_packet(&packet7);
		}

		// for development purposes only
		else if (command == "posh")
		{
			// packet
			PacketCreator packet8;
			packet8.carnival_pq_summon(0, 2, name_);
			send_packet(&packet8);
		}

		else if (command == "maxskills")
		{
			// max skills for 4th jobs

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
						skill.level_ = 20;
						skill.master_level_ = 20;
						skills_[skill_id] = skill;
					}
				}
				// send a packet
				PacketCreator packet1;
				packet1.UpdateSkills(this);
				send_packet(&packet1);
			}
			}
		}

		else if (command == "gmshop")
		{
			int shopid = (10000000 - stoi(message.substr(message.find(" ") + 1)));
			ShopData *gmshop = ShopDataProvider::get_instance()->get_shop_data(shopid);
			if (gmshop)
			{
				shop_ = gmshop;

				// packet
				PacketCreator packet16;
				packet16.ShowNpcShop(shop_);
				send_packet(&packet16);
			}
		}

		else if (command == "item")
		{
			size_t p = message.find(" ");

			if (p == std::string::npos)
			{
				return;
			}

			size_t ap = message.substr(p + 1).find(" ");
			int item_id = 0;
			int amount = 1;

			if (ap == std::string::npos)
			{
				item_id = stoi(message.substr(p));
			}
			else
			{
				item_id = stoi(message.substr(p + 1, ap));
				amount = stoi(message.substr(ap + p + 2));
			}

			give_item(item_id, amount);
		}

		else if (command == "mesos")
		{
			mesos_ = stoi(message.substr(message.find(" ") + 1));

			// packet
			PacketCreator packet55;
			packet55.UpdateStatInt(kCharacterStatsMesos, mesos_);
			send_packet(&packet55);
		}

		else if (command == "level")
		{
			set_exp(0);
			set_level(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "levelup")
		{
			level_up();
		}

		else if (command == "job")
		{
			set_job(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "sp")
		{
			set_sp(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "ap")
		{
			set_ap(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "str")
		{
			set_str(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "dex")
		{
			set_dex(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "int")
		{
			set_int(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "luk")
		{
			set_luk(stoi(message.substr(message.find(" ") + 1)));
		}

		else if (command == "hp")
		{
			short hp = stoi(message.substr(message.find(" ") + 1));
			set_max_hp(hp);
			set_hp(hp);
		}

		else if (command == "mp")
		{
			short mp = stoi(message.substr(message.find(" ") + 1));
			set_max_mp(mp);
			set_mp(mp);
		}

		else if (command == "heal")
		{
			set_hp(get_max_hp());
			set_mp(get_max_mp());
		}

		else if (command == "spawn")
		{
			size_t p = message.find(" ");
			if (p == std::string::npos)
			{
				return;
			}
			size_t ap = message.substr(p + 1).find(" ");
			int mob_id = 0;
			int amount = 1;

			if (ap == std::string::npos)
			{
				mob_id = stoi(message.substr(p));
			}
			else
			{
				mob_id = stoi(message.substr(p + 1, ap));
				amount = stoi(message.substr(ap + p + 2));
			}

			if (!MobDataProvider::get_instance()->get_data_by_id(mob_id))
			{
				return;
			}
			if (amount > 100)
			{
				amount = 100;
			}
			if (amount < 1)
			{
				amount = 1;
			}
			for (int i = 0; i < amount; ++i)
			{
				map_->spawn_monster(mob_id, position_x_, position_y_, 0, mob_constants::kSpawnTypesNewSpawn);
			}
		}

		else if (command == "header")
		{
			std::string header_message("");
			if (message.find(" ") != -1)
			{
				header_message = message.substr(message.find(" "));
			}
			world->set_header_message(header_message);

			// packet
			PacketCreator packet57;
			packet57.ShowMessage(header_message, 4);
			world->send_packet(&packet57);
		}

		else if (command == "say")
		{
			std::string notice = message.substr(message.find(" ") + 1);

			// packet
			PacketCreator packet58;
			packet58.ShowMessage("[" + get_name() + "] " + notice, 6);
			world->send_packet(&packet58);
		}

		else if (command == "notice")
		{
			std::string notice = message.substr(message.find(" ") + 1);

			// packet
			PacketCreator packet59;
			packet59.ShowMessage(notice, 0);
			world->send_packet(&packet59);
		}

		else if (command == "notice2")
		{
			std::string notice = message.substr(message.find(" ") + 1);

			// packet
			PacketCreator packet60;
			packet60.ShowMessage(notice, 1);
			world->send_packet(&packet60);
		}

		else if (command == "hide")
		{
			hide();
		}

		else if (command == "unhide")
		{
			unhide();
		}

		else if (command == "meeting")
		{
			set_map(180000000);
		}
		else if (command == "online")
		{
			auto players = world->get_players();

			for (auto it : *players)
			{
				Player *player = it.second;

				if (player)
				{
					std::string name = player->get_name();

					// packet
					PacketCreator packet90;
					packet90.ShowMessage(name, 5);
					send_packet(&packet90);
				}
			}

			int online_amount = static_cast<int>(players->size());

			// packet
			PacketCreator packet91;
			packet91.ShowMessage("There are " + std::to_string(online_amount) + " players online.", 5);
			send_packet(&packet91);
		}

		else if (command == "shutdown")
		{
			world->ShutdownServer();
		}

		else if (command == "dc")
		{
			Player *target = world->GetPlayerByName(message.substr(message.find(" ") + 1));
			if (target)
			{
				target->disconnect();
			}
		}

		else if (command == "ban")
		{
			std::string player_name = message.substr(message.find(" ") + 1);
			world->ban_account(player_name);

			// disconnect the target player
			Player *target = world->GetPlayerByName(player_name);
			if (target)
			{
				target->disconnect();
			}
		}

		else if (command == "unban")
		{
			std::string player_name = message.substr(message.find(" ") + 1);
			world->unban_account(player_name);
		}

		else if (command == "donationpoints")
		{
			size_t p = message.find(" ");
			if (p == std::string::npos)
			{
				return;
			}
			size_t ap = message.substr(p + 1).find(" ");
			if (ap == std::string::npos)
			{
				return;
			}
			int user_id;
			int point_amount = stoi(message.substr(p + 1, ap));
			std::string user_name = message.substr(ap + p + 2);

			// username check
			Poco::Data::Session &mysql_session = World::get_instance()->get_mysql_session();
			Poco::Data::Statement statement1(mysql_session);
			statement1 << "SELECT id FROM users WHERE username = '" << user_name << "'", Poco::Data::Keywords::into(user_id);
			if (statement1.execute() == 0)
			{
				return;
			}

			std::vector<int> ids;
			Poco::Data::Statement statement2(mysql_session);
			statement2 << "SELECT id FROM characters WHERE user_id = " << user_id, Poco::Data::Keywords::into(ids);
			statement2.execute();

			for (int id : ids)
			{
				Player *target_player = World::get_instance()->GetPlayerById(id);
				if (target_player)
				{
					target_player->add_donation_points(point_amount);
					
					// packet
					PacketCreator packet93;
					packet93.ShowMessage("You have just gained " + std::to_string(point_amount) + " donation points. Visit Donator Npc Mos in FM.", 1);
					target_player->send_packet(&packet93);
					
					return;
				}
			}

			// update donationpoints

			Poco::Data::Statement statement3(mysql_session);
			statement3 << "UPDATE users SET donation_points = donation_points + " << point_amount << " WHERE username = '" << user_name << "'";
			statement3.execute();
		}
	}

	else if (message.substr(0, 1) == "@")
	{
		std::string command = message.substr(1, message.find(" ") - 1);

		if (command == "gm")
		{
			unsigned long long ticks = GetTickCount64();

			// "cooldown" 3 minutes

			if (ticks < (last_gm_call_ticks_ + (3 * 60 * 1000)))
			{
				// packet
				PacketCreator packet95;
				packet95.ShowMessage("Contacting the staff has a time limit of 3 minutes.", 1);
				send_packet(&packet95);

				return;
			}

			last_gm_call_ticks_ = ticks;

			std::string text = message.substr(message.find(" ") + 1);

			auto players = world->get_players();

			for (auto &it : *players)
			{
				Player *player = it.second;
				if (player->get_is_gm())
				{
					// packet
					PacketCreator packet96;
					packet96.ShowMessage("[HELP CALL] player: " + name_ + " message: " + text, 6);
					player->send_packet(&packet96);
				}
			}

			// packet
			PacketCreator packet97;
			packet97.ShowMessage("A staff member will get to you as soon as possible.", 0);
			send_packet(&packet97);
		}

		else if (command == "pq")
		{
			set_map(10000);
		}

		else if (command == "check")
		{
			// packet
			PacketCreator packet98;
			packet98.ShowMessage("NX Cash: " + std::to_string(nx_cash_credit_), 5);
			send_packet(&packet98);

			// packet
			PacketCreator packet99;
			packet99.ShowMessage("Donation Points: " + std::to_string(donation_points_), 5);
			send_packet(&packet99);
		}

		else if (command == "str")
		{
			short add_str = stoi(message.substr(message.find(" ") + 1));
			if (add_str < 1)
			{
				return;
			}
			if (ap_ >= add_str)
			{
				set_str(str_ + add_str);
				set_ap(ap_ - add_str);
			}
		}

		else if (command == "dex")
		{
			short add_dex = stoi(message.substr(message.find(" ") + 1));
			if (add_dex < 1)
			{
				return;
			}
			if (ap_ >= add_dex)
			{
				set_dex(dex_ + add_dex);
				set_ap(ap_ - add_dex);
			}
		}

		else if (command == "int")
		{
			short add_int = stoi(message.substr(message.find(" ") + 1));
			if (add_int < 1)
			{
				return;
			}
			if (ap_ >= add_int)
			{
				set_int(int_ + add_int);
				set_ap(ap_ - add_int);
			}
		}

		else if (command == "luk")
		{
			short add_luk = stoi(message.substr(message.find(" ") + 1));
			if (add_luk < 1)
			{
				return;
			}
			if (ap_ >= add_luk)
			{
				set_luk(luk_ + add_luk);
				set_ap(ap_ - add_luk);
			}
		}

	}
	else
	{
		// send a packet
		PacketCreator packet;
		packet.ShowChatMessage(this, message, bubble_only);
		map_->send_packet(&packet);
	}
}

void Player::handle_use_group_chat()
{
	std::unordered_map<int, Player *> players;
	World *world = World::get_instance();

	signed char type = read<signed char>();
	signed char count = read<signed char>();

	for (signed char i = 0; i < count; ++i)
	{
		int player_id = read<int>();
		Player *target_player = world->GetPlayerById(player_id);

		if (target_player)
		{
			players[player_id] = target_player;
		}
	}

	const std::string &message = read<std::string>();

	if (message.empty())
	{
		return;
	}

	if (message.length() > 70)
	{
		return;
	}

	for (auto it : players)
	{
		Player *target_player = it.second;

		// packet
		PacketCreator packet1;
		packet1.ShowSpecialChat(type, name_, message);
		target_player->send_packet(&packet1);
	}
}
