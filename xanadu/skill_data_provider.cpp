//

#include "skill_data_provider.hpp"

#include "skill_data.hpp"
#include "skill_level_data.hpp"
#include "world.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"

// singleton

SkillDataProvider *SkillDataProvider::singleton_ = nullptr;

SkillDataProvider *SkillDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new SkillDataProvider();
	}

	return singleton_;
}

void SkillDataProvider::load_data()
{
	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Skill"];

	for (auto it1 : node1)
	{
		std::string valuename1 = it1.name();

		int job_id = -1;

		// additionally to the job skill xml's, there is some info stored in Skill.wz
		// which is not needed here, so the exception which is thrown when trying to
		// get the job id is handled locally here to continue processing the loop

		try
		{
			job_id = std::stoi(valuename1);
		}

		catch (...)
		{
			continue;
		}

		WZNode node2 = wz_reader->base_["Skill"][valuename1.c_str()]["skill"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			int skill_id = std::stoi(valuename2);

			SkillData *skill = new SkillData(skill_id);

			WZNode node3 = wz_reader->base_["Skill"][valuename1.c_str()]["skill"][valuename2.c_str()]["level"];

			for (auto it3 : node3)
			{
				std::string valuename3 = it3.name();

				int level = std::stoi(valuename3);

				if (level > 30)
				{
					continue;
				}

				SkillLevelData *level_data = static_cast<SkillLevelData*>(malloc(sizeof(SkillLevelData)));
				memset(level_data, 0, sizeof(SkillLevelData));

				level_data->id = level;

				WZNode node4 = wz_reader->base_["Skill"][valuename1.c_str()]["skill"][valuename2.c_str()]["level"][valuename3.c_str()];

				for (auto it4 : node4)
				{
					std::string valuename4 = it4.name();

					if (valuename4 == "time")
					{
						level_data->time = it4.get_int_value();
						level_data->time *= 1000; // WHY IS THIS NEEDED? TODO FIX
					}

					if (valuename4 == "cooltime")
					{
						level_data->cooldown_time = it4.get_int_value();
					}

					if (valuename4 == "hpCon")
					{
						level_data->hp_cost = it4.get_int_value();
					}

					if (valuename4 == "mpCon")
					{
						level_data->mp_cost = it4.get_int_value();
					}

					if (valuename4 == "hp")
					{
						level_data->hp_percent = it4.get_int_value();
					}

					if (valuename4 == "prop")
					{
						level_data->prop = it4.get_int_value();
					}

					if (valuename4 == "pad")
					{
						level_data->weapon_atk = it4.get_int_value();
					}

					if (valuename4 == "pdd")
					{
						level_data->weapon_def = it4.get_int_value();
					}

					if (valuename4 == "mad")
					{
						level_data->magic_atk = it4.get_int_value();
					}

					if (valuename4 == "mdd")
					{
						level_data->magic_def = it4.get_int_value();
					}

					if (valuename4 == "acc")
					{
						level_data->accuracy = it4.get_int_value();
					}

					if (valuename4 == "eva")
					{
						level_data->avoid = it4.get_int_value();
					}

					if (valuename4 == "speed")
					{
						level_data->speed = it4.get_int_value();
					}

					if (valuename4 == "jump")
					{
						level_data->jump = it4.get_int_value();
					}

					if (valuename4 == "itemCon")
					{
						level_data->required_item_id = it4.get_int_value();
					}

					if (valuename4 == "itemConNo")
					{
						level_data->required_item_count = it4.get_int_value();
					}

					if (valuename4 == "x")
					{
						level_data->x = it4.get_int_value();
					}

					if (valuename4 == "y")
					{
						level_data->y = it4.get_int_value();
					}

					if (valuename4 == "rb")
					{
						WZNode node5 = wz_reader->base_["Skill"][valuename1.c_str()]["skill"][valuename2.c_str()]["level"][valuename3.c_str()][valuename4.c_str()];

						for (auto it5 : node5)
						{
							std::string valuename5 = it5.name();

							if (valuename5 == "x")
							{
								level_data->rbx = it5.get_int_value();
							}

							if (valuename5 == "y")
							{
								level_data->rby = it5.get_int_value();
							}
						}
					}

					if (valuename4 == "lt")
					{
						WZNode node5 = wz_reader->base_["Skill"][valuename1.c_str()]["skill"][valuename2.c_str()]["level"][valuename3.c_str()][valuename4.c_str()];

						for (auto it5 : node5)
						{
							std::string valuename5 = it5.name();

							if (valuename5 == "x")
							{
								level_data->ltx = it5.get_int_value();
							}

							if (valuename5 == "y")
							{
								level_data->lty = it5.get_int_value();
							}
						}
					}
				}

				skill->add_skill_level(level_data);
			}

			skills_[skill_id] = skill;
		}
	}
}

SkillData *SkillDataProvider::get_skill_data(int skill_id)
{
	if (skills_.find(skill_id) == skills_.end())
	{
		return nullptr;
	}

	return skills_[skill_id];
}

std::unordered_map<int, SkillData *> *SkillDataProvider::get_data()
{
	return &skills_;
}
