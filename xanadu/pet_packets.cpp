//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"
#include "inventory.hpp"
#include "item.hpp"
#include "constants.hpp"

void PacketCreator::PetStatUpdate(Player *player)
{
	write<short>(send_headers::kUPDATE_STATS);
	write<bool>(true); // 1 / true = fix client lock
	write<int>(kCharacterStatsPet);

	auto pets = *player->get_pets();

	write<int>(pets.size() > 0 ? pets[0]->get_unique_id() : 0);
	write<int>(0);
	write<int>(pets.size() > 1 ? pets[1]->get_unique_id() : 0);
	write<int>(0);
	write<int>(pets.size() > 2 ? pets[2]->get_unique_id() : 0);
	write<int>(0);

	write<signed char>(0);
}

void PacketCreator::ShowOwnPetLevelUp(signed char pet_slot)
{
	write<short>(send_headers::kSHOW_ITEM_GAIN_INCHAT);
	write<signed char>(4);
	write<signed char>(0);
	write<signed char>(pet_slot);
}

void PacketCreator::ShowPetLevelUp(int owner_player_id, signed char pet_slot)
{
	write<short>(send_headers::kSHOW_FOREIGN_EFFECT);
	write<int>(owner_player_id);
	write<signed char>(4);
	write<signed char>(0);
	write<signed char>(pet_slot);
}

void PacketCreator::UpdatePet(Item *pet)
{
	write<short>(send_headers::kMODIFY_INVENTORY_ITEM);
	write<signed char>(0);
	write<signed char>(1);
	write<signed char>(0);
	write<signed char>(kInventoryConstantsTypesCash);
	write<signed char>(pet->get_slot());
	write<signed char>(0);
	ItemInfo(pet, false);
}

// show byte: 1 = show, 0 = remove

void PacketCreator::ShowPet(int owner_player_id, std::shared_ptr<Item> pet, bool show)
{
	write<short>(send_headers::kPET_SPAWN);
	write<int>(owner_player_id);
	write<signed char>(pet->get_pet_slot());
	write<signed char>(show);
	write<signed char>(0);

	if (show)
	{
		write<int>(pet->get_item_id());
		write<std::string>(pet->get_name());
		write<int>(pet->get_unique_id());
		write<int>(0);
		write<short>(pet->get_position_x());
		write<short>(pet->get_position_y());
		write<signed char>(pet->get_stance());
		write<short>(0); // foothold
		write<short>(0);
	}
}

void PacketCreator::MovePet(int owner_player_id, signed char pet_slot, int pet_object_id, unsigned char* buffer, int size)
{
	write<short>(send_headers::kPET_MOVE);
	write<int>(owner_player_id);
	write<signed char>(pet_slot);
	write<int>(pet_object_id);

	memcpy(buffer_ + length_, buffer, size);
	length_ += size;
}

void PacketCreator::PetCommandReplay(int owner_player_id, std::shared_ptr<Item> pet, signed char animation)
{
	write<short>(send_headers::kPET_COMMAND);
	write<int>(owner_player_id);
	write<signed char>(pet->get_pet_slot());
	write<signed char>(0); // if not food, this byte is needed
	write<bool>(animation == 1);
	write<signed char>(animation);
	write<signed char>(0); // success byte
	write<signed char>(0);
}

void PacketCreator::ShowPetChat(int owner_player_id, std::shared_ptr<Item> pet, signed char act, const std::string& message)
{
	write<short>(send_headers::kPET_CHAT);
	write<int>(owner_player_id);
	write<signed char>(pet->get_pet_slot());
	write<signed char>(0);
	write<signed char>(act);
	write<std::string>(message);
	write<signed char>(0);
}
