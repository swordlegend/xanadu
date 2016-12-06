//

#pragma once

#include "asio.hpp"

#include <memory>

class Timer
{
public:

	// constructor
	Timer(int seconds);

	// default constructor
	Timer() = delete;

	// copy constructor
	Timer(const Timer &other) = delete;

	// move constructor
	Timer(Timer &&other) = delete;

	// destructor
	~Timer();

	// copy assignment operator
	Timer &operator=(const Timer& other) = delete;

	// move assignment operator
	Timer &operator=(Timer &&other) = delete;

	// functions
	std::shared_ptr<asio::basic_waitable_timer<std::chrono::high_resolution_clock>> get_object();

private:

	// variables
	std::shared_ptr<asio::basic_waitable_timer<std::chrono::high_resolution_clock>> timer_;
};
