/*
See LICENSE file in root folder
*/
#include <iostream>

#include <ElypseClient.h>
#include <ElypsePlugin.h>

#include <Path.h>

#include <mutex>
#include <condition_variable>
#include <utility>

#include "DynamicLibrary.h"

#if !defined( WIN32 )
#	include <signal.h>
#	include <stdlib.h>
#	include <stdio.h>
#	include <unistd.h>
#else
#endif

using namespace General::Utils;
using namespace Elypse::Server;

typedef std::string String;

#if defined _WIN32
#	define dlerror() ::GetLastError()
#endif

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
#if defined( _MSC_VER )
				::OutputDebugStringA( ( m_prefix + " " + m_buffer ).c_str() );
#endif
			}
			else
			{
				printf( "\n" );
#if defined( _MSC_VER )
				::OutputDebugStringA( "\n" );
#endif
			}

			m_buffer.clear();
		}

		return 0;
	}

private:
	std::string m_buffer;
	std::string m_prefix;
};

class PluginApi
{
public:
	PluginApi( Path const & p_pluginFullPath )
		: m_fullPath( p_pluginFullPath )
	{
		String l_pluginName = m_fullPath.GetLeaf();
		std::cout << "Plugin name [" << l_pluginName << "]" << std::endl;

		if ( !m_library.Open( m_fullPath ) )
		{
			std::stringstream l_error;
			l_error << "Error encountered while loading plugin [" << l_pluginName << "] error: " << dlerror();
			throw std::runtime_error( l_error.str() );
		}

		try
		{
			m_library.GetFunction( "PluginFactory", m_pluginFactory );

			if ( !m_pluginFactory )
			{
				std::stringstream l_error;
				l_error << "Error encountered while loading plugin factory [" << l_pluginName << "] error: " << dlerror();
				throw std::runtime_error( l_error.str() );
			}

			m_library.GetFunction( "PluginDestroyer", m_pluginDestroyer );

			if ( !m_pluginDestroyer )
			{
				std::stringstream l_error;
				l_error << "Error encountered while loading plugin destroyer [" << l_pluginName << "] error: " << dlerror();
				throw std::runtime_error( l_error.str() );
			}
		}
		catch ( std::exception & )
		{
			m_library.Close();
			throw;
		}
	}

	~PluginApi()
	{
	}

	ElypsePlugin * Initialise()
	{
		String l_pluginName = m_fullPath.GetLeaf();
#if defined( _WIN32 )
		String l_pluginFolder = m_fullPath.GetPath();

		if ( !l_pluginFolder.empty() )
		{
			l_pluginFolder += d_path_slash;
		}

#else
		String l_pluginFolder = "/usr/local/lib/";
#endif
		m_plugin = m_pluginFactory( l_pluginFolder.c_str() );

		if ( !m_plugin )
		{
			throw std::runtime_error( "Error encountered while creating the plugin [" + l_pluginName + "]" );
		}

		std::cout << "Checking Plugin version..." << std::endl;

		if ( ElypsePlugin::sm_versionNo != m_plugin->GetVersionNo() )
		{
			std::stringstream l_error;
			l_error << "Server version not the same that plugin version, check this trouble before to continue please..." << std::endl;
			l_error << "    Server Version [" << ElypsePlugin::sm_versionNo << "] vs Plugin Version [" << m_plugin->GetVersionNo() << "]" << std::endl;
			m_pluginDestroyer( m_plugin );
			throw std::runtime_error( l_error.str() );
		}

		if ( m_plugin->GetServices().empty() )
		{
			m_pluginDestroyer( m_plugin );
			throw std::runtime_error( "Plugin [" + m_plugin->GetName() + "] doesn't seems to have any services available..." );
		}

		ElypseServiceArray const & l_services = m_plugin->GetServices();
		std::cout << "Nombres de services : [" << l_services.size() << "]" << std::endl;
		int i = 0;

		for ( auto && l_service : l_services )
		{
			if ( l_service->GetTypeService() == TcpService )
			{
				std::cout << "Launch service " << i++ << "..." << std::endl;
				std::static_pointer_cast< ElypseTcpService >( l_service )->Run();
				std::cout << "Service " << i << " launched..." << std::endl;
			}
		}

		return m_plugin;
	}

