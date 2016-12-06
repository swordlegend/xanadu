//

#include "../player.hpp"
#include "../constants.hpp"

void Player::handle_papulatus_npc()
{
	if (get_state() == 0)
	{
		send_yes_no("Beep... beep... you can make your escape to a safer place through me. Beep... beep... would you like to leave this place?");
	}

	if (get_state() == 1)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			set_map(220080000);
		}
	}
}
