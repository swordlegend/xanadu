//

#include "timer.hpp"

#include <chrono>

#include "world.hpp"

// constructor

Timer::Timer(int seconds) :
	timer_(new asio::basic_waitable_timer<std::chrono::high_resolution_clock>(World::get_instance()->get_io_service()))
{
	std::error_code ec;
	timer_->expires_from_now(std::chrono::seconds(seconds), ec);
}

// destructor

Timer::~Timer()
{
	std::error_code ec;
	timer_->cancel(ec);
}

// functions

std::shared_ptr<asio::basic_waitable_timer<std::chrono::high_resolution_clock>> Timer::get_object()
{
	return timer_;
}
