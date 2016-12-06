//

#include "mapnpc.hpp"

// constructor

Npc::Npc(bool flip, short pos_x, short pos_y, short rx0, short rx1, short foothold, int npc_id, int object_id) :
flip_(flip),
pos_x_(pos_x),
pos_y_(pos_y),
rx0_(rx0),
rx1_(rx1),
foothold_(foothold),
npc_id_(npc_id),
object_id_(object_id)
{
}

bool Npc::get_flip()
{
	return flip_;
}

short Npc::get_position_x()
{
	return pos_x_;
}

short Npc::get_position_y()
{
	return pos_y_;
}

short Npc::get_rx0()
{
	return rx0_;
}

short Npc::get_rx1()
{
	return rx1_;
}

short Npc::get_foothold()
{
	return foothold_;
}

int Npc::get_npc_id()
{
	return npc_id_;
}

int Npc::get_object_id()
{
	return object_id_;
}
