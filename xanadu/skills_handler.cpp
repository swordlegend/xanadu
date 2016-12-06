//

#include "player.hpp"

#include "skill_level_data.hpp"
#include "inventory.hpp"
#include "packetcreator.hpp"
#include "item.hpp"
#include "party.hpp"
#include "effect.hpp"
#include "skill_data.hpp"
#include "summon.hpp"
#include "map.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "skill_data_provider.hpp"
#include "buffstat_constants.hpp"
#include "job_constants.hpp"
#include "tools.hpp"
#include "constants.hpp"

void Player::handle_add_skill_point()
{
	skip_bytes(4);
	int skill_id = read<int>();

	short skill_job_id = tools::get_job_id_from_skill_id(skill_id);
	if (!is_beginner_job(skill_job_id))
	{
		if (sp_ < 1)
		{
			return;
		}
		set_sp(sp_ - 1);
	}
	
	int skill_level = 1;
	int master_level = 0;
	if (skills_.find(skill_id) == skills_.end())
	{
		Skill skill;
		skill.level_ = skill_level;
		skill.master_level_ = master_level;
		skills_[skill_id] = skill;
	}
	else
	{
		auto &skill = skills_[skill_id];
		skill_level = ++skill.level_;
		master_level = skill.master_level_;
	}
	
	// send a packet
	PacketCreator packet1;
	packet1.UpdateSkill(skill_id, skill_level, master_level);
	send_packet(&packet1);
}

void Player::handle_cancel_skill_buff()
{
	int skill_id = read<int>();

	// special skill effect
	if (tools::skill_id_is_special_skill(skill_id))
	{
		// send a packet
		PacketCreator packet1;
		packet1.CancelSkillEffect(id_, skill_id);
		map_->send_packet(&packet1, this);

		return;
	}

	// monster riding
	if (skill_id == mount_skill_id_ && mount_skill_id_ != 0)
	{
		Values vals;
		vals.get_values()->push_back(Value(buffstat_constants::kMonsterRiding, 0));

		// send a packet
		PacketCreator packet2;
		packet2.CancelPlayerBuff(&vals);
		send_packet(&packet2);

		// send a packet
		PacketCreator packet3;
		packet3.CancelMapBuff(id_, &vals);
		map_->send_packet(&packet3);

		mount_skill_id_ = 0;
		mount_item_id_ = 0;

		return;
	}

	switch (skill_id)
	{
	case 9101004: // SuperGM Hide
		{
			unhide();
			break;
		}
	case 1301007: // Hyper Body
	case 9101008: // SuperGM Hyper Body
		{
			set_max_hp(get_max_hp() - get_hyperbody_max_hp());
			set_max_mp(get_max_mp() - get_hyperbody_max_mp());
			set_hyperbody_max_hp(0);
			set_hyperbody_max_mp(0);

			break;
		}
	}

	cancel_buff(skill_id, false);
}

void Player::handle_use_skill()
{
	skip_bytes(4);
	int skill_id = read<int>();
	signed char skill_level = read<signed char>();

	SkillData *skill_data = SkillDataProvider::get_instance()->get_skill_data(skill_id);
	if (!skill_data)
	{
		return;
	}

	SkillLevelData *skill_level_data = skill_data->get_skill_level(skill_level);
	if (!skill_level_data)
	{
		return;
	}

	if (tools::skill_id_is_summon(skill_id))
	{
		add_summon(skill_id, skill_level, skill_level_data->x, skill_level_data->time);
		
		// packet
		PacketCreator packet1;
		packet1.EnableAction();
		send_packet(&packet1);
		
		return;
	}

	Effect *effect = Effect::get_skill_effect(skill_id, skill_level);
	if (!effect)
	{
		return;
	}

	effect->use_skill(this);

	switch (skill_id)
	{
	case 2301002: // heal
		{
			int healrate = skill_level_data->hp_percent;
			if (healrate > 100)
			{
				healrate = 100;
			}

			if (party_)
			{
				auto players = party_->get_players();
				int heal = ((healrate * get_max_hp() / 100) / static_cast<int>(players->size()));

				for (auto &it : *players)
				{
					Player *target_player = it.second;

					if (target_player->get_map() == map_
						&& target_player->get_position_x() > (position_x_ + skill_level_data->ltx)
						&& target_player->get_position_y() > (position_y_ + skill_level_data->lty)
						&& target_player->get_position_x() < (position_x_ + skill_level_data->rbx)
						&& target_player->get_position_y() < (position_y_ + skill_level_data->rby))
					{
						if (target_player->get_hp() > 0)
						{
							target_player->add_hp(heal);
						}
					}
				}
			}
			else
			{
				int heal = (healrate * get_max_hp() / 100);

				add_hp(heal);
			}
			break;
		}
	case 1211010: // HP Recovery
		{
			int healrate = skill_level_data->x;
			int heal = (healrate * get_max_hp() / 100);

			add_hp(heal);

			break;
		}
	case 2321006: // resurrection
		{
			if (party_)
			{
				auto players = party_->get_players();
				for (auto it : *players)
				{
					Player *target_player = it.second;

					if (target_player != this
						&& target_player->get_map() == map_
						&& target_player->get_hp() == 0)
					{
						// send a packet
						PacketCreator packet3;
						packet3.ShowBuffEffect(0, 2, skill_id, skill_level);
						target_player->send_packet(&packet3);

						target_player->set_hp(target_player->get_max_hp());
					}
				}
			}
			break;
		}
	case 9101004: // SuperGM Hide
		{
			hide();
			break;
		}
	}

	// packet
	PacketCreator packet9;
	packet9.EnableAction();
	send_packet(&packet9);
}

void Player::handle_use_special_skill()
{
	int skill_id = read<int>();
	signed char skill_level = read<signed char>();
	signed char flags = read<signed char>();
	signed char speed = read<signed char>();
	
	if (tools::skill_id_is_special_skill(skill_id))
	{
		PacketCreator packet;
		packet.ShowSkillEffect(id_, skill_id, skill_level, flags, speed);
		map_->send_packet(&packet, this);
	}
}
