//

#include "mob_drops_data_provider.hpp"

#include "drop_data.hpp"
#include "world.hpp"

#include "Poco\Data\RecordSet.h"

// singleton

MobDropsDataProvider *MobDropsDataProvider::singleton_ = nullptr;

MobDropsDataProvider *MobDropsDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new MobDropsDataProvider();
	}

	return singleton_;
}

void MobDropsDataProvider::load_data()
{
	// get the dropper id's

	Poco::Data::Session session = World::get_instance()->get_mysql_session();

	Poco::Data::Statement statement1(session);
	statement1 << "SELECT dropperid FROM dropdata GROUP BY dropperid";
	statement1.execute();

	Poco::Data::RecordSet recordset1(statement1);

	for (size_t col = 0; col < recordset1.rowCount(); ++col)
	{
		std::vector<DropData *> drops;

		int mob_id = recordset1["dropperid"];

		Poco::Data::Statement statement2(session);
		statement2 << "SELECT itemid, questid, chance FROM dropdata WHERE dropperid = " << mob_id << " AND itemid != 0 "; // NOTE: exclude mesos at the moment
		statement2.execute();

		Poco::Data::RecordSet recordset2(statement2);

		for (size_t row_num = 0; row_num < recordset2.rowCount(); ++row_num)
		{
			// WORKAROUND
			// not using quest item drops at the moment so they dont drop all the time

			int quest_id = recordset2["questid"];
			if (quest_id == 0)
			{
				int item_id = recordset2["itemid"];
				int chance = recordset2["chance"];

				DropData *drop_data = new DropData();
				drop_data->item_id = item_id;
				drop_data->chance = chance;

				drops.push_back(drop_data);
			}

			recordset2.moveNext();
		}

		drops_[mob_id] = drops;

		recordset1.moveNext();
	}
}

std::vector<DropData *> *MobDropsDataProvider::get_drop_data(int mob_id)
{
	if (drops_.find(mob_id) == drops_.end())
	{
		return nullptr;
	}

	return &drops_[mob_id];
}
