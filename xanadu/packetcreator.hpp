//

#pragma once

#include <memory>
#include <unordered_map>

#include "packet_constants.hpp"

class Character;
class Drop;
class Effect;
class Item;
class Inventory;
class Mob;
class Npc;
class Guild;
class GuildMember;
class HiredMerchant;
class Quest;
class Party;
class PartyMember;
class Player;
class PlayerAttackInfo;
class Summon;
class ShopData;
class Values;
class world;
struct MapReactorData;

class PacketCreator
{
public:

	// default constructor
	PacketCreator();

	// copy constructor
	PacketCreator(const PacketCreator&) = delete;

	// move constructor
	PacketCreator(PacketCreator&&) = delete;

	// destructor
	~PacketCreator();

	// copy assignment operator
	PacketCreator &operator=(const PacketCreator&) = delete;

	// move assignment operator
	PacketCreator &operator=(PacketCreator&&) = delete;

	void write_bytes(char *hex);
	void write_null(size_t amount);
	void write_string(std::string str, std::size_t slen);
	unsigned char *get_buffer();
	size_t get_length();

	// login
	void get_handshake(unsigned char *iv_recv, unsigned char *iv_send);
	void ConnectToChannel(int player_id);
	void ShowCharacter(Character *character);
	void GetAuthSuccessRequest(int user_id, std::string account_name);
	void GetLoginFailed(signed char reason);
	void LoginProcess();
	void ShowWorld();
	void EndWorlds();
	void ShowChannels();
	void ShowCharacters(std::unordered_map<int, Character *> *characters, int character_slots);
	void CheckName(std::string name, bool name_used);
	void AddCharacter(Character *character);
	void RemoveCharacter(int character_id);
	void RelogResponse();
	// buddy
	void BuddyList(Player *player);
	void BuddyListInvite(Player *player);
	void UpdateBuddyChannel(int player_id, int channel_id);
	void UpdateBuddyListCapacity(Player *player);
	// cash
	void EnterCashShop(Player *player);
	void ShowCashPoints(int nx_credit);
	void ShowBoughtCashItem(int account_id, int serial_number, int item_id, int amount);
	void IncreaseInventorySlots(signed char inventory, signed char slots);
	void IncreaseStorageSlots(signed char slots);
	void GetCashShopInventory(short storage_slots, short character_slots);
	void TakeOutFromCashShopInventory(Item *item, short position);
	void TransferToCashShopInventory(int account_id, int serial_number, int item_id, int amount);
	// chat
	void ShowChatMessage(Player *player, const std::string &message, bool bubble_only);
	void ShowMessage(const std::string &message, unsigned char type, unsigned char channel_id = 0, unsigned char whisper = 0, std::shared_ptr<Item> item = nullptr);
	void FindPlayerReply(const std::string &name, bool success);
	void FindPlayerCashShop(const std::string &name);
	void FindPlayerMap(const std::string &name, int mapid);
	void FindPlayerChannel(const std::string &name, int channel);
	void WhisperPlayer(Player *player, const std::string &message);
	void ShowSpecialChat(unsigned char type, const std::string &name, const std::string &message);
	// drops
	void ShowDrop(signed char type, std::shared_ptr<Drop> drop, short pos_x, short pos_y);
	void RemoveDrop(int drop_id, signed char animation, int player_id, signed char pet_slot);
	void get_inventory_full();
	void CantGetAnymoreItems();
	void GainExp(int exp, bool in_chat, bool white, int party_bonus);
	void GainItem(int itemid, short amount);
	void GainMesos(int amount);
	// fame
	void SendFame(std::string name, signed char type);
	void SendFamee(std::string name2, signed char type, int new_fame);
	// guilds
	void GuildMemberData(GuildMember *member);
	void CreateGuild();
	void GuildInfo(Guild *guild);
	void AddGuildPlayer(int guild_id, GuildMember *member);
	void InviteGuild(Player *inviter);
	void GuildPlayerLeave(int guild_id, int char_id, std::string char_name, bool expelled = false);
	void change_guild_emblem();
	void ShowGuildEmblem(Guild *guild);
	void ChangeRank(int guild_id, int user_id, int rank);
	void DisbandGuild(int guild_id);
	void UpdateGuildRanks(Guild *guild);
	void UpdateGuildNotice(Guild *guild);
	void GuildMemberOnline(int guild_id, int character_id, bool online = true);
	// inventory
	void UpdateSlot(std::shared_ptr<Item> item);
	void MoveItem(signed char inventory_id, short source_slot, short destination_slot);
	void NewItem(std::shared_ptr<Item> item, bool from_drop);
	void remove_item(signed char inventory_id, int slot, bool from_drop);
	void MoveItemMerge(signed char inventory_id, short source_slot, short destination_slot, short amount);
	void MoveItemMergeTwo(signed char inventory_id, short source_slot, short source_amount, short destination_slot, short destination_amount);
	void ScrolledItem(std::shared_ptr<Item> scroll, std::shared_ptr<Item> equip, bool destroyed);
	// items
	void ItemInfo(Item *item, bool show_position = true);
	void ShowAvatarMega(Player *player, unsigned char ear, int item_id, std::string message, std::string message2, std::string message3, std::string message4);
	void HandleCloseChalkboard(int player_id);
	void UseChalkBoard(int player_id, const std::string &message);
	void UseScroll(int player_id, bool success, bool cursed, bool legendary_spirit);
	void UseItemEffect(int player_id, int item_id);
	void ShowChair(int player_id, int item_id);
	void CancelChair();
	// pet
	void ShowPet(int owner_player_id, std::shared_ptr<Item> pet, bool show);
	void MovePet(int owner_player_id, signed char pet_slot, int pet_object_id, unsigned char *buffer, int size);
	void ShowOwnPetLevelUp(signed char pet_slot);
	void ShowPetLevelUp(int owner_player_id, signed char pet_slot);
	void UpdatePet(Item *pet);
	void PetCommandReplay(int owner_player_id, std::shared_ptr<Item> pet, signed char animation);
	void ShowPetChat(int owner_player_id, std::shared_ptr<Item> pet, signed char act, const std::string &message);
	// map
	void ShowPlayer(Player *player);
	void RemovePlayer(Player *player);
	void MapEffect(signed char mode, std::string name);
	void ShowTimer(int seconds);
	// mobs
	void ControlMob(Mob *mob, signed char spawn_type);
	void EndControlMob(int mob_object_id);
	void SpawnMonster(Mob *mob, signed char spawn_type, int from = -1);
	void MoveMob(int mob_object_id, bool use_skill, signed char action, int unk, short start_position_x, short start_position_y, unsigned char *buffer, int buffer_size);
	void MoveMobResponse(Mob *mob, short move_id, bool use_skill, signed char skill_id, signed char skill_level);
	void ShowBossHp(int mob_id, int hp, int max_hp, signed char color, signed char background_color);
	void ShowMobHp(int mob_object_id, signed char hp_percent);
	void KillMob(int mob_object_id);
	void PlayerAttack(PlayerAttackInfo &attack);
	void PlayerCloseRangedAttack(PlayerAttackInfo &attack);
	void PlayerRangedAttack(PlayerAttackInfo &attack);
	void PlayerMagicAttack(PlayerAttackInfo &attack);
	void PlayerEnergyAttack(PlayerAttackInfo &attack);
	void DamageSummon(int player_id, int skill_id, int damage, signed char unk, int mob_id_from);
	void DamageMobSummon(Player *player, int summon_object_id, signed char animation, std::unordered_map<int, int> damages);
	// npcs
	void ShowNpc(Npc *npc);
	void Bought(signed char code);
	void ShowNpcShop(ShopData *data);
	void send_simple(int npc_id, std::string text);
	void send_yes_no(int npc_id, std::string text);
	void send_back_next(int npc_id, std::string text, bool back, bool next);
	void send_style(int styles[], int size, int npc_id, std::string &text);
	// storage
	void FredrickMessage(signed char type);
	void GetFredrickStorage(std::unordered_map<signed char, std::shared_ptr<Item>> items, int mesos);
	void GetStorage(int npc_id, signed char slots, std::vector<std::shared_ptr<Item>> items, int mesos);
	void GetStorageFull();
	void MesoStorage(signed char slots, int mesos);
	void StoreStorage(signed char slots, signed char inventory_id, std::vector<std::shared_ptr<Item>> items);
	void TakeOutStorage(signed char slots, signed char inventory_id, std::vector<std::shared_ptr<Item>> items);
	// party
	void PartyInfo(Party *party);
	void PartyCreated(int party_id);
	void LeaveParty(Party *party, int player_id, bool leave, std::string &player_name, bool expel);
	void InviteParty(Player *from);
	void JoinParty(Player *player);
	void ChangeLeader(int new_leader_player_id, bool leader_disconnected);
	void UpdateParty(Party *party);
	void UpdatePartyHp(Player *player);
	// player
	void AddInventoryInfo(Player *player);
	void AddSkillInfo(Player *player);
	void AddCoolDownInfo(Player *player);
	void AddQuestInfo(Player *player);
	void AddRingInfo();
	void AddTeleportRockInfo();
	void writeRngSeeds();
	void AddCharStats(Character *character);
	void AddCharStats(Player *player);
	void AddCharLook(Character *character, bool megaphone = false);
	void AddCharLook(Player *player, bool megaphone = false);
	void writeCharacterData(Player *player);
	void UpdatePlayer(Player *player);
	void change_map(Player *player, bool is_connect_packet);
	void ShowKeymap(Player *player);
	void ShowPlayerMovement(int player_id, unsigned char *buffer, int buffer_size);
	void FaceExpression(int player_id, int face);
	void ShowInfo(Player *player);
	// skills
	void SendCooldown(int skill_id, int time);
	void ShowSkillEffect(int player_id, int skill_id, signed char skill_level, signed char flags, signed char speed);
	void CancelSkillEffect(int player_id, int skill_id);
	void UpdateSkills(Player *player);
	void UpdateSkill(int skill_id, int skill_level, int master_level);
	void ShowMonsterRiding(int player_id, int item_id, int skill_id);
	void ShowForeignEffect(int player_id, signed char effect);
	void ShowBuffEffect(int player_id, signed char effect_id, int skill_id, signed char skill_level);
	void writeBuffMask(Values *values);
	void ShowPlayerBuff(Values *values, int skill_id, int time);
	void CancelPlayerBuff(Values *values);
	void ShowMapBuff(int player_id, Values *values);
	void CancelMapBuff(int player_id, Values *values);
	void GiveEnergyCharge(int player_id, int bar, int time);
	void SpawnDoor(int object_id, bool town, short position_x, short position_y);
	void RemoveDoor(int object_id, bool town);
	void SpawnMist(int object_id, int owner_character_id, int skill_id, int mist_position_x, int mist_position_y, int mist_position_width, int mist_position_height, int skill_level);
	void RemoveMist(int object_id);
	// hired Merchant
	void HiredMerchantBox();
	void GetHiredMerchant(Player *player, std::shared_ptr<HiredMerchant> merchant, bool first_time);
	void UpdateHiredMerchant(HiredMerchant *hired_merchant);
	void HiredMerchantChat(signed char slot, std::string &message);
	void HiredMerchantVisitorLeave(signed char slot);
	void HiredMerchantVisitorAdd(Player *player, signed char slot);
	void CloseMerchantStoreResponse();
	void MerchantStoreMaintenanceResponse(int merchant_id);
	void SpawnHiredMerchant(std::shared_ptr<HiredMerchant> merchant);
	void DestroyHiredMerchant(int id);
	void LeaveHiredMerchant(int slot, int status2);
	void PlayerShopError(signed char type);
	void PlayerShopMessage(signed char message);
	// stats
	void EnableAction(bool unstuck = true);
	void UpdateLevel(unsigned char level);
	void UpdateSp(Player *player, short value);
	void UpdateApStats(short str, short dex, short intt, short luk, short ap);
	void UpdateStatShort(int stat, short value);
	void UpdateStatInt(int stat, int value);
	void PetStatUpdate(Player *player);
	// quests
	void ForfeitQuest(short quest_id);
	void UpdateQuest(Quest *quest, int npc_id);
	void UpdateQuestInfo(Quest *quest);
	void ItemGainChat(int item_id, int amount, signed char items_size = 1);
	void MesosGainChat(int amount);
	void FameGainChat(int amount);
	// reactors
	void SpawnReactor(int object_id, MapReactorData *reactor);
	void DestroyReactor(int object_id, MapReactorData *reactor);
	// summon
	void SpawnSummon(Player *player, Summon *summon, bool animated);
	void MoveSummon(int owner_player_id, unsigned char *buffer, int size);
	void remove_summon(int owner_player_id, int summon_id);
	// trade
	void ShowTrade(Player *player, Player *partner, signed char number);
	void InviteTrade(Player *player);
	void JoinTrade(Player *player);
	void TradeError(signed char side, signed char message);
	void ShowTradeChat(signed char side, std::string &message);
	void TradeMesos(int mesos, signed char number);
	void TradeItem(std::shared_ptr<Item> item, signed char number);
	void TradeConfirm();
	// messenger
	void MessengerAddPlayer(Player *player);
	void MessengerRemovePlayer(signed char pos);
	void MessengerJoin(signed char pos);
	void MessengerChat(std::string name, std::string text);
	void MessengerNote(std::string invited, bool invite_successful);
	void MessengerDeclined(std::string invited);
	void MessengerInvite(Player *inviter);
	// party quest
	void StartCarnivalPartyQuest(unsigned char team);
	void obtain_cp();
	void obtain_party_cp(unsigned char team);
	void carnival_pq_message(signed char message);
	void carnival_pq_summon(signed char tab, signed char summon_number, std::string player_name);

