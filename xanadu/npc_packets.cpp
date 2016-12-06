//

#include "packetcreator.hpp"

#include "send_packet_opcodes.hpp"
#include "mapnpc.hpp"
#include "item.hpp"
#include "shop_data.hpp"
#include "shop_item_data.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "tools.hpp"

// functions

void PacketCreator::ShowNpc(Npc * npc)
{
	write<short>(send_headers::kSPAWN_NPC);
	write<int>(npc->get_object_id());
	write<int>(npc->get_npc_id());
	write<short>(npc->get_position_x());
	write<short>(npc->get_position_y());
	write<bool>(!npc->get_flip());
	write<short>(npc->get_foothold());
	write<short>(npc->get_rx0());
	write<short>(npc->get_rx1());
	write<signed char>(1); // boolean wether npc is shown or not (1 = show, 0 = hide)
}

void PacketCreator::Bought(signed char code)
{
	write<short>(send_headers::kCONFIRM_SHOP_TRANSACTION);
	write<signed char>(code);
}

void PacketCreator::ShowNpcShop(ShopData *data)
{
	write<short>(send_headers::kOPEN_NPC_SHOP);

	write<int>(data->get_npc_id());

	auto items = data->get_shop_items();

	write<short>(static_cast<short>(items->size()));

	for (auto it : *items)
	{
		ShopItemData *item = it.second;
		ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->id);

		if (!data)
		{
			return;
		}

		write<int>(item->id);
		write<int>(item->price); // amount of mesos needed to buy the item

		if (tools::is_star(item->id))
		{
			double num = 0.3;
			long long value = *(long long*)(&num);
			write<long long>(value);
		}
		else
		{
			write<short>(1); // quantity
		}

		write<short>(data->max_per_slot);
	}
}

void PacketCreator::send_simple(int npc_id, std::string text)
{
	write<short>(send_headers::kNPC_TALK);
	write<signed char>(4);
	write<int>(npc_id);
	write<signed char>(4); // type: Simple
	write<std::string>(text);
}

void PacketCreator::send_yes_no(int npc_id, std::string text)
{
	write<short>(send_headers::kNPC_TALK);
	write<signed char>(4);
	write<int>(npc_id);
	write<signed char>(1); // type: YesNo
	write<std::string>(text);
}

void PacketCreator::send_back_next(int npc_id, std::string text, bool back, bool next)
{
	write<short>(send_headers::kNPC_TALK);
	write<signed char>(4);
	write<int>(npc_id);
	write<signed char>(0); // type: Normal
	write<std::string>(text);

	write<signed char>(back);
	write<signed char>(next);
}

void PacketCreator::send_style(int styles[], int size, int npc_id, std::string &text)
{
	write<short>(send_headers::kNPC_TALK);
	write<signed char>(4);
	write<int>(npc_id);
	write<signed char>(7); // type: Style
	write<std::string>(text);

	write<signed char>(size);

	for (int i = 0; i < size; ++i)
	{
		write<int>(styles[i]);
	}
}

// infos for the FredrickMessage function:
// the following type's are available, causing message popup's to the client with these texts:

// 30: You have retrieved your items and mesos. (disables the take out button for the client)
// 31: Unable to retrieve mesos and items due to too much money stored at the Store Bank.
// 32: Unable to retrieve mesos and items due to one of the items that can only be possessed one at a time.
// 33: Due to the lack of service fee, you were unable to retrieve mesos or items.
// 34: Unable to retrieve mesos and items due to full inventory.

void PacketCreator::FredrickMessage(signed char type)
{
	write<short>(send_headers::kFREDRICK_MESSAGE);
	write<signed char>(type);
}

void PacketCreator::GetFredrickStorage(std::unordered_map<signed char, std::shared_ptr<Item>> items, int mesos)
{
	write<short>(send_headers::kFREDRICK_OPERATION);
	write<signed char>(0x23); // action
	write<int>(9030000); // npc id
	write<int>(32272);
	write<int>(0);
	write<signed char>(0);
	write<int>(mesos);
	write<signed char>(0);

	write<signed char>(static_cast<signed char>(items.size()));

	for (auto item : items)
	{
		ItemInfo(item.second.get(), false);
	}

	write<short>(0);
	write<signed char>(0);
}

void PacketCreator::GetStorage(int npc_id, signed char slots, std::vector<std::shared_ptr<Item>> items, int mesos)
{
	write<short>(send_headers::kOPEN_STORAGE);
	write<signed char>(0x15); // action
	write<int>(npc_id);
	write<signed char>(slots);
	write<signed char>(0x7E);
	write<signed char>(0);
	write<short>(0);
	write<int>(0);
	write<int>(mesos);
	write<short>(0);

	write<signed char>(static_cast<signed char>(items.size()));

	for (auto item : items)
	{
		ItemInfo(item.get(), false);
	}

	write<short>(0);
	write<signed char>(0);
}

void PacketCreator::GetStorageFull()
{
	write<short>(send_headers::kOPEN_STORAGE);
	write<signed char>(0x10); // action
}

void PacketCreator::MesoStorage(signed char slots, int mesos)
{
	write<short>(send_headers::kOPEN_STORAGE);
	write<signed char>(0x12); // action
	write<signed char>(slots);
	write<short>(2);
	write<short>(0);
	write<int>(0);
	write<int>(mesos);
}

void PacketCreator::StoreStorage(signed char slots, signed char inventory_id, std::vector<std::shared_ptr<Item>> items)
{
	write<short>(send_headers::kOPEN_STORAGE);
	write<signed char>(0x0C); // action
	write<signed char>(slots);
	write<short>(static_cast<short>((2 << inventory_id)));
	write<short>(0);
	write<int>(0);

	signed char items_size = 0;

	for (auto &item : items)
	{
		if (item->get_inventory_id() == inventory_id)
		{
			++items_size;
		}
	}

	write<signed char>(items_size);

	for (auto &item : items)
	{
		if (item->get_inventory_id() == inventory_id)
		{
			ItemInfo(item.get(), false);
		}
	}
}

void PacketCreator::TakeOutStorage(signed char slots, signed char inventory_id, std::vector<std::shared_ptr<Item>> items)
{
	write<short>(send_headers::kOPEN_STORAGE);
	write<signed char>(9); // action
	write<signed char>(slots);
	write<short>(static_cast<short>((2 << inventory_id)));
	write<short>(0);
	write<int>(0);

	signed char items_size = 0;

	for (auto &item : items)
	{
		if (item->get_inventory_id() == inventory_id)
		{
			++items_size;
		}
	}

	write<signed char>(items_size);

	for (auto &item : items)
	{
		if (item->get_inventory_id() == inventory_id)
		{
			ItemInfo(item.get(), false);
		}
	}
}
