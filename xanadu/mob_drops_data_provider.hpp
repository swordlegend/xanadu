//

#pragma once

#include <unordered_map>

struct DropData;

class MobDropsDataProvider
{
public:

	// default constructor
	MobDropsDataProvider() = default;

	// copy constructor
	MobDropsDataProvider(const MobDropsDataProvider &other) = delete;

	// move constructor
	MobDropsDataProvider(MobDropsDataProvider &&other) = delete;

	// destructor
	~MobDropsDataProvider() = default;

	// copy assignment operator
	MobDropsDataProvider &operator=(const MobDropsDataProvider &other) = delete;

	// move assignment operator
	MobDropsDataProvider &operator=(MobDropsDataProvider &&other) = delete;

	static MobDropsDataProvider *get_instance();
	void load_data();
	std::vector<DropData *> *get_drop_data(int mob_id);

private:

	static MobDropsDataProvider *singleton_;
	std::unordered_map<int, std::vector<DropData *>> drops_;
};
