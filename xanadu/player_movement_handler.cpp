//

#include "player.hpp"

#include "map.hpp"
#include "session.hpp"
#include "packetcreator.hpp"

void Player::handle_player_movement()
{
	skip_bytes(1); // portal count
	skip_bytes(4); // unknown int, probably used in the packet

	// only send the packet if there are atleast 2 players in the map

	auto map_players = map_->get_players();
	if (map_players->size() >= 2)
	{
		// send a packet to all players in the map except the player to show the mvoement of the player
		// exclude some bytes from being sent as that data is not used for the packet

		constexpr int excluded_bytes = (1 + 2);
		int size = (recv_length_ - excluded_bytes);

		if (size < 1)
		{
			return;
		}

		PacketCreator packet;
		packet.ShowPlayerMovement(id_, session_->get_receive_buffer() + excluded_bytes, size);
		map_->send_packet(&packet, this);
	}

	signed char actions_amount = read<signed char>();

	for (signed char i = 0; i < actions_amount; ++i)
	{
		signed char action = read<signed char>();

		switch (action)
		{
		case 0: // Normal up/down/left/right movement
		case 5:
		case 17:
			position_x_ = read<short>();
			position_y_ = read<short>();
			skip_bytes(6);
			stance_ = read<signed char>();
			foothold_ = read<short>();
			break;
		case 15: // Jump down
			position_x_ = read<short>();
			position_y_ = read<short>();
			skip_bytes(8);
			stance_ = read<signed char>();
			foothold_ = read<short>();
			break;
		case 1: // Jumping
		case 2: // Jumping/knockback?
		case 6: // Flash Jump
		case 12: // Horntail knockback
		case 13: // Recoil Shot
		case 20:
			position_x_ = read<short>();
			position_y_ = read<short>();
			stance_ = read<signed char>();
			foothold_ = read<short>();
			break;
		case 3:
		case 4: // Teleport
		case 7: // Assaulter
		case 8: // Assassinate
		case 9: // Rush
		case 11: // Chair
			position_x_ = read<short>();
			position_y_ = read<short>();
			skip_bytes(2);
			foothold_ = read<short>();
			stance_ = read<signed char>();
			break;
		case 10: // Falling of some kind
			skip_bytes(1);
			break;
		case 14:
			skip_bytes(6);
			stance_ = read<signed char>();
			foothold_ = read<short>();
			break;
		case 16:
			skip_bytes(7);
			break;
		default:
			break;
		}
	}
}
