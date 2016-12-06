//

#include "inventory.hpp"

#include "item.hpp"
#include "packetcreator.hpp"
#include "player.hpp"
#include "item_data_provider.hpp"
#include "item_data.hpp"
#include "equip_data_provider.hpp"
#include "pet_data_provider.hpp"
#include "constants.hpp"

// constructor

Inventory::Inventory(Player* player, signed char id, signed char slots) :
id_(id),
slots_(slots),
player_(player)
{
}

bool Inventory::add_item_find_slot(std::shared_ptr<Item> item)
{
	if (!item)
	{
		return false;
	}

	ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

	if (!data)
	{
		return false;
	}

	//if (data->get_is_only_one() && get_item_amount(item->get_item_id()) > 0)
	//{
	//	return false;
	//}

	short max_per_slot = data->max_per_slot;

	for (signed char i = 1; i <= slots_; ++i)
	{
		if (items_.find(i) == items_.end() || !items_[i])
		{
			item->set_slot(i);
			items_[i] = item;

			// packet
			PacketCreator packet3;
			packet3.NewItem(item, true);
			player_->send_packet(&packet3);

			return true;
		}
		else
		{
			if (items_[i]->get_item_id() == item->get_item_id()
				&& !item->is_star()
				&& items_[i]->get_amount() < max_per_slot)
			{
				if (item->get_amount() + items_[i]->get_amount() <= max_per_slot)
				{
					items_[i]->set_amount(items_[i]->get_amount() + item->get_amount());

					// packet
					PacketCreator packet8;
					packet8.UpdateSlot(items_[i]);
					player_->send_packet(&packet8);

					return true;
				}
				else
				{
					short change = (max_per_slot - items_[i]->get_amount());
					item->set_amount(item->get_amount() - change);
					items_[i]->set_amount(max_per_slot);

					// packet
					PacketCreator packet10;
					packet10.UpdateSlot(items_[i]);
					player_->send_packet(&packet10);
				}
			}
		}
	}

	return false;
}

bool Inventory::add_item_no_find_slot(std::shared_ptr<Item> item, bool send)
{
	if (!item)
	{
		return false;
	}

	ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

	if (!data)
	{
		return false;
	}

	// TODO: check for equip isonlyone
	//if (data->get_is_only_one() && get_item_amount(item->get_item_id()) > 0)
	//{
	//	return false;
	//}

	short max_per_slot = data->max_per_slot;

	if (items_.find(item->get_slot()) != items_.end()
		&& items_[item->get_slot()]->get_item_id() == item->get_item_id())
	{
		Item *temp = items_[item->get_slot()].get();
		item->set_amount(temp->get_amount() + item->get_amount());

		if (item->get_amount() > max_per_slot)
		{
			item->set_amount(max_per_slot);
		}

		if (send)
		{
			// packet
			PacketCreator packet14;
			packet14.UpdateSlot(item);
			player_->send_packet(&packet14);
		}
	}
	else
	{
		if (send)
		{
			// packet
			PacketCreator packet20;
			packet20.NewItem(item, false);
			player_->send_packet(&packet20);
		}
	}

	items_[item->get_slot()] = item;

	return true;
}

bool Inventory::give_item(int item_id, int amount)
{
	if (EquipDataProvider::get_instance()->get_item_data(item_id))
	{
		std::shared_ptr<Item> equip(new Item(item_id, false));
		if (!add_item_find_slot(equip))
		{
			return false;
		}
	}
	else if (PetDataProvider::get_instance()->get_data_by_id(item_id))
	{
		std::shared_ptr<Item> pet(new Item(item_id));
		if (!add_item_find_slot(pet))
		{
			return false;
		}
	}
	else if (ItemDataProvider::get_instance()->get_item_data(item_id))
	{
		ItemData *data = ItemDataProvider::get_instance()->get_item_data(item_id);
		short max = data->max_per_slot;

		while (amount > 0)
		{
			short temp_max = max;
			if (amount % max != 0)
			{
				temp_max = (amount % max);
			}
			amount -= temp_max;

			std::shared_ptr<Item> item(new Item(item_id));
			item->set_amount(temp_max);

			if (!add_item_find_slot(item))
			{
				return false;
			}
		}
	}
	return true;
}

std::shared_ptr<Item> Inventory::give_item_special(bool &success, int item_id)
{
	std::shared_ptr<Item> equip(new Item(item_id, false));

	if (!add_item_find_slot(equip))
	{
		success = false;

		return equip;
	}

	success = true;

	return equip;
}

void Inventory::remove_item(signed char slot, bool drop, bool send)
{
	std::shared_ptr<Item> item = get_item_by_slot(slot);
	if (!item)
	{
		return;
	}

	if (send)
	{
		// packet
		PacketCreator packet1;
		packet1.remove_item(id_, slot, drop);
		player_->send_packet(&packet1);
	}

	items_.erase(slot);
}

std::shared_ptr<Item> Inventory::get_item_by_slot(signed char slot)
{
	if (items_.find(slot) != items_.end())
	{
		return items_[slot];
	}
	return std::shared_ptr<Item>(nullptr);
}

short Inventory::get_item_amount(int item_id)
{
	short amount = 0;
	for (auto it : items_)
	{
		std::shared_ptr<Item> item = it.second;
		if (item->get_item_id() == item_id)
		{
			amount += item->get_amount();
		}
	}
	return amount;
}

void Inventory::remove_item(int item_id, short amount)
{
	std::vector<std::shared_ptr<Item>> todel;

	for (auto it : items_)
	{
		if (amount == 0)
		{
			break;
		}

		std::shared_ptr<Item> item = it.second;

		if (!item)
		{
			continue;
		}

		if (item->get_item_id() == item_id)
		{
			if (item->get_amount() <= amount)
			{
				amount -= item->get_amount();
				todel.push_back(item);
			}
			else
			{
				item->set_amount(item->get_amount() - amount);

				// packet
				PacketCreator packet1;
				packet1.UpdateSlot(item);
				player_->send_packet(&packet1);

				break;
			}
		}
	}

	for (size_t i = 0; i < todel.size(); ++i)
	{
		remove_item(todel[i]->get_slot(), true, true);
	}
}

void Inventory::remove_item_by_slot(signed char slot, short amount, bool send, bool trading)
{
	std::shared_ptr<Item> item = get_item_by_slot(slot);
	if (!item)
	{
		return;
	}

	item->set_amount(item->get_amount() - amount);

	if (item->get_amount() < 1)
	{
		item->set_amount(0);

		if (!item->is_star() || trading) // stars are removed from inventory only in trade
		{
			remove_item(slot, true, send);
			return;
		}
	}

	// packet
	PacketCreator packet1;
	packet1.UpdateSlot(item);
	player_->send_packet(&packet1);
}

void Inventory::add_slots(signed char amount)
{
	slots_ += amount;
}

signed char Inventory::get_slots()
{
	return slots_;
}

std::unordered_map<signed char, std::shared_ptr<Item>> *Inventory::get_items()
{
	return &items_;
}

signed char Inventory::get_free_slots()
{
	return (slots_ - static_cast<signed char>(items_.size()));
}
