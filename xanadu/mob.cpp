//

#include "mob.hpp"

#include "packetcreator.hpp"
#include "party.hpp"
#include "player.hpp"
#include "map.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "quest.hpp"
#include "mob_data.hpp"
#include "mob_data_provider.hpp"
#include "buffstat_constants.hpp"
#include "constants.hpp"

// constructor

Mob::Mob(Map *map, int monster_id, int object_id, short position_x, short position_y, short foothold, signed char stance, int max_hp, int max_mp, bool is_boss, bool is_map_mob) :
map_(map),
is_dead_(false),
is_map_mob_(is_map_mob),
monster_id_(monster_id),
object_id_(object_id),
position_x_(position_x),
position_y_(position_y),
original_position_x_(position_x),
original_position_y_(position_y),
original_foothold_(foothold),
stance_(stance),
original_stance_(stance),
hp_(max_hp),
max_hp_(max_hp),
mp_(max_mp),
max_mp_(max_mp),
is_boss_(is_boss),
current_owner_(nullptr)
{
}

bool Mob::is_map_mob()
{
	return is_map_mob_;
}

void Mob::take_damage(Player *player, int damage)
{
	hp_ -= damage;

	int ndamage = damage;

	if (hp_ < 0)
	{
		ndamage += hp_;
		hp_ = 0;

		// add killed mob to quests

		auto quests = player->get_quests_in_progress();

		for (auto &it : *quests)
		{
			Quest *quest = it.second.get();

			if (quest)
			{
				if (quest->add_killed_mob(monster_id_, 1))
				{
					// packet
					PacketCreator packet10;
					packet10.UpdateQuestInfo(quest);
					player->send_packet(&packet10);
				}
			}
		}
	}

	if (playersa_.find(player->get_id()) == playersa_.end())
	{
		playersa_[player->get_id()] = ndamage;
	}
	else
	{
		playersa_[player->get_id()] += ndamage;
	}

	// show hp bar

	MobData *data = MobDataProvider::get_instance()->get_data_by_id(monster_id_);

	if (data->is_boss_)
	{
		signed char color = data->hp_color_;

		if (color != 0)
		{
			// packet
			PacketCreator packet28;
			packet28.ShowBossHp(monster_id_, hp_, data->max_hp_, color, data->hp_background_color_);
			map_->send_packet(&packet28);
		}
	}
	else
	{
		signed char hp_percentage = static_cast<signed char>(hp_ * 100.0 / data->max_hp_);

		for (auto &it : playersa_)
		{
			Player *p = World::get_instance()->GetPlayerById(it.first);
			if (p && p->get_map() == map_)
			{
				// packet
				PacketCreator packet29;
				packet29.ShowMobHp(object_id_, hp_percentage);
				p->send_packet(&packet29);
			}
		}
	}
}

