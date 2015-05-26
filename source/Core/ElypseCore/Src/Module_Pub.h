#ifndef ___MODULE_PUB_H___
#define ___MODULE_PUB_H___

#include "EMuse.h"

namespace EMuse
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

using namespace EMuse::Pub;

#endif
