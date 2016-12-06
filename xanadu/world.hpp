//
// by Buya (the developer's pseudonym)
//

#pragma once

#include <string>
#include <unordered_map>

#include "Poco\Data\Session.h"
#include "asio.hpp"

class Player;
class Channel;
class Party;
class Guild;
class Messenger;
class PacketCreator;
class WZMain;

class World
{
public:

	// constructor
	World(bool is_dedicated_server);

	// default constructor
	World() = delete;

	// copy constructor
	World(const World &other) = default;

	// move constructor
	World(World &&other) = default;

	// destructor
	~World() = default;

	// copy assignment operator
	World &operator=(const World &other) = default;

	// move assignment operator
	World &operator=(World &&other) = default;

	static World *get_instance();

	bool is_accepting_connections();
	void set_is_accepting_connections(bool value);

	bool is_dedicated_server();

	// unique ids
	int get_summon_id();
	int get_messenger_id();
	int get_party_id();
	int get_guild_id();
	int get_pet_id();

	// properties
	std::string get_name();
	int get_id();

	// ingame header message
	void set_header_message(std::string header_message);
	std::string get_header_message();

	// channels
	Channel *GetChannel(int id);

	// players
	Player *GetPlayerByName(std::string name);
	Player *GetPlayerById(int id);
	void AddPlayer(Player *player);
	void RemovePlayer(int player_id);
	void send_packet(PacketCreator *packet);
	std::unordered_map<int, Player *> *get_players();

	void ban_account(std::string player_name);
	void unban_account(std::string player_name);
	bool is_account_banned(int user_id);

	void add_name_id(std::string player_name, int player_id);
	void remove_name(std::string player_name, int player_id);
	bool is_name_used(std::string player_name);
	int get_player_id_from_name_(std::string player_name);
	std::string get_player_name_from_id(int player_id);

	void store_channel_id_for_user_id(unsigned char channel_id, int user_id);
	void erase_channel_id_for_user_id(int user_id);
	unsigned char get_channel_id_for_user_id(int user_id);

	void add_fame(int player_id);
	void remove_fame(int player_id);
	bool can_fame(int player_id);

	bool is_online_user(std::string user_name);
	void add_online_user(std::string user_name);
	void remove_online_user(std::string user_name);

	// messengers
	void remove_messenger(int id);
	void create_messenger(Player *player);
	void join_messenger(Player *player, int messenger_id);

	// parties
	Party *get_party(int id);
	void create_party(Player *player);
	void delete_party(int id);

	// guilds
	Guild *get_guild(int id);
	void create_guild(Player *player, std::string guild_name);
	void join_guild(Player *player, int id);
	void expel_guild_member(Guild *guild, int target_player_id, std::string target_player_name);
	void delete_guild(int guild_id);
	bool guild_name_used(std::string guild_name);

	// internal stuff
	void ShutdownServer();
	void initialize();
	Poco::Data::Session &get_mysql_session();
	asio::io_service &get_io_service();

	WZMain *wz_reader_;

private:

	static World *instance_;
	bool is_dedicated_server_;
	bool is_accepting_connections_;
	int id_;
	int summon_ids_;
	int messenger_ids_;
	int party_ids_;
	int guild_ids_;
	int pet_ids_;
	int exp_rate_;
	std::string header_message_;
	std::string name_;
	std::unordered_map<signed char, Channel *> channels_;
	std::unordered_map<int, Player *> online_players_;
	std::unordered_map<int, Messenger *> messengers_;
	std::unordered_map<int, Party *> parties_;
	std::unordered_map<int, Guild *> guilds_;
	std::unordered_map<std::string, int> online_users_;
	std::unordered_map<int, unsigned long long> player_fame_ticks_;
	std::unordered_map<int, std::string> player_id_name_;
	std::unordered_map<std::string, int> player_name_id_;
	std::unordered_map<int, unsigned char> user_id_channel_id_;
	Poco::Data::Session mysql_session_;
	asio::io_service io_service_;
};
