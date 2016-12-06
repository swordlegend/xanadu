//

#include "../player.hpp"
#include "../constants.hpp"

void Player::handle_regular_cab_npc()
{
	int maps[] = {104000000, 102000000, 101000000, 103000000, 100000000};
	int prices[] = {800, 1000, 1000, 1200, 1000};

	if (get_state() == 0)
	{
		send_simple("Please choose where you would like to go: \r\n#b#L0##m" + std::to_string(maps[0]) + "#(" + std::to_string(prices[0]) + " mesos)#l \r\n#L1##m" + std::to_string(maps[1]) + "#(" + std::to_string(prices[1]) + " mesos)#l \r\n#L2##m" + std::to_string(maps[2]) + "#(" + std::to_string(prices[2]) + " mesos)#l \r\n#L3##m" + std::to_string(maps[3]) + "#(" + std::to_string(prices[3]) + " mesos)#l \r\n#L4##m" + std::to_string(maps[4]) + "#(" + std::to_string(prices[4]) + " mesos)#l");
	}

	else if (get_state() == 1)
	{
		set_npc_variable("city", get_selected());
		send_yes_no("You don't have anything else to do here, huh? Do you really want to go to #b#m" + std::to_string(maps[get_selected()]) + "##k? it will cost you #b" + std::to_string(prices[get_selected()]) + " mesos#k.");
	}

	else if (get_state() == 2)
	{
		if (get_selected() == kNpcConstantsYes)
		{
			set_selected(get_npc_variable("city"));
			if (mesos_ >= prices[get_selected()])
			{
				add_mesos(-prices[get_selected()]);
				set_map(maps[get_selected()]);
			}
			else
			{
				send_ok("You need #b" + std::to_string(prices[get_selected()]) + " mesos#k to go to #m" + std::to_string(maps[get_selected()]) + "#.");
			}
		}
		else
		{
			send_ok("There's a lot to see in this town, too. Come back and find me when you need to go to a different town.");
		}
	}
}