void Mob::give_exp()
{
	std::unordered_map<int, int> maxdamage;
	std::unordered_map<int, Player *> maxplayer;
	std::unordered_map<int, int> damages;

	for (auto &it : playersa_)
	{
		Player *p = World::get_instance()->GetPlayerById(it.first);

		if (p)
		{
			Party *party = p->get_party();

			if (!party)
			{
				// TODO: improve this line of code
				int exp = static_cast<int>(lround(it.second * kExpRate * 100.0 * MobDataProvider::get_instance()->get_data_by_id(monster_id_)->exp_ / 100.0 / MobDataProvider::get_instance()->get_data_by_id(monster_id_)->max_hp_));

				// 2x exp card

				if (p->get_inventory(kInventoryConstantsTypesCash)->get_item_amount(5211000) > 0)
				{
					exp *= 2;
				}

				if (exp > 0)
				{
					// packet
					PacketCreator packet6;
					packet6.GainExp(exp, false, true, 0);
					p->send_packet(&packet6);

					p->set_exp(p->get_exp() + exp);
				}
			}
			else
			{
				int pid = party->get_id();

				if (maxdamage.find(pid) == maxdamage.end() || it.second > maxdamage[pid])
				{
					if (maxdamage.find(pid) == maxdamage.end())
					{
						damages[pid] = 0;
					}

					maxdamage[pid] = it.second;
					maxplayer[pid] = p;
					damages[pid] += it.second;
				}
			}
		}
	}
	for (auto &d : damages)
	{
		Party *party = World::get_instance()->get_party(d.first);

		if (!party)
		{
			continue;
		}

		std::vector<Player *> players;
		auto ps = party->get_players();
		int levels = 0;
		int count = 0;

		for (auto &it : *ps)
		{
			if (it.second->get_map() == map_)
			{
				++count;

				if (playersa_.find(it.second->get_id()) != playersa_.end()
					|| it.second->get_level() >= maxplayer[party->get_id()]->get_level() - 5
					|| it.second->get_level() >= MobDataProvider::get_instance()->get_data_by_id(monster_id_)->level_ - 5)
				{
					players.push_back(it.second);
					levels += it.second->get_level();
				}
			}
		}

		int baseexp = static_cast<int>(lround(d.second * kExpRate * 100.0 * MobDataProvider::get_instance()->get_data_by_id(monster_id_)->exp_ / 100.0 / MobDataProvider::get_instance()->get_data_by_id(monster_id_)->max_hp_));

		if (count == 1)
		{
			// packet
			PacketCreator packet10;
			packet10.GainExp(baseexp, false, true, 0);
			maxplayer[party->get_id()]->send_packet(&packet10);

			maxplayer[party->get_id()]->set_exp(maxplayer[party->get_id()]->get_exp() + baseexp);

			continue;
		}

		int bonus = (static_cast<int>(players.size()) * 5);

		for (auto &player : players)
		{
			if (player->get_hp() > 0)
			{
				int exp = static_cast<int>(lround(baseexp * 0.8 * player->get_level() / levels));

				if (player == maxplayer[party->get_id()])
				{
					exp += static_cast<int>(lround(baseexp * 0.6));
				}

				if (exp > 0)
				{
					exp = static_cast<int>((exp * (1 + bonus / 100.0)));

					// holy symbol
					if (player->is_buff_stat_active(buffstat_constants::kHolySymbol))
					{
						exp = static_cast<int>(lround(exp * 1.5));
					}

					int players_size = static_cast<int>(players.size());
					exp += ((players_size - 1) / 100 * exp);

					player->set_exp(player->get_exp() + exp);

					// packet
					PacketCreator packet20;
					packet20.GainExp(exp, false, (player == maxplayer[party->get_id()]), bonus);
					player->send_packet(&packet20);
				}
			}
		}
	}
}

void Mob::find_control(bool spawn)
{
	auto players = map_->get_players();
	int max_pos = INT_MAX;
	Player *p = nullptr;
	for (Player *player : *players)
	{
		int distance = tools::get_distance(position_x_, position_y_, player->get_position_x(), player->get_position_y());
		if (distance < max_pos)
		{
			max_pos = distance;
			p = player;
			break;
		}
	}
	current_owner_ = p;
	if (!current_owner_)
	{
		return;
	}
	MobData *data = MobDataProvider::get_instance()->get_data_by_id(monster_id_);
	if (!data)
	{
		return;
	}

	signed char spawn_type = (spawn ? mob_constants::kSpawnTypesNewSpawn : mob_constants::kSpawnTypesOldSpawn);

	// packet
	PacketCreator packet1;
	packet1.ControlMob(this, spawn_type);
	current_owner_->send_packet(&packet1);
}

int Mob::get_killer_player_id()
{
	int md = 0;
	int mdid = 0;
	for (auto it : playersa_)
	{
		if (it.second > md)
		{
			md = it.second;
			mdid = it.first;
		}
	}
	return mdid;
}

bool Mob::is_boss()
{
	return is_boss_;
}

bool Mob::is_dead()
{
	return is_dead_;
}

signed char Mob::get_stance()
{
	return stance_;
}

short Mob::get_position_x()
{
	return position_x_;
}

short Mob::get_position_y()
{
	return position_y_;
}

short Mob::get_original_foothold()
{
	return original_foothold_;
}

int Mob::get_object_id()
{
	return object_id_;
}

int Mob::get_monster_id()
{
	return monster_id_;
}

int Mob::get_hp()
{
	return hp_;
}

int Mob::get_mp()
{
	return mp_;
}

int Mob::get_max_mp()
{
	return max_mp_;
}

Player *Mob::get_owner()
{
	return current_owner_;
}

void Mob::update_position(short position_x, short position_y)
{
	position_x_ = position_x;
	position_y_ = position_y;
}

void Mob::set_is_dead(bool is_dead)
{
	is_dead_ = is_dead;
}

void Mob::set_hp(int hp)
{
	hp_ = hp;
}

void Mob::set_mp(int mp)
{
	mp_ = mp;
}

void Mob::set_owner(Player *owner)
{
	current_owner_ = owner;
}

void Mob::reset()
{
	playersa_.clear();
	is_dead_ = false;
	stance_ = original_stance_;
	position_x_ = original_position_x_;
	position_y_ = original_position_y_;
	hp_ = max_hp_;
	mp_ = max_mp_;

	// packet
	PacketCreator packet1;
	packet1.SpawnMonster(this, mob_constants::kSpawnTypesNewSpawn);
	map_->send_packet(&packet1);

	find_control(true);
}
