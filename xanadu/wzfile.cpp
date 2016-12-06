//

#include "wzfile.hpp"

#include <string>
#include <codecvt>

#include "crypto_constants.hpp"
#include "packet_constants.hpp"
#include "game_constants.hpp"
#include "wznode.hpp"
#include "wzimg.hpp"
#include "wzcrypto.hpp"
#include "wzmain.hpp"
#include "world.hpp"

void WZFile::initialize(WZNode n)
{
	std::string x(kGameFilePath + n.name() + ".wz");

	wzfile_open(x.c_str());

	if (read<unsigned int>() != *(unsigned int *)"PKG1")
	{
		return;
	}

	unsigned long long file_size = read<unsigned long long>();
	file_start = read<unsigned int>();
	seek(file_start);

	// determine the wz files version if it hasn't been done yet

	WZMain *wz_reader = World::get_instance()->wz_reader_;
	WZMain *wzmain = wz_reader;

	if (wzmain->wz_version_ == 0)
	{
		wzmain->wz_crypto_.enc_version = read<short>();
		int count = read_value();
		unsigned long long c = 0;

		for (int k = 0; k < count; ++k)
		{
			unsigned char type = read<unsigned char>();

			if (type == 3)
			{
				read_enc_string();
				read_value();
				read_value();
				read<unsigned int>();

				continue;
			}

			else if (type == 4)
			{
				read_enc_string();
				read_value();
				read_value();

				c = wzfile_tell();

				break;
			}
			else
			{
				return;
			}
		}

		if (c == 0)
		{
			return;
		}

		// try to read the contents of the wz file using the encryption key

		bool success = false;

		wzmain->wz_crypto_.Key = wzmain->wz_crypto_.Keys[1];
		wzmain->wz_crypto_.AKey = wzmain->wz_crypto_.AKeys[1];
		wzmain->wz_crypto_.WKey = wzmain->wz_crypto_.WKeys[1];

		const int rounds = 512;

		for (wzmain->wz_version_ = 0; wzmain->wz_version_ < rounds; ++wzmain->wz_version_)
		{
			std::string s = std::to_string(wzmain->wz_version_);
			size_t l = s.length();

			wzmain->wz_crypto_.version_hash = 0;

			for (int i = 0; i < l; ++i)
			{
				wzmain->wz_crypto_.version_hash = (32 * wzmain->wz_crypto_.version_hash + s[i] + 1);
			}

			unsigned int result = 0xFF ^ (wzmain->wz_crypto_.version_hash >> 24) ^ (wzmain->wz_crypto_.version_hash << 8 >> 24) ^ (wzmain->wz_crypto_.version_hash << 16 >> 24) ^ (wzmain->wz_crypto_.version_hash << 24 >> 24);

			if (result == wzmain->wz_crypto_.enc_version)
			{
				seek(c);

				unsigned int off = read_offset(file_start);
				if (off > file_size)
				{
					continue;
				}

				seek(off);

				unsigned char a = read<unsigned char>();
				if (a != 0x73)
				{
					continue;
				}

				std::string ss = read_enc_string();

				if (ss != "Property")
				{
					continue;
				}

				success = true;

				break;
			}
		}

		if (!success)
		{
			return;
		}

		seek(file_start + 2);
	}
	else
	{
		short eversion = read<short>();
		if (eversion != wzmain->wz_crypto_.enc_version)
		{
			return;
		}
	}

	if (read_value() == 0)
	{
		return;
	}

	seek(file_start + 2);
	directory(n);

	free(this);
}

// functions

void WZFile::directory(WZNode n)
{
	int count = read_value();

	if (count == 0)
	{
		WZFile *x = (WZFile*)malloc(sizeof(WZFile));
		x->initialize(n);

		return;
	}

	std::vector<WZNode> dirs;
	n.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		char *name;
		unsigned char type = read<unsigned char>();

		if (type == 1)
		{
			skip(10);

			return;
		}

		else if (type == 2)
		{
			int s = read<int>();

			unsigned long long p = wzfile_tell();

			seek(file_start + s);
			type = read<unsigned char>();
			name = read_enc_string();
			seek(p);
		}

		else if (type == 3)
		{
			name = read_enc_string();
		}

		else if (type == 4)
		{
			name = read_enc_string();
		}
		else
		{
			return;
		}

		unsigned int fsize = read_value();
		unsigned int checksum = read_value();
		unsigned int off = read_offset(file_start);

		if (type == 3)
		{
			dirs.emplace_back(n.g(name, i));
		}

		else if (type == 4)
		{
			size_t len = strlen(name);
			name[len - 4] = '\0';
			
			// create a an img
			WZImg *img = (WZImg*)malloc(sizeof(WZImg));
			// initialize it
			img->file = *this;
			img->n = n.g(name, i);
			img->size = fsize;
			img->offset = off;
			img->n.set(img);
		}

		else
		{
			return;
		}

	}

	for (WZNode nn : dirs)
	{
		directory(nn);
	}
}

