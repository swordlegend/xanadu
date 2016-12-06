//

#include "party_member.hpp"

// constructor

PartyMember::PartyMember(int player_id, int job_id, int level, int channel_id, int map_id, std::string player_name):
	player_id_(player_id),
	job_id_(job_id),
	level_(level),
	channel_id_(channel_id),
	map_id_(map_id),
	player_name_(player_name)
{
}

int PartyMember::get_player_id()
{
	return player_id_;
}

int PartyMember::get_job()
{
	return job_id_;
}

int PartyMember::get_level()
{
	return level_;
}

int PartyMember::get_channel_id()
{
	return channel_id_;
}

int PartyMember::get_map_id()
{
	return map_id_;
}

std::string PartyMember::get_player_name()
{
	return player_name_;
}

void PartyMember::set_job(int job_id)
{
	job_id_ = job_id;
}

void PartyMember::set_level(int level)
{
	level_ = level;
}

void PartyMember::set_channel_id(int channel_id)
{
	channel_id_ = channel_id;
}

void PartyMember::set_map_id(int map_id)
{
	map_id_ = map_id;
}
