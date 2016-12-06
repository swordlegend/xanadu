//

#include "player.hpp"

#include "map.hpp"
#include "hiredmerchant.hpp"
#include "hiredmerchant_item.hpp"
#include "world.hpp"
#include "item.hpp"
#include "item_data.hpp"
#include "item_data_provider.hpp"
#include "inventory.hpp"
#include "packetcreator.hpp"
#include "constants.hpp"

void Player::handle_hired_merchant_request()
{
	if (merchant_)
	{
		return;
	}

	if (!map_->can_open_store(this))
	{
		return;
	}

	// packet
	PacketCreator packet1;
	packet1.HiredMerchantBox();
	send_packet(&packet1);
}

void Player::handle_item_transportation()
{
	signed char action = read<signed char>();

	switch (action)
	{
	case 0: // Create
		{
			signed char creation_type = read<signed char>();
			switch (creation_type)
			{
			case 3: // Trade
				{
					if (trade_partner_)
					{
						return;
					}
					// send a packet
					PacketCreator packet2;
					packet2.ShowTrade(this, nullptr, 0);
					send_packet(&packet2);
					break;
				}
			case 5: // Hired Merchant
				{
					if (merchant_)
					{
						return;
					}
					if (!map_->can_open_store(this))
					{
						return;
					}
					if (merchant_storage_items_.size() > 0)
					{
						// packet
						PacketCreator packet19;
						packet19.ShowMessage("Please retrieve your stored items and mesos from NPC Fredrick in the Free Market Entrance first.", 1);
						send_packet(&packet19);

						return;
					}

					std::string description = read<std::string>();
					skip_bytes(1);
					short item_slot = read<short>();
					int item_id = read<int>();

					Inventory *inventory = get_inventory(kInventoryConstantsTypesCash);
					if (!inventory)
					{
						return;
					}

					auto store = inventory->get_item_by_slot(static_cast<signed char>(item_slot));
					
					if (!store || store->get_item_id() != item_id)
					{
						return;
					}

					merchant_.reset(new HiredMerchant(this, item_id, description));

					// packet
					PacketCreator packet13;
					packet13.GetHiredMerchant(this, merchant_, true);
					send_packet(&packet13);

					break;
				}
			}
			break;
		}
	case 2: // Send trade request
		{
			int target_player_id = read<int>();
			Player *invited = World::get_instance()->GetPlayerById(target_player_id);

			if (!invited || invited->get_trade_partner())
			{
				// send a packet
				PacketCreator packet5;
				packet5.TradeError(0, 1);
				send_packet(&packet5);
				return;
			}

			invited->set_trade_partner(this);
			trade_partner_ = invited;

			// send a packet
			PacketCreator packet21;
			packet21.InviteTrade(this);
			trade_partner_->send_packet(&packet21);

			break;
		}
	case 3: // Deny request
		{
			trade_partner_ = nullptr;

			break;
		}
	case 4: // Enter Miniroom
		{
			if (trade_partner_)
			{
				// send a packet
				PacketCreator packet22;
				packet22.JoinTrade(this);
				trade_partner_->send_packet(&packet22);
				// send a packet
				PacketCreator packet23;
				packet23.ShowTrade(this, trade_partner_, 1);
				send_packet(&packet23);
			}

			// a merchant can either be entered as a visitor
			// or as the owner (maintenance)

			else if (!merchant_)
			{
				int merchant_id = read<int>();
				auto merchant = map_->get_hired_merchant(merchant_id);

				if (merchant)
				{
					if (merchant->is_owner(id_))
					{
						merchant_ = merchant;
						merchant->set_open(false);
						merchant->remove_all_visitors();

						// packet
						PacketCreator packet15;
						packet15.GetHiredMerchant(this, merchant, false);
						send_packet(&packet15);
					}
					else if (!merchant->is_open())
					{
						// packet
						PacketCreator packet35;
						packet35.PlayerShopMessage(18);
						send_packet(&packet35);
					}
					else if (merchant->get_empty_visitor_slot() == -1)
					{
						// packet
						PacketCreator packet36;
						packet36.PlayerShopMessage(2);
						send_packet(&packet36);
					}
					else
					{
						merchant_ = merchant;
						merchant->add_visitor(this);

						// packet
						PacketCreator packet17;
						packet17.GetHiredMerchant(this, merchant, false);
						send_packet(&packet17);
					}
				}
			}

			break;
		}
	case 6: // Miniroom Chat
		{
			std::string message = (name_ + " : " + read<std::string>());

			if (trade_partner_)
			{
				// send a packet
				PacketCreator packet11;
				packet11.ShowTradeChat(1, message);
				send_packet(&packet11);
				// send a packet
				PacketCreator packet12;
				packet12.ShowTradeChat(0, message);
				trade_partner_->send_packet(&packet12);
			}

			else if (merchant_)
			{
				merchant_->room_chat(merchant_->get_playerslot(id_), message);
			}

			break;
		}
	case 0x0A: // Exit Miniroom
		{
			if (trade_partner_)
			{
				if (trade_partner_->get_trade_partner() == this)
				{
					add_mesos(trade_mesos);

					for (auto item : trade_items_)
					{
						Inventory *inventory = get_inventory(item->get_inventory_id());

						if (!inventory)
						{
							continue;
						}

						inventory->add_item_find_slot(item);
					}

					// manage trade stuff for the other trader

					trade_partner_->add_mesos(trade_partner_->get_trade_mesos());

					auto trade_partner__items = trade_partner_->get_trade_items();

					for (auto item : *trade_partner__items)
					{
						Inventory *inventory = trade_partner_->get_inventory(item->get_inventory_id());

						if (!inventory)
						{
							continue;
						}

						inventory->add_item_find_slot(item);
					}

					// send a packet
					PacketCreator packet9;
					packet9.TradeError(0, 3);
					trade_partner_->send_packet(&packet9);

					trade_partner_->reset_trade();
				}

				reset_trade();

				// packet
				PacketCreator packet16;
				packet16.EnableAction();
				send_packet(&packet16);
			}
			else if (merchant_)
			{
				if (merchant_->is_owner(id_))
				{
					merchant_->set_open(true);
				}
				else
				{
					merchant_->remove_visitor(this);
				}

				merchant_.reset();
			}
			break;
		}
	case 0x0B: // Open a Miniroom / Open up a hired merchant
		{
			if (merchant_ && map_->can_open_store(this))
			{
				merchant_->set_open(true);
				map_->add_hired_merchant(merchant_);

				// packet
				PacketCreator packet40;
				packet40.SpawnHiredMerchant(merchant_);
				map_->send_packet(&packet40);
			}
			break;
		}
	case 0x0E: // Add item to trade
		{
			// check if the trade partner is valid
			if (!trade_partner_ || trade_partner_->get_trade_partner() != this || trade_partner_->get_map() != map_)
			{
				return;
			}

			// read the data from the packet
			signed char inventory_id = read<signed char>();
			short item_slot = read<short>();
			short item_amount = read<short>();
			signed char trade_slot = read<signed char>();

			// check if the target inventory is valid
			Inventory *inventory = get_inventory(inventory_id);
			if (!inventory)
			{
				return;
			}

			// check if the target item is valid
			std::shared_ptr<Item> item = inventory->get_item_by_slot(static_cast<signed char>(item_slot));
			if (!item)
			{
				return;
			}

			// check if the target amount is valid
			if (item_amount < 1 || item_amount > item->get_amount())
			{
				return;
			}

			// create a copy of the original item
			std::shared_ptr<Item> copy(new Item(*item));

			// adjust copied item values
			copy->set_slot(trade_slot);
			copy->set_amount(item_amount);

			// add copied item to trade items
			trade_items_.push_back(copy);

			// display item in thread
			// send a packet
			PacketCreator packet17;
			packet17.TradeItem(copy, 0);
			send_packet(&packet17);
			// send a packet
			PacketCreator packet18;
			packet18.TradeItem(copy, 1);
			trade_partner_->send_packet(&packet18);

			// remove the original item amount from the inventory
			inventory->remove_item_by_slot(static_cast<signed char>(item_slot), item_amount, true, true);

			break;
		}
	case 0x0F: // add mesos to trade
		{
			int mesos = read<int>();
			if (!trade_partner_ || mesos < 1 || mesos > mesos_)
			{
				return;
			}
			add_mesos(-mesos);
			trade_mesos += mesos;
			// send a packet
			PacketCreator packet14;
			packet14.TradeMesos(trade_mesos, 0);
			send_packet(&packet14);
			// send a packet
			PacketCreator packet15;
			packet15.TradeMesos(trade_mesos, 1);
			trade_partner_->send_packet(&packet15);

			break;
		}
	case 0x10: // confirm the trade
		{
			// check if the trade partner is valid
			if (!trade_partner_ || trade_partner_->get_trade_partner() != this || trade_partner_->get_map() != map_)
			{
				return;
			}
			// send the confirmation packet to the other trader
			PacketCreator packet20;
			packet20.TradeConfirm();
			trade_partner_->send_packet(&packet20);

			// check if the trade partner has confirmed the trade already
			if (!trade_partner_->is_trade_confirmed())
			{
				trade_locked_ = true;
				return;
			}
			// give the items and mesos from this player to the trade partner
			for (size_t i = 0; i < trade_items_.size(); ++i)
			{
				auto item = trade_items_[i];
				trade_partner_->get_inventory(item->get_inventory_id())->add_item_find_slot(item);
			}
			trade_partner_->add_mesos(trade_mesos);

			// give the items and mesos from the trader partner to this player
			auto pitems = trade_partner_->get_trade_items();
			for (auto item : *pitems)
			{
				Inventory *inventory = get_inventory(item->get_inventory_id());
				if (inventory)
				{
					inventory->add_item_find_slot(item);
				}
			}
			add_mesos(this->trade_partner_->get_trade_mesos());

			// send packets to show that the trade has completed
			PacketCreator packet12;
			packet12.TradeError(0, 6);
			send_packet(&packet12);
			PacketCreator packet13;
			packet13.TradeError(1, 6);
			trade_partner_->send_packet(&packet13);

			// reset the trade data
			trade_partner_->reset_trade();
			reset_trade();

			break;
		}
	case 21: // Hired Merchant Maintenance
		{
			if (merchant_)
			{
				return;
			}

			skip_bytes(2);
			std::string pic = read<std::string>();
			int map_object_id = read<int>();

			auto merchant = map_->get_hired_merchant(map_object_id);

			if (merchant && merchant->is_owner(id_))
			{
				// packet
				PacketCreator packet50;
				packet50.MerchantStoreMaintenanceResponse(map_object_id);
				send_packet(&packet50);
			}

			break;
		}
	case 31: // Add item into Merchant
		{
			std::shared_ptr<HiredMerchant> merchant = get_hired_merchant();

			if (!merchant || !merchant->is_owner(id_))
			{
				return;
			}

			signed char inventory_id = read<signed char>();
			short item_slot = read<short>();
			short bundles = read<short>();
			short amount_per_bundle = read<short>();
			int price = read<int>();
			Inventory *inventory = get_inventory(inventory_id);
			if (!inventory)
			{
				return;
			}
			std::shared_ptr<Item> item = inventory->get_item_by_slot(static_cast<signed char>(item_slot));
			if (!item)
			{
				return;
			}
			if ((item->get_amount() < (bundles * amount_per_bundle)) && !item->is_star())
			{
				return;
			}
			if (price < 1)
			{
				return;
			}
			// create a copy of the original item
			std::shared_ptr<Item> copy(new Item(*item));
			if (!item->is_star())
			{
				copy->set_amount(amount_per_bundle);
			}
			inventory->remove_item_by_slot(static_cast<signed char>(item_slot), (item->is_star() ? item->get_amount() : (bundles * amount_per_bundle)), true, true);

			std::shared_ptr<HiredMerchantItem> hiredmerchant_item(new HiredMerchantItem(copy, bundles, amount_per_bundle, item_slot, price));
			merchant->add_item(hiredmerchant_item);

			// packet
			PacketCreator packet55;
			packet55.UpdateHiredMerchant(merchant.get());
			send_packet(&packet55);

			break;
		}
	case 32: // Buy item in Merchant
		{
			signed char slot = read<signed char>();
			short amount = read<short>();

			if (merchant_ && !merchant_->is_owner(id_) && amount > 0 && merchant_->get_item(slot))
			{
				merchant_->buy_item(this, slot, amount);
			}

			break;
		}
	case 36: // Remove item from Merchant
		{
			skip_bytes(1);
			short slot = read<short>();

			if (merchant_ && merchant_->is_owner(id_) && merchant_->get_item(slot))
			{
				merchant_->return_item(slot, this);
				merchant_->remove_item(slot);

				// packet
				PacketCreator packet21;
				packet21.UpdateHiredMerchant(merchant_.get());
				send_packet(&packet21);
			}

			break;
		}
	case 37: // End of maintenance (owner goes out of store)
	{
		// packet
		PacketCreator packet55;
		packet55.EnableAction();
		send_packet(&packet55);

		break;
	}
	case 38: // Tidy up (retrieves earned mesos and removes sold items from list)
	{
		// TO-DO code this action

		// packet
		PacketCreator packet56;
		packet56.EnableAction();
		send_packet(&packet56);

		break;
	}
	case 39: // Close Merchant
		{
			if (merchant_ && merchant_->is_owner(id_))
			{
				if (map_->get_hired_merchant(merchant_->get_id()))
				{
					map_->remove_hired_merchant(merchant_->get_id());

					// packet
					PacketCreator packet32;
					packet32.DestroyHiredMerchant(id_);
					map_->send_packet(&packet32);
				}

				// give mesos to fredrick
				merchant_storage_mesos_ += merchant_->get_merchant_mesos();

				// give items to fredrick
				int counter = 0;
				auto items = merchant_->get_items();

				for (auto it : *items)
				{
					auto merchant_item = it.second;

					if (merchant_item->get_bundles() > 0)
					{
						merchant_storage_items_[counter] = merchant_item->get_item();
						merchant_storage_items_[counter]->set_amount(merchant_item->get_quantity());
						++counter;
					}
				}

				// packet
				PacketCreator packet39;
				packet39.EnableAction();
				send_packet(&packet39);

				// packet
				PacketCreator packet40;
				packet40.CloseMerchantStoreResponse();
				send_packet(&packet40);
			}

			merchant_.reset();

			break;
		}
	}
}
