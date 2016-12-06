//

#include "map.hpp"

#include <functional>

#include "drop.hpp"
#include "inventory.hpp"
#include "map_foothold_data.hpp"
#include "map_portal_data.hpp"
#include "packetcreator.hpp"
#include "hiredmerchant.hpp"
#include "party_member.hpp"
#include "mob_data.hpp"
#include "mob_drops_data_provider.hpp"
#include "drop_data.hpp"
#include "party.hpp"
#include "player.hpp"
#include "quest.hpp"
#include "mob.hpp"
#include "mapnpc.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "item_data_provider.hpp"
#include "map_data_provider.hpp"
#include "map_data.hpp"
#include "item_data.hpp"
#include "tools.hpp"
#include "map_reactor_data.hpp"
#include "map_mob_data.hpp"
#include "mob_data_provider.hpp"
#include "drop_constants.hpp"
#include "constants.hpp"

// constructor

Map::Map(int id, Channel *channel) :
	id_(id),
	channel_(channel),
	cleanup_timer_running_(false),
	bossing_(false),
	has_monsters_(false),
	drop_ids_(0),
	mob_ids_(0),
	reactor_ids_(0),
	merchant_ids_(0),
	forced_return_(999999999),
	return_map_(999999999),
	mob_rate_(1.0),
	cleanup_timer_(World::get_instance()->get_io_service()),
	boss_timer_(World::get_instance()->get_io_service())
{
	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(id_);

	// mobs

	auto mobs = data->get_mobs();

	for (auto it : *mobs)
	{
		bool flip = it->flip;
		int life_id = it->mob_id;
		short x = it->position_x;
		short y = it->position_y;
		short foothold = it->foothold;

		spawn_monster_init(life_id, x, y, foothold, flip);

		has_monsters_ = true;
	}

	// reactors

	// TODO:
	// what is done: reactor data in maps seems to be done
	// need to create a real reactor system
	// need reactor objects which are created here

	auto reactors = data->get_reactors();

	reactors_.swap(*reactors);

	// portals

	auto portals = data->get_portals();

	for (auto it : *portals)
	{
		auto portal = it.second;

		if (portal->get_type() == 5)
		{
			portals_status_[portal->get_from_protal()] = false;
		}
	}

	// some map properties

	forced_return_ = data->get_forced_return();
	return_map_ = data->get_return_map();
	mob_rate_ = data->get_mob_rate();
}

int Map::get_return_map()
{
	return return_map_;
}

int Map::get_forced_return()
{
	return forced_return_;
}

double Map::get_mob_rate()
{
	return mob_rate_;
}

void Map::send_packet(PacketCreator *packet)
{
	for (auto &target_player : players_)
	{
		target_player->send_packet(packet);
	}
}

void Map::send_packet(PacketCreator *packet, Player *excluded_player)
{
	for (auto &target_player : players_)
	{
		if (target_player != excluded_player)
		{
			target_player->send_packet(packet);
		}
	}
}

std::shared_ptr<Drop> Map::get_drop(int object_id)
{
	if (drops_.find(object_id) != drops_.end())
	{
		return drops_[object_id];
	}

	return std::shared_ptr<Drop>(nullptr);
}

int Map::get_drop_object_id()
{
	return drop_ids_++;
}

Mob *Map::get_mob(int object_id)
{
	for (Mob *mob : mobs_)
	{
		if (mob->get_object_id() == object_id && !mob->is_dead())
		{
			return mob;
		}
	}

	return nullptr;
}

int Map::get_mob_object_id()
{
	return mob_ids_++;
}

Channel *Map::get_channel()
{
	return channel_;
}

