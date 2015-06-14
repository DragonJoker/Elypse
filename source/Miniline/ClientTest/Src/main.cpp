/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#include "Client.h"
#include "Server.h"

#include <Utils.h>
#include <Buffer.h>

using namespace General::Templates;
using namespace Elypse::Network;
using namespace boost;

asio::io_service * g_service;

void start()
{
	while ( true )
	{
		g_service->run();
	}
}

int ServerMain()
{
	TcpServer l_server( 31333 );
	l_server.Run();

	//chat_server_list servers;

	//tcp::endpoint endpoint( tcp::v4(), 31333);
	//chat_server_ptr server( new chat_server(io_service, endpoint));
	//servers.push_back( server);

	//io_service.run();

	return EXIT_SUCCESS;
}

int ClientMain()
{
	unsigned int l_count = 0;
	std::vector <ElypseTcpClient *> l_clients;
	ElypseTcpClient * l_client;

	g_service = new asio::io_service();
	std::thread l_thread( &start );


	char * l_connectMsg = new char[256];
	WriteBuffer l_buffer( l_connectMsg, 256 );
	l_buffer << ( int )0 << ( int )9;
	l_buffer.writeArray<char>( "anonymous", 9 );
	l_buffer << ( int )9;
	l_buffer.writeArray<char>( "anonymous", 9 );

	std::string l_mesg;
	l_mesg.assign( l_buffer.data(), l_buffer.size() );

	int l_max = 500;

	while ( true )
	{
		d_coucou;

		for ( int i = 0 ; i < l_max ; i++ )
		{
			std::cout << "creating client " << i << " out of [ " << l_count << " ]\n";
			l_count++;

			l_client = new ElypseTcpClient( g_service );
			l_client->AsyncConnect( "127.0.0.1", 48621);
			l_clients.push_back( l_client );
		}

		for ( int i = 0 ; i < l_max ; i++ )
		{
			if ( l_clients[i]->IsConnected() )
			{
				std::cout << "client " << i << " sends connect message\n";
				l_clients[i]->AsyncSend( l_mesg );
			}
			else
			{
				l_clients[i]->StoreMessage( l_mesg );
			}
		}

		for ( int i = 0 ; i < l_max ; i++ )
		{
			std::cout << "deleting client " << i << "\n";
			l_clients[i]->Stop();
		}

		l_clients.clear();
	}

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
