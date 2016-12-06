//

#include "shop_data.hpp"

// constructor

ShopData::ShopData(int id, int npc_id) :
	id_(id),
	npc_id_(npc_id)
{
}

int ShopData::get_id()
{
	return id_;
}

int ShopData::get_npc_id()
{
	return npc_id_;
}

std::unordered_map<int, ShopItemData *> *ShopData::get_shop_items()
{
	return &shop_items_;
}

ShopItemData *ShopData::get_shop_item(int item_id)
{
	return shop_items_[item_id];
}

void ShopData::add_shop_item(ShopItemData *shop_item)
{
	shop_items_[shop_item->id] = shop_item;
}
