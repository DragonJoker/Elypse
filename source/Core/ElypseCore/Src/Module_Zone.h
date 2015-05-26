/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___MODULE_SCENE_H___
#define ___MODULE_SCENE_H___

#include "EMuse.h"

namespace EMuse
{
	namespace Scene
	{
		class Zone;
		class ZoneObject;
		class Portal;
		class Universe;
		class UniverseManager;

//	typedef Manager<Universe> UniverseManager;

		typedef std::map<String, Portal *> PortalMap;
		typedef std::map<String, Zone *> ZoneMap;
		typedef std::set<ZoneObject *> ZoneObjectSet;
	}
}

using namespace EMuse::Scene;

#endif
