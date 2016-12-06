//

#include "packetcreator.hpp"

#include "guild.hpp"
#include "guild_member.hpp"
#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::GuildMemberData(GuildMember *member)
{
	write_string(member->get_name(), 13);
	write<int>(member->get_job());
	write<int>(member->get_level());
	write<int>(member->get_guild_rank());
	write<int>(member->get_online_status());
	write<int>(member->get_signature());
	write<int>(member->get_alliance_rank());
}

void PacketCreator::CreateGuild()
{	
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(1); // action
}

void PacketCreator::change_guild_emblem()
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(17); // action
}

void PacketCreator::GuildInfo(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(26); // action
	write<bool>(true); // 0/false = empty guild window, 1/true = in guild
	write<int>(guild->get_id());
	write<std::string>(guild->get_name());
	write<std::string>(guild->GetRank1());
	write<std::string>(guild->GetRank2());
	write<std::string>(guild->GetRank3());
	write<std::string>(guild->GetRank4());
	write<std::string>(guild->GetRank5());
	{
		// guild member data
		auto members = guild->get_members();

		write<signed char>(static_cast<unsigned char>(members->size()));

		for (auto &it : *members)
		{
			write<int>(it.second->get_id());
		}
		for (auto &it : *members)
		{
			GuildMemberData(it.second.get());
		}
	}
	write<int>(guild->get_capacity());
	write<short>(guild->get_logo_background());
	write<signed char>(guild->get_logo_background_color());
	write<short>(guild->get_logo());
	write<signed char>(guild->get_logo_color());
	write<std::string>(guild->GetNotice());
	write<int>(guild->get_points());
	write<int>(guild->get_alliance_id());
}

void PacketCreator::AddGuildPlayer(int guild_id, GuildMember *member)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(39); // action
	write<int>(guild_id);
	write<int>(member->get_id());
	GuildMemberData(member);
}

void PacketCreator::GuildPlayerLeave(int guild_id, int player_id, std::string charname, bool expelled)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(expelled ? 47 : 44); // action; expelled = 47, leaving = 44
	write<int>(guild_id);
	write<int>(player_id);
	write<std::string>(charname);
}

void PacketCreator::InviteGuild(Player *inviter)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(5); // action
	write<int>(inviter->get_guild()->get_id());
	write<std::string>(inviter->get_name());
}

void PacketCreator::ChangeRank(int guild_id, int player_id, int rank)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(64); // action
	write<int>(guild_id);
	write<int>(player_id);
	write<signed char>(rank);
}

void PacketCreator::UpdateGuildRanks(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(62); // action
	write<int>(guild->get_id());
	write<std::string>(guild->GetRank1());
	write<std::string>(guild->GetRank2());
	write<std::string>(guild->GetRank3());
	write<std::string>(guild->GetRank4());
	write<std::string>(guild->GetRank5());
}

void PacketCreator::UpdateGuildNotice(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(68); // action
	write<int>(guild->get_id());
	write<std::string>(guild->GetNotice());
}

void PacketCreator::DisbandGuild(int guild_id)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(50); // action
	write<int>(guild_id);
	write<signed char>(1);
}

void PacketCreator::ShowGuildEmblem(Guild *guild)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(66); // action
	write<int>(guild->get_id());
	write<short>(guild->get_logo_background());
	write<signed char>(guild->get_logo_background_color());
	write<short>(guild->get_logo());
	write<signed char>(guild->get_logo_color());
}

void PacketCreator::GuildMemberOnline(int guild_id, int player_id, bool online)
{
	write<short>(send_headers::kGUILD_OPERATION);
	write<signed char>(61); // action
	write<int>(guild_id);
	write<int>(player_id);
	write<bool>(online);
}
