//

#include "quest_data.hpp"

// constructor

QuestData::QuestData(int id) :
	id(id)
{
}

int QuestData::get_id()
{
	return id;
}

std::vector<QuestRewardData *> *QuestData::get_rewards_data()
{
	return &rewards_;
}

void QuestData::add_reward(QuestRewardData *reward)
{
	rewards_.push_back(reward);
}

void QuestData::add_item_requirement(int item_id, short amount)
{
	required_items_[item_id] = amount;
}

void QuestData::add_monster_requirement(int monster_id, short amount)
{
	required_monsters_[monster_id] = amount;
}

std::unordered_map<int, int> *QuestData::get_required_mobs()
{
	return &required_monsters_;
}

std::unordered_map<int, int> *QuestData::get_required_items()
{
	return &required_items_;
}
