//

#include "../player.hpp"

#include "../map.hpp"

void Player::handle_hpq_tory_npc()
{
	if (map_->get_id() == 100000200)
	{
		set_map(910010000);
	}

	else if (map_->get_id() == 910010400 || map_->get_id() == 910010100)
	{
		set_map(100000200);
	}
}

void Player::handle_hpq_tommy_npc()
{
	if (map_->get_id() == 910010300)
	{
		set_map(100000200);
	}

	else if (map_->get_id() == 910010100)
	{
		set_map(910010200);
	}

	else if (map_->get_id() == 910010200)
	{
		set_map(910010300);
	}
}

void Player::handle_hpq_growlie_npc()
{
	short amount = get_item_amount(4001101);

	if (amount >= 10)
	{
		give_item(4001101, -amount);
		set_exp(get_exp() + 1600 * 3);
		set_map(910010100);
	}
}
