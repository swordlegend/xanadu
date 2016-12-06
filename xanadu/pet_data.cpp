//

#include "pet_data.hpp"

// constructor

PetData::PetData(int id) :
	id_(id),
	name_("")
{
}

int PetData::get_id()
{
	return id_;
}

void PetData::set_name(std::string name)
{
	name_ = name;
}

std::string PetData::get_name()
{
	return name_;
}

void PetData::add_command(PetCommandData *command)
{
	commands_.push_back(command);
}

std::vector<PetCommandData *> *PetData::get_commands()
{
	return &commands_;
}
