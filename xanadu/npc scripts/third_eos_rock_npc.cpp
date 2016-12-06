//

#include "../player.hpp"

void Player::handle_third_eos_rock_npc()
{
	int type = get_selected();
	set_npc_variable("type", type);

	if (get_state() == 0)
	{
		if (get_item_amount(4001020) == 0)
		{
			send_ok("There's a rock that will enable you to teleport to #bSecond Eos Rock or Fourth Eos Rock#k, but it cannot be activated without the scroll.");
		}
		else
		{
			send_simple("You can use #bEos Rock Scroll#k to activate #bThird Eos Rock#k. Which of these rocks would you like to teleport to? #b\r\n#L0#Second Eos Rock(70th floor)#l\r\n#L1#Fourth Eos Rock(1st floor)#l#k");
		}
	}

	if (get_state() == 1)
	{
		if (type == 0)
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bThird Eos Rock#k. Will you teleport to #bSecond Eos Rock#k at the 70th floor?");
		}
		else if (type == 1)
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bThird Eos Rock#k. Will you teleport to #bFourth Eos Rock#k at the 1st floor?");
			set_state(get_state() + 1);
		}
	}

	if (get_state() == 2)
	{
		if (get_selected() == 1)
		{
			remove_item(4001020, 1);
			set_map(221022100);
		}
	}

	if (get_state() == 3)
	{
		if (get_selected() == 1)
		{
			remove_item(4001020, 1);
			set_map(221020000);
		}
	}
}
