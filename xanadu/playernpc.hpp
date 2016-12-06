//

#pragma once

#include <string>
#include <unordered_map>

class PlayerNpc
{
public:

	// default constructor
	PlayerNpc();

	// copy constructor
	PlayerNpc(const PlayerNpc &other) = delete;

	// move constructor
	PlayerNpc(PlayerNpc &&other) = delete;

	// destructor
	~PlayerNpc() = default;

	// copy assignment operator
	PlayerNpc &operator=(const PlayerNpc &other) = delete;

	// move assignment operator
	PlayerNpc &operator=(PlayerNpc &&other) = delete;

	void set_state(int state);
	void set_selected(int selected);
	void set_npc_variable(std::string name, int val);
	int get_npc_variable(std::string name);

	int state_;
	int selected_;
	int id_;
	std::unordered_map<std::string, int> npc_vars_;
};
