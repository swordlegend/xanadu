//

#include "../player.hpp"
#include "../constants.hpp"

void Player::handle_orbis_magic_spot_npc()
{
	if (get_state() == 0)
	{
		if (get_item_amount(4001019) == 0)
		{
			send_ok("There's a #bOrbis Magic Spot#k that'll enable you to teleport to where #bEl Nath Magic Spot#k is, but you can't activate it without the scroll.");
		}
		else
		{
			send_yes_no("You can use #bOrbis Rock Scroll#k to activate #bOrbis Magic Spot#k. Will you teleport to where #bEl Nath Magic Spot#k is?");
		}
	}

	if (get_state() == 1)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			remove_item(4001019, 1);
			set_map(200082100);
		}
	}
}
