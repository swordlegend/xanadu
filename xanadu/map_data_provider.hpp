//

#pragma once

#include <unordered_map>

class MapData;

class MapDataProvider
{
public:

	// default constructor
	MapDataProvider();

	// copy constructor
	MapDataProvider(const MapDataProvider &other) = delete;

	// move constructor
	MapDataProvider(MapDataProvider &&other) = delete;

	// destructor
	~MapDataProvider() = default;

	// copy assignment operator
	MapDataProvider &operator=(const MapDataProvider &other) = delete;

	// move assignment operator
	MapDataProvider &operator=(MapDataProvider &&other) = delete;

	static MapDataProvider *get_instance();
	void load_data();
	MapData *get_map_data_by_id(int id);
	std::unordered_map<int, MapData *> *get_data();

private:

	static MapDataProvider *singleton_;
	int npc_ids_;
	std::unordered_map<int, MapData *> maps_;
};
