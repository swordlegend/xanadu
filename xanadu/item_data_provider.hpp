//

#pragma once

#include <unordered_map>

struct ItemData;

class ItemDataProvider
{
public:

	// default constructor
	ItemDataProvider() = default;

	// copy constructor
	ItemDataProvider(const ItemDataProvider &other) = delete;

	// move constructor
	ItemDataProvider(ItemDataProvider &&other) = delete;

	// destructor
	~ItemDataProvider() = default;

	// copy assignment operator
	ItemDataProvider &operator=(const ItemDataProvider &other) = delete;

	// move assignment operator
	ItemDataProvider &operator=(ItemDataProvider &&other) = delete;

	static ItemDataProvider *get_instance();
	void load_data();
	ItemData *get_item_data(int item_id);
	std::unordered_map<int, ItemData *> *get_data();
	void add_item(ItemData *data);

private:

	static ItemDataProvider *singleton_;
	std::unordered_map<int, ItemData *> items_;
};
