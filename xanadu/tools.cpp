//

#include "tools.hpp"

#include <ctime>
#include <random>

namespace tools
{
	int random_int(int min, int max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> dist(min, max); // distribution in range [min, max]

		return dist(rng);
	}

	int get_distance(short pos1_x, short pos1_y, short pos2_x, short pos2_y)
	{
		return static_cast<int>((pos1_x - pos2_x) * (pos1_x - pos2_x) + (pos1_y - pos2_y) * (pos1_y - pos2_y));
	}

	long long time_to_tick()
	{
		time_t time_stamp = time(nullptr); // unix time in seconds
		time_stamp *= 1000; // convert to milliseconds
		time_stamp *= 10000; // convert to 100-nanonseconds
		time_stamp += 116444592000000000; // change timezone from UTC to EDT
		return time_stamp;
	}

	bool is_star(int item_id)
	{
		return (item_id / 10000 == 207 || item_id / 10000 == 233);
	}

	signed char get_inventory_id_from_item_id(int item_id)
	{
		return (item_id / 1000000);
	}

	bool is_monster_riding(int item_id)
	{
		return (item_id == 1004);
	}

	bool skill_id_is_summon(int skill_id)
	{
		switch (skill_id)
		{
		case 3111002:
		case 3211002:
		case 3120012:
		case 3220012:
		case 1321007:
		case 2121005:
		case 2221005:
		case 2321003:
		case 3101007:
		case 3201007:
		case 3111005:
		case 3211005:
		case 4111007:
		case 4211007:
			// Ranger Puppet, Sniper Puppet, Bowmaster Elite Puppet, Marksman Elite Puppet, Beholden, Ifrit, Elquines, Bahamut, Silver Hawk, Golden Eagle, Eagle Eye, Frostprey, Dark Flare, Dark Flare
			return true;
		default:
			return false;
		}
	}

	bool is_puppet(int skill_id)
	{
		if (skill_id == 3111002 || skill_id == 3211002 || skill_id == 3120012 || skill_id == 3220012) // Ranger Puppet, Sniper Puppet, Bowmaster Elite Puppet, Marksman Elite Puppet
		{
			return true;
		}

		return false;
	}

	bool skill_id_is_special_skill(int skill_id)
	{
		switch (skill_id)
		{
		case 2321001:
		case 2111002:
		case 3121004:
		case 3221001:
		case 5221004:
			// Big Bang, Explosion, Hurricane, Piercing Arrow, Rapid Fire
			return true;
		default:
			return false;
		}
	}

	short get_job_id_from_skill_id(int skill_id)
	{
		return (skill_id / 10000);
	}

	bool is_fourth_job(int skill_id)
	{
		return ((skill_id / 10000) % 10) == 2;
	}

}
