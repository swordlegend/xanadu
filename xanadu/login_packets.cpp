//

#include "packetcreator.hpp"

#include "character.hpp"
#include "world.hpp"
#include "tools.hpp"
#include "send_packet_opcodes.hpp"
#include "constants.hpp"
#include "game_constants.hpp"

void PacketCreator::get_handshake(unsigned char *iv_recv, unsigned char *iv_send)
{
	write<short>(kPacketHandshakeServerLength + static_cast<short>(kGameMinorVersion.length()));
	write<short>(kGameVersion);
	write<std::string>(kGameMinorVersion);
	write<int>(*(int*)iv_recv);
	write<int>(*(int*)iv_send);
	write<signed char>(kGameLocale);
}

void PacketCreator::ConnectToChannel(int player_id)
{
	write<short>(send_headers::kSERVER_IP);
	write<short>(0);

	// ip adress
	World *world = World::get_instance();
	if (world->is_dedicated_server())
	{
		// the server ip
		write<unsigned char>(185);
		write<signed char>(92);
		write<unsigned char>(223);
		write<unsigned char>(189);
	}
	else
	{
		// 127.0.0.1 is localhost
		write<signed char>(127);
		write<signed char>(0);
		write<signed char>(0);
		write<signed char>(1);
	}

	write<unsigned short>(kServerPort);
	write<int>(player_id);
	write<int>(0);
	write<signed char>(0);
}

void PacketCreator::GetAuthSuccessRequest(int user_id, std::string account_name)
{
	write<short>(send_headers::kLoginStatus);
	write<short>(0);
	write<int>(0);
	write<int>(user_id);
	write<signed char>(kGenderConstantsMale); // gender byte, is also used as trigger for gender select or pin select
	write<short>(0);
	write<std::string>(account_name);
	write<short>(0);
	write<long long>(0);
	write<long long>(0);
	write<int>(8);
}

/*
* 3: ID deleted or blocked
* 4: Incorrect password
* 5: Not a registered id
* 7: Already logged in
*/
void PacketCreator::GetLoginFailed(signed char reason)
{
	write<short>(send_headers::kLoginStatus);
	write<short>(reason);
	write<int>(0);
}

void PacketCreator::LoginProcess()
{
	write<short>(send_headers::kPIN_OPERATION);
	write<signed char>(0); // pin entered
}

void PacketCreator::ShowWorld()
{
	World *world = World::get_instance();
	write<short>(send_headers::kSERVER_LIST);
	write<signed char>(world->get_id());
	write<std::string>(world->get_name());
	write<signed char>(kWorld1Flag);
	write<std::string>(kWorld1EventMessage);
	write<short>(100);
	write<short>(100);
	write<signed char>(0);

	// channel data
	write<signed char>(kChannelsCount);

	for (signed char channel_id = 0; channel_id < kChannelsCount; ++channel_id)
	{
		std::string channel_name = (world->get_name() + "-" + std::to_string(channel_id + 1));
		write<std::string>(channel_name);
		write<int>(0); // channel population
		write<signed char>(world->get_id());
		write<signed char>(channel_id);
		write<signed char>(0);
	}

	// login screen balloons (credits to Eric from RageZone, who is also moderator there right now)

	//constexpr short balloon_size = 1;
	write<short>(0); // balloon size

	/*for (int i = 0; i < balloon_size; ++i)
	{
		write<short>(236); // X position
		write<short>(122); // Y position
		write<std::string>("test"); // message
	}*/
}

void PacketCreator::EndWorlds()
{
	write<short>(send_headers::kSERVER_LIST);
	write<signed char>(-1);
}

/*
Possible values for status:
* 0 - Normal
* 1 - Highly populated (a 2-sentence message pops up at world select as a warning of high population)
* 2 - Full (a 2-sentence message pops up at world select and the client does not request channel loading)
*/

void PacketCreator::ShowChannels()
{
	write<short>(send_headers::kSERVER_STATUS);
	write<short>(0); // status
}

