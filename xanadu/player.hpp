//

#pragma once

#include <unordered_map>
#include <string>

#include "character.hpp"
#include "item.hpp"
#include "key.hpp"
#include "skill.hpp"
#include "buddy.hpp"
#include "summon.hpp"
#include "tools.hpp"
#include "inventory.hpp"
#include "session.hpp"
#include "timer.hpp"
#include "timer_id.hpp"

class Effect;
class HiredMerchant;
class Messenger;
class Map;
class Party;
class Guild;
class ShopData;
class Quest;
class Timer;
class World;
class PacketCreator;
class PlayerAttackInfo;
class Summon;
class PlayerNpc;

class Player
{
public:

	// default constructor
	Player() = delete;

	// constructor
	Player(Session *session);

	// copy constructor
	Player(const Player &other) = delete;

	// move constructor
	Player(Player &&other) = default;
	
	// destructor
	~Player();

	// copy assignment operator
	Player &operator=(const Player &other) = default;

	// move assignment operator
	Player &operator=(Player &&other) = default;

	void player_connect();
	std::string get_name();
	bool is_married();
	signed char get_equip_slots();
	signed char get_use_slots();
	signed char get_setup_slots();
	signed char get_etc_slots();
	signed char get_cash_slots();
	signed char get_buddy_list_capacity();
	signed char get_gender();
	void set_skin_color(signed char id);
	signed char get_skin_color();
	void set_face(int id);
	int get_face();
	void set_hair(int id);
	int get_hair();
	void set_map(int mapid);
	Map *get_map();
	signed char get_spawn_point();
	int get_chair();
	std::string get_chalk_board();
	int get_item_effect();
	void set_party(Party *party);
	Party *get_party();
	void set_party_invitation(int invite);
	void set_guild(Guild *guild);
	Guild *get_guild();
	bool get_has_guild();
	void set_guild_rank(int rank);
	int get_guild_rank();
	bool get_is_in_cash_shop();
	bool get_is_gm();
	Inventory *get_inventory(signed char id);
	std::shared_ptr<HiredMerchant> get_hired_merchant();
	void set_hired_merchant(std::shared_ptr<HiredMerchant> merchant);

	void save();
	void save_account();
	void save_character();
	void save_skills();
	void save_equips();
	void save_items();
	void save_pets();
	void save_keymap();
	void save_quests();
	void save_buddylist();
	void save_storage_equips();
	void save_storage_items();
	void save_merchant_storage_equips();
	void save_merchant_storage_items();

