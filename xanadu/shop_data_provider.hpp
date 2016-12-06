//

#pragma once

#include <unordered_map>

class ShopData;
struct ShopItemData;

class ShopDataProvider
{
public:

	// default constructor
	ShopDataProvider() = default;

	// copy constructor
	ShopDataProvider(const ShopDataProvider &other) = delete;

	// move constructor
	ShopDataProvider(ShopDataProvider &&other) = delete;

	// destructor
	~ShopDataProvider() = default;

	// copy assignment operator
	ShopDataProvider &operator=(const ShopDataProvider &other) = delete;

	// move assignment operator
	ShopDataProvider &operator=(ShopDataProvider &&other) = delete;

	static ShopDataProvider *get_instance();
	void load_data();
	ShopData *get_shop_data(int shop_id);

private:

	static ShopDataProvider *singleton_;
	std::unordered_map<int, ShopItemData *> shop_items_;
	std::unordered_map<int, ShopData *> shops_;
};
