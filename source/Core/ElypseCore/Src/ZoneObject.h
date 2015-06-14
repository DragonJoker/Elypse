/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
