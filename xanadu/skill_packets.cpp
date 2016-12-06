//

#include "packetcreator.hpp"

#include "send_packet_opcodes.hpp"
#include "skill_data_provider.hpp"
#include "skill_data.hpp"
#include "skill.hpp"
#include "player.hpp"
#include "constants.hpp"

void PacketCreator::SendCooldown(int skill_id, int time)
{
	write<short>(send_headers::kCOOLDOWN);
	write<int>(skill_id);
	write<int>(time);
}

void PacketCreator::ShowSkillEffect(int player_id, int skill_id, signed char skill_level, signed char flags, signed char speed)
{
	write<short>(send_headers::kSKILL_EFFECT);
	write<int>(player_id);
	write<int>(skill_id);
	write<signed char>(skill_level);
	write<signed char>(flags);
	write<signed char>(speed);
}

void PacketCreator::CancelSkillEffect(int player_id, int skill_id)
{
	write<short>(send_headers::kCANCEL_SKILL_EFFECT);
	write<int>(player_id);
	write<int>(skill_id);
}

void PacketCreator::UpdateSkills(Player *player)
{
	auto skills = player->get_skills();
	short amount = static_cast<short>(skills->size());

	write<short>(send_headers::kUPDATE_SKILLS);
	write<signed char>(1);
	write<short>(amount);

	for (auto it : *skills)
	{
		auto &skill = it.second;
		int skill_id = it.first;
		int skill_level = skill.level_;
		int master_level = skill.master_level_;

		write<int>(skill_id);
		write<int>(skill_level);
		write<int>(master_level);
	}

	write<signed char>(1);
}

void PacketCreator::UpdateSkill(int skill_id, int skill_level, int master_level)
{
	write<short>(send_headers::kUPDATE_SKILLS);
	write<signed char>(1);
	write<short>(1); // size
	write<int>(skill_id);
	write<int>(skill_level);
	write<int>(master_level);
	write<signed char>(1);
}

// for the door skill

void PacketCreator::SpawnDoor(int object_id, bool town, short position_x, short position_y)
{
	write<short>(send_headers::kSPAWN_DOOR);
	write<signed char>(town ? 1 : 0);
	write<int>(object_id);
	write<short>(position_x);
	write<short>(position_y);
}

// for the door skill

void PacketCreator::RemoveDoor(int object_id, bool town)
{
	if (town)
	{
		write<short>(send_headers::kSPAWN_PORTAL);
		write<int>(999999999);
		write<int>(999999999);
	}
	else
	{
		write<short>(send_headers::kREMOVE_DOOR);
		write<signed char>(0);
		write<int>(object_id);
	}
}

// for the poison mist skill

void PacketCreator::SpawnMist(int object_id, int owner_character_id, int skill_id, int mist_position_x, int mist_position_y, int mist_position_width, int mist_position_height, int skill_level)
{
	write<short>(send_headers::kSPAWN_MIST);
	write<int>(object_id);
	write<int>(1); // mist.isMobMist() ? 0 : mist.isPoisonMist() ? 1 : 2)
	write<int>(owner_character_id);
	write<int>(skill_id);
	write<signed char>(skill_level);
	write<short>(8); // skill delay
	write<int>(mist_position_x); // left position
	write<int>(mist_position_y); // bottom position
	write<int>(mist_position_x + mist_position_width); // left position
	write<int>(mist_position_y + mist_position_height); // upper position
	write<int>(0);
}

// for the poison mist skill

void PacketCreator::RemoveMist(int object_id)
{
	write<short>(send_headers::kREMOVE_MIST);
	write<int>(object_id);
}

