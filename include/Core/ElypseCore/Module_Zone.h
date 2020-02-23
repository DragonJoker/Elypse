/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_SCENE_H___
#define ___MODULE_SCENE_H___

#include "Elypse.h"

#include <memory>

namespace Elypse
{
	namespace Scene
	{
		class Zone;
		class ZoneObject;
		class Portal;
		class Universe;
		class UniverseManager;

//	typedef Manager<Universe> UniverseManager;
		using ZonePtr = std::unique_ptr< Zone >;

		typedef std::map<String, ZonePtr > ZoneMap;
		typedef std::map<String, Portal * > PortalMap;
		typedef std::set< ZoneObject * > ZoneObjectSet;
	}
}

using namespace Elypse::Scene;

#endif
