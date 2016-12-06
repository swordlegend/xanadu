//

#pragma once

#include <string>
#include <memory>
#include <unordered_map>

class HiredMerchantItem;
class Player;
class Item;

class HiredMerchant
{
public:

	// constructor
	HiredMerchant(Player *player, int item_id, const std::string &description);

	// default constructor
	HiredMerchant() = delete;

	// copy constructor
	HiredMerchant(const HiredMerchant &other) = delete;

	// move constructor
	HiredMerchant(HiredMerchant &&other) = delete;

	// destructor
	~HiredMerchant() = default;

	// copy assignment operator
	HiredMerchant &operator=(const HiredMerchant &other) = delete;

	// move assignment operator
	HiredMerchant &operator=(HiredMerchant &&other) = delete;

	void add_item(std::shared_ptr<HiredMerchantItem> item);
	void add_visitor(Player *player);
	void buy_item(Player *player, short slot, short quantity);
	void remove_all_visitors();
	void remove_item(short slot);
	void remove_visitor(Player *player);
	void return_item(short slot, Player *owner);
	void room_chat(signed char slot, std::string &chat);
	void set_open(bool open);
	bool is_open();
	bool is_owner(int id);
	signed char get_empty_visitor_slot();
	signed char get_playerslot(int playerId);
	short get_position_x();
	short get_position_y();
	int get_id();
	int get_item_id();
	int get_owner_id();
	int get_merchant_mesos();
	size_t get_num_items();
	std::string get_owner_name();
	std::string get_description();
	Player *get_visitor(signed char slot);
	std::shared_ptr<HiredMerchantItem> get_item(short slot);
	std::unordered_map<int, std::shared_ptr<HiredMerchantItem>> *get_items();

private:

	bool open_;
	short pos_x_;
	short pos_y_;
	int id_;
	int item_id_;
	int owner_id_;
	int mesos_;
	std::string owner_name_;
	std::string description_;
	std::unordered_map<int, Player *> visitors_;
	std::unordered_map<int, std::shared_ptr<HiredMerchantItem>> items_;
};
