//

#include "packetcreator.hpp"

#include "send_packet_opcodes.hpp"
#include "hiredmerchant.hpp"
#include "hiredmerchant_item.hpp"
#include "player.hpp"
#include "constants.hpp"

void PacketCreator::HiredMerchantBox()
{
	write<short>(send_headers::kREQUEST_HIRED_MERCHANT);
	write<signed char>(7); // action
}

void PacketCreator::GetHiredMerchant(Player *player, std::shared_ptr<HiredMerchant> merchant, bool first_time)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(5); // action
	write<signed char>(5);
	write<signed char>(4);
	write<signed char>(merchant->is_owner(player->get_id()) ? 0 : merchant->get_playerslot(player->get_id()));
	write<signed char>(0);
	write<int>(merchant->get_item_id());
	write<std::string>("Hired Merchant");

	for (signed char slot = kHiredMerchantConstantsSlotsMin; slot <= kHiredMerchantConstantsSlotsMax; ++slot)
	{
		Player *visitor = merchant->get_visitor(slot - 1);

		if (visitor)
		{
			write<signed char>(slot);
			AddCharLook(visitor);
			write<std::string>(visitor->get_name());
		}
	}

	write<signed char>(-1);

	write<short>(0); // messages
	
	write<std::string>(merchant->get_owner_name());

	if (merchant->is_owner(player->get_id()))
	{
		write<int>(0); // time left
		write<bool>(first_time);

		write<signed char>(0); // sold size

		/*for (SoldItem s : sold)
		{
		write<int>(s.get_item_id()); // itemid of purchased
		write<short>(s.getQuantity()); // number of purchased
		write<int>(s.get_mesos()); // total price
		write<std::string>(s.getBuyer()); // name of the buyer
		}*/

		write<int>(merchant->get_merchant_mesos());
	}

	write<std::string>(merchant->get_description());
	write<signed char>(16); // slots
	write<int>(player->get_mesos());
	write<signed char>(static_cast<unsigned char>(merchant->get_num_items()));

	for (size_t i = 0; i < merchant->get_num_items(); ++i)
	{
		std::shared_ptr<HiredMerchantItem> sellItem = merchant->get_item(static_cast<short>(i));

		write<short>(sellItem->get_bundles());
		write<short>(sellItem->get_quantity());
		write<int>(sellItem->get_price());
		ItemInfo(sellItem->get_item().get(), false);
	}
}

// this packet is from v0.62 xiuzsource
// it is for updating the hired merchant on the map
// basically only to show if it's full, under maintenance or free to be entered
/*
public static MaplePacket updateHiredMerchant(HiredMerchant shop) {
MaplePacketLittleEndianWriter mplew = new MaplePacketLittleEndianWriter();
mplew.writeShort(SendPacketOpcode.UPDATE_HIRED_MERCHANT.getValue());
mplew.writeInt(shop.getOwnerId());
mplew.write(0x05);
mplew.writeInt(shop.getObjectId());
mplew.writeMapleAsciiString(shop.getDescription());
mplew.write(shop.getItemType());
mplew.write(shop.getFreeSlot() > -1 ? 3 : 2);
mplew.write(0x04);
return mplew.getPacket();
}
*/

void PacketCreator::UpdateHiredMerchant(HiredMerchant *hired_merchant)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(23); // action
	write<int>(hired_merchant->get_merchant_mesos());
	write<signed char>(static_cast<unsigned char>(hired_merchant->get_num_items()));

	for (int i = 0; i < hired_merchant->get_num_items(); i++)
	{
		std::shared_ptr<HiredMerchantItem> sellItem = hired_merchant->get_item(static_cast<short>(i));

		write<short>(sellItem->get_bundles());
		write<short>(sellItem->get_quantity());
		write<int>(sellItem->get_price());
		ItemInfo(sellItem->get_item().get(), false);
	}
}

void PacketCreator::HiredMerchantChat(signed char slot, std::string &message)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(6); // action
	write<signed char>(8);
	write<signed char>(slot);
	write<std::string>(message);
}

void PacketCreator::HiredMerchantVisitorLeave(signed char slot)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x0A); // action
	write<signed char>(slot);
}

void PacketCreator::HiredMerchantVisitorAdd(Player *player, signed char slot)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(4); // action
	write<signed char>(slot);
	AddCharLook(player);
	write<std::string>(player->get_name());
}

// mode:
// 0 = items and mesos successfully retrieved
// 1 = go to fredrick for items and mesos
void PacketCreator::CloseMerchantStoreResponse()
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(40); // action
	write<signed char>(1); // mode
}

void PacketCreator::MerchantStoreMaintenanceResponse(int merchant_id)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(21); // action
	write<signed char>(10);
	write<signed char>(6);
	write<signed char>(1);
	write<int>(merchant_id);
	write<signed char>(0);
}

void PacketCreator::SpawnHiredMerchant(std::shared_ptr<HiredMerchant> merchant)
{
	write<short>(send_headers::kSPAWN_HIRED_MERCHANT);
	write<int>(merchant->get_owner_id());
	write<int>(merchant->get_item_id());
	write<short>(merchant->get_position_x());
	write<short>(merchant->get_position_y());
	write<short>(0);
	write<std::string>(merchant->get_owner_name());
	write<signed char>(5);
	write<int>(merchant->get_id());
	write<std::string>(merchant->get_description());
	write<signed char>(merchant->get_item_id() % 10);
	write<signed char>(1);
	write<signed char>(7);
}

void PacketCreator::DestroyHiredMerchant(int id)
{
	write<short>(send_headers::kDESTROY_HIRED_MERCHANT);
	write<int>(id);
}

void PacketCreator::LeaveHiredMerchant(int slot, int status2)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x0A); // action
	write<signed char>(static_cast<signed char>(slot));
	write<signed char>(static_cast<signed char>(status2));
}

void PacketCreator::PlayerShopError(signed char type)
{
	// 01 = You don't have enough in stock
	// 02 = You do not have enough mesos
	// 03 = The price of item is too high for the trade
	// 04 = The player does not posses enough money to trade anymore
	// 05 = Please check if your inventory is full or not
	// 06 = You cannot carry more than one for this item
	// 07 = Due to an error, the trade did not happen
	// 08 = Player below Level 15, 1 mil trade limit per day msg
	// 09+ = Due to an error, the trade did not happen

	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(24); // action
	write<signed char>(type);
}

void PacketCreator::PlayerShopMessage(signed char message)
{
	// 02 = Shop full
	// 18 = Shop maintenance

	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(11); // action
	write<signed char>(0);
	write<signed char>(message);
}
