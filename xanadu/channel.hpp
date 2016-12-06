//

#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "tools.hpp"

class Map;

class Channel
{
public:

	// default constructor
	Channel(signed char channel_id);

	// default constructor
	Channel() = default;

	// copy constructor
	Channel(const Channel &other) = delete;

	// move constructor
	Channel(Channel &&other) = delete;

	// destructor
	~Channel() = default;

	// copy assignment operator
	Channel &operator=(const Channel &other) = delete;

	// move assignment operator
	Channel &operator=(Channel &&other) = delete;

	signed char get_id();
	bool has_map_loaded(int map_id);
	Map *get_map(int map_id);

private:

	signed char id_;
	std::vector<int> map_ids_;
	std::unordered_map<int, Map *> maps_;
};
