//

#pragma once

#include <vector>

#include "buffvalue.hpp"

class Values
{
public:

	// default constructor
	Values() = default;

	// copy constructor
	Values(const Values &other) = delete;

	// move constructor
	Values(Values &&other) = delete;
	
	// destructor
	~Values() = default;

	// copy assignment operator
	Values &operator=(const Values &other) = delete;

	// move assignment operator
	Values &operator=(Values &&other) = delete;

	std::vector<Value> *get_values();
	int get_value_by_stat(unsigned long long stat);
	void sort();

private:

	std::vector<Value> values_;
};
