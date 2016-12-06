//

#pragma once

#include <string>

class MapPortalData
{
public:

	// constructor
	MapPortalData(int id, int x, int y, int to_id, int type, std::string from, std::string to, std::string script);

	// default constructor
	MapPortalData() = delete;

	// copy constructor
	MapPortalData(const MapPortalData &other) = delete;

	// move constructor
	MapPortalData(MapPortalData &&other) = delete;

	// destructor
	~MapPortalData() = default;

	// copy assignment operator
	MapPortalData &operator=(const MapPortalData &other) = delete;

	// move assignment operator
	MapPortalData &operator=(MapPortalData &&other) = delete;

	int get_id();
	int get_x();
	int get_y();
	int get_to_map_id();
	int get_type();
	std::string get_from_protal();
	std::string get_to_portal();
	std::string get_script();

private:

	int id_;
	int x_;
	int y_;
	int to_id_;
	int type_;
	std::string from_;
	std::string to_;
	std::string script_;
};
