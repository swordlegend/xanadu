//

#include "shop_data_provider.hpp"
#include "shop_data.hpp"
#include "shop_item_data.hpp"

#include "world.hpp"

#include "Poco\Data\RecordSet.h"

// singleton

ShopDataProvider *ShopDataProvider::singleton_ = nullptr;

ShopDataProvider *ShopDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new ShopDataProvider();
	}

	return singleton_;
}

void ShopDataProvider::load_data()
{
	// get the shop id's

	std::vector<int> shop_ids;

	Poco::Data::Session session = World::get_instance()->get_mysql_session();

	Poco::Data::Statement statement1(session);
	statement1 << "SELECT shopid FROM shopdata", Poco::Data::Keywords::into(shop_ids);
	statement1.execute();

	// get the shop info based on each shop id

	for (auto shop_id : shop_ids)
	{
		Poco::Data::Statement statement2(session);
		statement2 << "SELECT npcid FROM shopdata WHERE shopid = " << shop_id;
		statement2.execute();

		Poco::Data::RecordSet recordset2(statement2);

		int npc_id = recordset2["npcid"];
		ShopData *shop = new ShopData(shop_id, npc_id);

		Poco::Data::Statement statement3(session);
		statement3 << "SELECT itemid, price FROM rechargedata";
		statement3.execute();

		Poco::Data::RecordSet recordset3(statement3);

		for (size_t col = 0; col < recordset3.rowCount(); ++col)
		{
			int item_id = recordset3["itemid"];
			int price = recordset3["price"];

			ShopItemData *shop_item_data = new ShopItemData();
			shop_item_data->id = item_id;
			shop_item_data->price = price;

			shop->add_shop_item(shop_item_data);

			recordset3.moveNext();
		}

		Poco::Data::Statement statement4(session);
		statement4 << "SELECT itemid, price FROM shopitemdata WHERE shopid = " << shop_id << " ORDER BY sort DESC";
		statement4.execute();

		Poco::Data::RecordSet recordset4(statement4);

		for (size_t col = 0; col < recordset4.rowCount(); ++col)
		{
			int item_id = recordset4["itemid"];
			int price = recordset4["price"];

			ShopItemData *shop_item_data = new ShopItemData();
			shop_item_data->id = item_id;
			shop_item_data->price = price;

			shop->add_shop_item(shop_item_data);

			recordset4.moveNext();
		}

		shops_[shop_id] = shop;
	}
}

ShopData *ShopDataProvider::get_shop_data(int shop_id)
{
	if (shops_.find(shop_id) == shops_.end())
	{
		return nullptr;
	}

	return shops_[shop_id];
}
