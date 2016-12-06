//
// by Buya (the developer's pseudonym)
//

#include "logger.hpp"
#include <iostream>
#include <fstream>

Logger::Logger(char *file_name)
{
	//file_ = fopen(file_name, "a");
	std::ofstream file;
	file.open(file_name);
	file << "Writing this to a file.\n";
	file.close();
}

Logger::~Logger()
{
	if (file_)
	{
		fclose(file_);
	}
}

void Logger::write(const char *message)
{
	if (file_)
	{
		fprintf(file_, "%s", message);
		fprintf(file_, "\n");
	}
}
