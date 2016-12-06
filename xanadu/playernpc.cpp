//

#include "playernpc.hpp"

#include "player.hpp"
#include "world.hpp"
#include "guild.hpp"
#include "packetcreator.hpp"

// default constructor

PlayerNpc::PlayerNpc() :
	id_(0),
	selected_(-1),
	state_(0)
{
}

void PlayerNpc::set_state(int state)
{
	state_ = state;
}

void PlayerNpc::set_selected(int selected)
{
	selected_ = selected;
}

void PlayerNpc::set_npc_variable(std::string name, int val)
{
	npc_vars_[name] = val;
}

int PlayerNpc::get_npc_variable(std::string name)
{
	return (npc_vars_.find(name) != npc_vars_.end() ? npc_vars_[name] : 0);
}

void Player::set_state(int state)
{
	npc_->set_state(state);
}

int Player::get_state()
{
	return npc_->state_;
}

void Player::set_selected(int selected)
{
	npc_->set_selected(selected);
}

int Player::get_selected()
{
	return npc_->selected_;
}

void Player::set_npc_variable(std::string name, int val)
{
	npc_->set_npc_variable(name, val);
}

int Player::get_npc_variable(std::string name)
{
	return npc_->get_npc_variable(name);
}

void Player::send_simple(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_simple(npc_->id_, text);
	send_packet(&packet1);
}

void Player::send_yes_no(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_yes_no(npc_->id_, text);
	send_packet(&packet1);
}

void Player::send_next(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_back_next(npc_->id_, text, false, true);
	send_packet(&packet1);
}

void Player::send_back_next(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_back_next(npc_->id_, text, true, true);
	send_packet(&packet1);
}

void Player::send_back_ok(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_back_next(npc_->id_, text, true, false);
	send_packet(&packet1);
}

void Player::send_ok(std::string text)
{
	// packet
	PacketCreator packet1;
	packet1.send_back_next(npc_->id_, text, false, false);
	send_packet(&packet1);
}

void Player::send_style(int styles[], int size)
{
	std::string text = "";

	// packet
	PacketCreator packet1;
	packet1.send_style(styles, size, npc_->id_, text);
	send_packet(&packet1);
}

void Player::set_style(int id)
{
	if (id / 10000 == 0)
	{
		set_skin_color(static_cast<signed char>(id));
	}
	else if (id / 10000 == 2)
	{
		set_face(id);
	}
	else if (id / 10000 == 3)
	{
		set_hair(id);
	}
}

void Player::create_guild()
{
	PacketCreator packet;
	packet.CreateGuild();
	send_packet(&packet);
}

void Player::disband_guild()
{
	World::get_instance()->delete_guild(guild_->get_id());
}

void Player::change_guild_emblem()
{
	PacketCreator packet;
	packet.change_guild_emblem();
	send_packet(&packet);
}
