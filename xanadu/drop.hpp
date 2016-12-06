//

#pragma once

#include <memory>

class Item;
class Player;

class Drop
{
public:

	// constructor
	Drop(int id, signed char type, short position_x, short position_y, int mesos, int owner_player_id, std::shared_ptr<Item> item);

	// default constructor
	Drop() = delete;

	// copy constructor
	Drop(const Drop &other) = delete;

	// move constructor
	Drop(Drop &&other) = delete;

	// destructor
	~Drop() = default;

	// copy assignment operator
	Drop &operator=(const Drop &other) = delete;

	// move assignment operator
	Drop &operator=(Drop &&other) = delete;

	bool loot_drop(Player *player, signed char pet_slot);
	bool is_mesos();
	signed char get_type();
	short get_position_x();
	short get_position_y();
	int get_mesos();
	int get_owner_player_id();
	int get_id();
	unsigned long long get_time();
	std::shared_ptr<Item> get_item();

private:

	signed char type_;
	short position_x_;
	short position_y_;
	int id_;
	int mesos_;
	int owner_player_id_;
	unsigned long long time_;
	std::shared_ptr<Item> item_;
};
