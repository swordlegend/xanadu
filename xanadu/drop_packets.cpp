//

#include "packetcreator.hpp"

#include "drop.hpp"
#include "item.hpp"
#include "send_packet_opcodes.hpp"
#include "drop_constants.hpp"
#include "constants.hpp"

void PacketCreator::ShowDrop(signed char type, std::shared_ptr<Drop> drop, short position_x, short position_y)
{
	write<short>(send_headers::kDROP_ITEM_FROM_MAPOBJECT);
	write<signed char>(type);
	write<int>(drop->get_id());

	bool is_mesos = drop->is_mesos();

	write<bool>(is_mesos);

	if (is_mesos)
	{
		write<int>(drop->get_mesos());
	}
	else
	{
		auto drop_item = drop->get_item();

		if (!drop_item)
		{
			return;
		}

		write<int>(drop_item->get_item_id());
	}

	write<int>(drop->get_owner_player_id());
	write<signed char>(drop->get_type());
	write<short>(drop->get_position_x()); // the target drop position
	write<short>(drop->get_position_y()); // the target drop position
	write<int>(0);

	if (type != kDropPacketConstantsAnimationTypesShowExisting)
	{
		write<short>(position_x); // the origin drop position
		write<short>(position_y); // the origin drop position
		write<short>(0);
	}

	if (!drop->is_mesos())
	{
		write<long long>(kNoExpirationTime);
	}

	write<signed char>(1); // 1 = pet pickup enabled (for mob drops), 0 = pet pickup disabled (for player drops)
}

void PacketCreator::RemoveDrop(int drop_id, signed char animation, int player_id, signed char pet_slot)
{
	write<short>(send_headers::kREMOVE_ITEM_FROM_MAP);
	write<signed char>(animation); // 0 = expire, 1 = without animation, 2 = pickup, 4 = explode, 5 = pet pickup
	write<int>(drop_id);

	if (animation >= 2)
	{
		write<int>(player_id);

		if (pet_slot >= 0)
		{
			write<int>(pet_slot);
		}
	}
}
