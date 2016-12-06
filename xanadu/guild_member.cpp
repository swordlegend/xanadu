//

#include "guild_member.hpp"

// constructor

GuildMember::GuildMember(int id, int job_id, int level, int guild_rank, int online_status, std::string name):
	id_(id),
	job_id_(job_id),
	level_(level),
	guild_rank_(guild_rank),
	online_status_(online_status),
	name_(name),
	signature_(1),
	alliance_rank_(5)
{
}

int GuildMember::get_id()
{
	return id_;
}

int GuildMember::get_job()
{
	return job_id_;
}

int GuildMember::get_level()
{
	return level_;
}

int GuildMember::get_guild_rank()
{
	return guild_rank_;
}

int GuildMember::get_online_status()
{
	return online_status_;
}

int GuildMember::get_signature()
{
	return signature_;
}

int GuildMember::get_alliance_rank()
{
	return alliance_rank_;
}

std::string GuildMember::get_name()
{
	return name_;
}

void GuildMember::set_job(int job_id)
{
	job_id_ = job_id;
}

void GuildMember::set_level(int level)
{
	level_ = level;
}

void GuildMember::set_guild_rank(int guild_rank)
{
	guild_rank_ = guild_rank;
}

void GuildMember::set_online_status(int online_status)
{
	online_status_ = online_status;
}

void GuildMember::set_signature(int signature)
{
	signature = signature;
}

void GuildMember::set_alliance_rank(int alliance_rank)
{
	alliance_rank_ = alliance_rank;
}
