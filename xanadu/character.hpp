//

#pragma once

#include <string>
#include <unordered_map>

class Character
{
public:

	// constructor
	Character(int player_id);

	// default constructor
	Character() = delete;

	// copy constructor
	Character(const Character &other) = delete;

	// move constructor
	Character(Character &&other) = delete;

	// destructor
	~Character() = default;

	// copy assignment operator
	Character &operator=(const Character &other) = delete;

	// move assignment operator
	Character &operator=(Character &&other) = delete;

	unsigned char get_level();
	signed char get_gender();
	signed char get_skin();
	short get_job();
	short get_str();
	short get_dex();
	short get_int();
	short get_luk();
	int get_face();
	int get_hair();
	int get_id();
	int get_fame();
	std::string get_name();
	std::unordered_map<int, signed char> *get_equips();
	void set_level(unsigned char amount);
	void set_gender(signed char id);
	void set_skin(signed char id);
	void set_job(short id);
	void set_str(short amount);
	void set_dex(short amount);
	void set_int(short amount);
	void set_luk(short amount);
	void set_face(int id);
	void set_hair(int id);
	void set_fame(int amount);
	void set_name(std::string name);
	void add_equip(int equip_id, signed char pos);

private:

	unsigned char level_amount_;
	signed char gender_id_;
	signed char skin_id_;
	short job_id_;
	short str_amount_;
	short dex_amount_;
	short int_amount_;
	short luk_amount_;
	int face_id_;
	int hair_id_;
	int player_id_;
	int fame_amount_;
	std::string name_;
	std::unordered_map<int, signed char> equips_;
};
