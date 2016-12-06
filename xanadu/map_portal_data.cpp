//

#pragma once

#include "map_portal_data.hpp"

// constructor

MapPortalData::MapPortalData(int id, int x, int y, int to_id, int type, std::string from, std::string to, std::string script) :
	id_(id),
	x_(x),
	y_(y),
	to_id_(to_id),
	type_(type),
	from_(from),
	to_(to),
	script_(script)
{
}

int MapPortalData::get_id()
{
	return id_;
}

int MapPortalData::get_x()
{
	return x_;
}

int MapPortalData::get_y()
{
	return y_;
}

int MapPortalData::get_to_map_id()
{
	return to_id_;
}

int MapPortalData::get_type()
{
	return type_;
}

std::string MapPortalData::get_from_protal()
{
	return from_;
}

std::string MapPortalData::get_to_portal()
{
	return to_;
}

std::string MapPortalData::get_script()
{
	return script_;
}
