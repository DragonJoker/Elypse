/*
See LICENSE file in root folder
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
