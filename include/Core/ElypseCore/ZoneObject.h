/*
See LICENSE file in root folder
*/
#ifndef ___ZONE_OBJECT_H___
#define ___ZONE_OBJECT_H___

#include "Module_Zone.h"

namespace Elypse
{
	namespace Scene
	{
		class ZoneObject
		{
		public:
			virtual ~ZoneObject() {}
			virtual void AddToZone( Zone * p_zone ) = 0;
			virtual void RemoveFromZone( Zone * p_zone ) = 0;
		};
	}
}

#endif
