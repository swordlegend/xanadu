//

#include "../player.hpp"

void Player::handle_orbis_platform_usher()
{
	switch (get_state())
	{
	case 0:
	{
		std::string text = "Orbis Station has a lot of platforms available to choose from. You need to choose the one that'll take you to the destination of your choice. Which platform will you take?";
		text.append(" \r\n#L1#  The platform to the ship that heads to Ellinia#l");
		text.append(" \r\n#L2#  The platform to the ship that heads to Ludibrium#l");
		text.append(" \r\n#L3#  The platform to the ship that heads to Leafre#l");
		text.append(" \r\n#L4#  The platform to the ship that heads to Mu Lung#l");
		text.append(" \r\n#L5#  The platform to the ship that heads to Ariant#l");

		send_simple(text);

		break;
	}
	case 1:
	{
		switch (get_selected())
		{
		case 1:
			set_map(200000110);
			break;
		case 2:
			set_map(200000120);
			break;
		case 3:
			set_map(200000130);
			break;
		case 4:
			set_map(200000140);
			break;
		case 5:
			set_map(200000150);
			break;
		}
	}
	}
}
