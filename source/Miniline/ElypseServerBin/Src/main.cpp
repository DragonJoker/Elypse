#include <iostream>


#include <ElypseClient.h>
#include <ElypsePlugin.h>

#include "Utils.h"
#include "DynamicLibrary.h"

using namespace General::Utils;
using namespace Elypse::ServerPlugin;

typedef std::string String;

#ifndef __GNUG__
#define dlerror() l_pluginName
#endif

int main( int p_argc, char ** p_argv)
{
	if (p_argc < 2)
	{
		std::cerr << "Usage : elypseserver [chemin du plugin]" << std::endl;
		return 1;
	}

	String l_pluginName( p_argv[1]);
	DynamicLibrary * l_library = new DynamicLibrary();

	std::cout << "Plugin name [" << l_pluginName << "]" << std::endl;

	if ( ! l_library->Open( l_pluginName))
	{
		std::cerr << "Error encountered while loading plugin [" << dlerror() << "]" << std::endl;
		delete l_library;
		return 1;
	}

	PluginFactoryFct l_pluginFactory = reinterpret_cast <PluginFactoryFct> ( l_library->GetFunction( "PluginFactory"));

	if (l_pluginFactory == NULL)
	{
		std::cerr << "Error encountered while loading plugin factory [" << dlerror() << "]" << std::endl;
		delete l_library;
		return 1;
	}

	PluginDestroyerFct l_pluginDestroyer = reinterpret_cast <PluginDestroyerFct> ( l_library->GetFunction( "PluginDestroyer"));

	if (l_pluginDestroyer == NULL)
	{
		std::cerr << "Error encountered while loading plugin destroyer [" << dlerror() << "]" << std::endl;
		delete l_library;
		return 1;
	}

	size_t l_index = std::min( l_pluginName.find_last_of( '/' ), l_pluginName.find_last_of( '\\' ) );
	l_pluginName = l_pluginName.substr( 0, l_index + 1 );
	ElypsePlugin * l_plugin = l_pluginFactory( l_pluginName.c_str() );

	std::cout << "Checking Plugin version..." << std::endl;

	if (ElypsePlugin::sm_versionNo != l_plugin->GetVersionNo())
	{
		std::cerr << "Error : Server version not the same that plugin version, check this trouble before to continue please..." << std::endl;
		std::cerr << "Server Version [" << ElypsePlugin::sm_versionNo << "] vs Plugin Version [" << l_plugin->GetVersionNo() << "]" << std::endl;

		l_pluginDestroyer( l_plugin);

		delete l_library;
		return 1;
	}

	if (l_plugin->GetServices().empty())
	{
		std::cerr << "Error : Plugin [" << l_plugin->GetName() << "] doesn't seems to have any services available..." << std::endl;

		l_pluginDestroyer( l_plugin);

		delete l_library;
		return 1;
	}

	ElypseServiceArray l_services = l_plugin->GetServices();
	size_t imax = l_services.size();

	std::cout << "Nombres de services : [" << imax << "]" << std::endl;

	for (size_t i = 0 ; i < imax ; i++)
	{
		if (l_services[i]->GetTypeService() == TcpService)
		{
			std::cout << "Launch service " << i << "..." << std::endl;
			reinterpret_cast <ElypseTcpService *> ( l_services[i])->Run();
			std::cout << "Service " << i << " launched..." << std::endl;
		}
	}

	std::cout << "Server main loop..." << std::endl;

	while (true)
	{
		Sleep( 3);
	}

	l_pluginDestroyer( l_plugin);

	delete l_library;
	return 0;
}
