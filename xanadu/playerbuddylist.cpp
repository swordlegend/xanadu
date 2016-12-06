//

#include "player.hpp"

#include "buddylist_constants.hpp"
#include "world.hpp"
#include "packetcreator.hpp"

void Player::handle_buddy_list_action()
{
	signed char type = read<signed char>();
	switch (type)
	{
	case BuddylistReceivePacketActions::kAdd:
	{
		std::string player_name = read<std::string>();
		add_buddy(player_name);
		break;
	}
	case BuddylistReceivePacketActions::kAccept:
	{
		int player_id = read<int>();
		accept_buddy(player_id);
		break;
	}
	case BuddylistReceivePacketActions::kDelete:
	{
		int player_id = read<int>();
		remove_buddy(player_id);
		break;
	}
	}
}

std::unordered_map<int, std::shared_ptr<Buddy>> *Player::get_buddylist()
{
	return &buddies_;
}

Buddy *Player::get_buddy(int player_id)
{
	if (buddies_.find(player_id) != buddies_.end())
	{
		return buddies_[player_id].get();
	}

	return nullptr;
}

void Player::add_buddy(int player_id, std::string name, int channel_id)
{
	buddies_[player_id] = std::shared_ptr<Buddy>(new Buddy(player_id, channel_id, name));
}

void Player::add_buddy(std::string player_name)
{
	if (buddies_.size() >= 100)
	{
		// packet
		PacketCreator packet1;
		packet1.ShowMessage("Your buddylist is full.", 5);
		send_packet(&packet1);

		return;
	}

	World *world = World::get_instance();
	int buddy_id = world->get_player_id_from_name_(player_name);
	if (buddy_id == 0)
	{
		// player does not exist

		// packet
		PacketCreator packet2;
		packet2.ShowMessage("The player does not exist.", 5);
		send_packet(&packet2);

		return;
	}

	if (buddies_.find(buddy_id) != buddies_.end())
	{
		// already in buddylist

		// packet
		PacketCreator packet3;
		packet3.ShowMessage("The player is already in your buddylist.", 5);
		send_packet(&packet3);

		return;
	}

	add_buddy(buddy_id, player_name, -1);
	Player *buddy = world->GetPlayerByName(player_name);
	if (buddy)
	{
		if (buddy->get_buddy(id_))
		{
			buddy->get_buddy(id_)->set_channel_id(channel_id_);
			// send a packet
			PacketCreator packet;
			packet.BuddyList(buddy);
			buddy->send_packet(&packet);
			//
			get_buddy(buddy_id)->set_channel_id(buddy->get_channel_id());
		}
		else
		{
			// send a packet
			PacketCreator packet;
			packet.BuddyListInvite(this);
			buddy->send_packet(&packet);
		}
	}

	// send a packet
	PacketCreator packet;
	packet.BuddyList(this);
	send_packet(&packet);
}

void Player::accept_buddy(int player_id)
{
	World *world = World::get_instance();
	Player *buddy = world->GetPlayerById(player_id);
	if (buddy)
	{
		if (buddy->get_buddy(id_))
		{
			buddy->get_buddy(id_)->set_channel_id(channel_id_);

			// send a packet
			PacketCreator packet;
			packet.UpdateBuddyChannel(id_, channel_id_);
			buddy->send_packet(&packet);
		}
		if (!get_buddy(player_id))
		{
			add_buddy(player_id, buddy->get_name(), buddy->get_channel_id());
		}
	}

	if (!get_buddy(player_id))
	{
		std::string player_name = world->get_player_name_from_id(player_id);
		if (player_name != "")
		{
			add_buddy(player_id, player_name, -1);
		}
	}

	// send a packet
	PacketCreator packet;
	packet.BuddyList(this);
	send_packet(&packet);
}

void Player::remove_buddy(int player_id)
{
	if (buddies_.find(player_id) == buddies_.end())
	{
		return;
	}

	Player *buddy = World::get_instance()->GetPlayerById(player_id);
	if (buddy)
	{
		Buddy *duaa = get_buddy(id_);
		if (duaa)
		{
			duaa->set_channel_id(-1);
			// send a packet
			PacketCreator packet;
			packet.BuddyList(buddy);
			buddy->send_packet(&packet);
		}
	}

	buddies_.erase(player_id);

	// send a packet
	PacketCreator packet;
	packet.BuddyList(this);
	send_packet(&packet);
}

void Player::change_buddy_channel(Player *buddy, int player_id, int channel_id)
{
	if (buddy->get_buddy(player_id))
	{
		buddy->get_buddy(player_id)->set_channel_id(channel_id);
	}

	// send a packet
	PacketCreator packet;
	packet.UpdateBuddyChannel(player_id, channel_id);
	buddy->send_packet(&packet);
}
