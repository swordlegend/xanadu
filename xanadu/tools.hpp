//

#pragma once

namespace tools
{
	int random_int(int min, int max);
	int get_distance(short pos1_x, short pos1_y, short pos2_x, short pos2_y);
	long long time_to_tick();
	bool is_star(int item_id);
	signed char get_inventory_id_from_item_id(int item_id);
	bool is_monster_riding(int item_id);
	bool skill_id_is_summon(int skill_id);
	bool is_puppet(int skill_id);
	bool skill_id_is_special_skill(int skill_id);
	short get_job_id_from_skill_id(int skill_id);
	bool is_fourth_job(int skill_id);
}
