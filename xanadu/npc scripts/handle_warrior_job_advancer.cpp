//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_warrior_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{
		if (level_ >= 10)
		{
			set_job(job_ids::kSwordsman);
		}

		break;
	}

	case job_ids::kSwordsman:
	{
		if (level_ >= 30)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#lFighter#1 \r\n#L1#Page#l \r\n#L2#Spearman#l");
			}

			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kFighter);

					break;

				case 1:
					set_job(job_ids::kPage);

					break;

				case 2:
					set_job(job_ids::kSpearman);

					break;
				}
			}
		}

		break;
	}

	case job_ids::kFighter:
	case job_ids::kPage:
	case job_ids::kSpearman:
	{
		if (level_ >= 70)
		{
			switch (job_)
			{
			case job_ids::kFighter:
				set_job(job_ids::kCrusader);
				break;

			case job_ids::kPage:
				set_job(job_ids::kWhiteKnight);
				break;

			case job_ids::kSpearman:
				set_job(job_ids::kDragonKnight);
				break;
			}
		}

		break;
	}

	case job_ids::kCrusader:
	case job_ids::kWhiteKnight:
	case job_ids::kDragonKnight:
	{
		if (level_ >= 120)
		{
			switch (job_)
			{
			case job_ids::kCrusader:
				set_job(job_ids::kHero);
				break;

			case job_ids::kWhiteKnight:
				set_job(job_ids::kPaladin);
				break;

			case job_ids::kDragonKnight:
				set_job(job_ids::kDarkKnight);
				break;
			}
		}

		break;
	}
	}
}
