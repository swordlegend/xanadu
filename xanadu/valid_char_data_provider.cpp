//

#include "valid_char_data_provider.hpp"

// singleton

ValidCharDataProvider *ValidCharDataProvider::singleton_ = nullptr;

ValidCharDataProvider *ValidCharDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new ValidCharDataProvider();
	}

	return singleton_;
}

void ValidCharDataProvider::load_data()
{
	// TODO: load from wz

	/*Poco::Data::Statement select(mysql_mcdb_session);
	select << "SELECT * FROM character_creation_data";
	select.execute();

	Poco::Data::RecordSet rs(select);
	for (size_t rowNum = 0; rowNum < rs.rowCount(); ++rowNum)
	{
		int object_id = rs["objectid"];
		const std::string & object_type = rs["object_type"];

		if (object_type == "top")
		{
			valid_items_[ValidItemType::Top].push_back(object_id);
		}
		else if (object_type == "bottom")
		{
			valid_items_[ValidItemType::Bottom].push_back(object_id);
		}
		else if (object_type == "shoes")
		{
			valid_items_[ValidItemType::Shoes].push_back(object_id);
		}
		else if (object_type == "weapon")
		{
			valid_items_[ValidItemType::Weapon].push_back(object_id);
		}

		rs.moveNext();
	}*/
}

bool ValidCharDataProvider::is_valid_item(int id, int type)
{
	return true;
	auto *test = &valid_items_[type];

	for (size_t i = 0; i < test->size(); i++)
	{
		if (id == (*test)[i])
		{
			return true;
		}
	}

	return false;
}
