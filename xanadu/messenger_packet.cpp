//

#include "packetcreator.hpp"

#include "messenger.hpp"
#include "player.hpp"
#include "send_packet_opcodes.hpp"
#include "messenger_constants.hpp"

void PacketCreator::MessengerAddPlayer(Player *player)
{
	write<short>(send_headers::kMESSENGER);
	write<signed char>(MessengerSendPaketActions::kOpen);
	write<signed char>(player->get_messenger_position());
	AddCharLook(player, true);
	write<std::string>(player->get_name());
	write<short>(player->get_channel_id());
}

void PacketCreator::MessengerJoin(signed char pos)
{
	write<short>(send_headers::kMESSENGER);
    write<signed char>(MessengerSendPaketActions::kJoin);
    write<signed char>(pos);
}

void PacketCreator::MessengerRemovePlayer(signed char pos)
{
	write<short>(send_headers::kMESSENGER);
	write<signed char>(MessengerSendPaketActions::kLeave);
    write<signed char>(pos);
}

void PacketCreator::MessengerInvite(Player *inviter)
{
	write<short>(send_headers::kMESSENGER);
    write<signed char>(MessengerSendPaketActions::kInvite);
	write<std::string>(inviter->get_name());
    write<signed char>(0);
	write<int>(inviter->get_messenger()->get_id());
    write<signed char>(0);
}

void PacketCreator::MessengerNote(std::string invited, bool invite_successful)
{
	write<short>(send_headers::kMESSENGER);
    write<signed char>(MessengerSendPaketActions::kNote);
    write<std::string>(invited);
	write<bool>(invite_successful);
}

void PacketCreator::MessengerDeclined(std::string invited)
{
	write<short>(send_headers::kMESSENGER);
	write<signed char>(MessengerSendPaketActions::kDecline);
	write<std::string>(invited);
	write<signed char>(0);
}

void PacketCreator::MessengerChat(std::string name, std::string text)
{
	write<short>(send_headers::kMESSENGER);
	write<signed char>(MessengerSendPaketActions::kChat);
	write<std::string>(name);
	write<std::string>(text);
}
