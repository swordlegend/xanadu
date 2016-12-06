//

#pragma once

#include <unordered_map>

class PacketCreator;
class Player;

class Messenger
{
public:

	// constructor
	Messenger(int id, Player *player);

	// default constructor
	Messenger() = delete;

	// copy constructor
	Messenger(const Messenger &other) = default;

	// move constructor
	Messenger(Messenger &&other) = default;

	// destructor
	~Messenger() = default;

	// copy assignment operator
	Messenger &operator=(const Messenger &other) = default;

	// move assignment operator
	Messenger &operator=(Messenger &&other) = default;

	int get_id();
	std::unordered_map<int, Player *> *get_players();
	void add_member(Player *player);
	void delete_member(int id);
	void send_packet(PacketCreator *packet);
	void send_packet(PacketCreator *packet, Player *excluded_player);

private:

	int id_;
	std::unordered_map<int, Player *> players_;
};
