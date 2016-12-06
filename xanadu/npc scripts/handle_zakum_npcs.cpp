//

#include "../player.hpp"

#include "../map.hpp"
#include "../constants.hpp"

void Player::handle_amon_zakum_npc()
{
	if (map_->get_id() == 280030000)
	{
		if (get_state() == 0)
		{
			send_yes_no("Do you want to get out of here?");
		}
		if (get_state() == 1)
		{
			if (get_selected() == kNpcConstantsYes)
			{
				set_map(211042400);
			}
		}
	}
}

void Player::handle_adobis_zakum_npc()
{
	if (get_state() == 0)
	{
		send_yes_no("Would you like to enter #bZakum's Altar#k?");
	}

	else if (get_state() == 1)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			set_map(280030000);
		}
		else
		{
			send_ok("Come to me when you want to face #bZakum#k.");
		}
	}
}
