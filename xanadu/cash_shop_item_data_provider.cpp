//

#include "cash_shop_item_data_provider.hpp"

#include "cash_item_data.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

// singleton

CashShopItemDataProvider *CashShopItemDataProvider::singleton_ = nullptr;

CashShopItemDataProvider *CashShopItemDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new CashShopItemDataProvider();
	}

	return singleton_;
}

void CashShopItemDataProvider::load_data()
{
	// this shows how to use the loader

	// WZ::Node nn = wz_reader->Base[wzfilename][.img name][name of value][(sometimes only) sub names of value][value (int/string or so return)]

	// Etc.wz contains cash commodity data
	// in Commodity.img
	// name of value starts with 0 , it's just increasing
	// return values:
	// SN (Serial Number)
	// Priority (useless)
	// Price
	// Period (useless)
	// OnSale (useless)
	// ItemId
	// Gender (useless)
	// Count

	WZMain *wz_reader = World::get_instance()->wz_reader_;

	WZNode node1 = wz_reader->base_["Etc"]["Commodity"];

	for (auto it : node1)
	{
		std::string valuename = it.name();

		WZNode node2 = wz_reader->base_["Etc"]["Commodity"][valuename.c_str()];

		CashItemData *item = (CashItemData*)malloc(sizeof(CashItemData));
		memset(item, 0, sizeof(CashItemData));

		for (auto it2 : node2)
		{
			std::string propertyname = it2.name();

			if (propertyname == "Count")
			{
				item->count = it2.get_int_value();
			}

			else if (propertyname == "SN")
			{
				item->id = it2.get_int_value();
			}

			else if (propertyname == "Price")
			{
				item->price = it2.get_int_value();
			}

			else if (propertyname == "ItemId")
			{
				item->item_id = it2.get_int_value();
			}
		}

		items_[item->id] = item;
	}
}

CashItemData *CashShopItemDataProvider::get_cash_item_data(int serial_number)
{
	if (items_.find(serial_number) == items_.end())
	{
		return nullptr;
	}

	return items_[serial_number];
}
