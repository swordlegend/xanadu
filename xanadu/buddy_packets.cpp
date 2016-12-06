//

#include "packetcreator.hpp"

#include "buddy.hpp"
#include "player.hpp"
#include "send_packet_opcodes.hpp"
#include "buddylist_constants.hpp"

void PacketCreator::BuddyList(Player *player)
{
	auto buddylist = player->get_buddylist();
	signed char buddylist_size = static_cast<signed char>(buddylist->size());
	
	write<short>(send_headers::kBUDDY_LIST);
	write<signed char>(BuddylistSendPacketActions::kUpdate);
	write<signed char>(buddylist_size);

	for (auto &it : *buddylist)
	{
		auto buddy = it.second;
		write<int>(buddy->get_player_id());
		write_string(buddy->get_player_name(), 13);
		write<signed char>(buddy->get_opposite_status());
		write<int>(buddy->get_channel_id());
	}

	for (signed char i = 0; i < buddylist_size; ++i)
	{
		write<int>(0);
	}
}

void PacketCreator::BuddyListInvite(Player *player)
{
	write<short>(send_headers::kBUDDY_LIST);
	write<signed char>(BuddylistSendPacketActions::kInvite);
	write<int>(player->get_id());
	write<std::string>(player->get_name());
	write<int>(player->get_id());
	write_string(player->get_name(), 13);
	write<signed char>(Buddylist::kOppositeStatusRequested);
	write<int>(1);
	write_string("Group Unknown", 16);
	write<signed char>(0x1C);
	write<signed char>(0);
}

void PacketCreator::UpdateBuddyChannel(int player_id, int channel_id)
{
	write<short>(send_headers::kBUDDY_LIST);
	write<signed char>(BuddylistSendPacketActions::kUpdateChannel);
	write<int>(player_id);
	write<signed char>(0);
	write<int>(channel_id);
}

void PacketCreator::UpdateBuddyListCapacity(Player *player)
{
	write<short>(send_headers::kBUDDY_LIST);
	write<signed char>(BuddylistSendPacketActions::kUpdateCapacity);
	write<signed char>(player->get_buddy_list_capacity());
}
