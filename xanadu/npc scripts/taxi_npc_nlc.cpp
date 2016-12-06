//

#include "../player.hpp"

#include "../map.hpp"

void Player::handle_nlc_taxi_npc()
{
	set_state(299);

	if (map_->get_id() == 600000000)
	{
		set_map(682000000);
	}

	else if (map_->get_id() == 682000000)
	{
		set_map(600000000);
	}
}
