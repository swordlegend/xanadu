//

#include "player.hpp"

#include "packetcreator.hpp"
#include "constants.hpp"

void Player::handle_add_stat()
{
	if (ap_ < 1)
	{
		return;
	}

	skip_bytes(4);

	int stat = read<int>();

	switch (stat)
	{
	    case kCharacterStatsStr:
			++str_;
			break;
		case kCharacterStatsDex:
			++dex_;
			break;
		case kCharacterStatsInt:
			++int_;
			break;
		case kCharacterStatsLuk:
			++luk_;
			break;
	}

	--ap_;

	// packet
	PacketCreator packet1;
	packet1.UpdateApStats(str_, dex_, int_, luk_, ap_);
	send_packet(&packet1);
}