void Map::cleanup_timer_callback(const std::error_code &ec)
{
	if (players_.size() == 0)
	{
		cleanup_timer_running_ = false;
		return;
	}

	// drops need to be checked every 3 minutes (= 180 seconds)
	// monsters need to be checked every 8 seconds
	// therefore here it tests if the maps contains monsters

	int timer_seconds = 180;

	if (has_monsters_)
	{
		check_life();
		timer_seconds = 8;
	}

	check_drops();

	// timer
	std::error_code ec2;
	cleanup_timer_.expires_from_now(std::chrono::seconds(timer_seconds), ec2);
	cleanup_timer_.async_wait(std::bind(&Map::cleanup_timer_callback, this, std::placeholders::_1));
}

void Map::check_life()
{
	for (auto mob : mobs_)
	{
		if (mob->is_dead())
		{
			mob->reset();
		}
	}
}

void Map::check_drops()
{
	if (drops_.size() == 0)
	{
		return;
	}

	unsigned long long ticks = GetTickCount64();

	auto it = drops_.begin();
	for ( ; it != drops_.end(); )
	{
		const std::shared_ptr<Drop> &drop = it->second;

		// 180 * 1000 nanoseconds = 180000 milliseconds = 180 seconds = 3 minutes
		// due to slight inaccuracy of GetTickCount64() and the timer i reduce the relevant duration
		// by 3 seconds = 3000 milliseconds so the timer will produce the intended results
		constexpr unsigned long long kDropDuration = (180000 - 3000);

		if (ticks > (drop->get_time() + kDropDuration))
		{
			// packet
			PacketCreator packet1;
			packet1.RemoveDrop(drop->get_id(), 0, 0, 0);
			send_packet(&packet1);

			it = drops_.erase(it);
		}
		else
		{
			++it;
		}
	}
}

int Map::get_reactor_object_id()
{
	return reactor_ids_++;
}

std::vector<Player *> *Map::get_players()
{
	return &players_;
}

