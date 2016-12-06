//

#pragma once

#include "constants.hpp"

// plain old data struct

struct WZCrypto
{
	unsigned char  *Key;
	unsigned char  *AKey;
	unsigned short *WKey;
	short          enc_version;
	unsigned int   version_hash;
	unsigned char  Keys[2][0x10000];
	unsigned char  AKeys[2][0x10000];
	unsigned short WKeys[2][0x8000];
};

void wz_crypto_initialize(struct WZCrypto *crypto);