	// templates

	// this function template should be used to write bytes based on integer types

	template<typename TYPE>
	void write(TYPE value)
	{
		// adjust buffer

		if (length_ + sizeof(TYPE) >= buffer_size_)
		{
			buffer_ = (unsigned char*)realloc(buffer_, buffer_size_ + kIncreasePacketBufferSize);
			buffer_size_ += kIncreasePacketBufferSize;
		}

		*(TYPE*)(buffer_ + length_) = value;
		length_ += sizeof(TYPE);
	}

	// template function specialisation for writing bytes based on the type bool

	template<>
	void write<bool>(bool value)
	{
		// adjust buffer

		if (length_ + sizeof(bool) >= buffer_size_)
		{
			buffer_ = (unsigned char*)realloc(buffer_, buffer_size_ + kIncreasePacketBufferSize);
			buffer_size_ += kIncreasePacketBufferSize;
		}

		buffer_[length_] = (value ? 1 : 0);
		++length_;
	}

	// template function specialisation for writing bytes based on the type std::string

	template<>
	void write<std::string>(std::string str)
	{
		size_t len = str.length();

		// adjust buffer

		if (length_ + len >= buffer_size_)
		{
			buffer_ = (unsigned char*)realloc(buffer_, buffer_size_ + kIncreasePacketBufferSize);
			buffer_size_ += kIncreasePacketBufferSize;
		}

		write<short>(static_cast<short>(len));

		memcpy(buffer_ + length_, str.c_str(), len);
		length_ += len;
	}

private:

	unsigned char *buffer_;
	size_t length_;
	size_t buffer_size_;
};
