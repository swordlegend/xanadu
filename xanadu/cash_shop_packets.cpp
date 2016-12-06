//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::EnterCashShop(Player *player)
{
	write<short>(send_headers::kOPEN_CASHSHOP);
	writeCharacterData(player);
	write<signed char>(1);
	write<std::string>("test"); // username

	for (int i = 0; i < 15; i++)
	{
		write<long long>(0);
	}

	write<int>(0);
	write<short>(0);
	write<signed char>(0);

	for (int i = 1; i <= 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			write<int>(i);
			write<int>(j);
			write<int>(50200004);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50200069);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50200117);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50100008);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50000047);
		}
	}

	write<int>(0);
	write<short>(0);
	write<signed char>(0);
	write<int>(75);
}

void PacketCreator::ShowCashPoints(int nx_credit)
{
	write<short>(send_headers::kSHOW_CASH);
	write<int>(nx_credit); // nx cash (credit)
	write<int>(0); // maple point
	write<int>(0); // nx cash (others)
}

void PacketCreator::ShowBoughtCashItem(int account_id, int serial_number, int item_id, int amount)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0xFE); // action
	write<short>(0);
	write<long long>(1); // cash unique id
	write<int>(account_id);
	write<int>(0);
	write<int>(item_id);
	write<int>(serial_number);
	write<short>(static_cast<short>(amount));
	write_string("", 13);
	write<long long>(0); // expiration time
	write<long long>(0);
}

void PacketCreator::IncreaseInventorySlots(signed char inventory_id, signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(127); // action
	write<signed char>(inventory_id);
	write<signed char>(slots);
	write<signed char>(0);
}

void PacketCreator::IncreaseStorageSlots(signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(129); // action
	write<signed char>(slots);
	write<signed char>(0);
}

void PacketCreator::GetCashShopInventory(short storage_slots, short character_slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x4B); // action
	short size = 0;
	write<short>(size); // cashshop inventory size
	// TODO: write items here that are in the cashshop inventory
	if (size > 0)
	{
		write<int>(0);
		// todo: write all info
	}
	write<short>(storage_slots);
	write<short>(character_slots);
}

void PacketCreator::TakeOutFromCashShopInventory(Item *item, short position)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x83); // action
	write<short>(position);
	ItemInfo(item, false);
	write<int>(0);
}

void PacketCreator::TransferToCashShopInventory(int account_id, int serial_number, int item_id, int amount)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x85); // action
	write<long long>(1); // cash unique id
	write<int>(account_id);
	write<int>(0);
	write<int>(item_id);
	write<int>(serial_number);
	write<short>(static_cast<short>(amount));
	write_bytes("000000000000000000000000007016AFFF60BCCE010000000000000000D28C8E010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}