void Map::add_player(Player *player)
{
	// display reactors
	for (auto reactor : reactors_)
	{
		PacketCreator packet1;
		packet1.SpawnReactor(get_reactor_object_id(), reactor);
		player->send_packet(&packet1);
	}

	// display hired merchants
	for (auto it : hired_merchants_)
	{
		std::shared_ptr<HiredMerchant> merchant = it.second;

		// packet
		PacketCreator packet7;
		packet7.SpawnHiredMerchant(merchant);
		player->send_packet(&packet7);
	}

	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(id_);
	auto npcs = data->get_npcs();

	// display npcs
	for (Npc *npc : *npcs)
	{
		PacketCreator packet;
		packet.ShowNpc(npc);
		player->send_packet(&packet);
	}

	// fm npcs
	/*if (id == 910000000)
	{
	short position_x = 300;
	short position_y = 34;
	short rx0 = 0;
	short rx1 = 400;
	short foothold = 0;
	int npc_id = 9100117; // Gachapon
	bool flip = false;

	Npc * npc1 = new Npc(flip, position_x, position_y, rx0, rx1, foothold, npc_id, get_npc_object_id());
	npcs_.push_back(npc1);

	position_x = 600;
	position_y = 34;
	rx0 = 0;
	rx1 = 700;
	foothold = 0;
	npc_id = 9001000; // Donator Npc
	flip = false;

	Npc * npc2 = new Npc(flip, position_x, position_y, rx0, rx1, foothold, npc_id, get_npc_object_id());
	npcs_.push_back(npc2);

	position_x = 760;
	position_y = 34;
	rx0 = 0;
	rx1 = 860;
	foothold = 0;
	npc_id = 9001001; // Maple Leaf Exchanger
	flip = false;

	Npc * npc3 = new Npc(flip, position_x, position_y, rx0, rx1, foothold, npc_id, get_npc_object_id());
	npcs_.push_back(npc3);
	}*/

	if (!cleanup_timer_running_)
	{
		check_drops();
	}

	// display drops
	for (auto it : drops_)
	{
		std::shared_ptr<Drop> drop = it.second;
		PacketCreator packet29;
		packet29.ShowDrop(kDropPacketConstantsAnimationTypesShowExisting, drop, 0, 0);
		player->send_packet(&packet29);
	}

	// display players, summons and dragons
	for (auto target_player : players_)
	{
		// display the target player to the player, if:
		// * The target player is not hidden
		// * Or the player has a GM rank
		if (!target_player->is_hidden() || player->get_is_gm())
		{
			PacketCreator packet6;
			packet6.ShowPlayer(target_player);
			player->send_packet(&packet6);
		}

		// display the player to the target player, if:
		// * The player is not hidden
		// * Or the target player has a GM rank
		if (!player->is_hidden() || target_player->get_is_gm())
		{
			PacketCreator packet7;
			packet7.ShowPlayer(player);
			target_player->send_packet(&packet7);
		}

		// diplay target players summon to the player, if:
		// * The target player has one and
		// * The target player is not hidden
		// * Or the player has a GM rank
		if (target_player->get_summon()
			&& (!target_player->is_hidden() || player->get_is_gm()))
		{
			// send a packet
			PacketCreator packet25;
			packet25.SpawnSummon(target_player, target_player->get_summon(), false);
			player->send_packet(&packet25);
		}

		// diplay players summon to the target player, if:
		// * The player has one and
		// * The player is not hidden
		// * Or the target player has a GM rank
		if (player->get_summon()
			&& (!player->is_hidden() || target_player->get_is_gm()))
		{
			// send a packet
			PacketCreator packet26;
			packet26.SpawnSummon(player, player->get_summon(), false);
			target_player->send_packet(&packet26);
		}
	}

	// add the player to the maps player container
	players_.push_back(player);

	// display the monsters
	for (auto mob : mobs_)
	{
		if (!mob->is_dead())
		{
			// packet
			PacketCreator packet37;
			packet37.SpawnMonster(mob, mob_constants::kSpawnTypesOldSpawn);
			player->send_packet(&packet37);

			if (!mob->get_owner())
			{
				mob->find_control(false);
			}
		}
	}

	// spawn summon for the player
	if (player->get_summon())
	{
		// send a packet
		PacketCreator packet27;
		packet27.SpawnSummon(player, player->get_summon(), false);
		player->send_packet(&packet27);
	}

	// spawn pets for the player
	auto pets = player->get_pets();

	for (auto pet : *pets)
	{
		pet->set_position(player->get_position_x(), player->get_position_y(), player->get_stance());

		// packet
		PacketCreator packet34;
		packet34.ShowPet(player->get_id(), pet, true);
		player->send_packet(&packet34);
	}

	// update players party
	Party *party = player->get_party();
	if (party)
	{
		PartyMember *member = party->get_member(player->get_id());
		if (!member)
		{
			return;
		}
		member->set_map_id(id_);
		auto players = party->get_players();
		for (auto it : *players)
		{
			Player *target = it.second;
			// send a packet
			PacketCreator packet10;
			packet10.UpdateParty(party);
			target->send_packet(&packet10);
			//
			if (target->get_map() == this && target != player)
			{
				// send a packet
				PacketCreator packet11;
				packet11.UpdatePartyHp(player);
				target->send_packet(&packet11);
				// send a packet
				PacketCreator packet12;
				packet12.UpdatePartyHp(target);
				player->send_packet(&packet12);
			}
		}
	}

	// timer
	if (!cleanup_timer_running_)
	{
		cleanup_timer_running_ = true;

		// drops need to be checked every 3 minutes (= 180 seconds)
		// monsters need to be checked every 8 seconds
		// therefore here it tests if the maps contains monsters

		int timer_seconds = 180;

		if (has_monsters_)
		{
			check_life();
			timer_seconds = 8;
		}

		std::error_code ec;
		cleanup_timer_.expires_from_now(std::chrono::seconds(timer_seconds), ec);
		cleanup_timer_.async_wait(std::bind(&Map::cleanup_timer_callback, this, std::placeholders::_1));
	}

	// the following code in this function is for development purposes only
	if (id_ == 980000101)
	{
		PacketCreator packet15;
		packet15.StartCarnivalPartyQuest(0);
		send_packet(&packet15);
	}
}

