//

#include "player.hpp"

#include "map.hpp"
#include "packetcreator.hpp"
#include "inventory.hpp"
#include "world.hpp"
#include "cash_shop_item_data_provider.hpp"
#include "cash_shop_package_data_provider.hpp"
#include "cash_item_data.hpp"
#include "cashshop_constants.hpp"

void Player::handle_cash_shop_enter()
{
	in_cash_shop_ = true;
	map_->remove_player(this);

	// send a packet
	PacketCreator packet1;
	packet1.EnterCashShop(this);
	send_packet(&packet1);

	// TO-DO needs to be fixed
	//send_packet(PacketCreator().get_cashshop_inventory(storage_slots_, character_slots_));

	// send a packet
	PacketCreator packet3;
	packet3.ShowCashPoints(nx_cash_credit_);
	send_packet(&packet3);
}

void Player::handle_leave_cash_shop()
{
	in_cash_shop_ = false;

	// send a packet
	PacketCreator packet;
	packet.change_map(this, true);
	send_packet(&packet);

	map_->add_player(this);
}

void Player::handle_update_cash_shop()
{
	PacketCreator packet;
	packet.ShowCashPoints(nx_cash_credit_);
	send_packet(&packet);
}

void Player::handle_cash_shop_action()
{
	signed char action = read<signed char>();

	switch (action)
	{
	case CashShopReceivePacketActions::kBuyCashItem:
		{
			skip_bytes(1);
			skip_bytes(4); // type of cash used
			int serial_number = read<int>();

			CashItemData *cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(serial_number);
			if (!cash_item)
			{
				return;
			}

			int price = cash_item->price;

			if (nx_cash_credit_ < price)
			{
				return;
			}

			int item_id = cash_item->item_id;
			int amount = cash_item->count;

			if (!give_item(item_id, amount))
			{
				// send a packet
				PacketCreator packet1;
				packet1.ShowCashPoints(nx_cash_credit_);
				send_packet(&packet1);
				// send a packet
				PacketCreator packet2;
				packet2.EnableAction();
				send_packet(&packet2);
				//
				return;
			}

			nx_cash_credit_ -= price;

			// send a packet
			PacketCreator packet10;
			packet10.ShowBoughtCashItem(user_id_, serial_number, item_id, amount);
			send_packet(&packet10);

			// send a packet
			PacketCreator packet11;
			packet11.ShowCashPoints(nx_cash_credit_);
			send_packet(&packet11);

			break;
		}
	/*case CashShopReceivePacketActions::kBuyInventorySlots:
		{
			skip_bytes(2);
			signed char inventory_id = read<signed char>();
			Inventory * inventory = get_inventory(inventory_id);

			if (!inventory)
			{
				return;
			}

			if (inventory->getSlots() > 92)
			{
				return;
			}

			if (nx_cash_credit_ < 4000)
			{
				return;
			}

			nx_cash_credit_ -= 4000;
			inventory->addSlots(4);
			send_packet(PacketCreator().increase_inventory_slots(inventory_id, inventory->getSlots()));

			break;
		}
	case CashShopReceivePacketActions::kBuyStorageSlots:
		{
			if (storage_slots_ > 92)
			{
				return;
			}

			if (nx_cash_credit_ < 4000)
			{
				return;
			}

			nx_cash_credit_ -= 4000;
			storage_slots_ += 4;
			send_packet(PacketCreator().increase_storage_slots(storage_slots_));

			break;
		}
	case CashShopReceivePacketActions::kBuyCharacterSlot:
		{
			if (character_slots_ < 21 && nx_cash_credit_ > 6700)
			{
				nx_cash_credit_ -= 6700;
				character_slots_ += 1;
				send_packet(PacketCreator().showMessage("1 slot has been added to your account character slots.", 1));
			}

			break;
		}
	case CashShopReceivePacketActions::kRetrieveCashItem:
		{
			//long long unique_id = read_int64();

			//send_packet(PacketCreator().take_out_from_cash_shop_inventory(nullptr, 0));

			break;
		}
	case CashShopReceivePacketActions::kStoreCashItem:
		{
			//long long unique_id = read_int64();

			//send_packet(PacketCreator().transfer_to_cash_shop_inventory(user_id_, 0, 0, 0));

			break;
		}*/
	case CashShopReceivePacketActions::kBuyPackage:
		{
			skip_bytes(1);
			skip_bytes(4); // type of cash used
			int package_serial_number = read<int>();
			CashItemData *package_cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(package_serial_number);
			if (!package_cash_item)
			{
				return;
			}
			
			int package_id = package_cash_item->item_id;
			std::vector<int> serial_numbers = CashShopPackageDataProvider::get_instance()->get_serial_numbers_in_cash_package(package_id);
			
			for (int serial_number : serial_numbers)
			{
				CashItemData *cash_item = CashShopItemDataProvider::get_instance()->get_cash_item_data(serial_number);
				if (!cash_item)
				{
					return;
				}
				int price = cash_item->price;
				if (nx_cash_credit_ < price)
				{
					return;
				}
				int item_id = cash_item->item_id;
				int amount = cash_item->count;
				if (!give_item(item_id, amount))
				{
					// send a packet
					PacketCreator packet15;
					packet15.ShowCashPoints(nx_cash_credit_);
					send_packet(&packet15);
					
					// send a packet
					PacketCreator packet16;
					packet16.EnableAction();
					send_packet(&packet16);

					return;
				}
				nx_cash_credit_ -= price;
				
				// send a packet
				PacketCreator packet17;
				packet17.ShowBoughtCashItem(user_id_, serial_number, item_id, amount);
				send_packet(&packet17);
			}

			break;
		}
	}
	
	// update nx cash for client
	// send a packet
	PacketCreator packet20;
	packet20.ShowCashPoints(nx_cash_credit_);
	send_packet(&packet20);
}
