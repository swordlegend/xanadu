
#pragma once

#include <string>

class Buddy
{
public:

	// constructor
	Buddy(int player_id, int channel_id, std::string player_name);

	// default constructor
	Buddy() = delete;

	// copy constructor
	Buddy(const Buddy &other) = delete;

	// move constructor
	Buddy(Buddy &&other) = delete;

	// destructor
	~Buddy() = default;

	// copy assignment operator
	Buddy &operator=(const Buddy &other) = delete;

	// move assignment operator
	Buddy &operator=(Buddy &&other) = delete;

	signed char get_opposite_status();
	int get_player_id();
	int get_channel_id();
	std::string get_player_name();
	void set_channel_id(int channel_id);

private:

	int player_id_;
	int channel_id_;
	std::string player_name_;
};
