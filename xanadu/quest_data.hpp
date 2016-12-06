//

#pragma once

#include <unordered_map>
#include <vector>

#include "quest_reward_data.hpp"

class QuestData
{
public:

	// constructor
	QuestData(int id);

	// default constructor
	QuestData() = delete;

	// copy constructor
	QuestData(const QuestData &other) = delete;

	// move constructor
	QuestData(QuestData &&other) = delete;

	// destructor
	~QuestData() = default;

	// copy assignment operator
	QuestData &operator=(const QuestData &other) = delete;

	// move assignment operator
	QuestData &operator=(QuestData &&other) = delete;

	int get_id();
	std::vector<QuestRewardData *> *get_rewards_data();
	void add_reward(QuestRewardData *reward);
	void add_item_requirement(int item_id, short amount);
	void add_monster_requirement(int monster_id, short amount);
	std::unordered_map<int, int> *get_required_mobs();
	std::unordered_map<int, int> *get_required_items();

private:

	int id;
	std::vector<QuestRewardData *> rewards_;
	std::unordered_map<int, int> required_monsters_;
	std::unordered_map<int, int> required_items_;
};
