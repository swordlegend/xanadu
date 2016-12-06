//

#pragma once

#include "wznode.hpp"

class WZFile
{
public:

	// default constructor
	WZFile() = default;

	// copy constructor
	WZFile(const WZFile&) = delete;

	// move constructor
	WZFile(WZFile&&) = delete;

	// destructor
	~WZFile() = default;

	// copy assignment operator
	WZFile &operator=(const WZFile&) = default;

	// move assignment operator
	WZFile &operator=(WZFile&&) = delete;

	void initialize(WZNode n);
	void wzfile_open(const char *filename);
	unsigned long long wzfile_tell();
	void seek(unsigned long long);
	void skip(unsigned long long);
	void *read_bin(int size);
	int read_value(void);
	char *read_enc_string(void);
	char *read_prop_string(unsigned int offset);
	unsigned int read_offset(unsigned int file_start);
	void directory(WZNode n);

	// this function template should be used to read bytes based on integer types

	template<typename TYPE>
	TYPE read()
	{
		return *(TYPE*)(read_bin(sizeof(TYPE)));
	}

	/*// template function specialisation for reading bytes based on the type char*

	template<>
	char* read<char*>()
	{

	}*/

private:

	unsigned int file_start;
	char *base;
	char *off;
};
