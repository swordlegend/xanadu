//

#include "../player.hpp"

#include "../constants.hpp"

void Player::handle_donator_npc() //NPC 9000032 Agent W
{
	switch (get_state())
	{
	case 0:
	{
		std::string text = "Hi! I am the Donator NPC in Ellin! You currently have " + std::to_string(donation_points_) + " Donation Points. How can I serve you?";

		text.append(" \r\n#L1#  Exchange 1.000 Donation Points into Red Luck Sack#l");
		text.append(" \r\n#L2#  Exchange 1.800 Donation Points into 7 days 2x EXP coupon#l");
		text.append(" \r\n#L3#  Exchange 1.600 Donation Points into 7 days 2x Drop/Mesos coupon#l");
		text.append(" /r/n#L4#  Exchange 1.000 Donation Points into 3 days Pet Vac#1");
		text.append(" /r/n#L5#  Exchange 7.000 Donation Points into 30 days Pet Vac#1");
		text.append(" /r/n#L6#  Exchange 600 Donation Points into 3 days Clone#1");


		send_simple(text);

		break;
	}
	case 1:
	{
		switch (get_selected())
		{
		case 1: // Red Luck Sack
		{
			if (donation_points_ >= 1000)
			{
				donation_points_ -= 1000;
				give_item(3993003, 1);
				send_ok("You gained Red Luck Sack.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		case 2: // 2x EXP coupon 7 days
		{
			if (donation_points_ >= 1800)
			{
				donation_points_ -= 1800;
				give_item(5211046, 1);
				send_ok("You gained 2x EXP Coupon 7 days.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		case 3: // 2x DROP/Mesos coupon 7 days
		{
			if (donation_points_ >= 1600)
			{
				donation_points_ -= 1600;
				give_item(5360000, 1);
				send_ok("You gained 2x Drop/Meso coupon 7 days.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		case 4: // 3 days Pet Vac
		{
			if (donation_points_ >= 1000)
			{
				donation_points_ -= 1000;
				give_item(4030003, 1);
				send_ok("You gained 3 days Pet Vac.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		case 5: // 30 days Pet Vac
		{
			if (donation_points_ >= 7000)
			{
				donation_points_ -= 7000;
				give_item(4030003, 1);
				send_ok("You gained 30 days Pet Vac.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		case 6: // 3 days clone
		{
			if (donation_points_ >= 600)
			{
				donation_points_ -= 600;
				give_item(4030004, 1);
				send_ok("You gained 3 days Clone.");
			}
			else
			{
				send_ok("You do not have enough Donation Points for this exchange option.");
			}
			break;
		}
		}
	}
	}
}
