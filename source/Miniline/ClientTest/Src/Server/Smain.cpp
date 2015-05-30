#include "../Common/Common.h"

#include "Server.h"

using namespace General::MultiThreading;

int main( int argc, char * argv[])
{
	try
	{
		TcpServer l_server( 31333);
		l_server.Run();

		/*
		chat_server_list servers;

		tcp::endpoint endpoint( tcp::v4(), 31333);
		chat_server_ptr server( new chat_server(io_service, endpoint));
		servers.push_back( server);

		io_service.run();
		*/
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	Sleep(10000);

	return 0;
}
