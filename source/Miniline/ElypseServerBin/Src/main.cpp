#include <iostream>

#include <ElypseClient.h>
#include <ElypsePlugin.h>

#include <mutex>
#include <condition_variable>

#include "DynamicLibrary.h"

using namespace General::Utils;
using namespace Elypse::ServerPlugin;

typedef std::string String;

#ifndef __GNUG__
#define dlerror() l_pluginName
#endif

bool ApplyMenuAction( std::string const & p_in, ElypseServiceArray const & p_services )
{
	bool l_exit = false;

	if ( p_in == "help" )
	{
		std::cout << "Commands list:" << std::endl;
		std::cout << "  count    Retrieves the connected users count" << std::endl;
		std::cout << "  list     Retrieves the connected users list" << std::endl;
		std::cout << "  help     Displays this message" << std::endl;
		std::cout << "  exit     Stop the server" << std::endl;
	}
	else if ( p_in == "count" )
	{
	}
	else if ( p_in == "list" )
	{
	}
	else if ( p_in == "exit" )
	{
		l_exit = true;
	}

	return l_exit;
}

int main( int p_argc, char ** p_argv )
{
	if ( p_argc < 2 )
	{
		std::cerr << "Usage : elypseserver [chemin du plugin]" << std::endl;
		return EXIT_FAILURE;
	}

	String l_pluginName( p_argv[1] );
	DynamicLibrary l_library;

	std::cout << "Plugin name [" << l_pluginName << "]" << std::endl;

	if ( !l_library.Open( l_pluginName ) )
	{
		std::cerr << "Error encountered while loading plugin [" << dlerror() << "]" << std::endl;
		return EXIT_FAILURE;
	}

	PluginFactoryFct l_pluginFactory;
	l_library.GetFunction( "PluginFactory", l_pluginFactory );

	if ( l_pluginFactory == NULL )
	{
		std::cerr << "Error encountered while loading plugin factory [" << dlerror() << "]" << std::endl;
		return EXIT_FAILURE;
	}

	PluginDestroyerFct l_pluginDestroyer;
	l_library.GetFunction( "PluginDestroyer", l_pluginDestroyer );

	if ( l_pluginDestroyer == NULL )
	{
		std::cerr << "Error encountered while loading plugin destroyer [" << dlerror() << "]" << std::endl;
		return EXIT_FAILURE;
	}

	size_t l_index = std::min( l_pluginName.find_last_of( '/' ), l_pluginName.find_last_of( '\\' ) );
	l_pluginName = l_pluginName.substr( 0, l_index + 1 );
	ElypsePlugin * l_plugin = l_pluginFactory( l_pluginName.c_str() );

	std::cout << "Checking Plugin version..." << std::endl;

	if ( ElypsePlugin::sm_versionNo != l_plugin->GetVersionNo() )
	{
		std::cerr << "Error : Server version not the same that plugin version, check this trouble before to continue please..." << std::endl;
		std::cerr << "Server Version [" << ElypsePlugin::sm_versionNo << "] vs Plugin Version [" << l_plugin->GetVersionNo() << "]" << std::endl;
		l_pluginDestroyer( l_plugin );
		return EXIT_FAILURE;
	}

	if ( l_plugin->GetServices().empty() )
	{
		std::cerr << "Error : Plugin [" << l_plugin->GetName() << "] doesn't seems to have any services available..." << std::endl;
		l_pluginDestroyer( l_plugin );
		return EXIT_FAILURE;
	}

	ElypseServiceArray l_services = l_plugin->GetServices();
	size_t imax = l_services.size();

	std::cout << "Nombres de services : [" << imax << "]" << std::endl;

	for ( size_t i = 0 ; i < imax ; i++ )
	{
		if ( l_services[i]->GetTypeService() == TcpService )
		{
			std::cout << "Launch service " << i << "..." << std::endl;
			std::static_pointer_cast< ElypseTcpService >( l_services[i] )->Run();
			std::cout << "Service " << i << " launched..." << std::endl;
		}
	}

	std::cout << "Server main loop..." << std::endl;
	std::mutex l_mutex;
	std::condition_variable l_variable;

	std::thread l_thread( [&l_variable, &l_services]()
	{
		std::string l_in;
		bool l_stop = false;

		do
		{
			std::cout << "Type 'help' to have a list of supported commands" << std::endl;
			std::cout << "> ";
			std::cin >> l_in;

			l_stop = ApplyMenuAction( l_in, l_services );
		}
		while ( !l_stop );

		l_variable.notify_all();
	} );

	while ( true )
	{
		std::unique_lock< std::mutex > l_lock( l_mutex );
		l_variable.wait_for( l_lock, std::chrono::milliseconds( 3 ) );
	}

	l_pluginDestroyer( l_plugin );
	return EXIT_SUCCESS;
}
