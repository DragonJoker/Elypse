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
#ifndef ___PUB_INSTANCE_H___
#define ___PUB_INSTANCE_H___

#include "Module_Pub.h"

#include "ZoneObject.h"

#include <OwnedBy.h>

namespace Elypse
{
	namespace Pub
	{
		class d_dll_export PubInstance : public ZoneObject, public owned_by <PubObject>
		{
		private:
			Entity * m_entity;
			Zone * m_zone;
			unsigned int m_submat;

		public:
			PubInstance( PubObject * p_owner, Entity * p_entity );
			~PubInstance();

		public:
			void Apply();

			virtual void AddToZone( Zone * p_zone )
			{
				m_zone = p_zone;
			}
			virtual void RemoveFromZone( Zone * p_zone );

			inline Entity * GetEntity()const
			{
				return m_entity;
			}

			inline void SetSubMat( unsigned int p_submat )
			{
				m_submat = p_submat;
			}
		};
	}
}

#endif
