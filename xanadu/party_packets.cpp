//

#include "packetcreator.hpp"

#include "player.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "constants.hpp"
#include "send_packet_opcodes.hpp"
#include "party_constants.hpp"

void PacketCreator::PartyInfo(Party *party)
{
	auto members = party->get_members();
	size_t count = (6 - members->size());

	// player ids
	for (auto &it : *members)
	{
		write<int>(it.second->get_player_id());
	}
	for (size_t i = 0; i < count; ++i)
	{
		write<int>(0);
	}
	
	// player names
	for (auto &it : *members)
	{
		write_string(it.second->get_player_name(), 13);
	}
	for (size_t i = 0; i < count; ++i)
	{
		write_null(13);
	}
	
	// player job ids
	for (auto &it : *members)
	{
		write<int>(it.second->get_job());
	}
	for (size_t i = 0; i < count; ++i)
	{
		write<int>(0);
	}

	// player levels
	for (auto &it : *members)
	{
		write<int>(it.second->get_level());
	}
	for (size_t i = 0; i < count; ++i)
	{
		write<int>(0);
	}
	
	// player channel ids
	for (auto &it : *members)
	{
		write<int>(it.second->get_channel_id());
	}
	for (size_t i = 0; i < count; ++i)
	{
		write<int>(-2);
	}

	write<int>(party->get_leader());

	// player map ids
	for (auto &it : *members)
	{
		write<int>(it.second->get_map_id());
	}
	for (size_t i = 0; i < count; ++i)
	{
		write<int>(kNoMap);
	}

	// information for the door skill from priest
	for (int i = 0; i < 6; ++i)
	{
		write<int>(kNoMap); // door town
		write<int>(kNoMap); // door target town
		write<int>(-1); // door position x
		write<int>(-1); // door position y
	}
}

void PacketCreator::InviteParty(Player *from)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kInvite);
	write<int>(from->get_party()->get_id());
	write<std::string>(from->get_name());
	write<signed char>(0);
}

void PacketCreator::PartyCreated(int party_id)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kCreate);
	write<int>(party_id);
	write<int>(kNoMap);
	write<int>(kNoMap);
	write<int>(0);
}

void PacketCreator::LeaveParty(Party *party, int player_id, bool leave, std::string &player_name, bool expel)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kRemoveOrLeave);
	write<int>(party->get_id());
	write<int>(player_id);
	write<bool>(leave);

	if (leave)
	{
		write<bool>(expel);
		write<std::string>(player_name);
		PartyInfo(party);
	}
	else
	{
		write<int>(party->get_leader());
	}
}

void PacketCreator::JoinParty(Player *player)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kJoin);
	write<int>(player->get_party()->get_id());
	write<std::string>(player->get_name());
	PartyInfo(player->get_party());
}

void PacketCreator::ChangeLeader(int new_leader_player_id, bool leader_disconnected)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kChangeLeader);
	write<int>(new_leader_player_id);
	write<bool>(leader_disconnected);
}

void PacketCreator::UpdateParty(Party *party)
{
	write<short>(send_headers::kPARTY_OPERATION);
	write<signed char>(PartySendPaketActions::kUpdate);
	write<int>(party->get_id());
	PartyInfo(party);
}

void PacketCreator::UpdatePartyHp(Player *player)
{
	write<short>(send_headers::kUPDATE_PARTYMEMBER_HP);
	write<int>(player->get_id());
	write<int>(player->get_hp());
	write<int>(player->get_max_hp());
}
