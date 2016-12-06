//

#pragma once

#include <string>

class GuildMember
{
public:

	// constructor
	GuildMember(int id, int job_id, int level, int guild_rank, int online_status, std::string name);

	// default constructor
	GuildMember() = delete;

	// copy constructor
	GuildMember(const GuildMember &other) = delete;

	// move constructor
	GuildMember(GuildMember &&other) = delete;

	// destructor
	~GuildMember() = default;

	// copy assignment operator
	GuildMember &operator=(const GuildMember &other) = delete;

	// move assignment operator
	GuildMember &operator=(GuildMember &&other) = delete;

	int get_id();
	int get_job();
	int get_level();
	int get_guild_rank();
	int get_online_status();
	int get_signature();
	int get_alliance_rank();
	std::string get_name();
	void set_job(int job_id);
	void set_level(int level);
	void set_guild_rank(int guild_rank);
	void set_online_status(int online);
	void set_signature(int signature);
	void set_alliance_rank(int alliance_rank);

private:

	int id_;
	int job_id_;
	int level_;
	int guild_rank_;
	int online_status_;
	int signature_;
	int alliance_rank_;
	std::string name_;
};
