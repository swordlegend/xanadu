//

#include "messenger.hpp"

#include "player.hpp"

// constructor

Messenger::Messenger(int id, Player *player):
	id_(id)
{
	add_member(player);
	player->set_messenger(this);
	player->set_messenger_position(0);
}

std::unordered_map<int, Player *> *Messenger::get_players()
{
	return &players_;
}

int Messenger::get_id()
{
	return id_;
}

void Messenger::add_member(Player *player)
{
	players_[player->get_id()] = player;
}

void Messenger::delete_member(int id)
{
	players_.erase(id);
}

void Messenger::send_packet(PacketCreator *packet)
{
	for (auto it : players_)
	{
		Player *target = it.second;
		target->send_packet(packet);
	}
}

void Messenger::send_packet(PacketCreator *packet, Player *excluded_player)
{
	for (auto it : players_)
	{
		Player *target = it.second;
		if (target != excluded_player)
		{
			target->send_packet(packet);
		}
	}
}
