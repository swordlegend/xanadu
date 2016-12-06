//

#include "channel.hpp"

#include "map.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

// constructor

Channel::Channel(signed char channel_id) :
	id_(channel_id)
{
	// this shows how to use the loader

	// WZ::Node nn = wz_reader->Base[wzfilename][.img name][name of value][(sometimes only) sub names of value][value (int/string or so return)]

	// Map.wz contains map data
	// sub value Map
	// sub value Map0, Map1,  .... Map9
	// <mapid>.img
	// multiple subtypes: foothold, life, ...
	// return values then...

	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Map"]["Map"];

	for (auto it : node1)
	{
		std::string valuename = it.name();

		if (valuename == "AreaCode")
		{
			continue;
		}

		WZNode node2 = wz_reader->base_["Map"]["Map"][valuename.c_str()];

		for (auto it2 : node2)
		{
			std::string propertyname = it2.name();
			int map_id = atoi(propertyname.c_str());

			map_ids_.push_back(map_id);
		}
	}
}

signed char Channel::get_id()
{
	return id_;
}

bool Channel::has_map_loaded(int map_id)
{
	if (maps_.find(map_id) == maps_.end())
	{
		return false;
	}

	return true;
}

Map *Channel::get_map(int map_id)
{
	// check if the map object exists, if so use it

	if (maps_.find(map_id) != maps_.end())
	{
		return maps_[map_id];
	}

	// check if the map_id exists

	if (std::find(map_ids_.begin(), map_ids_.end(), map_id) == map_ids_.end())
	{
		return nullptr;
	}

	// create a new map

	Map *map = new Map(map_id, this);
	maps_[map_id] = map;

	return map;
}
