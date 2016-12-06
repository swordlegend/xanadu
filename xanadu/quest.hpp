//

#pragma once

#include <string>
#include <unordered_map>

class Quest
{
public:

	// constructor
	Quest(int id, bool completed);

	// default constructor
	Quest() = delete;

	// copy constructor
	Quest(const Quest &other) = delete;

	// move constructor
	Quest(Quest &&other) = delete;

	// destructor
	~Quest() = default;

	// copy assignment operator
	Quest &operator=(const Quest &other) = delete;

	// move assignment operator
	Quest &operator=(Quest &&other) = delete;

	bool add_killed_mob(int id, int amount);
	void set_completed();
	bool is_completed();
	int get_id();
	long long get_completion_time();
	std::string get_killed_mobs1();
	std::unordered_map<int, int> *get_killed_mobs2();

private:

	bool completed_;
	int id_;
	std::unordered_map<int, int> killed_mobs_;
};
