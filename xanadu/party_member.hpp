//

#pragma once

#include <string>

class PartyMember
{
public:

	// constructor
	PartyMember(int player_id, int job_id, int level, int channel_id, int map_id, std::string player_name);

	// default constructor
	PartyMember() = delete;

	// copy constructor
	PartyMember(const PartyMember &other) = delete;

	// move constructor
	PartyMember(PartyMember &&other) = delete;

	// destructor
	~PartyMember() = default;

	// copy assignment operator
	PartyMember &operator=(const PartyMember &other) = delete;

	// move assignment operator
	PartyMember &operator=(PartyMember &&other) = delete;

	int get_player_id();
	int get_job();
	int get_level();
	int get_channel_id();
	int get_map_id();
	std::string get_player_name();
	void set_job(int job_id);
	void set_level(int level);
	void set_channel_id(int channel_id);
	void set_map_id(int map_id);

private:

	int player_id_;
	int job_id_;
	int level_;
	int channel_id_;
	int map_id_;
	std::string player_name_;
};
