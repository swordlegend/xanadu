//

#include "../player.hpp"

void Player::handle_fourth_eos_rock_npc()
{
	if (get_state() == 0)
	{
		if (get_item_amount(4001020) == 0)
		{
			send_ok("There's a rock that will enable you to teleport to #bThird Eos Rock#k, but it cannot be activated without the scroll.");
		}
		else
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bFourth Eos Rock#k. Will you teleport to #bThird Eos Rock#k at the 32nd floor?");
		}
	}

	if (get_state() == 1)
	{
		if (get_selected() == 1)
		{
			remove_item(4001020, 1);
			set_map(221021200);
		}
	}
}
