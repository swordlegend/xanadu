//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::ShowTrade(Player *player, Player *partner, signed char number)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(5); // action
	write<signed char>(3);
	write<signed char>(2);
	write<signed char>(number);
	write<signed char>(0);

	if (number == 1)
	{
		AddCharLook(partner);
		write<std::string>(partner->get_name());
		write<signed char>(1);
	}

	AddCharLook(player);
	write<std::string>(player->get_name());
	write<signed char>(-1);
}

void PacketCreator::InviteTrade(Player *player)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(2); // action
	write<signed char>(3);
	write<std::string>(player->get_name());
	write<int>(0); // trade id
}

void PacketCreator::JoinTrade(Player *player)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(4); // action
	write<signed char>(1);
	AddCharLook(player);
	write<std::string>(player->get_name());
}

void PacketCreator::TradeError(signed char side, signed char message)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x0A); // action
	write<signed char>(side);
	write<signed char>(message);
}

void PacketCreator::ShowTradeChat(signed char side, std::string &message)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(6); // action
	write<signed char>(8);
	write<signed char>(side);
	write<std::string>(message);
}

void PacketCreator::TradeMesos(int mesos, signed char number)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x0F); // action
	write<signed char>(number);
	write<int>(mesos);
}

void PacketCreator::TradeItem(std::shared_ptr<Item> item, signed char number)
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x0E); // action
	write<signed char>(number);
	write<signed char>(item->get_slot());
	ItemInfo(item.get(), false);
}

void PacketCreator::TradeConfirm()
{
	write<short>(send_headers::kPLAYER_INTERACTION);
	write<signed char>(0x10); // action
}
