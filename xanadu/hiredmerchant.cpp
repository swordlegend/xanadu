//

#include "hiredmerchant.hpp"

#include "item_data.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "map.hpp"
#include "packetcreator.hpp"
#include "hiredmerchant_item.hpp"
#include "player.hpp"
#include "world.hpp"
#include "channel.hpp"
#include "item_data_provider.hpp"
#include "constants.hpp"

// constructor

HiredMerchant::HiredMerchant(Player *player, int item_id, const std::string &description) :
open_(false),
pos_x_(player->get_position_x()),
pos_y_(player->get_position_y()),
id_(player->get_map()->get_merchant_object_id()),
item_id_(item_id),
owner_id_(player->get_id()),
mesos_(0),
owner_name_(player->get_name()),
description_(description)
{
}

void HiredMerchant::add_item(std::shared_ptr<HiredMerchantItem> item)
{
	items_[static_cast<int>(get_num_items())] = item;
}

void HiredMerchant::remove_item(short slot)
{
	for (int i = 0; i < get_num_items(); ++i)
	{
		if (i > slot)
		{
			items_[i - 1] = get_item(i);

			if (i == (get_num_items() - 1))
			{
				items_.erase(i);
			}
		}
		else if (!get_item(i + 1))
		{
			items_.erase(i);
		}
	}
}

std::shared_ptr<HiredMerchantItem> HiredMerchant::get_item(short slot)
{
	return ((items_.find(slot) != items_.end()) ? items_[slot] : 0);
}

std::unordered_map<int, std::shared_ptr<HiredMerchantItem>>* HiredMerchant::get_items()
{
	return &items_;
}

void HiredMerchant::return_item(short slot, Player *owner)
{
	std::shared_ptr<HiredMerchantItem> sellItem = get_item(slot);

	if (sellItem && sellItem->get_bundles() > 0)
	{
		std::shared_ptr<Item> item = sellItem->get_item();

		if (!item->is_star())
		{
			item->set_amount(sellItem->get_bundles() * sellItem->get_quantity());
		}

		slot = sellItem->get_slot();
		std::shared_ptr<Item> olditem = owner->get_inventory(item->get_inventory_id())->get_item_by_slot(static_cast<signed char>(slot));

		if (olditem)
		{
			ItemData *item_data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

			if (item->is_star()
				&& olditem->get_item_id() == item->get_item_id()
				&& olditem->get_amount() < item_data->max_per_slot)
			{
				if ((item->get_amount() + olditem->get_amount()) > item_data->max_per_slot)
				{
					item->set_amount(item->get_amount() - (item_data->max_per_slot - olditem->get_amount()));
					olditem->set_amount(item_data->max_per_slot);

					// packet
					PacketCreator packet7;
					packet7.NewItem(olditem, true);
					owner->send_packet(&packet7);
				}
				else
				{
					item->set_amount(item->get_amount() + olditem->get_amount());
					owner->get_inventory(item->get_inventory_id())->remove_item_by_slot(static_cast<signed char>(slot), 1);
					owner->give_item(item->get_item_id(), item->get_amount());

					// packet
					PacketCreator packet8;
					packet8.NewItem(item, true);
					owner->send_packet(&packet8);
				}
			}
		}
		else
		{
			owner->give_item(item->get_item_id(), item->get_amount());

			// packet
			PacketCreator packet9;
			packet9.NewItem(item, true);
			owner->send_packet(&packet9);
		}
	}
}

