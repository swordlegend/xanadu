//

#pragma once

#include "map_data.hpp"

#include "map_reactor_data.hpp"
#include "map_foothold_data.hpp"
#include "map_portal_data.hpp"
#include "map_mob_data.hpp"
#include "mapnpc.hpp"

// constructor

MapData::MapData(int id) :
	id_(id),
	forced_return_(999999999),
	return_map_(999999999),
	mob_rate_(1.0)
{
}

int MapData::get_id()
{
	return id_;
}

int MapData::get_forced_return()
{
	return forced_return_;
}

void MapData::set_forced_return(int forced_return)
{
	forced_return_ = forced_return;
}

int MapData::get_return_map()
{
	return return_map_;
}

void MapData::set_return_map(int return_map)
{
	return_map_ = return_map;
}

double MapData::get_mob_rate()
{
	return mob_rate_;
}

void MapData::set_mob_rate(double mob_rate)
{
	mob_rate_ = mob_rate;
}

void MapData::add_mob(MapMobData *data)
{
	mobs_.push_back(data);
}

void MapData::add_npc(Npc *data)
{
	npcs_.push_back(data);
}

void MapData::add_reactor(MapReactorData *data)
{
	reactors_.push_back(data);
}

void MapData::add_foothold(MapFootholdData *data)
{
	footholds_.push_back(data);
}

void MapData::add_portal(MapPortalData *data, std::string name)
{
	portals_[name] = data;
}

Npc *MapData::get_npc(int object_id)
{
	for (Npc *npc : npcs_)
	{
		if (npc->get_object_id() == object_id)
		{
			return npc;
		}
	}

	return nullptr;
}

std::vector<Npc *> *MapData::get_npcs()
{
	return &npcs_;
}

std::vector<MapFootholdData *> *MapData::get_footholds()
{
	return &footholds_;
}

std::vector<MapReactorData *> *MapData::get_reactors()
{
	return &reactors_;
}

std::vector<MapMobData *> *MapData::get_mobs()
{
	return &mobs_;
}

std::unordered_map<std::string, MapPortalData *> *MapData::get_portals()
{
	return &portals_;
}

MapPortalData *MapData::get_portal(const std::string &name)
{
	if (portals_.find(name) == portals_.end())
	{
		return nullptr;
	}

	return portals_[name];
}
