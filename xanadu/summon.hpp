//

#pragma once

class Summon
{
public:

	// default constructor
	Summon(int skill_id, signed char skill_level, int hp, short position_x, short position_y);

	// default constructor
	Summon() = delete;

	// copy constructor
	Summon(const Summon &other) = delete;

	// move constructor
	Summon(Summon &&other) = delete;

	// destructor
	~Summon() = default;

	// copy assignment operator
	Summon &operator=(const Summon &other) = delete;

	// move assignment operator
	Summon &operator=(Summon &&other) = delete;

	signed char skill_level_;
	short position_x_;
	short position_y_;
	int object_id_;
	int skill_id_;
	int hp_;
};

signed char summon_get_movement_type(int skill_id);
signed char summon_get_summon_type(int skill_id);
