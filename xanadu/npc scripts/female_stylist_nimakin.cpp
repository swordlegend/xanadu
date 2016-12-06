//

#include "../player.hpp"

void Player::handle_nimakin_npc()
{
	int skins[] = { 0, 1, 2, 3, 4 };
	int eyes[] = { 21000, 21001, 21002, 21003, 21004, 21005, 21006, 21007, 21008, 21009, 21010, 21011, 21012, 21013, 21014, 21016, 21017, 21018, 21019, 21020, 21022, 21023, 21024, 21025 };
	int hair[] = { 31000, 31010, 31020, 31030, 31040, 31050, 31060, 31070, 31080, 31090, 31100, 31110, 31120, 31130, 31140, 31150, 31160, 31170, 31180, 31190, 31200, 31210, 31220, 31230, 31240, 31250, 31260, 31270, 31280, 31290, 31300, 31310, 31320, 31330, 31340, 31350, 31400, 31410, 31420, 31430, 31440, 31450, 31460, 31470, 31480, 31490, 31510, 31520, 31530, 31540, 31550, 31560, 31570, 31580, 31590, 31600, 31610, 31620, 31630, 31640, 31650, 31660, 31670, 31680, 31690, 31700, 31710, 31720, 31730, 31740, 31750, 31760, 31770, 31790, 31800 };

	if (get_state() == 0)
	{
		send_simple("Hey there! I am the female stylist. How would you like to change your looks today?\r\n #b#L0#Hair Color#l\r\n#L1#Eyes Color#l\r\n#L2#Hair#l\r\n#L3#Eyes#l\r\n#L4#Skin Color#l#k");
	}

	else if (get_state() == 1)
	{
		int type = get_selected();
		if (type == 0)
		{
			int cur = (hair_ / 10 * 10);
			int colors[] = { cur, cur + 1, cur + 2, cur + 3, cur + 4, cur + 5, cur + 6, cur + 7 };

			send_style(colors, sizeof(colors) / sizeof(colors[0]));
			set_state(99);
		}
		else if (type == 1)
		{
			int cur = (face_ % 100 + 21000);
			int colors[] = { cur, cur + 100, cur + 200, cur + 300, cur + 400, cur + 500, cur + 600, cur + 700 };

			send_style(colors, sizeof(colors) / sizeof(colors[0]));
			set_state(199);
		}
		else if (type == 2)
		{
			send_style(hair, sizeof(hair) / sizeof(hair[0]));
			set_state(299);
		}
		else if (type == 3)
		{
			send_style(eyes, sizeof(eyes) / sizeof(eyes[0]));
			set_state(399);
		}
		else if (type == 4)
		{
			send_style(skins, sizeof(skins) / sizeof(skins[0]));
			set_state(499);
		}
	}
	else if (get_state() == 100)
	{
		set_style((hair_ / 10 * 10) + get_selected());
	}
	else if (get_state() == 200)
	{
		set_style(21000 + (face_ % 100) + (get_selected() * 100));
	}
	else if (get_state() == 300)
	{
		set_style(hair[get_selected()]);
	}
	else if (get_state() == 400)
	{
		set_style(eyes[get_selected()]);
	}
	else if (get_state() == 500)
	{
		set_style(skins[get_selected()]);
	}
}
