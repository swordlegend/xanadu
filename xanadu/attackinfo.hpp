//

#pragma once

#include <vector>
#include <unordered_map>

// holds information of a player attack for usage in
// player attack handlers/packets

class PlayerAttackInfo
{
public:

	// default constructor
	PlayerAttackInfo() = default;

	// copy constructor
	PlayerAttackInfo(const PlayerAttackInfo &other) = delete;

	// move constructor
	PlayerAttackInfo(PlayerAttackInfo &&other) = delete;

	// destructor
	~PlayerAttackInfo() = default;

	// copy assignment operator
	PlayerAttackInfo &operator=(const PlayerAttackInfo &other) = delete;

	// move assignment operator
	PlayerAttackInfo &operator=(PlayerAttackInfo &&other) = delete;
	
	signed char info_byte_;
	signed char skill_level_;
	signed char weapon_speed_;
	signed char direction_;
	signed char stance_;
	int player_id_;
	int skill_id_;
	int item_id_;
	int charge_;
	std::unordered_map<int, std::vector<int>> damages_;
};
