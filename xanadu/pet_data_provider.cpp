//

#include "pet_data_provider.hpp"

#include "pet_data.hpp"
#include "item_data_provider.hpp"
#include "item_data.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"
#include "world.hpp"

// singleton

PetDataProvider *PetDataProvider::singleton_ = nullptr;

PetDataProvider *PetDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new PetDataProvider();
	}

	return singleton_;
}

void PetDataProvider::load_data()
{
	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Item"]["Pet"];

	for (auto it1 : node1)
	{
		std::string valuename = it1.name();

		int pet_id = std::stoi(valuename);

		PetData *pet = new PetData(pet_id);

		std::string name = "default";

		WZNode node2 = wz_reader->base_["Item"]["Pet"][valuename.c_str()]["interact"];

		for (auto it2 : node2)
		{
			int prob = 0;
			int increase_closeness = 0;
			std::string command1s = it2.name();
			int command = stoi(command1s);

			WZNode node3 = wz_reader->base_["Item"]["Pet"][valuename.c_str()]["interact"][command1s.c_str()];

			for (auto it3 : node3)
			{
				std::string propertyname1 = it3.name();

				if (propertyname1 == "prob")
				{
					prob = it3.get_int_value();
				}

				else if (propertyname1 == "inc")
				{
					increase_closeness = it3.get_int_value();
				}
			}

			PetCommandData *command_data = new PetCommandData();
			command_data->id = command;
			command_data->prob = prob;
			command_data->closeness = increase_closeness;
			pet->add_command(command_data);
		}

		WZNode node4 = wz_reader->base_["String"]["Pet"][valuename.c_str()];

		for (auto it4 : node4)
		{
			std::string propertyname2 = it4.name();

			if (propertyname2 == "name")
			{
				name = it4.get_string_value();
			}
		}

		pet->set_name(name);
		
		// TODO: FIX THIS CODE
		// WORKAROUND add petdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = pet_id;
		item->price = 0;
		item->max_per_slot = 1;
		item->trade_able = true;
		item->is_cash = true;

		ItemDataProvider::get_instance()->add_item(item);

		data_[pet_id] = pet;
	}
}

PetData *PetDataProvider::get_data_by_id(int id)
{
	if (data_.find(id) != data_.end())
	{
		return data_[id];
	}

	return nullptr;
}

std::unordered_map<int, PetData *> *PetDataProvider::get_data()
{
	return &data_;
}
