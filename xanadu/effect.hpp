//

#pragma once

#include <unordered_map>

#include "buffvalues.hpp"
#include "tools.hpp"

class Player;
struct ItemData;
struct SkillLevelData;

class Effect
{
public:

	// constructors
	Effect(int id, ItemData *data);
	Effect(int id, SkillLevelData *data);

	// default constructor
	Effect() = delete;

	// copy constructor
	Effect(const Effect &other) = delete;

	// move constructor
	Effect(Effect &&other) = delete;

	// destructor
	~Effect() = default;

	// copy assignment operator
	Effect &operator=(const Effect &other) = delete;

	// move assignment operator
	Effect &operator=(Effect &&other) = delete;

	int get_id();
	Values *get_values();

	void use_attack_skill(Player *player);
	void use_skill(Player *player);
	void use_item(Player *player);

	static Effect *get_item_effect(int id);
	static Effect *get_skill_effect(int id, signed char level);

private:

	signed char level_;
	int id_;
	Values values_;
	SkillLevelData *skill_data_;
	ItemData *item_data_;

	static std::unordered_map<int, Effect *> item_effects_;
	static std::unordered_map<int, std::unordered_map<signed char, Effect *>> skill_effects_;
};
