//
// by Buya (the developer's pseudonym)
//

#include "session.hpp"

#include <functional>

#include "crypto.hpp"
#include "packetcreator.hpp"
#include "player.hpp"
#include "world.hpp"

//#include "logger.hpp"

// default constructor

Session::Session() :
	socket_(World::get_instance()->get_io_service()),
	player_(new Player(this)),
	receive_buffer_(new unsigned char[kInitialPacketBufferSize]()),
	iv_recv_(new unsigned char[kIvBufferSize]()),
	iv_send_(new unsigned char[kIvBufferSize]())
{
}

// destructor

Session::~Session()
{
	delete player_;
	delete[] iv_recv_;
	delete[] iv_send_;
}

asio::ip::tcp::socket &Session::get_socket()
{
	return socket_;
}

unsigned char *Session::get_receive_buffer()
{
	return receive_buffer_.get();
}

// initializes the session

void Session::initialize()
{
	// set the no delay option for the socket
	asio::ip::tcp::no_delay option(true);
	socket_.set_option(option);

	PacketCreator packet1;
	packet1.get_handshake(iv_recv_, iv_send_);

	size_t packet_length = packet1.get_length();
	unsigned char *packet_buffer = packet1.get_buffer();

	// create a new buffer
	size_t buffer_size = packet_length;
	unsigned char *send_buffer = new unsigned char[buffer_size]();

	// copy the packet bytes into the send_buffer
	memcpy(send_buffer, packet_buffer, packet_length);

	// start an async (non-blocking) write operation to send the packet
	asio::async_write(socket_,
		asio::buffer(send_buffer, packet_length),
		std::bind(&Session::send_handler,
			shared_from_this(), std::placeholders::_1, std::placeholders::_2, send_buffer));

	// start an async read operation to receive the header of the next packet
	start_read_header();
}

void Session::disconnect()
{
	socket_.close();
}

void Session::send_packet(PacketCreator *packet)
{
	if (!socket_.is_open())
	{
		disconnect();
		return;
	}

	size_t packet_length = packet->get_length();
	unsigned char *packet_buffer = packet->get_buffer();

	// create a new buffer
	size_t buffer_size = packet_length + 4;
	unsigned char *send_buffer = new unsigned char[buffer_size]();

	// copy the packet bytes into the send_buffer
	memcpy(send_buffer + 4, packet_buffer, packet_length);

	// create the packet header
	crypto::create_packet_header(send_buffer, iv_send_, static_cast<unsigned short>(packet_length));

	// encrypt the packet
	crypto::encrypt(send_buffer + 4, iv_send_, static_cast<unsigned short>(packet_length));

	// start an async (non-blocking) write operation to send the packet
	asio::async_write(socket_,
		asio::buffer(send_buffer, packet_length + 4),
		std::bind(&Session::send_handler,
			shared_from_this(), std::placeholders::_1, std::placeholders::_2, send_buffer));
}

void Session::start_read_header()
{
	receive_buffer_.reset(new unsigned char[4]());

	// start an async read operation to receive the header of the next packet
	asio::async_read(socket_,
		asio::buffer(receive_buffer_.get(), 4),
		std::bind(&Session::read_header_handler,
			shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void Session::read_header_handler(const std::error_code &ec, std::size_t bytes_transferred)
{
	if (!ec)
	{
		// get the packet length from the header buffer
		unsigned short packet_length = crypto::get_packet_length(receive_buffer_.get());

		// a packet must consist of 2 bytes atleast
		if (packet_length < 2)
		{
			disconnect();
			return;
		}

		// receive the data
		receive_buffer_.reset(new unsigned char[packet_length]());

		// start an async read operation to receive the rest of the packet
		asio::async_read(socket_,
			asio::buffer(receive_buffer_.get(), packet_length),
			std::bind(&Session::read_body_handler,
				shared_from_this(), std::placeholders::_1, std::placeholders::_2));
	}
	else
	{
		disconnect();
	}
}

void Session::read_body_handler(const std::error_code &ec, std::size_t bytes_transferred)
{
	if (!ec)
	{
		// get the packet length
		unsigned short bytes_amount = static_cast<unsigned short>(bytes_transferred);

		// decrypt the packet
		crypto::decrypt(receive_buffer_.get(), iv_recv_, bytes_amount);

		// handle the packet
		player_->handle_packet(bytes_amount);

		// start an async read operation to receive the header of the next packet
		start_read_header();
	}
	else
	{
		disconnect();
	}
}

void Session::send_handler(const std::error_code &ec, std::size_t bytes_transferred, unsigned char *send_buffer)
{
	// free the memory allocated for the send buffer
	delete[] send_buffer;
}
