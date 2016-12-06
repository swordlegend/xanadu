//

#pragma once

#include <unordered_map>
#include <memory>

class PacketCreator;
class GuildMember;
class Map;
class Player;

class Guild
{
public:

	// constructor
	Guild(int id, std::string name);

	// default constructor
	Guild() = delete;

	// copy constructor
	Guild(const Guild &other) = default;

	// move constructor
	Guild(Guild &&other) = default;

	// destructor
	~Guild();

	// copy assignment operator
	Guild &operator=(const Guild &other) = default;

	// move assignment operator
	Guild &operator=(Guild &&other) = default;

	signed char get_logo_color();
	signed char get_logo_background_color();
	short get_logo();
	short get_logo_background();
	int get_capacity();
	int get_points();
	int get_alliance_id();
	int get_id();
	std::string get_name();
	std::string GetNotice();
	std::string GetRank1();
	std::string GetRank2();
	std::string GetRank3();
	std::string GetRank4();
	std::string GetRank5();
	std::unordered_map<int, std::unique_ptr<GuildMember>> *get_members();
	GuildMember *GetMember(int player_id);
	void SetLogoColor(signed char color_);
	void SetLogoBackgroundColor(signed char background_color);
	void SetLogo(short logo);
	void SetLogoBackground(short background);
	void set_name(const std::string &name);
	void SetNotice(const std::string &notice);
	void SetRank1(std::string rank);
	void SetRank2(std::string rank);
	void SetRank3(std::string rank);
	void SetRank4(std::string rank);
	void SetRank5(std::string rank);
	void AddMember(Player *player);
	void AddMember(GuildMember *member);
	void DeleteMember(int player_id);
	void RemoveMember(int player_id);
	void send_packet(PacketCreator *packet);
	void send_packet(PacketCreator *packet, Player *excluded_player);

private:

	signed char logo_color_;
	signed char logo_background_color_;
	short logo_;
	short logo_background_;
	int points_;
	int capacity_;
	int alliance_id_;
	int id_;
	std::string name_;
	std::string notice_;
	std::string rank1_;
	std::string rank2_;
	std::string rank3_;
	std::string rank4_;
	std::string rank5_;
	std::unordered_map<int, Player *> players_;
	std::unordered_map<int, std::unique_ptr<GuildMember>> members_;
};
