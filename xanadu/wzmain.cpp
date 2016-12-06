//

#include "wzmain.hpp"

#include "game_constants.hpp"
#include "wzfile.hpp"
#include "wznode.hpp"
#include "wzimg.hpp"

// default constructor

WZMain::WZMain() :
	wz_version_(0)
{
}

// destructor

WZMain::~WZMain()
{
	for (auto it : wz_string_buffers_)
	{
		free(it);
	}
}

void WZMain::initialize()
{
	// prepare AES encryption

	wz_crypto_initialize(&wz_crypto_);

	// load Base.wz

	base_.init_top("Base");
	WZFile *x = new WZFile();
	x->initialize(base_);

	// check to make sure wz reading works and the right version is used

	if (wz_version_ != kGameVersion)
	{
		throw;
	}
}

char *WZMain::alloc_string(unsigned short len)
{
	static char *adata = nullptr;
	static std::size_t aremain = 0;

	if (aremain < len)
	{
		size_t _Size = 0x100000;
		void *memory = malloc(_Size);

		wz_string_buffers_.push_back(memory);

		adata = (char *)memory;
		aremain = _Size;
	}

	char *r = adata;
	aremain -= len;
	adata += len;

	return r;
}
