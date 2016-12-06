//

#include "../player.hpp"

// in ludibrium maze room 16 clear the pq

void Player::handle_npc_lmpq_pietri()
{
	short amount = get_item_amount(4001106);

	if (get_item_amount(4001106) >= 30)
	{
		int pq_exp_rate = 3;
		int gain_exp = 50 * amount * 1000 * pq_exp_rate;
		set_exp(get_exp() + gain_exp);

		set_map(809050017);
	}
	else
	{
		set_map(809050016);
	}

	give_item(4001106, -amount);
}

// in ludibrium, warps party into ludibrium maze room 1

void Player::handle_npc_lmpq_rolly()
{
	set_map(809050000);
}

// Rolly2, warps out of Ludi Maze PQ

void Player::handle_npc_lmpq_rolly2()
{
	set_map(220000000);
}

// Rolly3, warps out of Ludi Maze PQ and gives rewards

void Player::handle_npc_lmpq_rolly3()
{
	give_item(2000000, 1);
	set_map(220000000);
}
