//

#pragma once

#include <chrono>
#include <unordered_map>
#include <memory>

#include "asio.hpp"

#include "mob_constants.hpp"

class Channel;
class Item;
class DropsData;
class Drop;
class Npc;
class Mob;
class PacketCreator;
class HiredMerchant;
class Player;
class MapData;
struct MapReactorData;

class Map
{
public:

	// constructor
	Map(int id, Channel *channel);

	// default constructor
	Map() = delete;

	// copy constructor
	Map(const Map &other) = delete;

	// move constructor
	Map(Map &&other) = delete;

	// destructor
	~Map() = default;

	// copy assignment operator
	Map &operator=(const Map &other) = delete;

	// move assignment operator
	Map &operator=(Map &&other) = delete;

	int get_id();
	int get_mob_object_id();
	int get_drop_object_id();
	int get_reactor_object_id();
	int get_merchant_object_id();
	bool is_bossing();
	int get_return_map();
	int get_forced_return();
	double get_mob_rate();
	void send_packet(PacketCreator *packet);
	void send_packet(PacketCreator *packet, Player *excluded_player);
	std::shared_ptr<Drop> get_drop(int object_id);
	Mob *get_mob(int object_id);
	Channel *get_channel();
	void cleanup_timer_callback(const std::error_code &ec);
	void check_life();
	void check_drops();
	void add_player(Player *player);
	void remove_player(Player *player);
	signed char get_closest_spawn_point(short position_x, short position_y);
	void find_foothold(short position_x, short &position_y);
	std::vector<Player *> *get_players();
	std::unordered_map<int, std::shared_ptr<HiredMerchant>> *get_hired_merchants();
	bool can_open_store(Player *player);
	void add_hired_merchant(std::shared_ptr<HiredMerchant>);
	void remove_hired_merchant(int id);
	std::shared_ptr<HiredMerchant> get_hired_merchant(int id);
	void add_drop(std::shared_ptr<Drop> drop);
	void drop_from_mob(Mob *mob, int killer_id);
	void drop_mesos_from_mob(short position_x, short position_y, Mob *mob, int amount, int owner);
	void drop_item_from_mob(short position_x, short position_y, Mob *mob, std::shared_ptr<Item> item, int owner);
	void drop_from_player(std::shared_ptr<Item> item, Player *player);
	void loot_drop(Player *player, std::shared_ptr<Drop> drop, signed char pet_slot = -1);
	void kill(Mob *mob);
	void spawn_monster(int mob_id, short x, short y, short foothold = 0, signed char spawn_type = mob_constants::kSpawnTypesNewSpawn, bool flip = false, int from = -1);
	void spawn_monster_init(int mob_id, short x, short y, short foothold, bool flip);
	std::vector<Mob *> *get_original_mobs();
	void enable_portal(std::string portal_name);
	bool is_portal_enabled(std::string portal_name);

private:

	bool cleanup_timer_running_;
	bool bossing_;
	bool has_monsters_;
	int return_map_;
	int forced_return_;
	int drop_ids_;
	int mob_ids_;
	int reactor_ids_;
	int merchant_ids_;
	int id_;
	double mob_rate_;
	Channel *channel_;
	std::vector<Mob *> mobs_;
	std::vector<Player *> players_;
	std::vector<MapReactorData *> reactors_;
	std::unordered_map<std::string, bool> portals_status_;
	std::unordered_map<int, std::shared_ptr<Drop>> drops_;
	std::unordered_map<int, std::shared_ptr<HiredMerchant>> hired_merchants_;
	asio::basic_waitable_timer<std::chrono::high_resolution_clock> cleanup_timer_;
	asio::basic_waitable_timer<std::chrono::high_resolution_clock> boss_timer_;
};
