//

#pragma once

#include <unordered_map>
#include <memory>

class PacketCreator;
class PartyMember;
class Player;

class Party
{
public:

	// constructor
	Party(int party_id, Player *leader);

	// default constructor
	Party() = delete;

	// copy constructor
	Party(const Party &other) = default;

	// move constructor
	Party(Party &&other) = default;
	
	// destructor
	~Party();

	// copy assignment operator
	Party &operator=(const Party &other) = default;

	// move assignment operator
	Party &operator=(Party &&other) = default;

	int get_id();
	int get_leader();
	std::unordered_map<int, Player *> *get_players();
	std::unordered_map<int, std::unique_ptr<PartyMember>> *get_members();
	PartyMember *get_member(int player_id);
	void set_leader(int player_id);
	void add_member(Player *player);
	void delete_member(int player_id);
	void remove_member(int player_id);
	void send_packet(PacketCreator *packet);
	void set_variable(std::string name, int val);
	int get_variable(std::string name);
	void delete_variable(std::string &name);
	void clear_variables();

private:

	int id_;
	int leader_;
	std::unordered_map<int, Player *> players_;
	std::unordered_map<int, std::unique_ptr<PartyMember>> members_;
	std::unordered_map<std::string, int> variables_;
};
