//

#include "../player.hpp"
#include "../constants.hpp"

void Player::handle_maple_leaf_exchanger_npc()
{
	int item_ids[] =
	{
		//NPC Mia 9010002

		1002508, //Maple Hat
		1002509, //Maple Hat
		1002510, //Maple Hat
		1002511, //Maple Hat
		1102166, //Male Cape
		1102167, //Maple Cape
		1102168, //Maple Cape
		1032040, //Maple Earring
		1032041, //Maple Earring
		1032042, //Maple Earring
		1012101, //Maple Leaf
		1082252, //Maple Gage
		1092030, //Maple Shield
		1302020, //MapleSword
		1302030, //Soul Singer
		1302064, //Maple Glory Sword
		1302058, //Umbrella
		1302080, //Maplemas Lights
		1312032, //Steel Axe
		1322054, //Havoc Hammer
		1332025, //Wagner
		1332055, //Dark Mate
		1332056, //Asura Dagger
		1382009, //Maple Staff
		1382012, //Lama Staff
		1372034, //Shine Wand
		1382039, //Wisdom Staff
		1402039, //Soul Rohen
		1412011, //Dragon Axe
		1412027, //Demon Axe
		1422014, //Doom Singer
		1422029, //Belzet
		1432012, //Impaler
		1432040, //Soul Spear
		1432046, //Maplemas Tree
		1442024, //Scorpio
		1442051, //Karstan
		1442030, //Snowboard
		1452016, //Bow
		1452022, //Soul Searcher
		1452045, //Kandiva Bow
		1462014, //Crow
		1462019, //Crossbow
		1462040, //Nishada
		1472030, //Claw
		1472032, //Kandayo
		1472055, //Skanda
		1482020, //Knuckle
		1482021, //Storm Finger
		1482022, //Golden Claw
		1492020, //Gun
		1492021, //Storm Pistol
		1492022, //Canon Shooter
		4006001, //Summoning Rock
		4006000 //Magic Rock
	};

	int points[] =
	{
		50,
		150,
		200,
		250,
		50,
		100,
		150,
		50,
		100,
		150,
		300,
		300,
		500,
		300,
		150,
		50,
		100,
		150,
		200,
		100,
		100,
		100,
		150,
		150,
		200,
		200,
		100,
		150,
		200,
		200,
		200,
		150,
		200,
		150,
		200,
		150,
		200,
		100,
		150,
		200,
		100,
		100,
		150,
		200,
		100,
		150,
		200,
		100,
		150,
		200,
		100,
		150,
		200,
		100,
		150,
		200,
		1,
		1
	};

	if (get_state() == 0)
	{
		std::string text = "";

		text.append("Hi! I am the Maple Leaf Exchanger! You currently have #b" + std::to_string(get_inventory(kInventoryConstantsTypesEtc)->get_item_amount(4001126)) + " Maple Leafs#k. How can I serve you?");

		for (int i = 0; i < 56; ++i)
		{
			int item_id = item_ids[i];
			int maple_leafs = points[i];

			text.append("\r\n #L");
			text.append(std::to_string(i));
			text.append("#");

			text.append(" #v0");
			text.append(std::to_string(item_id));
			text.append("#");

			text.append(" - ");
			text.append(std::to_string(maple_leafs));

			text.append("#l ");
		}

		send_simple(text);
	}

	else if (get_state() == 1)
	{
		int i = get_selected();
		int item_id = item_ids[i];
		int maple_leafs = points[i];
		int maple_leafs_ = get_inventory(kInventoryConstantsTypesEtc)->get_item_amount(4001126);

		if (maple_leafs_ >= maple_leafs)
		{
			give_item(item_id, 1);
			remove_item(4001126, static_cast<short>(maple_leafs));
		}
	}
}
