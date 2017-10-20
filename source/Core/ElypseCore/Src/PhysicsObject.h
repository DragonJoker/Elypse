/*
See LICENSE file in root folder
*/
#ifndef ___PHYSICS_OBJECT_H___
#define ___PHYSICS_OBJECT_H___

#include "Module_Physics.h"

//#include <OgreUserDefinedObject.h>
#include <OgreAxisAlignedBox.h>

#include <ode/contact.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export ObjectState
		{
		public:
			Ogre::Vector3 position;
			Ogre::Quaternion orientation;

		public:
			ObjectState()
			{
				_zero();
			}

		private:
			void _zero()
			{
				position = Ogre::Vector3::ZERO;
				orientation = Ogre::Quaternion::IDENTITY;
			}
		};

		class d_dll_export PhysicsObject : public General::Theory::named,
			public General::Theory::owned_by <PhysicsSimulation>,
//							public Ogre::UserDefinedObject,
			d_noncopyable
		{
		private:
			Ogre::Entity * m_entity;
			Ogre::SceneNode * m_node;

			Space * m_space;
			Mass * m_mass;
			PhysicsMaterial * m_material;

			Ode::dBodyID	m_body;

			Ogre::Vector3 m_linearVelocity;
			Ogre::Vector3 m_angularVelocity;
			Ogre::Vector3 m_finiteAxis;
			Ogre::Vector3 m_forces;
			Ogre::Vector3 m_torque;
			Ogre::AxisAlignedBox m_box;

			bool m_enabled;
			bool m_static;
			bool m_phantom;
			bool m_applyMoveOut;
			bool m_isDamped;
			bool m_isLinearDamped;
			bool m_isAngularDamped;

			Real m_linearDamping;
			Real m_angularDamping;

			BoundingShapeArray m_bounds;

			ObjectState m_currentState;
			ObjectState m_previousState;
			ObjectState m_drawState;
			size_t m_stateLimit;

			uint32_t m_id;

			Real m_disableThresold_time;
			Real m_disableThresold_linear;
			Real m_disableThresold_angular;

			ContactMap m_currentContacts;

			Ogre::Vector3 m_outTranslate;

		public:
			PhysicsObject( PhysicsSimulation & p_parent, Ogre::Entity * p_entity, Space * p_space );
			PhysicsObject( PhysicsSimulation & p_parent, String const & p_name, Space * p_space );
			~PhysicsObject();

		public:
			void AddBound( BoundingShape * p_object );
			PhysicsObject * Clone( String const & p_name, Space * p_space );

			void UpdateState();
			void UpdateNode();

			void ClearContacts();
			void AddContact( PhysicsObject * p_object, Ode::dContact * p_contact );

			void AddForce( const Ogre::Vector3 & p_force );
			void AddTorque( const Ogre::Vector3 & p_torque );
			void AddRelativeForce( const Ogre::Vector3 & p_force );
			void AddRelativeTorque( const Ogre::Vector3 & p_torque );

			void AddForceAt( const Ogre::Vector3 & p_force, const Ogre::Vector3 & p_position );
			void AddForceAtRelative( const Ogre::Vector3 & p_force, const Ogre::Vector3 & p_position );
			void AddRelativeForceAt( const Ogre::Vector3 & p_force, const Ogre::Vector3 & p_position );
			void AddRelativeForceAtRelative( const Ogre::Vector3 & p_force, const Ogre::Vector3 & p_position );

			const Ogre::Vector3 & GetForce();
			const Ogre::Vector3 & GetTorque();

			Ogre::Vector3 GetActualPosition();

			void SetForce( const Ogre::Vector3 & p_force );
			void SetTorque( const Ogre::Vector3 & p_torque );

			Ogre::Vector3 GetLocalVelocity( const Ogre::Vector3 & p_position );

			void SetFloating( bool p_floating );
			bool GetFloating();

			const BoundingShapeArray & GetAllBounds()const
			{
				return m_bounds;
			}

			void SetLinearDamping( Real p_linearDamping );
			void SetAngularDamping( Real p_angularDamping );
			Real GetLinearDamping();
			Real GetAngularDamping();

			void ApplyDamping();

			void InterpolateSpacialState( Real p_factor );

			void ReassertSpatialState();

			void SetPosition( const Ogre::Vector3 & p_position );
			void SetOrientation( const Ogre::Quaternion & p_orientation );

			void SetSpace( Space * p_space );
			void SetAutoDisable( Real p_time, Real p_angular, Real p_linear );

			bool CollisionTest_Fast( PhysicsObject * p_collideWith );
			int NumCollisionTest_Fast( PhysicsObject * p_collideWith );
			bool CollisionTest_Complete( PhysicsObject * p_collidesWith );
			int NumCollisionTest_Complete( PhysicsObject * p_collidesWith );

			void SetEnabled( bool p_enabled );

			Ogre::Vector3 GetSpeed();
			void SetSpeed( const Ogre::Vector3 & p_speed );

			Ogre::Vector3 GetAngularSpeed();
			void SetAngularSpeed( const Ogre::Vector3 & p_speed );

			void SetMass( Real p_mass );

			void SetNode( Ogre::SceneNode * p_node );

			void ApplyMoveOut();

			void SetMoveOut( const Ogre::Vector3 & p_translate );

		public:
			inline bool IsEnabled()const
			{
				return m_enabled;
			}

			inline void SetPhantom( bool p_phantom )
			{
				m_phantom = p_phantom;

				if ( p_phantom )
				{
					SetEnabled( false );
				}
			}
			inline bool IsPhantom()const
			{
				return m_phantom;
			}

			inline void SetId( uint32_t p_id )
			{
				m_id = p_id;
			}
			inline uint32_t GetId()const
			{
				return m_id;
			}

			inline void SetStatic( bool p_static )
			{
				m_static = p_static;

				if ( p_static )
				{
					SetEnabled( false );
				}
			}
			inline bool IsStatic()const
			{
				return m_static;
			}

			inline const Ogre::Vector3 & GetLinearVelocity()const
			{
				return m_linearVelocity;
			}
			inline const Ogre::Vector3 & GetAngularVelocity()const
			{
				return m_angularVelocity;
			}

			inline const Ogre::AxisAlignedBox & GetBoundingBox()const
			{
				return m_box;
			}
			inline size_t GetNumBounds()const
			{
				return m_bounds.size();
			}
			inline BoundingShape * GetBoundIndexed( size_t p_index ) const
			{
				return m_bounds[p_index];
			}
			inline Space * GetSpace()const
			{
				return m_space;
			}

			inline const Ogre::Vector3 & GetPosition()
			{
				return m_drawState.position;
			}
			inline const Ogre::Quaternion & GetOrientation()
			{
				return m_drawState.orientation;
			}
			inline Ode::dBodyID GetBodyId()const
			{
				return m_body;
			}

			inline void SetMaterial( PhysicsMaterial * p_material )
			{
				m_material = p_material;
			}
			inline PhysicsMaterial * GetMaterial()const
			{
				return m_material;
			}

			inline const ContactMap & GetAllContacts()const
			{
				return m_currentContacts;
			}

			inline size_t GetNumContacts()const
			{
				return m_currentContacts.size();
			}

			inline Ogre::Entity * GetEntity()const
			{
				return m_entity;
			}

			inline void SetApplyMoveOut( bool p_move )
			{
				m_applyMoveOut = p_move;
			}
			inline void SetEntity( Ogre::Entity * p_entity )
			{
				m_entity = p_entity;
			}
		};
	}
}

#endif
