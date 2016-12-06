//

#pragma once

#include <vector>
#include <unordered_map>

namespace ValidItemType
{
	enum
	{
		Top,
		Bottom,
		Shoes,
		Weapon
	};
}

class ValidCharDataProvider
{
public:

	// default constructor
	ValidCharDataProvider() = default;

	// copy constructor
	ValidCharDataProvider(const ValidCharDataProvider &other) = delete;

	// move constructor
	ValidCharDataProvider(ValidCharDataProvider &&other) = delete;

	// destructor
	~ValidCharDataProvider() = default;

	// copy assignment operator
	ValidCharDataProvider &operator=(const ValidCharDataProvider &other) = delete;

	// move assignment operator
	ValidCharDataProvider &operator=(ValidCharDataProvider &&other) = delete;

	static ValidCharDataProvider *get_instance();
	void load_data();
	bool is_valid_item(int id, int type);

private:

	static ValidCharDataProvider *singleton_;
	std::unordered_map<int, std::vector<int>> valid_items_;
};