void Map::remove_player(Player *player)
{
	// remove the player from the map
	for (auto it = players_.begin(); it != players_.end(); ++it)
	{
		Player *temp = *it;
		if (temp == player)
		{
			players_.erase(it);
			break;
		}
	}

	PacketCreator packet1;
	packet1.RemovePlayer(player);
	send_packet(&packet1);

	// update mob controls
	for (auto mob : mobs_)
	{
		if (mob->get_owner() == player)
		{
			mob->find_control(false);
		}
	}

	if (bossing_ && players_.size() == 0)
	{
		bossing_ = false;

		std::error_code ec;
		boss_timer_.cancel(ec);
	}
}

bool Map::is_bossing()
{
	return bossing_;
}

signed char Map::get_closest_spawn_point(short position_x, short position_y)
{
	signed char id = 0;
	int distance = 200000;

	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(id_);
	auto portals = data->get_portals();

	for (auto it : *portals)
	{
		MapPortalData *portal = it.second;
		int dis = tools::get_distance(portal->get_x(), portal->get_y(), position_x, position_y);

		if (dis < distance)
		{
			id = portal->get_id();
			distance = dis;
		}
	}

	return id;
}

void Map::find_foothold(short position_x, short &position_y)
{
	// Determines where a drop falls using the footholds data
	// to check the platforms and find the correct one.

	short x = position_x;
	short y = position_y - 100;
	short maxy = position_y;
	bool firstcheck = true;

	MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(id_);
	auto footholds = data->get_footholds();

	for (MapFootholdData *fh : *footholds)
	{
		if ((x > fh->x1 && x <= fh->x2) || (x > fh->x2 && x <= fh->x1))
		{
			short cmax = static_cast<short>(static_cast<float>((fh->y1 - fh->y2) / (fh->x1 - fh->x2) * (x - fh->x1) + fh->y1));

			if ((cmax <= maxy || (maxy == position_y && firstcheck)) && cmax >= y)
			{
				maxy = cmax;
				firstcheck = false;
			}
		}
	}

	position_y = maxy;
}

int Map::get_id()
{
	return id_;
}

bool Map::can_open_store(Player *player)
{
	if (id_ < 910000001 || id_ > 910000022)
	{
		return false;
	}

	for (auto it : hired_merchants_)
	{
		std::shared_ptr<HiredMerchant> merchant = it.second;

		if (merchant != player->get_hired_merchant())
		{
			if (tools::get_distance(merchant->get_position_x(), merchant->get_position_y(), player->get_position_x(), player->get_position_y()) <= 23000)
			{
				// packet
				PacketCreator packet1;
				packet1.ShowMessage("You may not open a store here.", 5);
				player->send_packet(&packet1);

				// packet
				PacketCreator packet2;
				packet2.EnableAction();
				player->send_packet(&packet2);

				return false;
			}
		}
	}

	return true;
}

std::unordered_map<int, std::shared_ptr<HiredMerchant>> *Map::get_hired_merchants()
{
	return &hired_merchants_;
}

void Map::add_hired_merchant(std::shared_ptr<HiredMerchant> merchant)
{
	hired_merchants_[merchant->get_id()] = merchant;
}

int Map::get_merchant_object_id()
{
	return merchant_ids_++;
}

void Map::remove_hired_merchant(int id)
{
	if (hired_merchants_.find(id) != hired_merchants_.end())
	{
		hired_merchants_.erase(id);
	}
}

std::shared_ptr<HiredMerchant> Map::get_hired_merchant(int id)
{
	if (hired_merchants_.find(id) != hired_merchants_.end())
	{
		return hired_merchants_[id];
	}

	return std::shared_ptr<HiredMerchant>();
}

void Map::add_drop(std::shared_ptr<Drop> drop)
{
	drops_[drop->get_id()] = drop;
}

