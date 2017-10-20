/*
See LICENSE file in root folder
*/
#include "Client.h"
#include "Server.h"

#include <Utils.h>
#include <Buffer.h>

#include <atomic>

using namespace General::Templates;
using namespace Elypse::Network;
using namespace boost;

uint16_t const g_port = 31333;

int ServerMain()
{
	TcpServer l_server( g_port );
	l_server.Run();

	//chat_server_list servers;

	//tcp::endpoint endpoint( tcp::v4(), g_port);
	//chat_server_ptr server( new chat_server(io_service, endpoint));
	//servers.push_back( server);

	//io_service.run();

	return EXIT_SUCCESS;
}

int ClientMain()
{
	asio::io_service l_service;
	std::atomic_bool l_stop{ false };
	std::thread l_thread{ [&l_stop, &l_service]()
	{
		while ( !l_stop )
		{
			l_service.run();
		}
	} };

	std::array< char, 256 > l_connectMsg;
	WriteBuffer l_buffer( l_connectMsg.data(), 256 );
	l_buffer << int32_t{ 0 } << int32_t{ 9 };
	l_buffer.writeArray( "anonymous" );
	l_buffer << int32_t{ 9 };
	l_buffer.writeArray( "anonymous" );
	std::string const l_mesg( l_buffer.data(), l_buffer.size() );

	int const l_max = 200;

	uint32_t l_count = 0;
	std::vector< ElypseTcpClient * > l_clients;

	for ( int j = 0; j < l_max; ++j )
	{
		for ( int i = 0 ; i < l_max ; i++ )
		{
			std::cout << "creating client " << i << " out of [ " << l_count << " ]\n";
			l_count++;

			auto l_client = new ElypseTcpClient( &l_service );
			l_client->AsyncConnect( "127.0.0.1", g_port );
			l_clients.push_back( l_client );
		}

		for ( auto & l_client : l_clients )
		{
			if ( l_client->IsConnected() )
			{
				std::cout << "client sends connect message\n";
				l_client->AsyncSend( l_mesg );
			}
			else
			{
				l_client->StoreMessage( l_mesg );
			}
		}

		for ( auto & l_client : l_clients )
		{
			std::cout << "deleting client\n";
			l_client->Stop();
		}

		l_clients.clear();
	}

	l_stop = true;
	l_thread.join();

	return EXIT_SUCCESS;
}

int main( int argc, char * argv[] )
{
	std::string l_usage = "Usage: ElypseServerBin --server/--client";
	int l_return = EXIT_FAILURE;

	if ( argc < 2 )
	{
		std::cerr << l_usage << std::endl;
	}
	else
	{
		try
		{
			std::string l_param = argv[1];

			if ( l_param == "--server" )
			{
				l_return = ServerMain();
			}
			else if ( l_param == "--client" )
			{
				l_return = ClientMain();
			}
			else
			{
				std::cerr << l_usage << std::endl;
			}

			l_return = EXIT_SUCCESS;
		}
		catch ( std::exception & e )
		{
			std::cerr << "Exception: " << e.what() << std::endl;
		}
	}

	return l_return;
}
