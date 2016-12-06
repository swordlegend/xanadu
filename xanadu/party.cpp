//

#include "party.hpp"

#include "channel.hpp"
#include "map.hpp"
#include "party_member.hpp"
#include "player.hpp"
#include "packetcreator.hpp"

// constructor

Party::Party(int party_id, Player *leader) :
id_(party_id),
leader_(leader->get_id())
{
	add_member(leader);

	// send a packet
	PacketCreator packet;
	packet.PartyCreated(party_id);
	leader->send_packet(&packet);
}

// destructor

Party::~Party()
{
	for (auto &it : players_)
	{
		Player *player = it.second;
		player->set_party(nullptr);
	}
}

int Party::get_id()
{
	return id_;
}

int Party::get_leader()
{
	return leader_;
}

std::unordered_map<int, Player *> *Party::get_players()
{
	return &players_;
}

std::unordered_map<int, std::unique_ptr<PartyMember>> *Party::get_members()
{
	return &members_;
}

PartyMember *Party::get_member(int player_id)
{
	if (members_.find(player_id) != members_.end())
	{
		return members_[player_id].get();
	}

	return nullptr;
}

void Party::set_leader(int player_id)
{
	leader_ = player_id;

	// send a packet
	PacketCreator packet;
	packet.ChangeLeader(player_id, false);
	send_packet(&packet);
}

void Party::add_member(Player *player)
{
	player->set_party(this);
	players_[player->get_id()] = player;
	members_[player->get_id()].reset(new PartyMember(player->get_id(), player->get_job(), player->get_level(), player->get_channel_id(), player->get_map()->get_id(), player->get_name()));
}

void Party::delete_member(int player_id)
{
	if (players_.find(player_id) != players_.end())
	{
		players_.erase(player_id);
	}
	if (members_.find(player_id) != members_.end())
	{
		members_.erase(player_id);
	}
}

void Party::remove_member(int player_id)
{
	if (players_.find(player_id) != players_.end())
	{
		players_.erase(player_id);
	}
	if (members_.find(player_id) != members_.end())
	{
		PartyMember *member = members_[player_id].get();
		member->set_channel_id(-2);
		member->set_map_id(999999999);
	}
}

void Party::send_packet(PacketCreator *packet)
{
	for (auto it : players_)
	{
		Player *player = it.second;
		player->send_packet(packet);
	}
}

void Party::set_variable(std::string name, int val)
{
	variables_[name] = val;
}

int Party::get_variable(std::string name)
{
	if (variables_.find(name) == variables_.end())
	{
		return -1;
	}
	else
	{
		return variables_[name];
	}
}

void Party::delete_variable(std::string &name)
{
	if (variables_.find(name) != variables_.end())
	{
		variables_.erase(name);
	}
}

void Party::clear_variables()
{
	variables_.clear();
}
