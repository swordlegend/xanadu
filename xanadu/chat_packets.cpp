//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::ShowChatMessage(Player *player, const std::string &message, bool bubble_only)
{
	write<short>(send_headers::kCHATTEXT);
	write<int>(player->get_id());
	write<bool>(player->get_is_gm());
	write<std::string>(message);
	write<bool>(bubble_only);
}

void PacketCreator::ShowMessage(const std::string &message, unsigned char type, unsigned char channel_id, unsigned char whisper, std::shared_ptr<Item> item)
{
	/*
	* 0: [Notice] in blue
	* 1: Popup
	* 2: Megaphone
	* 3: Super Megaphone
	* 4: Scrolling message at top
	* 5: Red Text
	* 6: Blue Text
	* 8: Item megaphone
	*/

	write<short>(send_headers::kSERVERMESSAGE);
	write<signed char>(type);

	if (type == 4)
	{
		write<signed char>(message != "");
	}

	write<std::string>(message);

	if (type == 3 || type == 8)
	{
		write<signed char>(channel_id);
		write<signed char>(whisper);
	}

	// display item in item meagphone

	if (type == 8)
	{
		if (item)
		{
			write<signed char>(item->get_slot());
			ItemInfo(item.get(), false);
		}
		else
		{
			write<signed char>(0);
		}
	}
}

void PacketCreator::FindPlayerReply(const std::string &name, bool success)
{
	write<short>(send_headers::kWHISPER);
	write<signed char>(10);
	write<std::string>(name);
	write<bool>(success);
}

void PacketCreator::FindPlayerCashShop(const std::string &name)
{
	write<short>(send_headers::kWHISPER);
	write<signed char>(9);
	write<std::string>(name);
	write<signed char>(2);
	write<int>(-1);
}

void PacketCreator::FindPlayerMap(const std::string &name, int mapid)
{
	write<short>(send_headers::kWHISPER);
	write<signed char>(9);
	write<std::string>(name);
	write<signed char>(1);
	write<int>(mapid);
	write<long long>(0);
}

void PacketCreator::FindPlayerChannel(const std::string &name, int channel)
{
	write<short>(send_headers::kWHISPER);
	write<signed char>(9);
	write<std::string>(name);
	write<signed char>(3);
	write<int>(channel);
}

void PacketCreator::WhisperPlayer(Player *player, const std::string &message)
{
	write<short>(send_headers::kWHISPER);
	write<signed char>(18);
	write<std::string>(player->get_name());
	write<short>(player->get_channel_id());
	write<std::string>(message);
}

void PacketCreator::ShowSpecialChat(unsigned char type, const std::string &name, const std::string &message)
{
	write<short>(send_headers::kMULTICHAT);
	write<signed char>(type); // 0 = buddy chat, 1 = party chat, 2 = guild chat
	write<std::string>(name);
	write<std::string>(message);
}