void Map::loot_drop(Player *player, std::shared_ptr<Drop> drop, signed char pet_slot)
{
	if (drop->loot_drop(player, pet_slot))
	{
		drops_.erase(drop->get_id());
	}
}

void Map::drop_from_mob(Mob *mob, int killer_player_id)
{
	int count = 0;
	short drop_pos_x = 0;
	short drop_pos_y = 0;

	// 20% chance to drop Maple Leaf (4001126)

	if (tools::random_int(1, 100) <= 20)
	{
		drop_pos_x = (mob->get_position_x() + 25 * ((count + count % 2) / 2) * ((count % 2) ? 1 : -1));
		drop_pos_y = mob->get_position_y();
		++count;

		std::shared_ptr<Item> item(new Item(4001126));
		drop_item_from_mob(drop_pos_x, drop_pos_y, mob, item, killer_player_id);
	}

	// drop mesos

	// TODO: fix mesos from mobs

	int mesos = (100 * kMesoRate);

	drop_pos_x = (mob->get_position_x() + 25 * ((count + count % 2) / 2) * ((count % 2) ? 1 : -1));
	drop_pos_y = mob->get_position_y();

	drop_mesos_from_mob(drop_pos_x, drop_pos_y, mob, mesos, killer_player_id);

	auto drops = MobDropsDataProvider::get_instance()->get_drop_data(mob->get_monster_id());

	if (!drops)
	{
		return;
	}

	for (auto it : *drops)
	{
		DropData *drop = it;
		int chance = drop->chance;
		int drop_chance = (chance * kDropRate);
		int random_value = tools::random_int(1, 999999);

		if (random_value < drop_chance)
		{
			drop_pos_x = (mob->get_position_x() + 25 * ((count + count % 2) / 2) * ((count % 2) ? 1 : -1));
			drop_pos_y = mob->get_position_y();
			++count;

			std::shared_ptr<Item> item(new Item(drop->item_id, true));

			drop_item_from_mob(drop_pos_x, drop_pos_y, mob, item, killer_player_id);
		}
	}
}

void Map::drop_item_from_mob(short position_x, short position_y, Mob *mob, std::shared_ptr<Item> item, int owner)
{
	find_foothold(position_x, position_y);
	std::shared_ptr<Drop> drop(new Drop(get_drop_object_id(), kDropPacketConstantsPickupTypesNormal, position_x, position_y, 0, owner, item));

	drops_[drop->get_id()] = drop;

	PacketCreator packet29;
	packet29.ShowDrop(kDropPacketConstantsAnimationTypesDropAnimation, drop, mob->get_position_x(), mob->get_position_y());
	send_packet(&packet29);
}

void Map::drop_mesos_from_mob(short position_x, short position_y, Mob *mob, int amount, int owner)
{
	find_foothold(position_x, position_y);
	std::shared_ptr<Drop> drop(new Drop(get_drop_object_id(), kDropPacketConstantsPickupTypesNormal, position_x, position_y, amount, owner, nullptr));

	drops_[drop->get_id()] = drop;

	PacketCreator packet29;
	packet29.ShowDrop(kDropPacketConstantsAnimationTypesDropAnimation, drop, mob->get_position_x(), mob->get_position_y());
	send_packet(&packet29);
}

void Map::drop_from_player(std::shared_ptr<Item> item, Player *player)
{
	ItemData *data = ItemDataProvider::get_instance()->get_item_data(item->get_item_id());

	if (!data->trade_able)
	{
		return;
	}

	int drop_id = get_drop_object_id();

	short x = player->get_position_x();
	short y = player->get_position_y();
	short player_position_y = y;

	find_foothold(x, y);

	std::shared_ptr<Drop> drop(new Drop(drop_id, kDropPacketConstantsPickupTypesFreeForAll, x, y, 0, 0, item));

	drops_[drop_id] = drop;

	PacketCreator packet;
	packet.ShowDrop(kDropPacketConstantsAnimationTypesDropAnimation, drop, x, player_position_y);
	send_packet(&packet);
}

