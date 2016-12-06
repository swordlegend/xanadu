//

#include "guild.hpp"

#include "guild_member.hpp"
#include "player.hpp"

// constructor

Guild::Guild(int id, std::string name) :
	logo_color_(0),
	logo_background_color_(0),
	logo_(0),
	logo_background_(0),
	capacity_(100),
	points_(0),
	alliance_id_(0),
	id_(id),
	name_(name),
	notice_("Welcome to the Guild!"),
	rank1_("Master"),
	rank2_("Jr. Master"),
	rank3_("Member"),
	rank4_("Member"),
	rank5_("Member")
{
}

// destructor

Guild::~Guild()
{
	for (auto &it : players_)
	{
		Player *player = it.second;
		player->set_guild(0);
		player->set_guild_rank(5);
	}
}

signed char Guild::get_logo_color()
{
	return logo_color_;
}

signed char Guild::get_logo_background_color()
{
	return logo_background_color_;
}

short Guild::get_logo()
{
	return logo_;
}

short Guild::get_logo_background()
{
	return logo_background_;
}

int Guild::get_capacity()
{
	return capacity_;
}

int Guild::get_points()
{
	return points_;
}

int Guild::get_alliance_id()
{
	return alliance_id_;
}

int Guild::get_id()
{
	return id_;
}

std::string Guild::get_name()
{
	return name_;
}

std::string Guild::GetNotice()
{
	return notice_;
}

std::string Guild::GetRank1()
{
	return rank1_;
}

std::string Guild::GetRank2()
{
	return rank2_;
}

std::string Guild::GetRank3()
{
	return rank3_;
}

std::string Guild::GetRank4()
{
	return rank4_;
}

std::string Guild::GetRank5()
{
	return rank5_;
}

std::unordered_map<int, std::unique_ptr<GuildMember>> *Guild::get_members()
{
	return &members_;
}

void Guild::SetLogoColor(signed char color_)
{
	logo_color_ = color_;
}

void Guild::SetLogoBackgroundColor(signed char background_color)
{
	logo_background_color_ = background_color;
}

void Guild::SetLogo(short logo)
{
	logo_ = logo;
}

void Guild::SetLogoBackground(short background)
{
	logo_background_ = background;
}

void Guild::set_name(const std::string &name)
{
	name_ = name;
}

void Guild::SetNotice(const std::string &notice)
{
	notice_ = notice;
}

void Guild::SetRank1(std::string rank)
{
	rank1_ = rank;
}

void Guild::SetRank2(std::string rank)
{
	rank2_ = rank;
}

void Guild::SetRank3(std::string rank)
{
	rank3_ = rank;
}

void Guild::SetRank4(std::string rank)
{
	rank4_ = rank;
}

void Guild::SetRank5(std::string rank)
{
	rank5_ = rank;
}

GuildMember *Guild::GetMember(int player_id)
{
	return (members_.find(player_id) != members_.end() ? members_[player_id].get() : nullptr);
}

void Guild::AddMember(Player *player)
{
	int player_id = player->get_id();
	players_[player_id] = player;
	members_[player_id].reset(new GuildMember(player_id, player->get_job(), player->get_level(), player->get_guild_rank(), 1, player->get_name()));
}

void Guild::AddMember(GuildMember *member)
{
	members_[member->get_id()].reset(member);
}

void Guild::RemoveMember(int player_id)
{
	if (players_.find(player_id) != players_.end())
	{
		players_.erase(player_id);
	}

	GuildMember *member = GetMember(player_id);
	if (member)
	{
		member->set_online_status(0);
	}
}

void Guild::DeleteMember(int player_id)
{
	if (players_.find(player_id) != players_.end())
	{
		players_.erase(player_id);
	}

	if (GetMember(player_id))
	{
		members_.erase(player_id);
	}
}

void Guild::send_packet(PacketCreator *packet)
{
	for (auto it : players_)
	{
		Player *target_player = it.second;
		target_player->send_packet(packet);
	}
}

void Guild::send_packet(PacketCreator *packet, Player *excluded_player)
{
	for (auto it : players_)
	{
		Player *target_player = it.second;
		if (target_player != excluded_player)
		{
			target_player->send_packet(packet);
		}
	}
}
