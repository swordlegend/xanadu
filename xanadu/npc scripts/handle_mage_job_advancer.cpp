//

#include "../player.hpp"
#include "../job_constants.hpp"

void Player::handle_mage_job_advancer()
{
	switch (job_)
	{

	case job_ids::kBeginner:
	{
		if (level_ >= 8)
		{
			set_job(job_ids::kMagician);
		}

		break;
	}

	case job_ids::kMagician:
	{
		if (level_ >= 30)
		{
			if (get_state() == 0)
			{
				send_simple("Choose the path you wish to follow. \r\n#L0#lFpWizard#1 \r\n#L1#IlWizard#l \r\n#L2#Cleric#l");
			}

			else if (get_state() == 1)
			{
				switch (get_selected())
				{
				case 0:
					set_job(job_ids::kFpWizard);

					break;

				case 1:
					set_job(job_ids::kIlWizard);

					break;

				case 2:
					set_job(job_ids::kCleric);

					break;
				}
			}
		}

		break;
	}

	case job_ids::kFpWizard:
	case job_ids::kIlWizard:
	case job_ids::kCleric:
	{
		if (level_ >= 70)
		{
			switch (job_)
			{
			case job_ids::kFpWizard:
				set_job(job_ids::kFpMage);
				break;

			case job_ids::kIlWizard:
				set_job(job_ids::kIlMage);
				break;

			case job_ids::kCleric:
				set_job(job_ids::kPriest);
				break;
			}
		}

		break;
	}

	case job_ids::kFpMage:
	case job_ids::kIlMage:
	case job_ids::kPriest:
	{
		if (level_ >= 120)
		{
			switch (job_)
			{
			case job_ids::kFpMage:
				set_job(job_ids::kFpArchMage);
				break;

			case job_ids::kIlMage:
				set_job(job_ids::kIlArchMage);
				break;

			case job_ids::kPriest:
				set_job(job_ids::kBishop);
				break;
			}
		}

		break;
	}
	}
}
