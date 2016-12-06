//

#include "drop.hpp"

#include "item.hpp"
#include "map.hpp"
#include "player.hpp"
#include "packetcreator.hpp"
#include "inventory.hpp"

// constructor

Drop::Drop(int id, signed char type, short position_x, short position_y, int mesos, int owner_player_id, std::shared_ptr<Item> item) :
id_(id),
item_(item),
type_(type),
position_x_(position_x),
position_y_(position_y),
mesos_(mesos),
owner_player_id_(owner_player_id),
time_(GetTickCount64())
{
}

bool Drop::loot_drop(Player *player, signed char pet_slot)
{
	if (is_mesos())
	{
		if (player->add_mesos(mesos_))
		{
			// packet
			PacketCreator packet1;
			packet1.GainMesos(mesos_);
			player->send_packet(&packet1);
		}
		else
		{
			return false;
		}
	}
	else
	{
		int item_id = item_->get_item_id();
		short amount = item_->get_amount();

		Inventory *inventory = player->get_inventory(item_->get_inventory_id());

		if (!inventory)
		{
			return false;
		}

		if (inventory->add_item_find_slot(item_))
		{
			// packet
			PacketCreator packet10;
			packet10.GainItem(item_id, amount);
			player->send_packet(&packet10);
		}
		else
		{
			// packet
			PacketCreator packet11;
			packet11.get_inventory_full();
			player->send_packet(&packet11);

			// packet
			PacketCreator packet12;
			packet12.CantGetAnymoreItems();
			player->send_packet(&packet12);

			return false;
		}
	}

	// packet
	PacketCreator packet13;
	packet13.RemoveDrop(id_, 2, player->get_id(), pet_slot);
	player->get_map()->send_packet(&packet13);

	return true;
}

bool Drop::is_mesos()
{
	return mesos_ > 0;
}

signed char Drop::get_type()
{
	return type_;
}

short Drop::get_position_x()
{
	return position_x_;
}

short Drop::get_position_y()
{
	return position_y_;
}

int Drop::get_mesos()
{
	return mesos_;
}

int Drop::get_owner_player_id()
{
	return owner_player_id_;
}

int Drop::get_id()
{
	return id_;
}

unsigned long long Drop::get_time()
{
	return time_;
}

std::shared_ptr<Item> Drop::get_item()
{
	return item_;
}
