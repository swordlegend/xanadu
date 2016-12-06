//

#include "item.hpp"

#include "constants.hpp"
#include "world.hpp"
#include "pet_data_provider.hpp"
#include "pet_data.hpp"
#include "packetcreator.hpp"
#include "player.hpp"
#include "map.hpp"
#include "equip_data_provider.hpp"
#include "equip_data.hpp"

short getRandomStat(short stat)
{
	if (stat == 0)
	{
		return stat;
	}

	stat += (tools::random_int(0, 6) - 3);

	if (stat < 1)
	{
		stat = 1;
	}

	return stat;
}

// constructor

Item::Item(int item_id, bool random_equip_stats):
	slot_(0),
	flag_(kItemConstantsFlagsNone),
	amount_(1),
	item_id_(item_id),
	unique_id_(1),
	name_(""),
	// pet specific
	stance_(0),
	pet_level_(1),
	pet_slot_(-1),
	closeness_(0),
	position_x_(0),
	position_y_(0),
	// equip specific
	slots_(0),
	used_scrolls_(0),
	str_(0),
	dex_(0),
	intt_(0),
	luk_(0),
	hp_(0),
	mp_(0),
	weapon_atk_(0),
	magic_atk_(0),
	weapon_def_(0),
	magic_def_(0),
	acc_(0),
	avo_(0),
	hand_(0),
	speed_(0),
	jump_(0)
{
	if (is_pet())
	{
		PetData *data = PetDataProvider::get_instance()->get_data_by_id(item_id);
		if (data)
		{
			unique_id_ = World::get_instance()->get_pet_id();
			name_ = data->get_name();
		}
	}

	if (is_equip())
	{
		EquipData *data = EquipDataProvider::get_instance()->get_item_data(item_id);
		if (!data)
		{
			return;
		}

		slots_ = data->slots;

		if (random_equip_stats)
		{
			str_ = getRandomStat(data->str);
			dex_ = getRandomStat(data->dex);
			intt_ = getRandomStat(data->intt);
			luk_ = getRandomStat(data->luk);
			hp_ = getRandomStat(data->hp);
			mp_ = getRandomStat(data->mp);
			weapon_atk_ = getRandomStat(data->watk);
			magic_atk_ = getRandomStat(data->matk);
			weapon_def_ = getRandomStat(data->wdef);
			magic_def_ = getRandomStat(data->mdef);
			acc_ = getRandomStat(data->acc);
			avo_ = getRandomStat(data->avo);
			hand_ = getRandomStat(data->hand);
			jump_ = getRandomStat(data->jump);
			speed_ = getRandomStat(data->speed);
		}
		else
		{
			str_ = data->str;
			dex_ = data->dex;
			intt_ = data->intt;
			luk_ = data->luk;
			hp_ = data->hp;
			mp_ = data->mp;
			weapon_atk_ = data->watk;
			magic_atk_ = data->matk;
			weapon_def_ = data->wdef;
			magic_def_ = data->mdef;
			acc_ = data->acc;
			avo_ = data->avo;
			hand_ = data->hand;
			jump_ = data->jump;
			speed_ = data->speed;
		}
	}
}

bool Item::is_star()
{
	return tools::is_star(item_id_);
}

bool Item::is_pet()
{
	bool check = PetDataProvider::get_instance()->get_data_by_id(item_id_) != nullptr;
	return check;
}

bool Item::is_equip()
{
	bool check = EquipDataProvider::get_instance()->get_item_data(item_id_) != nullptr;
	return check;
}

signed char Item::get_inventory_id()
{
	return tools::get_inventory_id_from_item_id(item_id_);
}

signed char Item::get_slot()
{
	return slot_;
}

short Item::get_flag()
{
	return flag_;
}

short Item::get_amount()
{
	return amount_;
}

int Item::get_item_id()
{
	return item_id_;
}

int Item::get_unique_id()
{
	return unique_id_;
}

long long Item::get_expiration_time()
{
	return (is_pet() ? kPermanentTime : kNoExpirationTime);
}

void Item::set_slot(signed char value)
{
	slot_ = value;
}

void Item::set_amount(short value)
{
	amount_ = value;
}

