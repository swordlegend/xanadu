//

#pragma once

#include <unordered_map>

class PetData;

class PetDataProvider
{
public:

	// default constructor
	PetDataProvider() = default;

	// copy constructor
	PetDataProvider(const PetDataProvider &other) = delete;

	// move constructor
	PetDataProvider(PetDataProvider &&other) = delete;

	// destructor
	~PetDataProvider() = default;

	// copy assignment operator
	PetDataProvider &operator=(const PetDataProvider &other) = delete;

	// move assignment operator
	PetDataProvider &operator=(PetDataProvider &&other) = delete;

	static PetDataProvider *get_instance();
	void load_data();
	PetData *get_data_by_id(int id);
	std::unordered_map<int, PetData *> *get_data();

private:

	static PetDataProvider *singleton_;
	std::unordered_map<int, PetData *> data_;
};
