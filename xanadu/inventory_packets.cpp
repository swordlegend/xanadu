//

#include "packetcreator.hpp"

#include "item_data_provider.hpp"
#include "item_data.hpp"
#include "item.hpp"
#include "send_packet_opcodes.hpp"
#include "constants.hpp"

void PacketCreator::ItemInfo(Item *item, bool show_position)
{
	int item_id = item->get_item_id();
	ItemData *item_data = ItemDataProvider::get_instance()->get_item_data(item_id);
	if (!item_data)
	{
		return;
	}

	enum ItemTypes
	{
		_Equip = 1,
		_Item = 2,
		_Pet = 3
	};

	signed char item_type = (item->is_pet() ? _Pet : item->is_equip() ? _Equip : _Item);

	bool is_cash = false;

	if (item_type == _Pet)
	{
		is_cash = true;
	}

	if (show_position)
	{
		char slot = item->get_slot();

		// equipped

		if (slot < 0)
		{
			slot *= -1;

			if (slot > 100)
			{
				is_cash = true;

				write<signed char>(0);
				write<signed char>(slot - 100);
			}
			else
			{
				write<signed char>(slot);
			}
		}
		else
		{
			write<signed char>(slot);
		}
	}

	write<signed char>(item_type);
	write<int>(item_id);
	write<signed char>(is_cash);

	if (is_cash)
	{
		write<long long>(item->get_unique_id());
	}

	write<long long>(item->get_expiration_time());

	switch (item_type)
	{

	case _Equip:
	{
		write<signed char>(item->get_free_slots());
		write<signed char>(item->get_used_scrolls());
		write<short>(item->get_str());
		write<short>(item->get_dex());
		write<short>(item->get_int());
		write<short>(item->get_luk());
		write<short>(item->get_hp());
		write<short>(item->get_mp());
		write<short>(item->get_weapon_attack());
		write<short>(item->get_magic_attack());
		write<short>(item->get_weapon_defense());
		write<short>(item->get_magic_defense());
		write<short>(item->get_acc());
		write<short>(item->get_avoid());
		write<short>(item->get_hand());
		write<short>(item->get_speed());
		write<short>(item->get_jump());
		write<std::string>(item->get_owner_name());
		write<signed char>(static_cast<signed char>(item->get_flag()));

		if (!is_cash)
		{
			write<signed char>(0);
			write<long long>(0);
		}

		break;
	}

	case _Item:
	{
		write<short>(item->get_amount());
		write<std::string>(item->get_owner_name());
		write<short>(item->get_flag());

		if (item->is_star())
		{
			write<long long>(-1);
		}

		break;
	}

	case _Pet:
	{
		write_string(item->get_name(), 13);
		write<signed char>(item->get_pet_level());
		write<short>(item->get_pet_closeness());
		write<signed char>(item->get_pet_fullness());
		write<long long>(kPermanentTime); // expire date
		write<int>(0);

		break;
	}
	}
}

void PacketCreator::UpdateSlot(std::shared_ptr<Item> item)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(1); // how many items to upgrade
	write<signed char>(1); // bag
	write<signed char>(item->get_inventory_id());
	write<short>(item->get_slot());
	write<short>(item->get_amount());
}

void PacketCreator::MoveItem(signed char inventory_id, short source_slot, short destination_slot)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(1); // how many items to update
	write<signed char>(2);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<short>(destination_slot);
	write<signed char>(0);
}

void PacketCreator::NewItem(std::shared_ptr<Item> item, bool from_drop)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(from_drop); // unstuck the client
	write<signed char>(1); // how many items to add
	write<signed char>(0);
	write<signed char>(item->get_inventory_id());
	write<short>(item->get_slot());
	ItemInfo(item.get(), false);
}

void PacketCreator::remove_item(signed char inventory_id, int slot, bool from_drop)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(from_drop); // unstuck the client
	write<signed char>(1); // how many items to remove
	write<signed char>(3);
	write<signed char>(inventory_id);
	write<short>(slot);

	if (slot < 0)
	{
		write<signed char>(1);
	}
}

void PacketCreator::MoveItemMerge(signed char inventory_id, short source_slot, short destination_slot, short amount)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(2); // how many items to update
	write<signed char>(3);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(destination_slot);
	write<short>(amount);
}

void PacketCreator::MoveItemMergeTwo(signed char inventory_id, short source_slot, short source_amount, short destination_slot, short destination_amount)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(2); // how many items to update
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(source_slot);
	write<short>(source_amount);
	write<signed char>(1);
	write<signed char>(inventory_id);
	write<short>(destination_slot);
	write<short>(destination_amount);
}

void PacketCreator::ScrolledItem(std::shared_ptr<Item> scroll, std::shared_ptr<Item> equip, bool destroyed)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<bool>(true); // unstuck the client
	write<signed char>(destroyed ? 2 : 3);
	write<signed char>((scroll->get_amount() > 1) ? 1 : 3);
	write<signed char>(scroll->get_inventory_id());
	write<short>(scroll->get_slot());

	if (scroll->get_amount() > 1)
	{
		write<short>(scroll->get_amount() - 1);
	}

	write<signed char>(3);
	write<signed char>(kInventoryConstantsTypesEquip);
	write<short>(equip->get_slot());

	if (!destroyed)
	{
		write<signed char>(0);
		write<signed char>(kInventoryConstantsTypesEquip);
		write<short>(equip->get_slot());
		ItemInfo(equip.get(), false);
	}

	write<signed char>(1);
}
