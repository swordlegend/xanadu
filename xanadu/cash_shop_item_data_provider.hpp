//

#pragma once

#include <vector>
#include <unordered_map>

struct CashItemData;

class CashShopItemDataProvider
{
public:

	// default constructor
	CashShopItemDataProvider() = default;

	// copy constructor
	CashShopItemDataProvider(const CashShopItemDataProvider &other) = delete;

	// move constructor
	CashShopItemDataProvider(CashShopItemDataProvider &&other) = delete;

	// destructor
	~CashShopItemDataProvider() = default;

	// copy assignment operator
	CashShopItemDataProvider &operator=(const CashShopItemDataProvider &other) = delete;

	// move assignment operator
	CashShopItemDataProvider &operator=(CashShopItemDataProvider &&other) = delete;

	static CashShopItemDataProvider *get_instance();
	void load_data();
	CashItemData *get_cash_item_data(int serial_number);

private:

	static CashShopItemDataProvider *singleton_;
	std::unordered_map<int, CashItemData *> items_;
};
