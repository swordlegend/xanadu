//

#include "../player.hpp"
#include "../map.hpp"
#include "../party.hpp"
#include "../packetcreator.hpp"

// this file contains the npcs for the kerning city party quest

// this npc warps players to the kpq exit

void Player::handle_kpq_nella_npc()
{
	set_state(299);

	if (map_->get_id() == 103000890) // exit
	{
		set_map(103000000);
	}
	else
	{
		set_map(103000890);
	}
}

// this npc warps to party into the kerning pq

void Player::handle_kpq_lakelis_npc()
{
	if (!party_ || party_->get_leader() != id_)
	{
		std::string text = "How about you and your party members collectively beating a quest? Here you'll find obstacles and problems where you won't be able to beat it unless with great teamwork. If you want to try it, please tell the #bleader of your party#k to talk to me.";

		send_next(text);

		return;
	}

	int party_size = static_cast<int>(party_->get_players()->size());

	if (party_size != 4 && !get_is_gm())
	{
		std::string text = "Your party is not a party of four. Please come back when you have four party members.";

		send_next(text);

		return;
	}

	if (level_ < 21 || level_ > 40)
	{
		std::string text = "Someone in your your party does not have a level between 21 ~ 40. Please double-check.";

		send_next(text);

		return;
	}

	// todo: check if kpq is already being done in this channel



	// warp party in

	set_state(299);
	set_map(103000800);
}

constexpr int kAns[] = { 8, 10, 10, 10, 20, 25, 25, 35, 15 };

const std::string kText[] =
{
	"the number of levels needed to make the first job advancement as the wizard.",
	"the number of levels needed to make the first job advancement as the bowman.",
	"the number of levels needed to make the first job advancement as the thief.",
	"the number of levels needed to make the first job advancement as the warrior.",
	"the minimum amount of INT level needed to make the first job advancement as the wizard.",
	"the minimum amount of DEX level needed to make the first job advancement as the bowman.",
	"the minimum amount of DEX level needed to make the first job advancement as the thief.",
	"the minimum amount of STR level needed to make the first job advancement as the warrior.",
	"the experience points needed to advance from #rlevel 1 to level 2.#k"
};

int GetPlayersOnFoothold(Map *map, short fh)
{
	int count = 0;

	auto players = *map->get_players();

	for (int i = 0; i < players.size(); i++)
	{
		auto player = players[i];

		if (player->get_foothold() == fh)
		{
			count++;
		}
	}

	return count;
}

int GetPlayersOnFoothold(Map *map, short min, short max)
{
	int count = 0;

	auto players = *map->get_players();

	for (int i = 0; i < players.size(); i++)
	{
		auto player = players[i];

		if (player->get_foothold() >= min && player->get_foothold() <= max)
		{
			count++;
		}
	}

	return count;
}

void Wrong(Map *map)
{
	//map->send_packet(PacketCreator().map_effect(3, "quest/party/wrong_kor"));
	//map->send_packet(PacketCreator().map_effect(4, "Party1 / Failed"));
}

void FinishStage(Map *map, bool portal)
{
	//map->send_packet(PacketCreator().map_effect(2, "gate"));
	//map->send_packet(PacketCreator().map_effect(3, "quest/party/clear"));
	//map->send_packet(PacketCreator().map_effect(4, "Party1/Clear"));

	if (portal)
	{
		map->enable_portal("next00");
	}
}

const int PARTY_QUEST_EXP_RATE_1 = 2;
const int PARTY_QUEST_EXP_RATE_2 = 3;

void GiveExp(Map *map, int amount)
{
	auto players = *map->get_players();

	for (int i = 0; i < players.size(); i++)
	{
		auto player = players[i];

		int amountx = 0;

		if (player->get_level() <= 30)
		{
			amountx = (amount * PARTY_QUEST_EXP_RATE_1);
		}
		else if (player->get_level() <= 40)
		{
			amountx = (amount * PARTY_QUEST_EXP_RATE_2);
		}

		player->set_exp(player->get_exp() + amountx);
	}
}

// this npc handles stages: 1, 2, 3, 4, last stage

void Player::handle_kpq_cloto_npc()
{
	const int QUESTIONS = 9;
	const int ROPES = 4;
	const int PLATFORMS = 5;
	const int BARRELS = 6;
	const int REWARDS = 42;

	const int stage1_exp = 100;
	const int stage2_exp = 200;
	const int stage3_exp = 200;
	const int stage4_exp = 200;
	const int laststage_exp = 200;

	const short ropes_fh = 510;
	const short platforms_fh_min[] = { 14, 2, 8, 34, 25 };
	const short platforms_fh_max[] = { 20, 6, 12, 38, 32 };
	const short barrels_fh[] = { 117, 115, 113, 116, 114, 112 };

	set_state(299);

	switch (map_->get_id())
	{

	case 103000800: // 1st stage
	{
		if (!party_)
		{
			return;
		}

		if (give_item(4001007, 10))
		{
			FinishStage(map_, true);
			GiveExp(map_, stage1_exp);
		}

		break;
	}

	case 103000801: // 2nd stage
	{
		if (!party_)
		{
			return;
		}

		if (party_->get_leader() != id_)
		{
			return;
		}

		if (party_->get_variable("kpq_2s_first") == -1)
		{
			party_->set_variable("kpq_2s_first", 1);

			int ropes[] = { 0, 0, 0, 0 };

			for (int i = 0; i < 3;)
			{
				int r = tools::random_int(1, ROPES);

				if (ropes[r] == 0)
				{
					ropes[r] = 1;
					i++;
				}
			}

			for (int i = 0; i < ROPES; i++)
			{
				party_->set_variable("kpq_2s_rope" + std::to_string(i), ropes[i]);
			}
		}
		else
		{
			int ropes[] = { 0, 0, 0, 0 };

			for (int i = 0; i < ROPES; i++)
			{
				ropes[i] = GetPlayersOnFoothold(map_, ropes_fh);
			}

			int onropes = 0;

			for (int i = 0; i < ROPES; i++)
			{
				onropes += ropes[i];
			}

			if (onropes != 3)
			{
				send_next("It looks like you haven't found the 3 ropes just yet. Please think of a different combination of ropes. Only 3 are allowed to hang on to ropes, and if you hang on to too low, it won't count as an answer, so please keep that in mind. Keep going!");
			}
			else
			{
				bool ans = true;

				for (int i = 0; i < ROPES; i++)
				{
					if (ropes[i] != party_->get_variable("kpq_2s_rope" + std::to_string(i)))
					{
						ans = false;
						break;
					}
				}

				if (ans)
				{
					FinishStage(map_, true);
					GiveExp(map_, stage2_exp);
					party_->set_variable("kpq_2s_done", 1);
				}
				else
				{
					Wrong(map_);
				}
			}
		}

		break;
	}

	case 103000802: // 3rd stage
	{
		FinishStage(map_, true);
		GiveExp(map_, stage3_exp);

		break;
	}

	case 103000803: // 4th stage
	{
		FinishStage(map_, true);
		GiveExp(map_, stage4_exp);

		break;
	}

	case 103000804: // last stage
	{
		if (!party_)
		{
			return;
		}

		if (party_->get_variable("kpq_5_done") == 1)
		{
			set_map(103000805);
			give_item(2041016, 1);
		}

		if (party_->get_leader() == id_)
		{
			if (give_item(4001008, -10))
			{
				FinishStage(map_, true);
				GiveExp(map_, laststage_exp);
				party_->set_variable("kpq_5_done", 1);
			}
		}

		break;
	}

	}
}