void WZFile::wzfile_open(const char *filename)
{
	HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, 0, nullptr);
	if (file == INVALID_HANDLE_VALUE)
	{
		return;
	}

	HANDLE map = CreateFileMappingA(file, nullptr, PAGE_READONLY, 0, 0, nullptr);
	if (!map)
	{
		return;
	}

	base = (char*)(MapViewOfFile(map, FILE_MAP_READ, 0, 0, 0));
	if (!base)
	{
		return;
	}

	off = base;
}

unsigned long long WZFile::wzfile_tell(void)
{
	return off - base;
}

void WZFile::seek(unsigned long long o)
{
	off = base + o;
}

void WZFile::skip(unsigned long long o)
{
	off += o;
}

void *WZFile::read_bin(int size)
{
	void *a = off;
	off += size;
	return a;
}

int WZFile::read_value(void)
{
	signed char a = read<signed char>();

	if (a != -128)
	{
		return a;
	}
	else
	{
		return read<int>();
	}
}

char *WZFile::read_enc_string(void)
{
	WZMain *wz_reader = World::get_instance()->wz_reader_;
	static std::codecvt_utf8<char16_t> conv;
	static char16_t ws[0x8000];
	static char ns[0x10000];

	int len = read<signed char>();

	if (len == 0)
	{
		return nullptr;
	}

	else if (len > 0) // unicode
	{
		if (len == 127)
		{
			len = read<int>();
		}

		if (len <= 0)
		{
			return nullptr;
		}

		char16_t *ows = (char16_t *)(read_bin(len * 2));

		// crypt

		__m128i *m1 = (__m128i *)(ws),
			*m2 = (__m128i *)(ows),
			*m3 = (__m128i *)(wz_reader->wz_crypto_.WKey);

		for (int i = 0; i <= len >> 3; ++i)
		{
			_mm_store_si128(m1 + i, _mm_xor_si128(_mm_loadu_si128(m2 + i), _mm_load_si128(m3 + i)));
		}

		mbstate_t state;
		const char16_t *fnext;
		char *tnext;
		conv.out(state, ws, ws + len, fnext, ns, ns + 0x10000, tnext);
		char *s = wz_reader->alloc_string((unsigned short)(tnext - ns + 1));
		memcpy(s, ns, tnext - ns + 1);
		s[tnext - ns] = '\0';

		return s;
	}
	else // non-unicode
	{
		if (len == -128)
		{
			len = read<int>();
		}
		else
		{
			len = -len;
		}

		if (len <= 0)
		{
			return nullptr;
		}

		char *os = (char *)(read_bin(len));

		// crypt

		__m128i *m1 = (__m128i *)(ns),
			*m2 = (__m128i *)(os),
			*m3 = (__m128i *)(wz_reader->wz_crypto_.AKey);

		for (int i = 0; i <= len >> 4; ++i)
		{
			_mm_store_si128(m1 + i, _mm_xor_si128(_mm_loadu_si128(m2 + i), _mm_load_si128(m3 + i)));
		}

		char *s = wz_reader->alloc_string(len + 1);
		memcpy(s, ns, len);
		s[len] = '\0';

		return s;
	}
}

char *WZFile::read_prop_string(unsigned int offset)
{
	unsigned char a = read<unsigned char>();

	switch (a)
	{
	case 0x00:
	case 0x73:
		return read_enc_string();
	case 0x01:
	case 0x1B:
	{
		unsigned int o = read<unsigned int>() + offset;
		unsigned long long p = wzfile_tell();

		seek(o);

		char *s = read_enc_string();
		seek(p);

		return s;
	}
	default:
		return nullptr;
	}
}

unsigned int WZFile::read_offset(unsigned int file_start)
{
	WZMain *wz_reader = World::get_instance()->wz_reader_;

	unsigned int p = (unsigned int)~(off - base - file_start);

	p *= wz_reader->wz_crypto_.version_hash;
	p -= kWzFileOffsetKey;
	p = (p << (p & 0x1F)) | (p >> (32 - p & 0x1F));

	unsigned int more = read<unsigned int>();
	p ^= more;
	p += file_start * 2;

	return p;
}
