//

#include "mob_data_provider.hpp"

#include "mob_data.hpp"
#include "world.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"

// singleton

MobDataProvider *MobDataProvider::singleton_ = nullptr;

MobDataProvider *MobDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new MobDataProvider();
	}

	return singleton_;
}

void MobDataProvider::load_data()
{
	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Mob"];

	for (auto it : node1)
	{
		std::string valuename = it.name();
		int mob_id = std::stoi(valuename);

		MobData *mob = new MobData();

		data_[mob_id] = mob;

		mob->exp_ = 0;
		mob->hp_background_color_ = 0;
		mob->hp_color_ = 0;
		mob->id_ = 0;
		mob->is_boss_ = false;
		mob->level_ = 0;
		mob->max_hp_ = 0;
		mob->max_mp_ = 0;

		WZNode node2 = wz_reader->base_["Mob"][valuename.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string propertyname = it2.name();
			if (propertyname == "boss")
			{
				int boss = it2.get_int_value();
				mob->is_boss_ = boss ? 1 : 0;
			}
			else if (propertyname == "exp")
			{
				mob->exp_ = it2.get_int_value();
			}
			else if (propertyname == "maxHP")
			{
				mob->max_hp_ = it2.get_int_value();
			}
			else if (propertyname == "maxMP")
			{
				mob->max_mp_ = it2.get_int_value();
			}
			else if (propertyname == "hpTagColor")
			{
				mob->hp_color_ = it2.get_int_value();
			}
			else if (propertyname == "hpTagBgcolor")
			{
				mob->hp_background_color_ = it2.get_int_value();
			}
			else if (propertyname == "level")
			{
				mob->level_ = it2.get_int_value();
			}
			else if (propertyname == "revive")
			{
				WZNode node3 = wz_reader->base_["Mob"][valuename.c_str()]["info"]["revive"];

				for (auto it3 : node3)
				{
					int summon_mob_id = it3.get_int_value();
					mob->mob_summons_.push_back(summon_mob_id);
				}
			}
		}
	}
}

MobData *MobDataProvider::get_data_by_id(int id)
{
	if (data_.find(id) != data_.end())
	{
		return data_[id];
	}

	return nullptr;
}

std::unordered_map<int, MobData *> *MobDataProvider::get_data()
{
	return &data_;
}