void Map::kill(Mob *mob)
{
	Player *control = mob->get_owner();

	if (control && control->get_map() == this)
	{
		// packet
		PacketCreator packet1;
		packet1.EndControlMob(mob->get_object_id());
		control->send_packet(&packet1);
	}

	mob->set_owner(nullptr);
	mob->set_is_dead(true);

	mob->give_exp();

	drop_from_mob(mob, mob->get_killer_player_id());

	MobData *data = MobDataProvider::get_instance()->get_data_by_id(mob->get_monster_id());

	if (!data)
	{
		return;
	}

	// TODO: fix bug: mobs spawned by mobs always respawn

	for (auto monster_id : data->mob_summons_)
	{
		spawn_monster(monster_id, mob->get_position_x(), mob->get_position_y(), mob->get_original_foothold(), mob_constants::kSpawnTypesSummonByMob, mob->get_stance() % 2 == 0, mob->get_object_id());
	}

	// packet
	PacketCreator packet8;
	packet8.KillMob(mob->get_object_id());
	send_packet(&packet8);

	// zakum and other bosses check
	if (bossing_)
	{
		// zakum
		if (mob->get_monster_id() >= 8800003 && mob->get_monster_id() <= 8800010)
		{
			Mob *zakum_body = nullptr;

			for (Mob *temp_mob : mobs_)
			{
				if (!temp_mob->is_dead())
				{
					if (temp_mob->get_monster_id() >= 8800003 && temp_mob->get_monster_id() <= 8800010)
					{
						return;
					}
					if (temp_mob->get_monster_id() == 8800000)
					{
						zakum_body = temp_mob;
					}
				}
			}
			if (zakum_body)
			{
				Player *control = zakum_body->get_owner();

				if (control && control->get_map() == this)
				{
					// packet
					PacketCreator packet19;
					packet19.EndControlMob(zakum_body->get_object_id());
					control->send_packet(&packet19);
				}

				zakum_body->set_owner(nullptr);

				// packet
				PacketCreator packet34;
				packet34.KillMob(zakum_body->get_object_id());
				send_packet(&packet34);

				zakum_body->reset();
			}
		}
	}

	// if the mob is not part of the map, it isn't supposed to be respawned
	// so delete the object and erase the pointer from the container

	if (!mob->is_map_mob())
	{
		delete mob;

		auto it = mobs_.begin();
		while (it != mobs_.end())
		{
			if (*it == mob)
			{
				it = mobs_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void Map::spawn_monster(int mob_id, short x, short y, short foothold, signed char spawn_type, bool flip, int from)
{
	MobData *data = MobDataProvider::get_instance()->get_data_by_id(mob_id);

	if (!data)
	{
		return;
	}

	Mob *mob = new Mob(this, mob_id, get_mob_object_id(), x, y, foothold, flip ? 3 : 2, data->max_hp_, data->max_mp_, data->is_boss_, false);

	// packet
	PacketCreator packet1;
	packet1.SpawnMonster(mob, spawn_type, from);
	send_packet(&packet1);

	mob->find_control(true);
	mobs_.push_back(mob);
}

void Map::spawn_monster_init(int mob_id, short x, short y, short foothold, bool flip)
{
	MobData *data = MobDataProvider::get_instance()->get_data_by_id(mob_id);
	if (!data)
	{
		return;
	}

	Mob *mob = new Mob(this, mob_id, get_mob_object_id(), x, y, foothold, flip ? 3 : 2, data->max_hp_, data->max_mp_, data->is_boss_, true);
	mobs_.push_back(mob);
}

std::vector<Mob *> *Map::get_original_mobs()
{
	return &mobs_;
}

void Map::enable_portal(std::string portal_name)
{
	portals_status_[portal_name] = true;
}

bool Map::is_portal_enabled(std::string portal_name)
{
	return portals_status_[portal_name];
}
