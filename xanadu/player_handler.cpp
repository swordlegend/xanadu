//

#include "player.hpp"

#include "item_data.hpp"
#include "packetcreator.hpp"
#include "hiredmerchant.hpp"
#include "map_portal_data.hpp"
#include "map.hpp"
#include "messenger.hpp"
#include "party.hpp"
#include "party_member.hpp"
#include "mob.hpp"
#include "effect.hpp"
#include "item.hpp"
#include "key.hpp"
#include "channel.hpp"
#include "world.hpp"
#include "inventory.hpp"
#include "summon.hpp"
#include "quest_data.hpp"
#include "quest_data_provider.hpp"
#include "receive_packet_opcodes.hpp"
#include "map_data_provider.hpp"
#include "map_data.hpp"
#include "buffstat_constants.hpp"
#include "constants.hpp"

void Player::handle_face_expression()
{
	// nothing to handle if there are no other players in the map
	if (map_->get_players()->size() == 1)
	{
		return;
	}

	// 1-7 are the standard emotes, above numbers are the cash emotes
	int emote = read<int>();

	if (emote < 1)
	{
		return;
	}

	// if it is a cash emote, check if the player has the cash item
	if (emote > 7)
	{
		Inventory *inventory = get_inventory(kInventoryConstantsTypesCash);

		if (!inventory)
		{
			return;
		}

		int emote_id = (5159992 + emote);

		if (inventory->get_item_amount(emote_id) == 0)
		{
			return;
		}
	}

	// create and send the packet
	PacketCreator packet1;
	packet1.FaceExpression(id_, emote);
	map_->send_packet(&packet1, this);
}

void Player::handle_hp_mp_recovering()
{
	skip_bytes(4);

	short heal_hp = read<short>();
	if (heal_hp > 0)
	{
		set_hp(hp_ + heal_hp);
	}

	short heal_mp = read<short>();
	if (heal_mp > 0)
	{
		set_mp(mp_ + heal_mp);
	}
}

void Player::handle_quest_action()
{
	signed char action = read<signed char>();
	int quest_id = read<unsigned short>();

	switch (action)
	{
	case 1: // Start Quest
		give_quest(quest_id);
		break;
	case 2: // Complete Quest
	{
		int npc_id = read<int>();
		complete_quest(quest_id, npc_id);
		break;
	}
	case 3: // Forfeit Quest
		remove_quest(quest_id);
		break;
	case 4: // Scripted Start Quest
		break;
	case 5: // Scripted End Quest
		break;
	}
}

void Player::handle_key_map_changes()
{
	int mode = read<int>();

	if (mode == 0)
	{
		int changes = read<int>();

		if (changes > kMaxKeymapPos)
		{
			return;
		}

		for (int i = 0; i < changes; ++i)
		{
			int pos = read<int>();
			signed char type = read<signed char>();
			int action = read<int>();

			keys_[pos] = Key();

			Key &key = keys_[pos];
			key.type = type;
			key.action = action;
		}
	}
}

void Player::handle_player_hit()
{
	skip_bytes(6);

	int damage = read<int>();
	if (damage < 1)
	{
		return;
	}

	if (is_buff_stat_active(buffstat_constants::kMagicGuard))
	{
		int mp_damage = static_cast<int>((damage * get_buff_value(buffstat_constants::kMagicGuard)) / 100.0);
		int hp_damage = (damage - mp_damage);

		if (mp_damage >= mp_)
		{
			add_hp(-(hp_damage + (mp_damage - mp_)));
			set_mp(0);
		}
		else
		{
			add_hp(-hp_damage);
			add_mp(-mp_damage);
		}
	}
	else
	{
		add_hp(-damage);
	}
}

void Player::handle_change_map()
{
	skip_bytes(1);
	int type = read<int>();

	if (type == 0 && hp_ == 0) // death
	{
		set_hp(50);

		if (map_->get_id() == 270050100 || map_->get_id() == 211070100)
		{
			change_map(map_);
		}
		else
		{
			set_map(map_->get_return_map());
		}
	}

	else if (type == -1) // normal portal
	{
		std::string portal_name = read<std::string>();

		MapData *data = MapDataProvider::get_instance()->get_map_data_by_id(map_->get_id());
		MapPortalData *portal = data->get_portal(portal_name);
		if (!portal)
		{
			return;
		}

		Map *tomap = World::get_instance()->GetChannel(channel_id_)->get_map(portal->get_to_map_id());
		if (!tomap)
		{
			return;
		}

		if (portal->get_type() == 5 && !map_->is_portal_enabled(portal->get_from_protal()))
		{
			// packet
			PacketCreator packet3;
			packet3.ShowMessage("You cannot enter this portal yet.", 0);
			send_packet(&packet3);

			// packet
			PacketCreator packet4;
			packet4.EnableAction();
			send_packet(&packet4);

			return;
		}

		MapData *data2 = MapDataProvider::get_instance()->get_map_data_by_id(tomap->get_id());
		MapPortalData *tportal = data2->get_portal(portal->get_to_portal());

		signed char new_spawnpoint = 0;

		if (tportal)
		{
			position_x_ = tportal->get_x();
			position_y_ = tportal->get_y();
			new_spawnpoint = tportal->get_id();
		}

		change_map(tomap, new_spawnpoint);
	}
}

void Player::handle_change_channel()
{
	// check if the target channel is valid
	signed char new_channel_id = read<signed char>();

	if (!World::get_instance()->GetChannel(new_channel_id))
	{
		return;
	}

	channel_id_ = new_channel_id;
	set_map(map_->get_id());
}

void Player::handle_get_player_info()
{
	skip_bytes(4);
	int target_player_id = read<int>();
	Player *target_player = World::get_instance()->GetPlayerById(target_player_id);

	// send a packet
	PacketCreator packet;
	if (target_player)
	{
		packet.ShowInfo(target_player);
	}
	else
	{
		packet.EnableAction();
	}

	send_packet(&packet);
}