void Item::set_flag(short value)
{
	flag_ = value;
}

void Item::set_item_id(int value)
{
	item_id_ = value;
}

std::string Item::get_name()
{
	return name_;
}

std::string Item::get_owner_name()
{
	return name_;
}

// pet specific

signed char Item::get_stance()
{
	return stance_;
}

signed char Item::get_pet_level()
{
	return pet_level_;
}

signed char Item::get_pet_slot()
{
	return pet_slot_;
}

signed char Item::get_pet_fullness()
{
	return 100;
}

short Item::get_pet_closeness()
{
	return closeness_;
}

short Item::get_position_x()
{
	return position_x_;
}

short Item::get_position_y()
{
	return position_y_;
}

void Item::set_pet_level(signed char level)
{
	pet_level_ = level;
}

void Item::set_pet_slot(signed char pet_slot)
{
	pet_slot_ = pet_slot;
}

void Item::set_closeness(short closeness, Player *player)
{
	closeness_ = closeness;

	if (closeness >= kPetLevels[pet_level_ - 1])
	{
		++pet_level_;
		
		// packet
		PacketCreator packet1;
		packet1.ShowOwnPetLevelUp(pet_slot_);
		player->send_packet(&packet1);

		// packet
		PacketCreator packet2;
		packet2.ShowPetLevelUp(player->get_id(), pet_slot_);
		player->get_map()->send_packet(&packet2, player);
	}

	// packet
	PacketCreator packet3;
	packet3.UpdatePet(this);
	player->send_packet(&packet3);
}

void Item::set_closeness(short closeness)
{
	closeness_ = closeness;
}

void Item::set_position(short x, short y, signed char stance)
{
	position_x_ = x;
	position_y_ = y;
	stance_ = stance;
}

// equip specific

void Item::set_free_slots(unsigned char slots)
{
	slots_ = slots;
}

unsigned char Item::get_free_slots()
{
	return slots_;
}

void Item::set_used_scrolls(unsigned char used_scrolls)
{
	used_scrolls_ = used_scrolls;
}

unsigned char Item::get_used_scrolls()
{
	return used_scrolls_;
}

signed char Item::get_boss_damage()
{
	return 0;
}

void Item::set_str(short str)
{
	str_ = str;
}

short Item::get_str()
{
	return str_;
}

void Item::set_dex(short dex)
{
	dex_ = dex;
}

short Item::get_dex()
{
	return dex_;
}

void Item::set_int(short intt)
{
	intt_ = intt;
}

short Item::get_int()
{
	return intt_;
}

void Item::set_luk(short luk)
{
	luk_ = luk;
}

short Item::get_luk()
{
	return luk_;
}

void Item::set_hp(short hp)
{
	hp_ = hp;
}

short Item::get_hp()
{
	return hp_;
}

void Item::set_mp(short mp)
{
	mp_ = mp;
}

short Item::get_mp()
{
	return mp_;
}

void Item::set_weapon_attack(short weapon_atk)
{
	weapon_atk_ = weapon_atk;
}

short Item::get_weapon_attack()
{
	return weapon_atk_;
}

void Item::set_magic_attack(short magic_atk)
{
	magic_atk_ = magic_atk;
}

short Item::get_magic_attack()
{
	return magic_atk_;
}

void Item::set_weapon_defense(short weapon_def)
{
	weapon_def_ = weapon_def;
}

short Item::get_weapon_defense()
{
	return weapon_def_;
}

void Item::set_magic_defense(short magic_def)
{
	magic_def_ = magic_def;
}

short Item::get_magic_defense()
{
	return magic_def_;
}

void Item::set_acc(short acc)
{
	acc_ = acc;
}

short Item::get_acc()
{
	return acc_;
}

void Item::set_avoid(short avo)
{
	avo_ = avo;
}

short Item::get_avoid()
{
	return avo_;
}

void Item::set_hand(short hand)
{
	hand_ = hand;
}

short Item::get_hand()
{
	return hand_;
}

void Item::set_speed(short speed)
{
	speed_ = speed;
}

short Item::get_speed()
{
	return speed_;
}

void Item::set_jump(short jump)
{
	jump_ = jump;
}

short Item::get_jump()
{
	return jump_;
}
