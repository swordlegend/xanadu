//

#pragma once

class Npc
{
public:

	Npc(bool flip, short pos_x, short pos_y, short rx0, short rx1, short foothold, int npc_id, int object_id);

	// default constructor
	Npc() = delete;

	// copy constructor
	Npc(const Npc &other) = delete;

	// move constructor
	Npc(Npc &&other) = delete;

	// destructor
	~Npc() = default;

	// copy assignment operator
	Npc &operator=(const Npc &other) = delete;

	// move assignment operator
	Npc &operator=(Npc &&other) = delete;

	bool get_flip();
	short get_position_x();
	short get_position_y();
	short get_rx0();
	short get_rx1();
	short get_foothold();
	int get_npc_id();
	int get_object_id();

private:

	bool flip_;
	short pos_x_;
	short pos_y_;
	short rx0_;
	short rx1_;
	short foothold_;
	int npc_id_;
	int object_id_;
};
