//

#include "../player.hpp"

void Player::handle_horntail_entrance_npc()
{
	if (get_state() == 0)
	{
		send_simple("Welcome to Cave of Life - Entrance! Would you like to go inside and fight #rHorntail#k? If you want to fight him, you may might need some #b#v2000005##k, so you can recover some HP if you have been hit by #rHorntail#k.\r\n#L0#I would like to buy 10 for 100,000 Mesos!#l\r\n#L1#No thanks, let me in now!#l");
	}

	else if (get_state() == 1)
	{
		int type = get_selected();

		if (type == 0)
		{
			if (mesos_ >= 100000)
			{
				add_mesos(-100000);
				give_item(2000005, 10);
				send_ok("Thank you for buying the potion. Use it as well!");
			}
			else
			{
				send_ok("Sorry, you don't have enough mesos to buy them!");
			}
		}

		else if (type == 1)
		{
			set_map(240050000);
		}
	}
}
