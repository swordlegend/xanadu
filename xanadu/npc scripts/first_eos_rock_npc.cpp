//

#include "../player.hpp"
#include "../constants.hpp"

void Player::handle_first_eos_rock_npc()
{
	if (get_state() == 0)
	{
		if (get_item_amount(4001020) == 0)
		{
			send_ok("There's a rock that will enable you to teleport to #bSecond Eos Rock#k, but it cannot be activated without the scroll.");
		}
		else
		{
			send_yes_no("You can use #bEos Rock Scroll#k to activate #bFirst Eos Rock#k. Will you teleport to #bSecond Eos Rock#k at the 70th floor?");
		}
	}
	if (get_state() == 1)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			remove_item(4001020, 1);
			set_map(221022100);
		}
	}
}
