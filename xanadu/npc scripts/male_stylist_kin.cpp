//

#include "../player.hpp"

void Player::handle_kin_npc()
{
	int skins[] = { 0, 1, 2, 3, 4 };
	int eyes[] = { 20000, 20001, 20002, 20003, 20004, 20005, 20006, 20007, 20008, 20009, 20010, 20011, 20012, 20013, 20014, 20016, 20017, 20018, 20019, 20020, 20021, 20022, 20023, 20024, 20025, 20026 };
	int hair[] = { 30000, 30010, 30020, 30030, 30040, 30050, 30060, 30070, 30080, 30090, 30100, 30110, 30120, 30130, 30140, 30150, 30160, 30170, 30180, 30190, 30200, 30210, 30220, 30230, 30240, 30250, 30260, 30270, 30280, 30290, 30300, 30310, 30320, 30330, 30340, 30350, 30360, 30370, 30400, 30410, 30420, 30430, 30440, 30450, 30460, 30470, 30480, 30490, 30510, 30520, 30530, 30540, 30550, 30560, 30570, 30580, 30590, 30600, 30610, 30620, 30630, 30640, 30650, 30660, 30670, 30680, 30690, 30700, 30710, 30720, 30730, 30740, 30750, 30760, 30770, 30780, 30790, 30800, 30810, 30820 };

	if (get_state() == 0)
	{
		send_simple("Hey there! I am the male stylist. How would you like to change your looks today?\r\n #b#L0#Hair Color#l\r\n#L1#Eyes Color#l\r\n#L2#Hair#l\r\n#L3#Eyes#l\r\n#L4#Skin Color#l#k");
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
			int cur = (face_ % 100 + 20000);
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
		set_style(20000 + (face_ % 100) + (get_selected() * 100));
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
