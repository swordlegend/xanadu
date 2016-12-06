//

#pragma once

#include <memory>
#include <unordered_map>

class Item;
class Player;

class Inventory
{
public:

	// constructor
	Inventory(Player* player, signed char id, signed char slots);

	// default constructor
	Inventory() = delete;

	// copy constructor
	Inventory(const Inventory &other) = delete;

	// move constructor
	Inventory(Inventory &&other) = delete;

	// destructor
	~Inventory() = default;

	// copy assignment operator
	Inventory &operator=(const Inventory &other) = delete;

	// move assignment operator
	Inventory &operator=(Inventory &&other) = delete;

	bool add_item_find_slot(std::shared_ptr<Item> item);
	bool add_item_no_find_slot(std::shared_ptr<Item> item, bool send);
	bool give_item(int item_id, int amount);
	std::shared_ptr<Item> give_item_special(bool &success, int item_id);
	signed char get_slots();
	signed char get_free_slots();
	short get_item_amount(int item_id);
	std::shared_ptr<Item> get_item_by_slot(signed char slot);
	std::unordered_map<signed char, std::shared_ptr<Item>> *get_items();
	void add_slots(signed char amount);
	void remove_item(signed char slot, bool drop = false, bool send = true);
	void remove_item(int item_id, short amount);
	void remove_item_by_slot(signed char slot, short amount, bool send = true, bool trading = false);

private:

    signed char id_;
	signed char slots_;
	Player *player_;
	std::unordered_map<signed char, std::shared_ptr<Item>> items_;
};
