//

#include "map_reactor_data.hpp"
#include "packetcreator.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::SpawnReactor(int object_id, MapReactorData *reactor)
{
	write<short>(send_headers::kREACTOR_SPAWN);
	write<int>(object_id);
	write<int>(reactor->reactor_id);
	write<signed char>(reactor->state);
	write<short>(reactor->position_x);
	write<short>(reactor->position_y);
	write<signed char>(reactor->stance);
}

void PacketCreator::DestroyReactor(int object_id, MapReactorData *reactor)
{
	write<short>(send_headers::kREACTOR_DESTROY);
	write<int>(object_id);
	write<signed char>(reactor->state);
	write<short>(reactor->position_x);
	write<short>(reactor->position_y);
}
