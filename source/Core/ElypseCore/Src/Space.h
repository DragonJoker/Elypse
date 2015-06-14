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
#ifndef ___PHYSICAL_SPACE_H___
#define ___PHYSICAL_SPACE_H___

#include "Module_Physics.h"

#include <OgreAxisAlignedBox.h>

//Used for space partitionning, later.

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export Space : public named, noncopyable, public owned_by<PhysicsSimulation>
		{
		private:
			dSpaceID m_space;

			PhysicsObjectSet m_objects;
			AxisAlignedBox m_box;

			bool m_autoUpdated;
			bool m_internalCollisions;

		public:
			Space( const String & p_name, PhysicsSimulation * p_simulation, bool p_autoUpdated );
			~Space();

		public:
			void AddObject( PhysicsObject * p_object );
			void RemoveObject( PhysicsObject * p_object );
			const AxisAlignedBox & GetBoundingBox();

			void CollideInternal();
			void CollideSpace( Space * p_collidesWith );
			void CollideShape( BoundingShape * p_collidesWith );
			void CollideObject( PhysicsObject * p_collidesWith );

			void Clear() d_no_throw;

			void Render( Real l_factor );
			void ApplyDamping();
			void UpdateState();

		public:
			inline dSpaceID GetSpaceID()const
			{
				return m_space;
			}
			inline dGeomID GetSpaceAsGeomID()const
			{
				return reinterpret_cast <dGeomID>( m_space );
			}
			inline bool GetInternalCollisions()const
			{
				return m_internalCollisions;
			}
			inline size_t GetNumObjects()const
			{
				return m_objects.size();
			}
			inline const PhysicsObjectSet & GetAllObjects()const
			{
				return m_objects;
			}
			inline void SetInternalCollisions( bool p_collide )
			{
				m_internalCollisions = p_collide;
			}
		};
	}
}

#endif
