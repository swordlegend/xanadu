//

#include "packetcreator.hpp"

#include "send_packet_opcodes.hpp"
#include "buffvalues.hpp"
#include "buffstat_constants.hpp"

void PacketCreator::GiveEnergyCharge(int player_id, int bar, int time)
{
	if (player_id > 0)
	{
		write<short>(send_headers::kGIVE_FOREIGN_BUFF);
		write<int>(player_id);
	}
	else
	{
		write<short>(send_headers::kGIVE_BUFF);
	}

	write<long long>(0);
	write<long long>(buffstat_constants::kEnergyCharge);

	write_null(13);

	if (player_id > 0)
	{
		write_null(14);
	}

	if (bar < 10000)
	{
		write<int>(bar);
	}
	else
	{
		write<int>(10000);
	}

	write<long long>(0);
	write<signed char>(0);
	write<int>(bar >= 10000 ? time : 0);
	write<short>(0);
	write<signed char>(6);
}

void PacketCreator::ShowMonsterRiding(int player_id, int item_id, int skill_id)
{
	if (player_id > 0)
	{
		write<short>(send_headers::kGIVE_FOREIGN_BUFF);
		write<int>(player_id);
	}
	else
	{
		write<short>(send_headers::kGIVE_BUFF);
	}

	write<long long>(0);
	write<long long>(buffstat_constants::kMonsterRiding);

	write<short>(0);
	write<int>(item_id);
	write<int>(skill_id);
	write<int>(0);
	write<short>(0);
}

void PacketCreator::ShowForeignEffect(int player_id, signed char effect)
{
	write<short>(send_headers::kSHOW_FOREIGN_EFFECT);
	write<int>(player_id);
	write<signed char>(effect); // 0 = level up, 8 = job change
}

void PacketCreator::ShowBuffEffect(int player_id, signed char effect_id, int skill_id, signed char skill_level)
{
	if (player_id > 0)
	{
		write<short>(send_headers::kSHOW_FOREIGN_EFFECT);
		write<int>(player_id);
	}
	else
	{
		write<short>(send_headers::kSHOW_ITEM_GAIN_INCHAT);
	}

	write<signed char>(effect_id);
	write<int>(skill_id);
	write<signed char>(skill_level);
}

void PacketCreator::ShowPlayerBuff(Values *values, int skill_id, int time)
{
	write<short>(send_headers::kGIVE_BUFF);

	values->sort();

	writeBuffMask(values);

	auto values2 = values->get_values();

	for (auto &value : *values2)
	{
		write<short>(value.get_value());
		write<int>(skill_id);
		write<int>(time);
	}

	write_null(10);
}

void PacketCreator::CancelPlayerBuff(Values *values)
{
	write<short>(send_headers::kCANCEL_BUFF);

	writeBuffMask(values);

	write<signed char>(1);
}

void PacketCreator::ShowMapBuff(int player_id, Values *values)
{
	write<short>(send_headers::kGIVE_FOREIGN_BUFF);
	write<int>(player_id);

	values->sort();

	writeBuffMask(values);

	auto values2 = values->get_values();

	for (Value &value : *values2)
	{
		// morph is 8 bit here except for pirate morph
		write<short>(value.get_value());
	}

	write<int>(0);
	write<short>(0);
}

void PacketCreator::CancelMapBuff(int player_id, Values *values)
{
	write<short>(send_headers::kCANCEL_FOREIGN_BUFF);
	write<int>(player_id);

	writeBuffMask(values);
}

void PacketCreator::writeBuffMask(Values *values)
{
	unsigned long long mask = 0;
	auto values2 = values->get_values();

	for (Value &value : *values2)
	{
		mask |= value.get_stat();
	}

	write<unsigned long long>(0);
	write<unsigned long long>(mask);
}
