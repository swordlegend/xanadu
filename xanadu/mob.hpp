//

#pragma once

#include <unordered_map>

class Map;
class Player;

class Mob
{
public:

	// constructor
	Mob(Map *map, int monster_id, int object_id, short position_x, short position_y, short foothold, signed char stance, int max_hp, int max_mp, bool is_boss, bool is_map_mob);

	// default constructor
	Mob() = delete;

	// copy constructor
	Mob(const Mob &other) = delete;

	// move constructor
	Mob(Mob &&other) = delete;

	// destructor
	~Mob() = default;

	// copy assignment operator
	Mob &operator=(const Mob &other) = delete;

	// move assignment operator
	Mob &operator=(Mob &&other) = delete;

	bool is_boss();
	bool is_dead();
	bool is_map_mob();
	signed char get_stance();
	short get_position_x();
	short get_position_y();
	short get_original_foothold();
	int get_object_id();
	int get_monster_id();
	int get_hp();
	int get_mp();
	int get_max_mp();
	int get_killer_player_id();
	Player *get_owner();
	void set_is_dead(bool dead);
	void set_hp(int hp);
	void set_mp(int mp);
	void set_owner(Player *owner);
	void reset();
	void take_damage(Player *player, int damage);
	void give_exp();
	void find_control(bool spawn);
	void update_position(short position_x, short position_y);

private:

	bool is_boss_;
	bool is_dead_;
	bool is_map_mob_;
	signed char original_stance_;
	signed char stance_;
	short original_position_x_;
	short original_position_y_;
	short position_x_;
	short position_y_;
	short original_foothold_;
	int object_id_;
	int monster_id_;
	int max_hp_;
	int max_mp_;
	int hp_;
	int mp_;
	Map *map_;
	Player *current_owner_;
	std::unordered_map<int, int> playersa_;
};
