//

#pragma once

#include <vector>

class MobData
{
public:

	// default constructor
	MobData() = default;

	// copy constructor
	MobData(const MobData&) = delete;

	// move constructor
	MobData(MobData&&) = delete;

	// destructor
	~MobData() = default;

	// copy assignment operator
	MobData &operator=(const MobData&) = delete;

	// move assignment operator
	MobData &operator=(MobData&&) = delete;

	bool is_boss_;
	unsigned char level_;
	signed char hp_color_;
	signed char hp_background_color_;
	int id_;
	int max_hp_;
	int max_mp_;
	int exp_;
	std::vector<int> mob_summons_;
};
