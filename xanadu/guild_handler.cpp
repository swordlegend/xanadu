//

#include "player.hpp"

#include "packetcreator.hpp"
#include "guild.hpp"
#include "guild_member.hpp"
#include "map.hpp"
#include "world.hpp"

void Player::handle_guild_action()
{
	signed char action = read<signed char>();

	switch (action)
	{
		case 2: // Create a guild
			{
			if (guild_)
			{
				return;
			}
			std::string &name = read<std::string>();
			if (name.length() < 4 || name.length() > 12 || World::get_instance()->guild_name_used(name))
			{
				return;
			}
			guild_rank_ = 1;
			World::get_instance()->create_guild(this, name);

			break;
		}
		case 5: // Invite a player
			{
				if (!guild_ || guild_rank_ > 2)
				{
					return;
				}
			std::string &target = read<std::string>();
			Player *invited = World::get_instance()->GetPlayerByName(target);
			if (!invited || invited->get_guild())
			{
				return;
			}
			// send a packet
			PacketCreator packet21;
			packet21.InviteGuild(this);
			invited->send_packet(&packet21);

			break;
		}
		case 6: // Join a guild
			{
			if (guild_)
			{
				return;
			}

			int guildid = read<int>();
			World::get_instance()->join_guild(this, guildid);

			break;
		}
		case 7: // Leave a guild
			{
			if (!guild_ || guild_rank_ == 1)
			{
				return;
			}
			// send a packet
			PacketCreator packet22;
			packet22.GuildPlayerLeave(guild_->get_id(), id_, name_);
			guild_->send_packet(&packet22);
			//
			guild_->DeleteMember(id_);
			guild_ = nullptr;
			guild_rank_ = 5;

			break;
		}
		case 8: // Expell a player
			{
			int target_player_id = read<int>();
			std::string target_player_name = read<std::string>();

			if (!guild_ || guild_rank_ > 2)
			{
				return;
			}

			World::get_instance()->expel_guild_member(guild_, target_player_id, target_player_name);
			
			break;
		}
		case 13: // Change the title's
			{
			if (!guild_ || guild_rank_ != 1)
			{
				return;
			}

			std::string rank1 = read<std::string>();
			std::string rank2 = read<std::string>();
			std::string rank3 = read<std::string>();
			std::string rank4 = read<std::string>();
			std::string rank5 = read<std::string>();

			guild_->SetRank1(rank1);
			guild_->SetRank2(rank2);
			guild_->SetRank3(rank3);
			guild_->SetRank4(rank4);
			guild_->SetRank5(rank5);

			// send a packet
			PacketCreator packet24;
			packet24.UpdateGuildRanks(guild_);
			guild_->send_packet(&packet24);

			break;
		}
		case 14: // Change someones rank
			{
			int target_player_id = read<int>();
			signed char new_rank = read<signed char>();

			if (!guild_ || guild_rank_ > 2 || (new_rank == 2 && guild_rank_ != 1) || new_rank < 2 || new_rank > 5)
			{
				return;
			}

			GuildMember *member = guild_->GetMember(target_player_id);
			if (!member)
			{
				return;
			}
			member->set_guild_rank(new_rank);

			Player *target_player = World::get_instance()->GetPlayerById(target_player_id);
			if (target_player)
			{
				target_player->set_guild_rank(new_rank);
			}
			// send a packet
			PacketCreator packet25;
			packet25.ChangeRank(guild_->get_id(), target_player_id, new_rank);
			guild_->send_packet(&packet25);
			
			break;
		}
		case 15: // Change emblem
			{
			if (!guild_ || guild_rank_ != 1)
			{
				return;
			}

			guild_->SetLogoBackground(read<short>());
			guild_->SetLogoBackgroundColor(read<signed char>());
			guild_->SetLogo(read<short>());
			guild_->SetLogoColor(read<signed char>());
			// send a packet
			PacketCreator packet26;
			packet26.ShowGuildEmblem(guild_);
			guild_->send_packet(&packet26);
			
			break;
		}
		case 16: // Change the notice
			{
			if (!guild_ || guild_rank_ > 2)
			{
				return;
			}

			std::string &notice = read<std::string>();
			guild_->SetNotice(notice);

			// send a packet
			PacketCreator packet27;
			packet27.UpdateGuildNotice(guild_);
			guild_->send_packet(&packet27);
			//
			break;
		}
	}
}