	bool add_mesos(int amount);
	int get_mesos();
	void set_level(unsigned char level);
	unsigned char get_level();
	void set_job(short job);
	short get_job();
	void set_str(short str);
	short get_str();
	void set_dex(short dex);
	short get_dex();
	void set_int(short intt);
	short get_int();
	void set_luk(short luk);
	short get_luk();
	void set_hp(short hp);
	short get_hp();
	void set_mp(short mp);
	short get_mp();
	void add_hp(short hp);
	void add_mp(short mp);
	void set_max_hp(short mhp);
	short get_max_hp();
	void set_max_mp(short mmp);
	short get_max_mp();
	void set_hyperbody_max_hp(short value);
	short get_hyperbody_max_hp();
	void set_hyperbody_max_mp(short value);
	short get_hyperbody_max_mp();
	void set_ap(short ap);
	short get_ap();
	void set_sp(short sp);
	void add_sp(short sp);
	short get_sp();
	void set_fame(int fame);
	int get_fame();
	void set_exp(int exp);
	int get_exp();
	void add_nx_cash_credit(int nx_cash_credit);
	std::shared_ptr<Item> get_pet(int id);
	std::vector<std::shared_ptr<Item>> *get_pets();
	void level_up();
	void change_map(Map *new_map, signed char new_spawn_point = 0);
	short get_item_amount(int itemid);
	bool give_item(int itemid, short amount);
	void remove_item(int itemid, short amount);
	// messenger
	signed char get_messenger_position();
	void set_messenger_position(signed char position);
	Messenger *get_messenger();
	void set_messenger(Messenger *messenger);
	// summons
	Summon *get_summon();
	Summon *get_summon_by_id(int id);
	void add_summon(int skill_id, int skill_level, int hp, int time);
	void remove_summon();
	// buddies
	std::unordered_map<int, std::shared_ptr<Buddy>> *get_buddylist();
	Buddy *get_buddy(int id);
	void add_buddy(int id, std::string name, int channel);
	void add_buddy(std::string name);
	void accept_buddy(int id);
	void remove_buddy(int id);
	void change_buddy_channel(Player *buddy, int playerid, int channel);
	// keymap
	Key &get_key(int pos);
	// quests
	void give_quest(int quest_id);
	void complete_quest(int quest_id, int npc_id);
	void remove_quest(int quest_id);
	void initialize_player_quests(int quest_id, bool isCompleted, int mob_id, int amount);
	void rewards_data(int quest_id, bool start);
	std::unordered_map<int, std::shared_ptr<Quest>> *get_quests_in_progress();
	std::unordered_map<int, std::shared_ptr<Quest>> *get_completed_quests();
	// buffs
	bool is_buff_stat_active(long long buff_stat);
	bool is_buff_id_active(int buff_id);
	int get_buff_value(long long buff_stat);
	void cancel_buff(int buffid, bool byTimer);
	void delete_buff(Effect *effect, bool byTimer);
	void add_buff(Effect *effect);
	// npc Stuff
	void send_simple(std::string text);
	void send_yes_no(std::string text);
	void send_next(std::string text);
	void send_back_next(std::string text);
	void send_back_ok(std::string text);
	void send_ok(std::string text);
	void send_style(int styles[], int size);
	void set_state(int state);
	int get_state();
	void set_selected(int selected);
	int get_selected();
	void set_npc_variable(std::string name, int val);
	int get_npc_variable(std::string name);
	void set_style(int id);
	void create_guild();
	void disband_guild();
	void change_guild_emblem();
	// trading
	void reset_trade();
	void set_trade_partner(Player *partner);
	Player *get_trade_partner();
	bool is_trade_confirmed();
	int get_trade_mesos();
	std::vector<std::shared_ptr<Item>> *get_trade_items();
	std::unordered_map<int, Skill> *get_skills();
	unsigned char get_channel_id();
	void set_stance(signed char stance);
	signed char get_stance();
	short get_position_x();
	short get_position_y();
	short get_foothold();
	int get_id();
	bool is_hidden();
	void hide();
	void unhide();
	void send_packet(PacketCreator *packet);
	void disconnect();
	int get_mount_item_id();
	void set_mount_item_id(int mount_item_id);
	int get_mount_skill_id();
	void set_mount_skill_id(int mount_skill_id);
	std::unordered_map<timer::Id, std::shared_ptr<Timer>> *get_timers();
	void apply_attack_skill_costs(int skillId, signed char skillLevel);
	void add_donation_points(int amount);
	std::unordered_map<signed char, std::shared_ptr<Item>> *get_merchant_storage_items();
	void add_merchant_storage_mesos(int amount);
	signed char get_crusader_combo_value();
	void set_crusader_combo_value(signed char value);

	void send_npc(int npc_id);
	void send_npc_final();

