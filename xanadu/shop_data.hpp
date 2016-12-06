//

#pragma once

#include <unordered_map>

#include "shop_item_data.hpp"

class ShopData
{
public:

	// constructor
	ShopData(int id, int npc_id);

	// default constructor
	ShopData() = delete;

	// copy constructor
	ShopData(const ShopData &other) = delete;

	// move constructor
	ShopData(ShopData &&other) = delete;

	// destructor
	~ShopData() = default;

	// copy assignment operator
	ShopData &operator=(const ShopData &other) = delete;

	// move assignment operator
	ShopData &operator=(ShopData &&other) = delete;

	int get_id();
	int get_npc_id();
	std::unordered_map<int, ShopItemData *> *get_shop_items();
	ShopItemData *get_shop_item(int item_id);
	void add_shop_item(ShopItemData *shop_item);

private:

	int id_;
	int npc_id_;
	std::unordered_map<int, ShopItemData *> shop_items_;
};
