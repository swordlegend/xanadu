//

#include "player.hpp"

#include "messenger.hpp"
#include "packetcreator.hpp"
#include "world.hpp"
#include "messenger_constants.hpp"

void Player::handle_messenger_action()
{
	signed char action = read<signed char>();
	switch (action)
	{
	case MessengerReceivePaketActions::kOpen:
	{
		if (!messenger_)
		{
			int messenger_id = read<int>();
			if (messenger_id == 0)
			{
				World::get_instance()->create_messenger(this);
			}
			else
			{
				World::get_instance()->join_messenger(this, messenger_id);
			}
		}
		break;
	}
	case MessengerReceivePaketActions::kLeave:
	{
		if (messenger_)
		{
			if (messenger_->get_players()->size() == 1)
			{
				World::get_instance()->remove_messenger(messenger_->get_id());
			}
			else
			{
				messenger_->delete_member(id_);

				// send a packet
				PacketCreator packet45;
				packet45.MessengerRemovePlayer(get_messenger_position());
				messenger_->send_packet(&packet45);
			}

			messenger_ = nullptr;
		}

		break;
	}
	case MessengerReceivePaketActions::kInvite:
	{
		if (messenger_)
		{
			std::string invited_name = read<std::string>();
			Player *target = World::get_instance()->GetPlayerByName(invited_name);
			if (target)
			{
				if (!target->get_messenger())
				{
					// send a packet
					PacketCreator packet60;
					packet60.MessengerInvite(this);
					target->send_packet(&packet60);

					// send a packet
					PacketCreator packet62;
					packet62.MessengerNote(invited_name, true);
					send_packet(&packet62);
				}
				else
				{
					// send a packet
					PacketCreator packet63;
					packet63.MessengerChat(name_, invited_name + " is already using Maple Messenger.");
					send_packet(&packet63);
				}
			}
			else
			{
				// send a packet
				PacketCreator packet64;
				packet64.MessengerNote(invited_name, false);
				send_packet(&packet64);
			}
		}
		break;
	}
	case MessengerReceivePaketActions::kDecline:
	{
		std::string inviter_name = read<std::string>();
		Player *target = World::get_instance()->GetPlayerByName(inviter_name);
		if (target)
		{
			// send a packet
			PacketCreator packet51;
			packet51.MessengerDeclined(name_);
			target->send_packet(&packet51);
		}
		break;
	}
	case MessengerReceivePaketActions::kChat:
	{
		if (messenger_)
		{
			std::string name = read<std::string>();
			std::string text = read<std::string>();

			// send a packet
			PacketCreator packet52;
			packet52.MessengerChat(name, text);
			messenger_->send_packet(&packet52, this);
		}
		break;
	}
	}
}
