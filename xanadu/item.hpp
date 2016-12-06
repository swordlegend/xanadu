//

#pragma once

#include <string>

class Player;

class Item
{
public:

	// constructor
	Item(int item_id, bool random_equip_stats = false);

	// default constructor
	Item() = delete;

	// copy constructor
	Item(const Item &other) = default;

	// move constructor
	Item(Item &&other) = default;

	// destructor
	~Item() = default;

	// copy assignment operator
	Item &operator=(const Item &other) = delete;

	// move assignment operator
	Item &operator=(Item &&other) = delete;

	bool is_star();
	bool is_pet();
	bool is_equip();
	signed char get_inventory_id();
	signed char get_slot();
	short get_flag();
	short get_amount();
	int get_item_id();
	int get_unique_id();
	long long get_expiration_time();
	std::string get_name();
	std::string get_owner_name();
	void set_slot(signed char value);
	void set_amount(short value);
	void set_flag(short value);
	void set_item_id(int value);
	// pet specific
	signed char get_stance();
	signed char get_pet_level();
	signed char get_pet_slot();
	signed char get_pet_fullness();
	short get_pet_closeness();
	short get_position_x();
	short get_position_y();
	void set_pet_level(signed char level);
	void set_pet_slot(signed char slot);
	void set_closeness(short closeness, Player * player);
	void set_closeness(short closeness);
	void set_position(short x, short y, signed char stance);
	// equip specific
	void set_free_slots(unsigned char slots);
	unsigned char get_free_slots();
	void set_used_scrolls(unsigned char used_scrolls);
	unsigned char get_used_scrolls();
	signed char get_boss_damage();
	void set_str(short str);
	short get_str();
	void set_dex(short dex);
	short get_dex();
	void set_int(short intt);
	short get_int();
	void set_luk(short luk);
	short get_luk();
	void set_hp(short hp);
	short get_hp();
	void set_mp(short mp);
	short get_mp();
	void set_weapon_attack(short weapon_atk);
	short get_weapon_attack();
	void set_magic_attack(short magic_atk);
	short get_magic_attack();
	void set_weapon_defense(short weapon_def);
	short get_weapon_defense();
	void set_magic_defense(short magic_def);
	short get_magic_defense();
	void set_acc(short acc);
	short get_acc();
	void set_avoid(short avo);
	short get_avoid();
	void set_hand(short hand);
	short get_hand();
	void set_speed(short speed);
	short get_speed();
	void set_jump(short jump);
	short get_jump();

private:

	signed char slot_;
	short flag_;
	short amount_;
	int item_id_;
	int unique_id_;
	std::string name_;
	// pet specific
	signed char stance_;
	signed char pet_level_;
	signed char pet_slot_;
	short closeness_;
	short position_x_;
	short position_y_;
	// equip specific
	unsigned char slots_;
	unsigned char used_scrolls_;
	short str_;
	short dex_;
	short intt_;
	short luk_;
	short hp_;
	short mp_;
	short weapon_atk_;
	short magic_atk_;
	short weapon_def_;
	short magic_def_;
	short acc_;
	short avo_;
	short hand_;
	short speed_;
	short jump_;
};
