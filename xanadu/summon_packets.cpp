//

#include "packetcreator.hpp"

#include "player.hpp"
#include "summon.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::SpawnSummon(Player *player, Summon *summon, bool animated)
{
	write<short>(send_headers::kSUMMON_SPAWN);
	write<int>(player->get_id());
	write<int>(summon->object_id_);
	write<int>(summon->skill_id_);
	write<signed char>(summon->skill_level_);
	write<short>(summon->position_x_);
	write<short>(summon->position_y_);
	write<signed char>(0);
	write<short>(0);
	write<signed char>(summon_get_movement_type(summon->skill_id_));
	write<signed char>(summon_get_summon_type(summon->skill_id_));
	write<bool>(animated);
}

void PacketCreator::MoveSummon(int owner_player_id, unsigned char* buffer, int size)
{
	write<short>(send_headers::kSUMMON_MOVE);
	write<int>(owner_player_id);
	memcpy(buffer_ + length_, buffer, size);
	length_ += size;
}

void PacketCreator::remove_summon(int owner_player_id, int summon_id)
{
	write<short>(send_headers::kSUMMON_REMOVE);
	write<int>(owner_player_id);
	write<int>(summon_id);
	write<signed char>(0);
}

void PacketCreator::DamageSummon(int player_id, int skill_id, int damage, signed char unk, int mob_id_from)
{
	write<short>(send_headers::kDAMAGE_SUMMON);
	write<int>(player_id);
	write<int>(skill_id);
	write<signed char>(unk);
	write<int>(damage);
	write<int>(mob_id_from);
	write<signed char>(0);
}

void PacketCreator::DamageMobSummon(Player *player, int summon_object_id, signed char animation, std::unordered_map<int, int> damages)
{
	write<short>(send_headers::kSUMMON_ATTACK);
	write<int>(player->get_id());
	write<int>(summon_object_id);
	write<signed char>(animation);
	write<signed char>(static_cast<unsigned char>(damages.size()));

	for (auto &it : damages)
	{
		int mob_object_id = it.first;
		int damage = it.second;

		write<int>(mob_object_id);
		write<signed char>(6);
		write<int>(damage);
	}
}
