//

#include "player.hpp"

#include "packetcreator.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "party_constants.hpp"
#include "world.hpp"
#include "map.hpp"

void Player::handle_party_action()
{
	signed char action = read<signed char>();
	switch (action)
	{
	case PartyReceivePacketActions::kCreate:
	{
		// check wether the player is already in a party
		if (party_)
		{
			return;
		}
		// create a new party with the player as the leader
		World::get_instance()->create_party(this);
		break;
	}
	case PartyReceivePacketActions::kLeave:
	{
		if (!party_)
		{
			return;
		}
		if (party_->get_leader() == id_)
		{
			// send a packet
			PacketCreator packet;
			packet.LeaveParty(party_, id_, false, std::string(""), false);
			party_->send_packet(&packet);

			World::get_instance()->delete_party(party_->get_id());
		}
		else
		{
			// send a packet
			PacketCreator packet1;
			packet1.LeaveParty(party_, id_, true, name_, false);
			party_->send_packet(&packet1);

			party_->delete_member(id_);

			// send a packet
			PacketCreator packet2;
			packet2.UpdateParty(party_);
			party_->send_packet(&packet2);

			party_ = nullptr;
		}
		break;
	}
	case PartyReceivePacketActions::kJoin:
	{
		int party_id = read<int>();
		// check if the player is already in a party
		if (party_)
		{
			return;
		}
		// check if the player was invited by party_id
		if (party_id != party_invite_)
		{
			return;
		}
		// check if the party is valid
		Party *party = World::get_instance()->get_party(party_id);
		if (!party)
		{
			return;
		}
		// check wether the party is full
		if (party->get_members()->size() >= 6)
		{
			return;
		}
		// reset the party invitation
		party_invite_ = 0;
		{
			// add the player to the party
			party->add_member(this);
			// send a packet
			PacketCreator packet;
			packet.JoinParty(this);
			party->send_packet(&packet);
		}
		{
			// display party hp bar to all members
			auto players = party->get_players();
			for (auto &it : *players)
			{
				Player *target = it.second;
				if (target != this && target->get_map() == map_)
				{
					PacketCreator packet1;
					PacketCreator packet2;
					packet1.UpdatePartyHp(this);
					packet2.UpdatePartyHp(target);
					target->send_packet(&packet1);
					send_packet(&packet2);
				}
			}
		}
		break;
	}
	case PartyReceivePacketActions::kInvite:
	{
		if (!party_)
		{
			return;
		}
		std::string &target = read<std::string>();
		Player *invited = World::get_instance()->GetPlayerByName(target);
		if (!invited)
		{
			return;
		}
		if (invited->get_party())
		{
			return;
		}
		invited->set_party_invitation(party_->get_id());

		// send a packet
		PacketCreator packet;
		packet.InviteParty(this);
		invited->send_packet(&packet);

		break;
	}
	case PartyReceivePacketActions::kExpel:
	{
		if (!party_)
		{
			return;
		}
		int target_player_id = read<int>();
		PartyMember *member = party_->get_member(target_player_id);
		if (!member)
		{
			return;
		}

		// send a packet
		PacketCreator packet1;
		packet1.LeaveParty(party_, target_player_id, true, member->get_player_name(), true);
		party_->send_packet(&packet1);


		party_->delete_member(target_player_id);

		// send a packet
		PacketCreator packet2;
		packet2.UpdateParty(party_);
		party_->send_packet(&packet2);

		Player *expelled = World::get_instance()->GetPlayerById(target_player_id);
		if (expelled)
		{
			expelled->set_party(nullptr);
		}
		break;
	}
	case PartyReceivePacketActions::kChangeLeader:
	{
		// check wether the player is in a party
		if (!party_)
		{
			return;
		}
		// check wether the player is the party leader
		if (party_->get_leader() != id_)
		{
			return;
		}
		// check wether the target new leader is valid
		int new_leader_player_id = read<int>();
		if (!party_->get_member(new_leader_player_id))
		{
			return;
		}
		// set the target as the new leader
		party_->set_leader(new_leader_player_id);
		break;
	}
	}
}

void Player::handle_party_invitation()
{
	signed char action = read<signed char>();
	switch (action)
	{
		// the player declined the party invitation
	case PartyReceivePacketActions::kInvite:
	{
		// reset the party invitation
		party_invite_ = 0;
		break;
	}
	}
}

