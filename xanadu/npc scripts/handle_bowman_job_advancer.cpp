//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_bowman_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{
		if (level_ >= 10)
		{
			set_job(job_ids::kArcher);
		}

		break;
	}

	case job_ids::kArcher:
	{
		if (level_ >= 30)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#lHunter#1 \r\n#L1#Crossbowman#l");
			}

			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kHunter);

					break;

				case 1:
					set_job(job_ids::kCrossbowman);

					break;
				}
			}
		}

		break;
	}

	case job_ids::kHunter:
	case job_ids::kCrossbowman:
	{
		if (level_ >= 70)
		{
			switch (job_)
			{
			case job_ids::kHunter:
				set_job(job_ids::kRanger);
				break;

			case job_ids::kCrossbowman:
				set_job(job_ids::kSniper);
				break;
			}
		}

		break;
	}

	case job_ids::kRanger:
	case job_ids::kSniper:
	{
		if (level_ >= 120)
		{
			switch (job_)
			{
			case job_ids::kRanger:
				set_job(job_ids::kBowmaster);
				break;

			case job_ids::kSniper:
				set_job(job_ids::kMarksman);
				break;
			}
		}

		break;
	}
	}
}
