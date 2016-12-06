//

#include "summon.hpp"

#include "world.hpp"
#include "tools.hpp"

// constructor

Summon::Summon(int skill_id, signed char skill_level, int hp, short position_x, short position_y)
{
	skill_id_ = skill_id;
	skill_level_ = skill_level;
	hp_ = hp;
	position_x_ = position_x;
	position_y_ = position_y;
	object_id_ = World::get_instance()->get_summon_id();
}

signed char summon_get_movement_type(int skill_id)
{
	enum movement_types
	{
		kNoMovement = 0,
		kFollow = 1,
		kWalkNoMovement = 2,
		kFlyingFollow = 4
	};

	switch (skill_id)
	{
	case 3111002: // Puppet (Ranger)
	case 3211002: // Puppet (Sniper)
	case 13111004: // Puppet (Wind Archer)
	case 5211001: // Octopus (Outlaw)
	case 5220002: // Wrath of the Octopi (Corsair)
	case 4341006: // Mirrored Target (Blade Master)
	case 4111007: // Dark Flare
	case 4211007: // Dark Flare
	case 35111002: // Mechanic Magnetic Field
	case 35121003: // Giant Robot SG-88
	case 35111005: // Acceleration Bot EX-7
	case 35111011: // Healing Robot H-LX
	case 35121009: // Bots 'n Tots
	case 36121002: // Hologram Graffitti: Cut Through
	case 36121013: // Xenon: Hologram Graffitti: Stationmaster
	case 36121014: // Xenon: Hologram Graffitti: Back-up
		return kNoMovement;
	case 32111006: // Summon Reaper Buff
		return kWalkNoMovement;
	case 2311006: // Summon Dragon
	case 3111005: // Silver Hawk
	case 3211005: // Golden Eagle
	case 3121006: // Phoenix
	case 3221005: // Frostprey
	case 5211002: // Gaviota
	case 33101011: // Silver Hawk
	case 33111005: // Silver Hawk
		return kFlyingFollow;
	default:
		return kFollow;
	}
}

signed char summon_get_summon_type(int skill_id)
{
	if ((skill_id != 33111003 && skill_id != 3120012 && skill_id != 3220012 && tools::is_puppet(skill_id)) || skill_id == 33101008 || skill_id == 35111002)
	{
		return 0;
	}

	switch (skill_id)
	{
	case 1321007: // Beholden
	case 36121014: // Xenon: Hologram Graffitti: Back-up
		return 2; // buffs and stuff
	case 35111001: // Satellite
	case 35111009: // Satellite
	case 35111010: // Satellite
	case 23111008:
	case 23111009:
	case 23111010:
	case 36121002: // Xenon: Hologram Graffitti: Cut Through
		return 3; // attacks what you attack
	case 35121009: // Bots 'n Tots
		return 5; // sub summons
	case 35121003: // Giant Robot SG-88
		return 6; // charge
	case 4111007: // Dark Flare
	case 4211007: // Dark Flare
		return 7; // attacks what you get hit by
	default:
		return 1;
	}
}
