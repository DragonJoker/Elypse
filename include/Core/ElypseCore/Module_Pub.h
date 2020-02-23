/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_PUB_H___
#define ___MODULE_PUB_H___

#include "Elypse.h"

namespace Elypse
{
	namespace Pub
	{
		class PubInstance;
		class PubObject;
		class PubManager;

		typedef std::map <String, PubInstance *> PubInstanceMap;
		typedef std::map <String, PubObject *> PubObjectMap;
	}
}

using namespace Elypse::Pub;

#endif
