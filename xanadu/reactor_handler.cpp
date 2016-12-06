//

#include "player.hpp"

void Player::handle_hit_reactor()
{
	int reactorid = read<int>();
	int pos = read<int>();
	short stance = read<short>();

	//Reactor* reactor = player->getMap()->getReactor(reactorid);

	//if (reactor == nullptr)
	//{
	//	return;
	//}

	//player->getMap()->getReactors()->hitReactor(reactor, player, stance, pos);
}