void HiredMerchant::buy_item(Player *player, short slot, short quantity)
{
	std::shared_ptr<HiredMerchantItem> shopItem = get_item(slot);
	std::shared_ptr<Item> item = shopItem->get_item();
	signed char inventory_id = item->get_inventory_id();

	if (shopItem->get_bundles() > 0)
	{
		int price = (shopItem->get_price() * quantity);

		if (player->get_mesos() >= price)
		{
			Inventory *inventory = player->get_inventory(inventory_id);
			if (!inventory)
			{
				return;
			}

			std::shared_ptr<Item> copy(new Item(*item));

			if (!inventory->add_item_find_slot(copy))
			{
				// packet
				PacketCreator packet3;
				packet3.PlayerShopError(5);
				player->send_packet(&packet3);

				return;
			}

			player->add_mesos(-price);
			mesos_ += price;
			shopItem->set_bundles(shopItem->get_bundles() - quantity);

			for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
			{
				Player *visitor = get_visitor(i);
				if (visitor)
				{
					// packet
					PacketCreator packet5;
					packet5.UpdateHiredMerchant(this);
					visitor->send_packet(&packet5);
				}
			}

			// packet
			PacketCreator packet8;
			packet8.EnableAction();
			player->send_packet(&packet8);
		}
		else
		{
			// packet
			PacketCreator packet10;
			packet10.PlayerShopError(2);
			player->send_packet(&packet10);
		}
	}
	else
	{
		// packet
		PacketCreator packet12;
		packet12.PlayerShopError(7);
		player->send_packet(&packet12);
	}
}

void HiredMerchant::room_chat(signed char slot, std::string &chat)
{
	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);
		if (visitor)
		{
			// packet
			PacketCreator packet1;
			packet1.HiredMerchantChat(slot, chat);
			visitor->send_packet(&packet1);
		}
	}
}

signed char HiredMerchant::get_empty_visitor_slot()
{
	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);

		if (!visitor)
		{
			return (i + 1);
		}
	}

	return -1;
}

signed char HiredMerchant::get_playerslot(int player_id)
{
	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);

		if (visitor)
		{
			if (visitor->get_id() == player_id)
			{
				return (i + 1);
			}
		}
	}

	return 0;
}

Player *HiredMerchant::get_visitor(signed char slot)
{
	return (visitors_.find(slot) != visitors_.end() ? visitors_[slot] : 0);
}

void HiredMerchant::remove_all_visitors()
{
	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);

		if (visitor)
		{
			// packet
			PacketCreator packet1;
			packet1.LeaveHiredMerchant(i + 1, 17);
			visitor->send_packet(&packet1);

			visitor->set_hired_merchant(nullptr);
		}
	}

	visitors_.clear();
}

void HiredMerchant::add_visitor(Player *player)
{
	signed char slot = get_empty_visitor_slot();

	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);

		if (visitor)
		{
			// packet
			PacketCreator packet1;
			packet1.HiredMerchantVisitorAdd(player, slot);
			visitor->send_packet(&packet1);
		}
	}

	visitors_[slot - 1] = player;
}

void HiredMerchant::remove_visitor(Player *player)
{
	signed char slot = get_playerslot(player->get_id());
	visitors_.erase(slot - 1);
	bool owner = is_owner(player->get_id());

	for (int i = 0; i < kHiredMerchantConstantsSlotsMax; ++i)
	{
		Player *visitor = get_visitor(i);

		if (visitor)
		{
			// packet
			PacketCreator packet1;
			packet1.HiredMerchantVisitorLeave(slot);
			visitor->send_packet(&packet1);
		}
	}
}

bool HiredMerchant::is_open()
{
	return open_;
}

void HiredMerchant::set_open(bool open)
{
	open_ = open;
}

bool HiredMerchant::is_owner(int id)
{
	return (owner_id_ == id);
}

short HiredMerchant::get_position_x()
{
	return pos_x_;
}

short HiredMerchant::get_position_y()
{
	return pos_y_;
}

int HiredMerchant::get_id()
{
	return id_;
}

int HiredMerchant::get_item_id()
{
	return item_id_;
}

int HiredMerchant::get_owner_id()
{
	return owner_id_;
}

int HiredMerchant::get_merchant_mesos()
{
	return mesos_;
}

size_t HiredMerchant::get_num_items()
{
	return items_.size();
}

std::string HiredMerchant::get_owner_name()
{
	return owner_name_;
}

std::string HiredMerchant::get_description()
{
	return description_;
}