	// npc script handlers
	void handle_donator_npc();
	void handle_maple_leaf_exchanger_npc();
	void handle_gachapon_npc();
	void handle_kin_npc();
	void handle_nimakin_npc();
	void handle_regular_cab_npc();
	void handle_nlc_taxi_npc();
	void handle_heracle_guild_creator_npc();
	void handle_lea_guild_emblem_npc();
	void handle_orbis_magic_spot_npc();
	void handle_el_nath_magic_spot_npc();
	void handle_jeff_npc();
	void handle_amon_zakum_npc();
	void handle_adobis_zakum_npc();
	void handle_first_eos_rock_npc();
	void handle_second_eos_rock_npc();
	void handle_third_eos_rock_npc();
	void handle_fourth_eos_rock_npc();
	void handle_papulatus_npc();
	void handle_horntail_entrance_npc();
	void handle_mage_job_advancer();
	void handle_thief_job_advancer();
	void handle_bowman_job_advancer();
	void handle_warrior_job_advancer();
	void handle_pirate_job_advancer();
	void handle_orbis_platform_usher();
	void handle_kpq_lakelis_npc();
	void handle_kpq_cloto_npc();
	void handle_kpq_nella_npc();
	void handle_hpq_tory_npc();
	void handle_hpq_tommy_npc();
	void handle_hpq_growlie_npc();
	void handle_lpq_red_sign_npc();
	void handle_npc_opq_wonky();
	void handle_npc_ppq_guon();
	void handle_npc_ppq_guon2();
	void handle_npc_lmpq_rolly();
	void handle_npc_lmpq_rolly2();
	void handle_npc_lmpq_rolly3();
	void handle_npc_lmpq_pietri();
	void handle_npc_cpq_spiegelmann();

	// packet handlers
	void handle_packet(unsigned short bytes_amount);
	void handle_packet_in_cashshop();
	void handle_packet_in_game();
	void handle_packet_in_login();
	void handle_login_request();
	void handle_pin_operation();
	void handle_channel_selection();
	void handle_world_selection();
	void handle_world_list_request();
	void handle_connect_game();
	void handle_character_creation_name_check();
	void handle_create_character();
	void handle_delete_character();
	void handle_relog_request();
	void handle_hp_mp_recovering();
	void handle_key_map_changes();
	void handle_summon_movement();
	void handle_puppet_damage();
	void handle_faming();
	void handle_cash_shop_enter();
	void handle_update_cash_shop();
	void handle_cash_shop_action();
	void handle_quest_action();
	void handle_player_hit();
	void handle_messenger_action();
	void handle_leave_cash_shop();
	void handle_change_map();
	void handle_use_scripted_portal();
	void handle_change_channel();
	void handle_get_player_info();
	void handle_player_drop_mesos();
	void handle_player_loot_drop();
	void handle_hired_merchant_request();
	void handle_item_transportation();
	void handle_player_movement();
	void handle_request_npc();
	void handle_npc_chat();
	void handle_npc_shop();
	void handle_storage_reqest();
	void handle_face_expression();
	void handle_add_stat();
	void handle_buddy_list_action();
	void handle_guild_action();
	void handle_party_action();
	void handle_party_invitation();
	void handle_use_pet();
	void handle_pet_movement();
	void handle_pet_command();
	void handle_pet_chat();
	void handle_pet_loot();
	void handle_player_use_chair();
	void handle_use_skill_book();
	void handle_player_cancel_chair();
	void handle_use_item_effect();
	void handle_close_chalkboard();
	void handle_cancel_item_buff();
	void handle_use_cash_item();
	void handle_move_item();
	void handle_use_item();
	void handle_use_return_scroll();
	void handle_use_scroll();
	void handle_inventory_sort();
	void handle_chat_command();
	void handle_use_chat();
	void handle_use_group_chat();
	void handle_add_skill_point();
	void handle_cancel_skill_buff();
	void handle_use_skill();
	void handle_use_special_skill();
	void handle_use_magic_attack();
	void handle_use_close_range_attack();
	void handle_use_ranged_attack();
	void handle_use_energy_attack();
	void handle_use_attack_base(PlayerAttackInfo &attack, signed char attack_handler_type);
	void handle_mob_movement();
	void handle_damage_mob_summon();
	void handle_merchant_storage_request();
	void handle_hit_reactor();

	// packet reading

	void skip_bytes(int amount);

