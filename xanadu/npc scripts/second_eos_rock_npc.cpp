//

#include "../player.hpp"

void Player::handle_second_eos_rock_npc()
{
	int type = get_selected();
	set_npc_variable("type", type);

	if (get_state() == 0)
	{
		if (get_item_amount(4001020) == 0)
		{
			send_ok("There's a rock that will enable you to teleport to #bFirst Eos Rock or Third Eos Rock#k, but it cannot be activated without the scroll.");
		}
		else
		{
			send_simple("You can use #bEos Rock Scroll#k to activate #bSecond Eos Rock#k. Which of these rocks would you like to teleport to? #b\r\n#L0#First Eos Rock(100th floor)#l\r\n#L1#Third Eos Rock(32nd floor)#l#k");
		}
	}

	if (get_state() == 1)
	{
		if (type == 0)
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bSecond Eos Rock#k. Will you teleport to #bFirst Eos Rock#k at the 100th floor?");
		}
		else if (type == 1)
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bSecond Eos Rock#k. Will you teleport to #bThird Eos Rock#k at the 32nd floor?");
			set_state(get_state() + 1);
		}
	}

	if (get_state() == 2)
	{
		if (get_selected() == 1)
		{
			remove_item(4001020, 1);
			set_map(221023200);
		}
	}

	if (get_state() == 3)
	{
		if (get_selected() == 1)
		{
			remove_item(4001020, 1);
			set_map(221021200);
		}
	}
}
