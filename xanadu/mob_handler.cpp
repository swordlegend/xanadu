//

#include "player.hpp"

#include "packetcreator.hpp"
#include "mob.hpp"
#include "map.hpp"
#include "session.hpp"

void Player::handle_mob_movement()
{
	int monster_object_id = read<int>();

	Mob *mob = map_->get_mob(monster_object_id);
	if (!mob)
	{
		return;
	}

	short move_id = read<short>();
	bool use_skill = read<bool>();
	signed char skill = read<signed char>();
	int unk = read<int>();
	skip_bytes(5);

	short start_position_x = read<short>();
	short start_position_y = read<short>();

	mob->update_position(start_position_x, start_position_y);

	short mob_position_x = 0;
	short mob_position_y = 0;
	short mob_foothold = 0;
	short mob_stance = 0;

	signed char actions_amount = read<signed char>();

	for (signed char i = 0; i < actions_amount; ++i)
	{
		signed char action = read<signed char>();

		switch (action)
		{
		case 0: // Normal up/down/left/right movement
		case 5:
		case 17:
			mob_position_x = read<short>();
			mob_position_y = read<short>();
			skip_bytes(6);
			mob_stance = read<signed char>();
			mob_foothold = read<short>();
			break;
		case 15: // Jump down
			mob_position_x = read<short>();
			mob_position_y = read<short>();
			skip_bytes(8);
			mob_stance = read<signed char>();
			mob_foothold = read<short>();
			break;
		case 1: // Jumping
		case 2: // Jumping/knockback?
		case 6: // Flash Jump
		case 12: // Horntail knockback
		case 13: // Recoil Shot
		case 20:
			mob_position_x = read<short>();
			mob_position_y = read<short>();
			mob_stance = read<signed char>();
			mob_foothold = read<short>();
			break;
		case 3:
		case 4: // Teleport
		case 7: // Assaulter
		case 8: // Assassinate
		case 9: // Rush
		case 11: // Chair
			mob_position_x = read<short>();
			mob_position_y = read<short>();
			skip_bytes(2);
			mob_foothold = read<short>();
			mob_stance = read<signed char>();
			break;
		case 10: // Falling of some kind
			skip_bytes(1);
			break;
		case 14:
			skip_bytes(6);
			mob_stance = read<signed char>();
			mob_foothold = read<short>();
			break;
		case 16:
			skip_bytes(7);
			break;
		default:
			break;
		}
	}

	if (mob_position_x != 0 || mob_position_y != 0 || mob_foothold != 0)
	{
		mob->update_position(mob_position_x, mob_position_y);
	}

	signed char skill_id = 0;
	signed char skill_level = 0;

	// send a packet to the player which is used to make the client continue mob movement for the player

	PacketCreator packet1;
	packet1.MoveMobResponse(mob, move_id, use_skill, skill_id, skill_level);
	send_packet(&packet1);

	// only send the packet if there are atleast 2 players in the map

	auto map_players = map_->get_players();
	if (map_players->size() >= 2)
	{
		// send a packet to all players in the map except the player to show the movement of the mob
		// exclude some bytes from being sent as that data is not used for the packet

		const int excluded_bytes = (21 + 2);
		PacketCreator packet2;
		packet2.MoveMob(monster_object_id, use_skill, skill, unk, start_position_x, start_position_y, session_->get_receive_buffer() + excluded_bytes, recv_length_ - excluded_bytes);
		map_->send_packet(&packet2, this);
	}
}