	// templates

	// this function template should be used to read bytes based on integer types

	template<typename TYPE>
	TYPE read()
	{
		if (recv_length_ <= recv_pos_)
		{
			return 0;
		}

		if (sizeof(TYPE) > (recv_length_ - recv_pos_))
		{
			return 0;
		}

		TYPE val = *(TYPE *)(session_->get_receive_buffer() + recv_pos_);
		recv_pos_ += sizeof(TYPE);

		return val;
	}

	// template function specialisation for reading bytes based on the type bool

	template<>
	bool read<bool>()
	{
		bool x = read<signed char>() != 0;

		return x;
	}

	// template function specialisation for reading bytes based on the type std::string

	template<>
	std::string read<std::string>()
	{
		int len = read<short>();

		if (recv_length_ <= recv_pos_)
		{
			return "";
		}

		if (len > (recv_length_ - recv_pos_))
		{
			return "";
		}

		std::string s((char*)session_->get_receive_buffer() + recv_pos_, len);
		recv_pos_ += len;

		return s;
	}

private:

	bool in_hide_;
	bool trade_locked_;
	bool in_game_;
	bool in_cash_shop_;
	unsigned char level_;
	unsigned char channel_id_;
	signed char crusader_combo_value_;
	signed char storage_slots_;
	signed char gender_;
	signed char messenger_position_;
	signed char spawn_point_;
	signed char skin_color_;
	signed char stance_;
	short job_;
	short str_;
	short dex_;
	short int_;
	short luk_;
	short ap_;
	short sp_;
	short position_x_;
	short position_y_;
	short foothold_;
	short hp_;
	short max_hp_;
	short hyperbody_max_hp_;
	short mp_;
	short max_mp_;
	short hyperbody_max_mp_;
	int id_;
	int selected_equip_slot_;
	int donation_points_;
	int energy_bar_;
	int mount_skill_id_;
	int mount_item_id_;
	int character_slots_;
	int fame_;
	int face_;
	int hair_;
	int chair_;
	int exp_;
	int item_effect_;
	int is_gm_;
	int party_invite_;
	int guild_rank_;
	int nx_cash_credit_;
	int user_id_;
	int recv_length_;
	int recv_pos_;
	int storage_mesos_;
	int merchant_storage_mesos_;
	int trade_mesos;
	int mesos_;
	unsigned long long last_gm_call_ticks_;
	Map *map_;
	ShopData *shop_;
	Session *session_;
	Player *trade_partner_;
	Messenger *messenger_;
	Party *party_;
	Guild *guild_;
	Summon *summon_;
	PlayerNpc *npc_;
	std::shared_ptr<HiredMerchant> merchant_;
	std::string chalk_board_text_;
	std::string name_;
	std::string user_name_;
	std::vector<std::shared_ptr<Item>> trade_items_;
	std::vector<std::shared_ptr<Item>> pets_;
	std::vector<std::shared_ptr<Item>> storage_items_;
	std::unordered_map<int, std::shared_ptr<Item>> mark_slot_;
	std::unordered_map<int, std::shared_ptr<Item>> equip_slot_;
	std::unordered_map<int, Character *> characters_;
	std::unordered_map<int, Skill> skills_;
	std::unordered_map<int, Key> keys_;
	std::unordered_map<int, std::shared_ptr<Buddy>> buddies_;
	std::unordered_map<int, std::shared_ptr<Quest>> quests_in_progress_;
	std::unordered_map<int, std::shared_ptr<Quest>> completed_quests_;
	std::unordered_map<long long, Effect *> buffs_;
	std::unordered_map<int, Effect *> id_buffs_;
	std::unordered_map<signed char, std::shared_ptr<Item>> merchant_storage_items_;
	std::unordered_map<signed char, Inventory *> inventories_;
	std::unordered_map<timer::Id, std::shared_ptr<Timer>, std::hash<timer::Id>> timers_;
};
