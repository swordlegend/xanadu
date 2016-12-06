//

#pragma once

#include <vector>

#include "wznode.hpp"
#include "wzcrypto.hpp"

class WZMain
{
public:

	// default constructor
	WZMain();

	// copy constructor
	WZMain(const WZMain &other) = delete;

	// move constructor
	WZMain(WZMain &&other) = delete;

	// destructor
	~WZMain();

	// copy assignment operator
	WZMain &operator=(const WZMain &other) = delete;

	// move assignment operator
	WZMain &operator=(WZMain &&other) = delete;

	void initialize();
	char *alloc_string(unsigned short len);

	WZCrypto wz_crypto_;
	WZNode base_;
	unsigned short wz_version_;
	std::vector<void *> wz_string_buffers_;
};
