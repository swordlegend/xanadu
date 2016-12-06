//

#include "equip_data_provider.hpp"

#include "equip_data.hpp"
#include "item_data_provider.hpp"
#include "item_data.hpp"
#include "world.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"

// singleton

EquipDataProvider *EquipDataProvider::singleton_ = nullptr;

EquipDataProvider *EquipDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new EquipDataProvider();
	}

	return singleton_;
}

void EquipDataProvider::load_data()
{
	// Character.wz -> Accessory / Cap / Cape / ... / Weapon

	WZMain *wz_reader = World::get_instance()->wz_reader_;

	WZNode node1 = wz_reader->base_["Character"]["Weapon"];

	for (auto it1 : node1)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Weapon"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}
			
			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node2 = wz_reader->base_["Character"]["Shield"];

	for (auto it1 : node2)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Shield"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node3 = wz_reader->base_["Character"]["Shoes"];

	for (auto it1 : node3)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Shoes"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node4 = wz_reader->base_["Character"]["Cap"];

	for (auto it1 : node4)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Cap"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node5 = wz_reader->base_["Character"]["Longcoat"];

	for (auto it1 : node5)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Longcoat"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node6 = wz_reader->base_["Character"]["Pants"];

	for (auto it1 : node6)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Pants"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node7 = wz_reader->base_["Character"]["Glove"];

	for (auto it1 : node7)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Glove"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node8 = wz_reader->base_["Character"]["Cape"];

	for (auto it1 : node8)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Cape"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node9 = wz_reader->base_["Character"]["Accessory"];

	for (auto it1 : node9)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Accessory"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node10 = wz_reader->base_["Character"]["Coat"];

	for (auto it1 : node10)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["Coat"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				else if (islot == "Ma") // Coat
				{
					slots = 7;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node11 = wz_reader->base_["Character"]["TamingMob"];

	for (auto it1 : node11)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["TamingMob"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				else if (islot == "Ma") // Coat
				{
					slots = 7;
				}

				else if (islot == "Tm") // TamingMob
				{
					slots = 0;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}

	WZNode node12 = wz_reader->base_["Character"]["PetEquip"];

	for (auto it1 : node12)
	{
		std::string valuename1 = it1.name();
		int equip_id = std::stoi(valuename1);
		int price = 0;
		short max_per_slot = 1;
		bool trade_able = true;
		bool is_cash = false;

		EquipData *equip = new EquipData();
		equip->id = equip_id;
		equip->hand = 0;
		equip->slots = 0;
		equip->hp = 0;
		equip->mp = 0;
		equip->str = 0;
		equip->dex = 0;
		equip->intt = 0;
		equip->luk = 0;
		equip->watk = 0;
		equip->wdef = 0;
		equip->matk = 0;
		equip->mdef = 0;
		equip->acc = 0;
		equip->avo = 0;
		equip->jump = 0;
		equip->speed = 0;

		WZNode node2 = wz_reader->base_["Character"]["PetEquip"][valuename1.c_str()]["info"];

		for (auto it2 : node2)
		{
			std::string valuename2 = it2.name();

			if (valuename2 == "slotMax")
			{
				max_per_slot = it2.get_int_value();
			}

			else if (valuename2 == "price")
			{
				price = it2.get_int_value();
			}

			else if (valuename2 == "tradeBlock")
			{
				trade_able = it2.get_int_value() != 0;
			}

			else if (valuename2 == "cash")
			{
				is_cash = it2.get_int_value() != 0;
			}

			// equip stats

			else if (valuename2 == "islot")
			{
				std::string islot = it2.get_string_value();
				int slots = 0;

				if (islot == "Wp") // weapon
				{
					slots = 7;
				}

				else if (islot == "Si") // shield
				{
					slots = 7;
				}

				else if (islot == "So") // shoes
				{
					slots = 5;
				}

				else if (islot == "Gv") // gloves
				{
					slots = 5;
				}

				else if (islot == "Cp") // cap
				{
					slots = 7;
				}

				else if (islot == "MaPn") // overall
				{
					slots = 10;
				}

				else if (islot == "Pn") // pants
				{
					slots = 7;
				}

				else if (islot == "Sr") // Cape
				{
					slots = 5;
				}

				else if (islot == "Af") // Accessory
				{
					slots = 5;
				}

				else if (islot == "Ma") // Coat
				{
					slots = 7;
				}

				else if (islot == "Tm") // TamingMob
				{
					slots = 0;
				}

				equip->slots = slots;
			}

			else if (valuename2 == "incMHP")
			{
				int incMHP = it2.get_int_value();
				equip->hp = incMHP;
			}

			else if (valuename2 == "incMMP")
			{
				int incMMP = it2.get_int_value();
				equip->mp = incMMP;
			}

			else if (valuename2 == "incSTR")
			{
				int incSTR = it2.get_int_value();
				equip->str = incSTR;
			}

			else if (valuename2 == "incDEX")
			{
				int incDEX = it2.get_int_value();
				equip->dex = incDEX;
			}

			else if (valuename2 == "incINT")
			{
				int incINT = it2.get_int_value();
				equip->intt = incINT;
			}

			else if (valuename2 == "incLUK")
			{
				int incLUK = it2.get_int_value();
				equip->luk = incLUK;
			}

			else if (valuename2 == "incPAD")
			{
				int incPAD = it2.get_int_value();
				equip->watk = incPAD;
			}

			else if (valuename2 == "incMAD")
			{
				int incMAD = it2.get_int_value();
				equip->matk = incMAD;
			}

			else if (valuename2 == "incPDD")
			{
				int incPDD = it2.get_int_value();
				equip->wdef = incPDD;
			}

			else if (valuename2 == "incMDD")
			{
				int incMDD = it2.get_int_value();
				equip->mdef = incMDD;
			}

			else if (valuename2 == "incACC")
			{
				int incACC = it2.get_int_value();
				equip->acc = incACC;
			}

			else if (valuename2 == "incEVA")
			{
				int incEVA = it2.get_int_value();
				equip->avo = incEVA;
			}

			else if (valuename2 == "incJump")
			{
				int incJump = it2.get_int_value();
				equip->jump = incJump;
			}

			else if (valuename2 == "incSpeed")
			{
				int incSpeed = it2.get_int_value();
				equip->speed = incSpeed;
			}
		}

		// TODO: FIX THIS CODE
		// WORKAROUND add equipdata to itemdata to pass checks
		// need to evaluate if this way is alright

		ItemData *item = (ItemData*)malloc(sizeof(ItemData));
		memset(item, 0, sizeof(ItemData));

		item->id = equip_id;
		item->price = price;
		item->max_per_slot = max_per_slot;
		item->trade_able = trade_able;
		item->is_cash = is_cash;

		ItemDataProvider::get_instance()->add_item(item);

		items_[equip_id] = equip;
	}
}

EquipData *EquipDataProvider::get_item_data(int item_id)
{
	if (items_.find(item_id) == items_.end())
	{
		return nullptr;
	}

	return items_[item_id];
}

std::unordered_map<int, EquipData *> *EquipDataProvider::get_data()
{
	return &items_;
}
