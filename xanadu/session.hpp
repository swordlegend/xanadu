//
// by Buya (the developer's pseudonym)
//

#pragma once

#include "asio.hpp"

class PacketCreator;
class Player;

class Session : public std::enable_shared_from_this<Session>
{
public:

	// default constructor
	Session();

	// copy constructor
	Session(const Session &other) = delete;

	// move constructor
	Session(Session &&other) = delete;
	
	// destructor
	~Session();

	// copy assignment operator
	Session &operator=(const Session &other) = delete;

	// move assignment operator
	Session &operator=(Session &&other) = delete;

	asio::ip::tcp::socket &get_socket();
	unsigned char *get_receive_buffer();
	void initialize();
	void disconnect();
	void send_packet(PacketCreator *packet);
	void start_read_header();
	void read_header_handler(const std::error_code &ec, std::size_t bytes_transferred);
	void read_body_handler(const std::error_code &ec, std::size_t bytes_transferred);
	void send_handler(const std::error_code &ec, std::size_t bytes_transferred, unsigned char *send_buffer);

private:

	asio::ip::tcp::socket socket_;
	Player *player_;
	std::unique_ptr<unsigned char> receive_buffer_;
	unsigned char *iv_recv_;
	unsigned char *iv_send_;
};
