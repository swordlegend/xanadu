//

#include "packetcreator.hpp"

#include "constants.hpp"

// default constructor

PacketCreator::PacketCreator() :
length_(0),
buffer_size_(kInitialPacketBufferSize),
buffer_(new unsigned char[kInitialPacketBufferSize]())
{
}

// destructor

PacketCreator::~PacketCreator()
{
	delete[] buffer_;
}

void PacketCreator::write_bytes(char *hex)
{
	size_t loop_counter = 0;
	size_t row_count = (strlen(hex) / 2);

	for (; loop_counter < row_count; ++loop_counter)
	{
		unsigned char byte1 = hex[loop_counter * 2];
		unsigned char byte2 = hex[loop_counter * 2 + 1];

		if (byte1 >= 'A' && byte1 <= 'F')
		{
			byte1 -= 'A' - 0xa;
		}
		else if (byte1 >= 'a' && byte1 <= 'f')
		{
			byte1 -= 'a' - 0xa;
		}
		else if (byte1 >= '0' && byte1 <= '9')
		{
			byte1 -= '0';
		}

		if (byte2 >= 'A' && byte2 <= 'F')
		{
			byte2 -= 'A' - 0xa;
		}
		else if (byte2 >= 'a' && byte2 <= 'f')
		{
			byte2 -= 'a' - 0xa;
		}
		else if (byte2 >= '0' && byte2 <= '9')
		{
			byte2 -= '0';
		}

		unsigned char byte = (byte1 * 0x10 + byte2);
		buffer_[length_] = byte;

		++length_;
	}
}

void PacketCreator::write_null(size_t amount)
{
	// adjust buffer

	if (length_ + amount >= buffer_size_)
	{
		buffer_ = (unsigned char*)realloc(buffer_, buffer_size_ + amount);
		buffer_size_ += amount;
	}

	for (; amount > 0; --amount)
	{
		buffer_[length_] = 0;
		++length_;
	}
}

void PacketCreator::write_string(std::string str, std::size_t slen)
{
	// adjust buffer

	if (length_ + slen >= buffer_size_)
	{
		buffer_ = (unsigned char*)realloc(buffer_, buffer_size_ + slen);
		buffer_size_ += slen;
	}

	memcpy(buffer_ + length_, str.data(), str.length());

	size_t len = str.length();
	length_ += len;

	for (; len < slen; ++len)
	{
		buffer_[length_] = 0;
		++length_;
	}
}

unsigned char *PacketCreator::get_buffer()
{
	return buffer_;
}

size_t PacketCreator::get_length()
{
	return length_;
}
