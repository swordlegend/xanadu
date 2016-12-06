//

#include "player.hpp"

void Player::reset_trade()
{
	trade_partner_ = nullptr;
	trade_locked_ = false;
	trade_mesos = 0;
	trade_items_.clear();
}

void Player::set_trade_partner(Player *partner)
{
	trade_partner_ = partner;
}

Player *Player::get_trade_partner()
{
	return trade_partner_;
}

bool Player::is_trade_confirmed()
{
	return trade_locked_;
}

int Player::get_trade_mesos()
{
	return trade_mesos;
}

std::vector<std::shared_ptr<Item>> *Player::get_trade_items()
{
	return &trade_items_;
}
