//

#pragma once

#include <unordered_map>

class MobData;

class MobDataProvider
{
public:

	// default constructor
	MobDataProvider() = default;

	// copy constructor
	MobDataProvider(const MobDataProvider &other) = delete;

	// move constructor
	MobDataProvider(MobDataProvider &&other) = delete;

	// destructor
	~MobDataProvider() = default;

	// copy assignment operator
	MobDataProvider &operator=(const MobDataProvider &other) = delete;

	// move assignment operator
	MobDataProvider &operator=(MobDataProvider &&other) = delete;

	static MobDataProvider *get_instance();
	void load_data();
	MobData *get_data_by_id(int id);
	std::unordered_map<int, MobData *> *get_data();

private:

	static MobDataProvider *singleton_;
	std::unordered_map<int, MobData *> data_;
};
