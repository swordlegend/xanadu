//

#include "buddy.hpp"

#include "buddylist_constants.hpp"

// constructor

Buddy::Buddy(int player_id, int channel_id, std::string player_name) :
player_id_(player_id),
channel_id_(channel_id),
player_name_(player_name)
{
}

signed char Buddy::get_opposite_status()
{
	return Buddylist::kOppositeStatusRegistered;
}

int Buddy::get_player_id()
{
	return player_id_;
}

int Buddy::get_channel_id()
{
	return channel_id_;
}

std::string Buddy::get_player_name()
{
	return player_name_;
}

void Buddy::set_channel_id(int channel_id)
{
	channel_id_ = channel_id;
}
