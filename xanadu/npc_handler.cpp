//

#include "player.hpp"

#include "packetcreator.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "guild.hpp"
#include "map.hpp"
#include "mapnpc.hpp"
#include "playernpc.hpp"
#include "item.hpp"
#include "inventory.hpp"
#include "shop_data.hpp"
#include "shop_item_data.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "map_data_provider.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "mob.hpp"
#include "map_data.hpp"
#include "constants.hpp"

void Player::handle_request_npc()
{
	int map_object_id = read<int>();

	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(map_->get_id());
	Npc *map_object = data->get_npc(map_object_id);
	if (!map_object)
	{
		return;
	}

	int npc_id = map_object->get_npc_id();
	if (shop_ || (npc_->id_ == npc_id && npc_->state_ == 0))
	{
		return;
	}

	npc_->id_ = npc_id;
	npc_->set_selected(-1);
	npc_->set_state(0);

	send_npc_final();
}

void Player::handle_npc_chat()
{
	signed char type = read<signed char>();
	signed char what = read<signed char>();

	switch (type)
	{
	case 0: // Normal
		npc_->set_state(1000);
		return;
		break;
	case 1: // Yes/No
		npc_->set_state(npc_->state_ + 1);
		switch (what)
		{
		case 0:
			npc_->set_selected(0);
			break;
		case 1:
			npc_->set_selected(1);
			break;
		default:
			npc_->set_state(1000);
			return;
		}
		break;
	case 4: // Simple
		npc_->set_state(npc_->state_ + 1);
		if (what == 0)
		{
			npc_->set_state(1000);
			return;
		}
		else
		{
			npc_->set_selected(read<signed char>());
		}
		break;
	case 7: // Style
		npc_->set_state(npc_->state_ + 1);
		if (what == 1)
		{
			npc_->set_selected(read<unsigned char>());
		}
		else
		{
			npc_->set_state(1000);
			return;
		}
		break;
	default:
		npc_->set_state(1000);
		return;
	}

	send_npc_final();
}

void Player::handle_npc_shop()
{
	if (!shop_)
	{
		return;
	}
	npc_->set_state(1000);
	signed char action = read<signed char>();
	switch (action)
	{
	case 0: // Buy
		{
			skip_bytes(2);
			int item_id = read<int>();
			ShopItemData *item = shop_->get_shop_item(item_id);
			if (!item)
			{
				return;
			}
			ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->id);
			if (!data)
			{
				return;
			}
			short amount = read<short>();
			if (amount < 1)
			{
				return;
			}
			short max_per_slot = data->max_per_slot;

			if (amount > max_per_slot)
			{
				return;
			}
			int price = (amount * item->price);
			if (tools::is_star(item_id))
			{
				amount = max_per_slot;
				price = item->price;
			}
			if (!add_mesos(-price))
			{
				return;
			}
			if (give_item(item_id, amount))
			{
				// packet
				PacketCreator packet26;
				packet26.Bought(0);
				send_packet(&packet26);
			}
			else
			{
				add_mesos(price);
				
				// packet
				PacketCreator packet27;
				packet27.Bought(3);
				send_packet(&packet27);
			}
			break;
		}
	case 1: // Sell
		{
			short slot = read<short>();
			int itemId = read<int>();
			signed char inventory_id = tools::get_inventory_id_from_item_id(itemId);
			Inventory *inventory = get_inventory(inventory_id);
			if (!inventory)
			{
				return;
			}
			std::shared_ptr<Item> item = inventory->get_item_by_slot(static_cast<signed char>(slot));
			if (!item)
			{
				return;
			}
			ItemData *data = ItemDataProvider::get_instance()->get_item_data(itemId);
			if (!data)
			{
				return;
			}
			if (tools::is_star(itemId))
			{
				add_mesos(item->get_amount());
			}
			else
			{
				add_mesos(item->get_amount() * data->price);
			}
			inventory->remove_item(static_cast<signed char>(slot));

			// packet
			PacketCreator packet28;
			packet28.Bought(0);
			send_packet(&packet28);

			break;
		}
	case 2: // Recharge
		{
			short slot = read<short>();
			std::shared_ptr<Item> item = get_inventory(kInventoryConstantsTypesUse)->get_item_by_slot(static_cast<signed char>(slot));
			if (!item)
			{
				return;
			}

			if (!item->is_star())
			{
				return;
			}

			ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());
			if (!data)
			{
				return;
			}
			item->set_amount(data->max_per_slot);

			// packet
			PacketCreator packet29;
			packet29.UpdateSlot(item);
			send_packet(&packet29);

			// packet
			PacketCreator packet30;
			packet30.Bought(0);
			send_packet(&packet30);

			break;
		}
	case 3: // Leave
		{
			shop_ = nullptr;
			break;
		}
	}
}
