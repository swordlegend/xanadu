//

#include "skill_data.hpp"

#include "skill_level_data.hpp"

// constructor

SkillData::SkillData(int id) :
	id_(id)
{
}

int SkillData::get_id()
{
	return id_;
}

void SkillData::add_skill_level(SkillLevelData *data)
{
	skill_levels_[data->id] = data;
}

SkillLevelData *SkillData::get_skill_level(int skill_level)
{
	return skill_levels_[skill_level];
}
