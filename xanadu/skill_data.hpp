//

#pragma once

#include <unordered_map>

struct SkillLevelData;

class SkillData
{
public:

	// constructor
	SkillData(int id);

	// default constructor
	SkillData() = delete;

	// copy constructor
	SkillData(const SkillData &other) = delete;

	// move constructor
	SkillData(SkillData &&other) = delete;

	// destructor
	~SkillData() = default;

	// copy assignment operator
	SkillData &operator=(const SkillData &other) = delete;

	// move assignment operator
	SkillData &operator=(SkillData &&other) = delete;

	int get_id();
	void add_skill_level(SkillLevelData *data);
	SkillLevelData *get_skill_level(int skill_level);

private:

	int id_;
	std::unordered_map<int, SkillLevelData *> skill_levels_;
};
