//

#include "player.hpp"

#include "world.hpp"
#include "packetcreator.hpp"

void Player::handle_faming()
{
	int target_player_id = read<int>();
	if (target_player_id == id_)
	{
		return;
	}

	World *world = World::get_instance();
	Player *target_player = world->GetPlayerById(target_player_id);
	if (!target_player)
	{
		return;
	}

	if (world->can_fame(id_))
	{
		signed char type = (read<signed char>() == 1 ? 1 : -1); // increase if type = 1, otherwise decrease
		target_player->set_fame(target_player->get_fame() + type);
		
		// send a packet
		PacketCreator packet1;
		packet1.SendFame(name_, type);
		target_player->send_packet(&packet1);
		
		// send a packet
		PacketCreator packet2;
		packet2.SendFamee(target_player->get_name(), type, target_player->get_fame());
		send_packet(&packet2);

		world->add_fame(id_);
	}
}
