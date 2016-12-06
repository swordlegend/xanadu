//

#include "wzcrypto.hpp"

#include "crypto_constants.hpp"

#include <cstring>

#include <emmintrin.h>

#include "aes/aes.h"

void wz_crypto_initialize(struct WZCrypto *crypto)
{
	crypto->Key = NULL;
	crypto->AKey = NULL;
	crypto->WKey = NULL;
	memset(crypto->Keys[0], 0, 0x10000);

	unsigned char big_iv[16];
	for (int i = 0; i < 16; i += 4)
	{
		memcpy(big_iv + i, kWzFileKeyIv, 4);
	}

	aes_encrypt_ctx cx[1];
	aes_init();
	aes_encrypt_key256(kAesKeys, cx);
	aes_encrypt(big_iv, crypto->Keys[1], cx);

	for (int i = 16; i < 0x10000; i += 16)
	{
		aes_encrypt(crypto->Keys[1] + i - 16, crypto->Keys[1] + i, cx);
	}

	// hide warnings about shortened values
#pragma warning(push)
#pragma warning(disable : 4309)

	__m128i amask = _mm_setr_epi8(0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9);
	__m128i wmask = _mm_setr_epi16(0xAAAA, 0xAAAB, 0xAAAC, 0xAAAD, 0xAAAE, 0xAAAF, 0xAAA0, 0xAAA1);

	// restore warnings
#pragma warning(pop)

	__m128i *ak0 = (__m128i *)crypto->AKeys[0];
	__m128i *ak1 = (__m128i *)crypto->AKeys[1];
	__m128i *wk0 = (__m128i *)crypto->WKeys[0];
	__m128i *wk1 = (__m128i *)crypto->WKeys[1];
	__m128i *k0 = (__m128i *)crypto->Keys[0];
	__m128i *k1 = (__m128i *)crypto->Keys[1];

	for (int i = 0x1000; i > 0; --i)
	{
		_mm_store_si128(ak0, _mm_xor_si128(amask, _mm_load_si128(k0)));
		_mm_store_si128(ak1, _mm_xor_si128(amask, _mm_load_si128(k1)));
		_mm_store_si128(wk0, _mm_xor_si128(wmask, _mm_load_si128(k0)));
		_mm_store_si128(wk1, _mm_xor_si128(wmask, _mm_load_si128(k1)));

		++ak0;
		++ak1;
		++wk0;
		++wk1;
		++k0;
		++k1;
	}

	crypto->Key = crypto->Keys[0];
	crypto->AKey = crypto->AKeys[0];
	crypto->WKey = crypto->WKeys[0];
}
