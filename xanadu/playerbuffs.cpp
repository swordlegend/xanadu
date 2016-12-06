//

#include "player.hpp"

#include "map.hpp"
#include "effect.hpp"
#include "packetcreator.hpp"
#include "timer_id.hpp"
#include "buffstat_constants.hpp"

// functions

bool Player::is_buff_stat_active(long long buff_stat)
{
	if (buffs_.find(buff_stat) != buffs_.end())
	{
		return true;
	}
	return false;
}

bool Player::is_buff_id_active(int buff_id)
{
	if (id_buffs_.find(buff_id) != id_buffs_.end())
	{
		return true;
	}
	return false;
}

int Player::get_buff_value(long long buff_stat)
{
	int value = 0;
	if (is_buff_stat_active(buff_stat))
	{
		Effect *buff = buffs_[buff_stat];
		Values *values = buff->get_values();
		value = values->get_value_by_stat(buff_stat);
	}
	return value;
}

void Player::cancel_buff(int buff_id, bool by_timer)
{
	if (!is_buff_id_active(buff_id))
	{
		return;
	}

	Effect *effect = id_buffs_[buff_id];

	// cancel buff for the player
	Values *values = effect->get_values();

	if (values->get_values()->size() > 0)
	{
		PacketCreator packet1;
		packet1.CancelPlayerBuff(values);
		send_packet(&packet1);

		// cancel buff for the map
		Values m;
		auto vals = m.get_values();

		if (values->get_value_by_stat(buffstat_constants::kDarksight) != 0)
		{
			vals->push_back(Value(buffstat_constants::kDarksight, 0));
		}
		if (values->get_value_by_stat(buffstat_constants::kShadowPartner) != 0)
		{
			vals->push_back(Value(buffstat_constants::kShadowPartner, 0));
		}
		if (values->get_value_by_stat(buffstat_constants::kCombo) != 0)
		{
			vals->push_back(Value(buffstat_constants::kCombo, 0));
			crusader_combo_value_ = 1;
		}
		if (vals->size() > 0)
		{
			PacketCreator packet2;
			packet2.CancelMapBuff(id_, &m);
			map_->send_packet(&packet2);
		}
	}

	// delete player buff
	delete_buff(effect, by_timer);
	id_buffs_.erase(buff_id);
}

void Player::delete_buff(Effect *effect, bool by_timer)
{
	if (by_timer == false)
	{
		//timer::TimerId timer_id;
		//timer_id.type_ = timer::kTimerTypesBuffTimer;
		//timer_id.id_ = effect->get_id();

		/*if (timers_.find(timer_id) != timers_.end())
		{
			timers_.erase(timer_id);
		}*/
	}
	std::vector<long long> todel;
	for (auto &it : buffs_)
	{
		if (it.second == effect)
		{
			long long buff_stat = it.first;
			todel.push_back(buff_stat);
		}
	}
	for (size_t i = 0; i < todel.size(); ++i)
	{
		long long buff_stat = todel[i];
		buffs_.erase(buff_stat);
	}
}

void Player::add_buff(Effect *effect)
{
	int id = effect->get_id();

	// check if a buff with the target id (item/skill) is already being used, cancel it if that's the case
	if (is_buff_id_active(id) == true)
	{
		Effect *buff = id_buffs_[id];
		delete_buff(buff, false);
		id_buffs_.erase(id);
	}

	// check if the a buff with a target buffStat is already being used, cancel it if that's the case
	auto values = effect->get_values()->get_values();
	for (auto &value : *values)
	{
		long long buff_stat = value.get_stat();
		if (is_buff_stat_active(buff_stat) == true)
		{
			Effect *buff = buffs_[buff_stat];
			id_buffs_.erase(buff->get_id());
			delete_buff(buff, false);
		}
		buffs_[buff_stat] = effect;
	}

	id_buffs_[id] = effect;
}
