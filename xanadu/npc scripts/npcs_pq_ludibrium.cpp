//

#include "../player.hpp"
#include "../party.hpp"

// This npc warps the party into the ludibrium party quests and gives broken glasses.

void Player::handle_lpq_red_sign_npc()
{
	if (party_ == nullptr || party_->get_leader() != id_)
	{
		std::string text = "How about you and your party members collectively beating a quest? Here you'll find obstacles and problems where you won't be able to beat it unless with great teamwork. If you want to try it, please tell the #bleader of your party#k to talk to me.";

		send_next(text);

		return;
	}

	int party_size = static_cast<int>(party_->get_players()->size());

	if (party_size != 1)
	{
		std::string text = "Your party is not a party of six. Please come back when you have six party members.";

		send_next(text);

		return;
	}

	if (level_ != 180)
	{
		std::string text = "Someone in your your party does not have a level between 21 ~ 40. Please double-check.";

		send_next(text);

		return;
	}

	// todo: check if lpq is already being done in this channel



	// warp party in

	set_state(299);
	set_map(922010100); // Abandoned Tower<Stage 1>
}
