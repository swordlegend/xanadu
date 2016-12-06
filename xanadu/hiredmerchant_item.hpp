//

#pragma once

#include <memory>

#include "item.hpp"

class HiredMerchantItem
{
public:

	// constructor
	HiredMerchantItem(std::shared_ptr<Item> item, short bundles, short quantity, short slot, int price);

	// default constructor
	HiredMerchantItem() = delete;

	// copy constructor
	HiredMerchantItem(const HiredMerchantItem &other) = delete;

	// move constructor
	HiredMerchantItem(HiredMerchantItem &&other) = delete;

	// destructor
	~HiredMerchantItem() = default;

	// copy assignment operator
	HiredMerchantItem &operator=(const HiredMerchantItem &other) = delete;

	// move assignment operator
	HiredMerchantItem &operator=(HiredMerchantItem &&other) = delete;

	void set_bundles(short bundles);
	std::shared_ptr<Item> get_item();
	short get_bundles();
	short get_quantity();
	short get_slot();
	int get_price();

private:

	std::shared_ptr<Item> item_;
	short bundles_;
	short quantity_;
	short slot_;
	int price_;
};
