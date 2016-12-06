/*     ________           ________
	 /  ______  \       /  ______  \
   /  /        \  \   /  /        \  \
 /  /            \  "  /            \  \
|  |               \ /               |  |
|  |                "                |  |
|  |                                 |  |
 |  |                              |  |
  |  |                            |  |
	|  |                         |  |
	  |  |                     |  |
		|  |                 |  |
		  |  |             |  |
			|  |         |  |
			  |  |     |  |
				|  | |  |
				  | | |
				   | |
					|
*/

//
// by Buya (the developer's pseudonym)
//

#include <thread>

#include "constants.hpp"
#include "session.hpp"
#include "world.hpp"

#include "Poco\Data\MySQL\Connector.h"

void ConnectionAcceptorThread()
{
	World *world = World::get_instance();

	auto endpoint = asio::ip::tcp::endpoint(asio::ip::tcp::v4(), kServerPort);
	asio::ip::tcp::acceptor acceptor(world->get_io_service(), endpoint);

	while (world->is_accepting_connections())
	{
		std::shared_ptr<Session> session(new Session());

		std::error_code ec;
		acceptor.accept(session->get_socket(), ec);

		if (!ec)
		{
			session->initialize();
		}
	}
}

// entry point of the program

int main(int argc, char *argv[])
{
	// get arguments

	bool is_dedicated_server = false;

	for (int i = 0; i < argc; ++i)
	{
		char *argument = argv[i];

		if (strcmp(argument, "-dedicated") == 0)
		{
			is_dedicated_server = true;
		}
	}

	// initialize mysql

	Poco::Data::MySQL::Connector::registerConnector();

	// detach the console from the process, therefore close it, the process then runs as a background thread
	// this is a windows-WINAPI function
	
	//FreeConsole();

	// initialize the random number generator

	time_t time_value = time(nullptr);
	srand(static_cast<unsigned int>(time_value));

	// create and initialize world

	World world(is_dedicated_server);
	world.initialize();

	// start the connection acceptor thread

	std::thread thread1(ConnectionAcceptorThread);

	// start the i/o work

	asio::io_service &io = world.get_io_service();
	asio::io_service::work work(io);

	for (;;)
	{
		std::error_code ec;
		io.run(ec);
	}

	// program exits successfully

	return EXIT_SUCCESS;
}
