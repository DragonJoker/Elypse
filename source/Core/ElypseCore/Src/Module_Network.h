/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_NETWORK_H___
#define ___MODULE_NETWORK_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Network
	{
		class NetworkManager;
		class ElypseTcpClient;
	}
}

typedef std::map< String, std::unique_ptr< Elypse::Network::ElypseTcpClient > > ElypseTcpClientStrMap;

#endif
