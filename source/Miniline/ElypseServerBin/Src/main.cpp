#include <iostream>

#include <ElypseClient.h>
#include <ElypsePlugin.h>

#include <Path.h>

#include <mutex>
#include <condition_variable>

#include "DynamicLibrary.h"

using namespace General::Utils;
using namespace Elypse::Server;

typedef std::string String;

#if defined _WIN32
#	define dlerror() ::GetLastError()
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

class dummy_streambuf
	: public std::streambuf
{
public:
	int_type overflow( int_type c = traits_type::eof() ) override
	{
		if ( traits_type::eq_int_type( c, traits_type::eof() ) )
		{
			sync();
		}

		return c;
	}
};

class prefix_streambuf
	: public std::streambuf
{
public:
	prefix_streambuf( std::string const & p_prefix )
		: m_prefix( p_prefix )
	{
	}

	int_type overflow( int_type c = traits_type::eof() ) override
	{
		if ( traits_type::eq_int_type( c, traits_type::eof() ) )
		{
			sync();
		}
		else
		{
			m_buffer += traits_type::to_char_type( c );
		}

		return c;
	}

	int sync() override
	{
		if ( !m_buffer.empty() )
		{
			if ( m_buffer != "\n" )
			{
				printf( "%s %s", m_prefix.c_str(), m_buffer.c_str() );
			}
			else
			{
				printf( "\n" );
			}

			m_buffer.clear();
		}

		return 0;
	}

private:
	std::string m_buffer;
	std::string m_prefix;
};

int main( int p_argc, char ** p_argv )
{
	int l_return = EXIT_FAILURE;
	std::clog.rdbuf( new prefix_streambuf( "[  ]" ) );
	std::cerr.rdbuf( new prefix_streambuf( "[EE]" ) );
	std::cout.rdbuf( new prefix_streambuf( "[--]" ) );

	try
	{
		if ( p_argc < 2 )
		{
			throw std::runtime_error( "Usage : elypseserver [chemin du plugin]" );
		}

		Path l_pluginFullPath( p_argv[1] );
		String l_pluginName = l_pluginFullPath.GetLeaf();
		DynamicLibrary l_library;

		std::cout << "Plugin name [" << l_pluginName << "]" << std::endl;

		if ( !l_library.Open( l_pluginFullPath ) )
		{
			throw std::runtime_error( "Error encountered while loading plugin [" + l_pluginName + "] error: " + std::to_string( dlerror() ) );
		}

		PluginFactoryFct l_pluginFactory;
		l_library.GetFunction( "PluginFactory", l_pluginFactory );

		if ( !l_pluginFactory )
		{
			throw std::runtime_error( "Error encountered while loading plugin factory [" + l_pluginName + "] error: " + std::to_string( dlerror() ) );
		}

		PluginDestroyerFct l_pluginDestroyer;
		l_library.GetFunction( "PluginDestroyer", l_pluginDestroyer );

		if ( !l_pluginDestroyer )
		{
			throw std::runtime_error( "Error encountered while loading plugin destroyer [" + l_pluginName + "] error: " + std::to_string( dlerror() ) );
		}

		String l_pluginFolder = l_pluginFullPath.GetPath();

		if ( !l_pluginFolder.empty() )
		{
			l_pluginFolder += d_path_slash;
		}

		ElypsePlugin * l_plugin = l_pluginFactory( l_pluginFolder.c_str() );

		if ( !l_plugin )
		{
			throw std::runtime_error( "Error encountered while creating the plugin [" + l_pluginName + "]" );
		}

		std::cout << "Checking Plugin version..." << std::endl;

		if ( ElypsePlugin::sm_versionNo != l_plugin->GetVersionNo() )
		{
			std::stringstream l_error;
			l_error << "Server version not the same that plugin version, check this trouble before to continue please..." << std::endl;
			l_error << "    Server Version [" << ElypsePlugin::sm_versionNo << "] vs Plugin Version [" << l_plugin->GetVersionNo() << "]" << std::endl;
			l_pluginDestroyer( l_plugin );
			throw std::runtime_error( l_error.str() );
		}

		if ( l_plugin->GetServices().empty() )
		{
			l_pluginDestroyer( l_plugin );
			throw std::runtime_error( "Plugin [" + l_plugin->GetName() + "] doesn't seems to have any services available..." );
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
		l_return = EXIT_SUCCESS;
	}
	catch ( std::exception & l_ex )
	{
		std::cerr << l_ex.what() << std::endl;
	}

	return l_return;
}
