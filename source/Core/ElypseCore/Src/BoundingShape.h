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
#ifndef ___BOUNDING_SHAPE_H___
#define ___BOUNDING_SHAPE_H___

#include "Module_Physics.h"

#include <ode/collision.h>

#include "PhysicsObject.h"
#include "Space.h"

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export BoundingShape : d_noncopyable
		{
		public:
			BoundingShape( BoundingShapeClass p_class );
			virtual ~BoundingShape();

		protected:
			void _register();

		public:
			void SetParent( PhysicsObject * p_parent );
			void SetMaxContacts( uint32_t p_maxContacts );
			void SetSpace( Space * p_space );
			bool Collide( BoundingShape * p_collideWith );
			const Ogre::AxisAlignedBox &	GetBoundingBox();

			virtual BoundingShape * Clone() = 0;

			void SetPosition( const Ogre::Vector3 & p_position );
			void SetOrientation( const Ogre::Quaternion & p_orientation );

		public:
			inline void SetRadius( Real p_radius )
			{
				m_radius = p_radius;
			}
			inline void SetAABB( Ogre::AxisAlignedBox p_box )
			{
				m_box = p_box;
			}
			inline void SetEnabled( bool p_enabled )
			{
				m_enabled = p_enabled;
			}

			inline Real GetRadius()const
			{
				return m_radius;
			}
			inline bool IsEnabled()const
			{
				return m_enabled;
			}
			inline Ode::dGeomID GetGeomID()const
			{
				return m_geom;
			}
			inline Space * GetSpace()const
			{
				return m_space;
			}
			inline BoundingShapeClass GetClass()const
			{
				return m_class;
			}

		protected:
			Ogre::AxisAlignedBox m_box;

			BoundingShapeClass m_class;
			Real m_radius;
			bool m_enabled;
			Ogre::Vector3 m_position;
			uint32_t m_maxContacts;

			Ode::dGeomID m_geom;
			Space * m_space;
			PhysicsObject * m_parentObject;
			Ode::dContact * m_contacts;
		};
	}
}

#endif
