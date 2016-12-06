//

#pragma once

#include <unordered_map>

struct EquipData;

class EquipDataProvider
{
public:

	// default constructor
	EquipDataProvider() = default;

	// copy constructor
	EquipDataProvider(const EquipDataProvider &other) = delete;

	// move constructor
	EquipDataProvider(EquipDataProvider &&other) = delete;

	// destructor
	~EquipDataProvider() = default;

	// copy assignment operator
	EquipDataProvider &operator=(const EquipDataProvider &other) = delete;

	// move assignment operator
	EquipDataProvider &operator=(EquipDataProvider &&other) = delete;

	static EquipDataProvider *get_instance();
	void load_data();
	EquipData *get_item_data(int item_id);
	std::unordered_map<int, EquipData *> *get_data();

private:

	static EquipDataProvider *singleton_;
	std::unordered_map<int, EquipData *> items_;
};
