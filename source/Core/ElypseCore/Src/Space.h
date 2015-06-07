#ifndef ___PHYSICAL_SPACE_H___
#define ___PHYSICAL_SPACE_H___

#include "Module_Physics.h"

#include <OgreAxisAlignedBox.h>

//Used for space partitionning, later.

namespace EMuse
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

			void __declspec( nothrow ) Clear();

			void Render( Real l_factor );
			void ApplyDamping();
			void UpdateState();

		public:
			inline dSpaceID						   GetSpaceID()const
			{
				return m_space;
			}
			inline dGeomID					 GetSpaceAsGeomID()const
			{
				return reinterpret_cast <dGeomID>( m_space );
			}
			inline bool					GetInternalCollisions()const
			{
				return m_internalCollisions;
			}
			inline size_t						GetNumObjects()const
			{
				return m_objects.size();
			}
			inline const PhysicsObjectSet 	&	GetAllObjects()const
			{
				return m_objects;
			}

			inline void			SetInternalCollisions( bool p_collide )
			{
				m_internalCollisions = p_collide;
			}
		};
	}
}

#endif
