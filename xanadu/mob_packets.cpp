//

#include "packetcreator.hpp"

#include "mob.hpp"
#include "send_packet_opcodes.hpp"
#include "mob_constants.hpp"

void PacketCreator::ControlMob(Mob *mob, signed char spawn_type)
{
	write<short>(send_headers::kSPAWN_MONSTER_CONTROL);

	write<signed char>(1);
	write<int>(mob->get_object_id());
	write<signed char>(mob_constants::kControlStatusControlNormal);
	write<int>(mob->get_monster_id());
	write<signed char>(0);
	write<short>(0);
	write<signed char>(8);
	write<int>(0);
	write<short>(mob->get_position_x());
	write<short>(mob->get_position_y());
	write<signed char>(mob->get_stance());
	write<short>(0); // foothold
	write<short>(mob->get_original_foothold());
	write<signed char>(spawn_type);

	write<signed char>(-1);
	write<int>(0);
}

void PacketCreator::EndControlMob(int mob_object_id)
{
	write<short>(send_headers::kSPAWN_MONSTER_CONTROL);

	write<signed char>(0);
	write<int>(mob_object_id);
}

void PacketCreator::SpawnMonster(Mob *mob, signed char spawn_type, int from)
{
	write<short>(send_headers::kSPAWN_MONSTER);

	write<int>(mob->get_object_id());
	write<signed char>(mob_constants::kControlStatusControlNormal);
	write<int>(mob->get_monster_id());
	write<signed char>(0);
	write<short>(0);
	write<signed char>(8);
	write<int>(0);
	write<short>(mob->get_position_x());
	write<short>(mob->get_position_y());
	write<signed char>(mob->get_stance());
	write<short>(0); // foothold
	write<short>(mob->get_original_foothold());
	write<signed char>(spawn_type);

	if (spawn_type == -3 || from >= 0)
	{
		// object id of the summoner mob
		write<int>(from);
	}

	write<signed char>(-1);
	write<int>(0);
}

void PacketCreator::MoveMob(int mob_object_id, bool use_skill, signed char action, int unk, short start_position_x, short start_position_y, unsigned char *buffer, int buffer_size)
{
	write<short>(send_headers::kMOVE_MONSTER);

	write<int>(mob_object_id);
	write<bool>(use_skill);
	write<signed char>(action);
	write<int>(unk);
	write<short>(start_position_x);
	write<short>(start_position_y);

	// copy the movement data into the packet buffer
	memcpy(buffer_ + length_, buffer, buffer_size);
	length_ += buffer_size;
}

void PacketCreator::MoveMobResponse(Mob *mob, short move_id, bool use_skill, signed char skill_id, signed char skill_level)
{
	write<short>(send_headers::kMOVE_MONSTER_RESPONSE);

	write<int>(mob->get_object_id());
	write<short>(move_id);
	write<bool>(use_skill);
	write<short>(mob->get_mp());
	write<signed char>(skill_id);
	write<signed char>(skill_level);
}

void PacketCreator::ShowBossHp(int mob_id, int hp, int max_hp, signed char color, signed char background_color)
{
	write<short>(send_headers::kMAP_EFFECT);

	write<signed char>(5);
	write<int>(mob_id);
	write<int>(hp);
	write<int>(max_hp);
	write<signed char>(color);
	write<signed char>(background_color);
}

void PacketCreator::ShowMobHp(int mob_object_id, signed char hp_percent)
{
	write<short>(send_headers::kSHOW_MONSTER_HP);

	write<int>(mob_object_id);
	write<signed char>(hp_percent);
}

void PacketCreator::KillMob(int mob_object_id)
{
	write<short>(send_headers::kKILL_MONSTER);

	write<int>(mob_object_id);
	write<signed char>(1); // animation
	write<signed char>(1); // animation
}
