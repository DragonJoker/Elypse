#include "Client.h"

#include <General/Utils.h>
#include <General/Buffer.h>

using namespace General::MultiThreading;
using namespace General::Templates;
using namespace boost;

asio::io_service * g_service;

void start()
{
	while (true)
	{
		g_service->run();
		Sleep( 100);
	}
}

void main()
{
	unsigned int l_count = 0;
	try
	{
		std::vector <ElypseTcpClient *> l_clients;
		asio::io_service * l_service;
		ElypseTcpClient * l_client;

		g_service = new asio::io_service();
		new Thread( start);
		

		char * l_connectMsg = new char[256];
		WriteBuffer l_buffer( l_connectMsg, 256);
		l_buffer << (int)0 << (int)9;
		l_buffer.writeArray<char>( "anonymous", 9);
		l_buffer << (int)9;
		l_buffer.writeArray<char>( "anonymous", 9);

		std::string l_mesg;
		l_mesg.assign( l_buffer.c_str(), l_buffer.size());

		int l_max = 500;

		while (true)
		{
			d_coucou;
			for (int i = 0 ; i < l_max ; i++)
			{
				std::cout << "creating client " << i << " out of [ " << l_count << " ]\n";
				l_count++;

				l_client = new ElypseTcpClient( g_service);
//				l_client->AsyncConnect( "192.168.0.115", 48621);
				l_client->AsyncConnect( "ns368997.ovh.nettail ps -ef", 48621);
//				l_client->Run();
//				Sleep( 100);
				
/*				
				if (l_client->IsConnected())
				{
					l_client->AsyncSend( l_mesg);
				}
				else
				{
					l_client->StoreMessage( l_mesg);
				}
*/				

				l_clients.push_back( l_client);

			}

			
			Sleep(1000);
			for (int i = 0 ; i < l_max ; i++)
			{
				if (l_clients[i]->IsConnected())
				{
//					std::cout << "client " << i << " sends connect message\n";
					l_clients[i]->AsyncSend( l_mesg);
				}
				else
				{
//					d_coucou;
					l_clients[i]->StoreMessage( l_mesg);
				}
			}
			

			Sleep(1000);
			for (int i = 0 ; i < l_max ; i++)
			{
				std::cout << "deleting client " << i << "\n";
//				l_service = l_clients[i]->GetService();
//				delete l_clients[i];
				l_clients[i]->Stop();
//				delete l_service;
			}
			Sleep( 100);
			l_clients.clear();
		}

	}
	catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	
	Sleep( 100000);
}

