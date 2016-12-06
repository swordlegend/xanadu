//

#include "player.hpp"

#include "map.hpp"
#include "packetcreator.hpp"
#include "drop.hpp"
#include "drop_constants.hpp"

void Player::handle_player_drop_mesos()
{
	skip_bytes(4);
	int dropped_mesos = read<int>();

	if (dropped_mesos < 10)
	{
		return;
	}

	if (dropped_mesos > 50000)
	{
		return;
	}

	if (!add_mesos(-dropped_mesos))
	{
		return;
	}

	short player_position_y = position_y_;

	map_->find_foothold(position_x_, player_position_y);

	std::shared_ptr<Drop> drop(new Drop(map_->get_drop_object_id(), kDropPacketConstantsPickupTypesFreeForAll, position_x_, player_position_y, dropped_mesos, 0, nullptr));
	map_->add_drop(drop);

	// send the packet to display the drop on the map

	PacketCreator packet;
	packet.ShowDrop(kDropPacketConstantsAnimationTypesDropAnimation, drop, position_x_, position_y_);
	map_->send_packet(&packet);
}

void Player::handle_player_loot_drop()
{
	skip_bytes(9);
	int drop_object_id = read<int>();
	std::shared_ptr<Drop> drop = map_->get_drop(drop_object_id);

	if (drop)
	{
		map_->loot_drop(this, drop);
	}
}
