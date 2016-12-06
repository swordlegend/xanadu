//

#pragma once

#include <string>
#include <vector>

#include "pet_command_data.hpp"

class PetData
{
public:

	// constructor
	PetData(int id);

	// default constructor
	PetData() = delete;

	// copy constructor
	PetData(const PetData &other) = delete;

	// move constructor
	PetData(PetData &&other) = delete;

	// destructor
	~PetData() = default;

	// copy assignment operator
	PetData &operator=(const PetData &other) = delete;

	// move assignment operator
	PetData &operator=(PetData &&other) = delete;

	int get_id();
	void set_name(std::string name);
	std::string get_name();
	void add_command(PetCommandData *command);
	std::vector<PetCommandData *> *get_commands();

private:

	int id_;
	std::string name_;
	std::vector<PetCommandData *> commands_;
};
