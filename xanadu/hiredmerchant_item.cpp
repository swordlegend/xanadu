//

#include "hiredmerchant_item.hpp"

// constructor

HiredMerchantItem::HiredMerchantItem(std::shared_ptr<Item> item, short bundles, short quantity, short slot, int price) :
item_(item),
bundles_(bundles),
quantity_(quantity),
slot_(slot),
price_(price)
{
}

void HiredMerchantItem::set_bundles(short bundles)
{
	bundles_ = bundles;
}

std::shared_ptr<Item> HiredMerchantItem::get_item()
{
	return item_;
}

short HiredMerchantItem::get_bundles()
{
	return bundles_;
}

short HiredMerchantItem::get_quantity()
{
	return quantity_;
}

short HiredMerchantItem::get_slot()
{
	return slot_;
}

int HiredMerchantItem::get_price()
{
	return price_;
}
