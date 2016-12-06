//
// by Buya (the developer's pseudonym)
//

#pragma once

#include <cstdio>

class Logger
{
public:

	// default constructor
	Logger() = delete;

	// constructor
	Logger(char *file_name);

	// destructor
	~Logger();

	// functions
	void write(const char *message);

private:

	// copy constructor
	Logger(const Logger &other) = delete;

	// move constructor
	Logger(Logger &&other) = delete;

	// copy assignment operator
	Logger &operator=(const Logger &other) = delete;

	// move assignment operator
	Logger &operator=(Logger &&other) = delete;

	// variables
	FILE *file_;
};
