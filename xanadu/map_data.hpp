//

#pragma once

#include <unordered_map>
#include <vector>

class MapPortalData;
class Npc;
struct MapReactorData;
struct MapFootholdData;
struct MapMobData;

class MapData
{
public:

	// constructor
	MapData(int id);

	// default constructor
	MapData() = delete;

	// copy constructor
	MapData(const MapData &other) = delete;

	// move constructor
	MapData(MapData &&others) = delete;

	// destructor
	~MapData() = default;

	// copy assignment operator
	MapData &operator=(const MapData &other) = delete;

	// move assignment operator
	MapData &operator=(MapData &&other) = delete;

	int get_id();
	int get_forced_return();
	void set_forced_return(int forced_return);
	int get_return_map();
	void set_return_map(int return_map);
	double get_mob_rate();
	void set_mob_rate(double mob_rate);
	void add_mob(MapMobData *data);
	void add_npc(Npc *data);
	void add_reactor(MapReactorData *data);
	void add_foothold(MapFootholdData *data);
	void add_portal(MapPortalData *data, std::string name);
	Npc *get_npc(int object_id);
	std::vector<Npc *> *get_npcs();
	std::vector<MapFootholdData *> *get_footholds();
	std::vector<MapReactorData *> *get_reactors();
	std::vector<MapMobData *> *get_mobs();
	std::unordered_map<std::string, MapPortalData *> *get_portals();
	MapPortalData *get_portal(const std::string &name);

private:

	int id_;
	int forced_return_;
	int return_map_;
	double mob_rate_;
	std::vector<MapMobData *> mobs_;
	std::vector<Npc *> npcs_;
	std::vector<MapReactorData *> reactors_;
	std::vector<MapFootholdData *> footholds_;
	std::unordered_map<std::string, MapPortalData *> portals_;
};
