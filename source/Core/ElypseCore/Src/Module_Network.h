#ifndef ___MODULE_NETWORK_H___
#define ___MODULE_NETWORK_H___

#include "EMuse.h"

namespace EMuse
{
	namespace Network
	{
		class NetworkManager;
		class ElypseTcpClient;
	}
}

typedef std::map <String, EMuse::Network::ElypseTcpClient *> ElypseTcpClientStrMap;
using namespace EMuse::Network;

#endif
