//

#include "player.hpp"

#include "inventory.hpp"
#include "map.hpp"
#include "session.hpp"
#include "packetcreator.hpp"
#include "constants.hpp"

void Player::handle_use_pet()
{
	skip_bytes(4);
	signed char slot = read<signed char>();

	std::shared_ptr<Item> pet = get_inventory(kInventoryConstantsTypesCash)->get_item_by_slot(slot);

	if (!pet || !pet->is_pet())
	{
		return;
	}

	std::shared_ptr<Item> pet_test = get_pet(pet->get_unique_id());

	if (pet_test)
	{
		// remove pet
		for (size_t i = 0; i < pets_.size(); ++i)
		{
			if (pets_[i] == pet)
			{
				// packet
				PacketCreator packet4;
				packet4.ShowPet(id_, pet, false);
				map_->send_packet(&packet4);

				pet->set_pet_slot(-1);
				pets_.erase(pets_.begin() + i);

				break;
			}
		}

		for (size_t i = 0; i < pets_.size(); ++i)
		{
			pets_[i]->set_pet_slot(static_cast<signed char>(i));
		}

		// packet
		PacketCreator packet118;
		packet118.EnableAction();
		send_packet(&packet118);
	}
	else
	{
		// add pet
		if (pets_.size() == 3)
		{
			// instead of the first pet
			
			pets_[0]->set_pet_slot(-1);
			pet->set_pet_slot(0);
			pets_[0] = pet;
		}
		else
		{
			pet->set_pet_slot(static_cast<signed char>(pets_.size()));
			pets_.push_back(pet);
		}
		
		pet->set_position(position_x_, position_y_, 0);

		// packet
		PacketCreator packet7;
		packet7.ShowPet(id_, pet, true);
		map_->send_packet(&packet7);

		// packet
		PacketCreator packet8;
		packet8.PetStatUpdate(this);
		send_packet(&packet8);
	}
}

void Player::handle_pet_movement()
{
	int pet_object_id = read<int>();

	std::shared_ptr<Item> pet = get_pet(pet_object_id);

	if (!pet)
	{
		return;
	}

	skip_bytes(4);

	short start_position_x = read<short>();
	short start_position_y = read<short>();

	pet->set_position(start_position_x, start_position_y, 0);

	// only send if there are other players in the map
	if (map_->get_players()->size() > 1)
	{
		// packet
		PacketCreator packet1;
		// exclude the first 2 bytes (header) + 12 bytes = 14 and from the packet that is to be sent
		packet1.MovePet(id_, pet->get_pet_slot(), pet_object_id, session_->get_receive_buffer() + 14, recv_length_ - 14);
		map_->send_packet(&packet1, this);
	}
}

void Player::handle_pet_command()
{
	int pet_object_id = read<int>();
	std::shared_ptr<Item> pet = get_pet(pet_object_id);

	if (!pet)
	{
		return;
	}

	skip_bytes(5);
	signed char command = read<signed char>();
	pet->set_closeness(pet->get_pet_closeness() + 1, this);

	// packet
	PacketCreator packet1;
	packet1.PetCommandReplay(id_, pet, command);
	send_packet(&packet1);
}

void Player::handle_pet_chat()
{
	int pet_object_id = read<int>();
	std::shared_ptr<Item> pet = get_pet(pet_object_id);

	if (!pet)
	{
		return;
	}

	skip_bytes(5);
	signed char act = read<signed char>();
	std::string message = read<std::string>();

	// packet
	PacketCreator packet1;
	packet1.ShowPetChat(id_, pet, act, message);
	map_->send_packet(&packet1, this);
}

void Player::handle_pet_loot()
{
	int pet_object_id = read<int>();

	std::shared_ptr<Item> pet = get_pet(pet_object_id);

	if (!pet)
	{
		return;
	}
	skip_bytes(13);

	int drop_object_id = read<int>();

	std::shared_ptr<Drop> drop = map_->get_drop(drop_object_id);

	if (drop)
	{
		map_->loot_drop(this, drop, pet->get_pet_slot());
	}

	// packet
	PacketCreator packet118;
	packet118.EnableAction();
	send_packet(&packet118);
}