	void Cleanup()
	{
		m_pluginDestroyer( m_plugin );
	}

private:
	Path m_fullPath;
	DynamicLibrary m_library;
	PluginFactoryFct m_pluginFactory;
	PluginDestroyerFct m_pluginDestroyer;
	ElypsePlugin * m_plugin;
};

namespace
{
	struct SEventsHandler
	{
		SEventsHandler()
		{
#if !defined( WIN32 )
			struct sigaction l_sigIntHandler;
			l_sigIntHandler.sa_handler = SignalHandler;
			sigemptyset( &l_sigIntHandler.sa_mask );
			l_sigIntHandler.sa_flags = 0;
			sigaction( SIGINT, &l_sigIntHandler, nullptr );
#else
			::SetConsoleCtrlHandler( SignalHandler, TRUE );
#endif
		}
		~SEventsHandler()
		{
#if !defined( WIN32 )
#else
			::SetConsoleCtrlHandler( SignalHandler, FALSE );
#endif
		}

		bool Wait( std::string & p_in )
		{
			m_thread = std::thread( [&p_in]()
			{
				std::cout << "$> ";
				std::cin >> p_in;
				std::unique_lock< std::mutex > l_lock( m_mutex );
				m_stop = false;
				m_variable.notify_one();
			} );

			bool l_continue = false;

			while ( !l_continue )
			{
				std::unique_lock< std::mutex > l_lock( m_mutex );
				l_continue = m_variable.wait_for( l_lock, std::chrono::milliseconds( 10 ) ) == std::cv_status::no_timeout;
			}

			m_thread.join();
			return m_stop;
		}

	private:
#if !defined( WIN32 )
		static void SignalHandler( int s )
		{
			std::unique_lock< std::mutex > l_lock( m_mutex );
			m_variable.notify_all();
		}
#else
		static BOOL __stdcall SignalHandler( DWORD p_event )
		{
			BOOL l_return = FALSE;

			switch ( p_event )
			{
			case CTRL_C_EVENT:
			case CTRL_CLOSE_EVENT:
				std::cout << "Detected Ctrl+C" << std::endl;
				std::unique_lock< std::mutex > l_lock( m_mutex );
				m_stop = true;
				m_variable.notify_one();
				l_return = TRUE;
				break;
			}

			return l_return;
		}
#endif
	public:
		static bool m_stop;

	private:
		static std::thread m_thread;
		static std::condition_variable m_variable;
		static std::mutex m_mutex;
	};

	bool SEventsHandler::m_stop;
	std::thread SEventsHandler::m_thread;
	std::condition_variable SEventsHandler::m_variable;
	std::mutex SEventsHandler::m_mutex;

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
		else
		{
			std::cout << "Type 'help' to have a list of supported commands" << std::endl;
		}

		return l_exit;
	}

	void MainLoop( ElypseServiceArray const & p_services )
	{
		std::cout << "Server main loop..." << std::endl;
		std::mutex l_mutex;
		std::condition_variable l_variable;
		SEventsHandler l_handler;

		std::thread l_thread( [&l_handler, &l_variable, &l_mutex, &p_services]()
		{
			std::string l_in;
			bool l_stop = false;

			std::cout << "Type 'help' to have a list of supported commands" << std::endl;

			do
			{
				l_handler.Wait( l_in );
				l_stop = ApplyMenuAction( l_in, p_services );
			}
			while ( !l_stop && !SEventsHandler::m_stop );

			std::unique_lock< std::mutex > l_lock( l_mutex );
			l_variable.notify_all();
		} );

		bool l_stop = false;

		while ( !l_stop )
		{
			std::unique_lock< std::mutex > l_lock( l_mutex );
			l_stop = l_variable.wait_for( l_lock, std::chrono::milliseconds( 10 ) ) == std::cv_status::no_timeout;
		}

		l_thread.join();
	}
}

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

		{
			PluginApi l_api( l_pluginFullPath );
			ElypsePlugin * l_plugin = l_api.Initialise();
			MainLoop( l_plugin->GetServices() );
			l_api.Cleanup();
		}

		l_return = EXIT_SUCCESS;
	}
	catch ( std::exception & l_ex )
	{
		std::cerr << l_ex.what() << std::endl;
	}

	return l_return;
}
