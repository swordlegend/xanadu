//

#include "item_data_provider.hpp"

#include "item_data.hpp"
#include "world.hpp"
#include "wznode.hpp"
#include "wzmain.hpp"

// singleton

ItemDataProvider *ItemDataProvider::singleton_ = nullptr;

ItemDataProvider *ItemDataProvider::get_instance()
{
	if (!singleton_)
	{
		singleton_ = new ItemDataProvider();
	}

	return singleton_;
}

void ItemDataProvider::load_data()
{
	// Item.wz -> Cash / Consume / Etc / Install

	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZNode node1 = wz_reader->base_["Item"]["Cash"];

	for (auto it1 : node1)
	{
		std::string valuename1 = it1.name();

		WZNode node11 = wz_reader->base_["Item"]["Cash"][valuename1.c_str()];

		for (auto it11 : node11)
		{
			std::string valuename11 = it11.name();

			ItemData *item = (ItemData*)malloc(sizeof(ItemData));
			memset(item, 0, sizeof(ItemData));

			item->id = std::stoi(valuename11);
			item->max_per_slot = 100;
			item->trade_able = true;
			item->is_cash = true;

			items_[item->id] = item;
		}
	}

	WZNode node2 = wz_reader->base_["Item"]["Consume"];

	for (auto it2 : node2)
	{
		std::string valuename2 = it2.name();

		WZNode node21 = wz_reader->base_["Item"]["Consume"][valuename2.c_str()];

		for (auto it21 : node21)
		{
			std::string valuename21 = it21.name();

			ItemData *item = (ItemData*)malloc(sizeof(ItemData));
			memset(item, 0, sizeof(ItemData));

			item->id = std::stoi(valuename21);
			item->max_per_slot = 100;
			item->trade_able = true;

			WZNode node22 = wz_reader->base_["Item"]["Consume"][valuename2.c_str()][valuename21.c_str()]["info"];

			for (auto it22 : node22)
			{
				std::string valuename22 = it22.name();

				if (valuename22 == "slotMax")
				{
					item->max_per_slot = it22.get_int_value();
				}

				else if (valuename22 == "price")
				{
					item->price = it22.get_int_value();
				}

				else if (valuename22 == "tradeBlock")
				{
					item->trade_able = it22.get_int_value() != 0;
				}
			}

			// consume specs

			WZNode node23 = wz_reader->base_["Item"]["Consume"][valuename2.c_str()][valuename21.c_str()]["spec"];

			for (auto it23 : node23)
			{
				std::string valuename23 = it23.name();

				if (valuename23 == "hpR")
				{
					item->hpr = it23.get_int_value();
				}

				else if (valuename23 == "mpR")
				{
					item->mpr = it23.get_int_value();
				}

				else if (valuename23 == "time")
				{
					item->time = it23.get_int_value();
				}

				else if (valuename23 == "hp")
				{
					item->hhp = it23.get_int_value();
				}

				else if (valuename23 == "mp")
				{
					item->hmp = it23.get_int_value();
				}

				else if (valuename23 == "acc")
				{
					item->acc = it23.get_int_value();
				}

				else if (valuename23 == "eva")
				{
					item->avo = it23.get_int_value();
				}

				else if (valuename23 == "speed")
				{
					item->speed = it23.get_int_value();
				}

				else if (valuename23 == "jump")
				{
					item->jump = it23.get_int_value();
				}

				else if (valuename23 == "pdd")
				{
					item->wdef = it23.get_int_value();
				}

				else if (valuename23 == "mdd")
				{
					item->mdef = it23.get_int_value();
				}

				else if (valuename23 == "pad")
				{
					item->watk = it23.get_int_value();
				}

				else if (valuename23 == "mad")
				{
					item->matk = it23.get_int_value();
				}

				else if (valuename23 == "moveTo")
				{
					item->move_to = it23.get_int_value();
				}

				// scroll data

				WZNode node24 = wz_reader->base_["Item"]["Consume"][valuename2.c_str()][valuename21.c_str()]["info"];

				for (auto it24 : node24)
				{
					std::string valuename24 = it24.name();

					if (valuename24 == "success")
					{
						item->success = it24.get_int_value();
					}

					else if (valuename24 == "cursed")
					{
						item->cursed = it24.get_int_value();
					}

					else if (valuename24 == "recover")
					{
						item->recover = it24.get_int_value() != 0;
					}

					else if (valuename24 == "incJump")
					{
						item->jump = it24.get_int_value();
					}

					else if (valuename24 == "incSpeed")
					{
						item->speed = it24.get_int_value();
					}

					else if (valuename24 == "incSTR")
					{
						item->str = it24.get_int_value();
					}

					else if (valuename24 == "incDEX")
					{
						item->dex = it24.get_int_value();
					}

					else if (valuename24 == "incINT")
					{
						item->intt = it24.get_int_value();
					}

					else if (valuename24 == "incLUK")
					{
						item->luk = it24.get_int_value();
					}

					else if (valuename24 == "incMHP")
					{
						item->hp = it24.get_int_value();
					}

					else if (valuename24 == "incMMP")
					{
						item->mp = it24.get_int_value();
					}

					else if (valuename24 == "incEVA")
					{
						item->avo = it24.get_int_value();
					}

					else if (valuename24 == "incACC")
					{
						item->acc = it24.get_int_value();
					}

					else if (valuename24 == "incPAD")
					{
						item->watk = it24.get_int_value();
					}

					else if (valuename24 == "incMAD")
					{
						item->matk = it24.get_int_value();
					}

					else if (valuename24 == "incPDD")
					{
						item->wdef = it24.get_int_value();
					}

					else if (valuename24 == "incMDD")
					{
						item->mdef = it24.get_int_value();
					}
				}
			}

			items_[item->id] = item;
		}
	}

	WZNode node3 = wz_reader->base_["Item"]["Etc"];

	for (auto it3 : node3)
	{
		std::string valuename3 = it3.name();

		WZNode node31 = wz_reader->base_["Item"]["Etc"][valuename3.c_str()];

		for (auto it31 : node31)
		{
			std::string valuename31 = it31.name();

			ItemData *item = (ItemData*)malloc(sizeof(ItemData));
			memset(item, 0, sizeof(ItemData));

			item->id = std::stoi(valuename31);
			item->max_per_slot = 100;
			item->trade_able = true;

			WZNode node32 = wz_reader->base_["Item"]["Etc"][valuename3.c_str()][valuename31.c_str()]["info"];

			for (auto it32 : node32)
			{
				std::string valuename32 = it32.name();

				if (valuename32 == "slotMax")
				{
					item->max_per_slot = it32.get_int_value();
				}

				else if (valuename32 == "price")
				{
					item->price = it32.get_int_value();
				}

				else if (valuename32 == "tradeBlock")
				{
					item->trade_able = it32.get_int_value() != 0;
				}
			}

			items_[item->id] = item;
		}
	}

	WZNode node4 = wz_reader->base_["Item"]["Install"];

	for (auto it4 : node4)
	{
		std::string valuename4 = it4.name();

		WZNode node41 = wz_reader->base_["Item"]["Install"][valuename4.c_str()];

		for (auto it41 : node41)
		{
			std::string valuename41 = it41.name();

			ItemData *item = (ItemData*)malloc(sizeof(ItemData));
			memset(item, 0, sizeof(ItemData));

			item->id = std::stoi(valuename41);
			item->max_per_slot = 100;
			item->trade_able = true;

			WZNode node42 = wz_reader->base_["Item"]["Install"][valuename4.c_str()][valuename41.c_str()]["info"];

			for (auto it42 : node42)
			{
				std::string valuename42 = it42.name();

				if (valuename42 == "slotMax")
				{
					item->max_per_slot = it42.get_int_value();
				}

				else if (valuename42 == "price")
				{
					item->price = it42.get_int_value();
				}

				else if (valuename42 == "tradeBlock")
				{
					item->trade_able = it42.get_int_value() != 0;
				}
			}

			items_[item->id] = item;
		}
	}
}

ItemData *ItemDataProvider::get_item_data(int item_id)
{
	if (items_.find(item_id) == items_.end())
	{
		return nullptr;
	}

	return items_[item_id];
}

std::unordered_map<int, ItemData *> *ItemDataProvider::get_data()
{
	return &items_;
}

void ItemDataProvider::add_item(ItemData *data)
{
	items_[data->id] = data;
}
