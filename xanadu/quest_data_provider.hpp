//

#pragma once

#include <unordered_map>

class QuestData;

class QuestDataProvider
{
public:

	// default constructor
	QuestDataProvider() = default;

	// copy constructor
	QuestDataProvider(const QuestDataProvider &other) = delete;

	// move constructor
	QuestDataProvider(QuestDataProvider &&other) = delete;

	// destructor
	~QuestDataProvider() = default;

	// copy assignment operator
	QuestDataProvider &operator=(const QuestDataProvider &other) = delete;

	// move assignment operator
	QuestDataProvider &operator=(QuestDataProvider &&other) = delete;

	static QuestDataProvider *get_instance();
	void load_data();
	QuestData *get_quest_data(int quest_id);
	std::unordered_map<int, QuestData *> *get_data();

private:

	static QuestDataProvider *singleton_;
	std::unordered_map<int, QuestData *> quests_;
};
