//

#include "character.hpp"

// constructor

Character::Character(int player_id):
player_id_(player_id)
{
}

unsigned char Character::get_level()
{
	return level_amount_;
}

signed char Character::get_gender()
{
	return gender_id_;
}

signed char Character::get_skin()
{
	return skin_id_;
}

short Character::get_job()
{
	return job_id_;
}

short Character::get_str()
{
	return str_amount_;
}

short Character::get_dex()
{
	return dex_amount_;
}

short Character::get_int()
{
	return int_amount_;
}

short Character::get_luk()
{
	return luk_amount_;
}

int Character::get_face()
{
	return face_id_;
}

int Character::get_hair()
{
	return hair_id_;
}

int Character::get_id()
{
	return player_id_;
}

int Character::get_fame()
{
	return fame_amount_;
}

std::string Character::get_name()
{
	return name_;
}

std::unordered_map<int, signed char> *Character::get_equips()
{
	return &equips_;
}

void Character::set_level(unsigned char amount)
{
	level_amount_= amount;
}

void Character::set_gender(signed char id)
{
	gender_id_ = id;
}

void Character::set_skin(signed char id)
{
	skin_id_ = id;
}

void Character::set_job(short id)
{
	job_id_ = id;
}

void Character::set_str(short amount)
{
	str_amount_ = amount;
}

void Character::set_dex(short amount)
{
	dex_amount_ = amount;
}

void Character::set_int(short amount)
{
	int_amount_ = amount;
}

void Character::set_luk(short amount)
{
	luk_amount_ = amount;
}

void Character::set_face(int face_id)
{
	face_id_ = face_id;
}

void Character::set_hair(int hair_id)
{
	hair_id_ = hair_id;
}

void Character::set_fame(int amount)
{
	fame_amount_ = amount;
}

void Character::set_name(std::string name)
{
	name_ = name;
}

void Character::add_equip(int equip_id, signed char pos)
{
	equips_[equip_id] = pos;
}
