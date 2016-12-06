//

#include "player.hpp"

#include "summon.hpp"
#include "map.hpp"
#include "packetcreator.hpp"
#include "timer.hpp"
#include "world.hpp"

void handle_stop_summon(int player_id, int skill_id, /*const boost::system::error_code& error*/int error)
{
	if (error)
	{
		return;
	}

	Player *player = World::get_instance()->GetPlayerById(player_id);
	if (!player)
	{
		return;
	}

	player->remove_summon();
}

Summon *Player::get_summon()
{
	if (summon_)
	{
		return summon_;
	}
	return nullptr;
}

Summon *Player::get_summon_by_id(int id)
{
	if (summon_)
	{
		if (summon_->object_id_ == id)
		{
			return summon_;
		}
	}
	return nullptr;
}

void Player::add_summon(int skill_id, int skill_level, int hp, int time)
{
	if (summon_)
	{
		remove_summon();
	}

	summon_ = new Summon(skill_id, skill_level, hp, position_x_, position_y_);

	// send a packet
	PacketCreator packet1;
	packet1.SpawnSummon(this, summon_, true);
	map_->send_packet(&packet1);
	//

	if (time > 0)
	{
		// timer
		//boost::system::error_code error;
		//summon_timer_.expires_from_now(boost::posix_time::milliseconds(time), error);
		//summon_timer_.async_wait(boost::bind(&handle_stop_summon, id_, skill_id, boost::asio::placeholders::error));
	}
}

void Player::remove_summon()
{
	if (summon_)
	{
		// send a packet
		PacketCreator packet1;
		packet1.remove_summon(id_, summon_->object_id_);
		map_->send_packet(&packet1);
		//
		free(summon_);
		summon_ = nullptr;
	}
}