void PacketCreator::AddCharStats(Character *character)
{
	write<int>(character->get_id());
	write_string(character->get_name(), 13);
	write<signed char>(character->get_gender());
	write<signed char>(character->get_skin());
	write<int>(character->get_face());
	write<int>(character->get_hair());
	write<long long>(0); // pet unique id 1
	write<long long>(0); // pet unique id 2
	write<long long>(0); // pet unique id 3
	write<signed char>(character->get_level());
	write<short>(character->get_job());
	write<short>(character->get_str());
	write<short>(character->get_dex());
	write<short>(character->get_int());
	write<short>(character->get_luk());
	write<short>(600); // hp
	write<short>(600); // maxhp
	write<short>(300); // mp
	write<short>(300); // maxmp
	write<short>(0); // ap
	write<short>(0); // sp
	write<int>(0); // exp
	write<short>(character->get_fame());
	write<int>(0);
	write<int>(0); // map id
	write<signed char>(0); // map spawn point
	write<int>(0);
}

void PacketCreator::AddCharLook(Character *character, bool megaphone)
{
	write<signed char>(character->get_gender());
	write<signed char>(character->get_skin());
	write<int>(character->get_face());
	write<bool>(megaphone);
	write<int>(character->get_hair());

	std::unordered_map<signed char, int> visible_equips;
	std::unordered_map<signed char, int> invisible_equips;
	int cash_weapon_id = 0;

	auto equips = character->get_equips();
	for (auto it : *equips)
	{
		int item_id = it.first;
		signed char pos = -it.second;

		// cash weapon
		if (pos == 111)
		{
			cash_weapon_id = item_id;

			continue;
		}

		if (pos < 100)
		{
			if (visible_equips.find(pos) == visible_equips.end())
			{
				visible_equips[pos] = item_id;
			}
			else
			{
				invisible_equips[pos] = item_id;
			}
		}
		else
		{
			// cash equips
			pos -= 100;

			if (visible_equips.find(pos) != visible_equips.end())
			{
				invisible_equips[pos] = visible_equips[pos];
			}

			visible_equips[pos] = item_id;
		}
	}

	enum
	{
		kEndEquipInfo = -1
	};

	// visible equips info
	for (auto it : visible_equips)
	{
		write<signed char>(it.first);
		write<int>(it.second);
	}
	write<signed char>(kEndEquipInfo);

	// invisible equips info
	for (auto it : invisible_equips)
	{
		write<signed char>(it.first);
		write<int>(it.second);
	}
	write<signed char>(kEndEquipInfo);

	// cash weapon itemid
	write<int>(cash_weapon_id);
	// show pets
	write<int>(0); // pet 1 itemid
	write<int>(0); // pet 2 itemid
	write<int>(0); // pet 3 itemid
}

void PacketCreator::ShowCharacter(Character *character)
{
	AddCharStats(character);
	AddCharLook(character);
	write<signed char>(1);
	// rankings
	write<int>(0); // world rank
	write<int>(0); // world rank move
	write<int>(0); // job rank
	write<int>(0); // job rank move
}

void PacketCreator::ShowCharacters(std::unordered_map<int, Character *> *characters, int character_slots)
{
	write<short>(send_headers::kCHARACTER_LIST);
	write<signed char>(0);
	write<signed char>(static_cast<unsigned char>(characters->size()));

	for (auto &it : *characters)
	{
		Character *character = it.second;
		ShowCharacter(character);
	}

	write<int>(character_slots);
}

void PacketCreator::CheckName(std::string name, bool name_used)
{
	write<short>(send_headers::kCHECK_CHARACTER_NAME);
	write<std::string>(name);
	write<bool>(name_used);
}

void PacketCreator::AddCharacter(Character *character)
{
	write<short>(send_headers::kCREATE_NEW_CHARACTER);
	write<signed char>(0);
	ShowCharacter(character);
}

/*
state can be:
0 = ok
others?
12 = invalid birthday?
guildmaster state?
*/
void PacketCreator::RemoveCharacter(int characterid)
{
	write<short>(send_headers::kDELETE_CHARACTER);
	write<int>(characterid);
	write<signed char>(0); // state
}

void PacketCreator::RelogResponse()
{
	write<short>(send_headers::kRELOG_RESPONSE);
	write<signed char>(1);
}
