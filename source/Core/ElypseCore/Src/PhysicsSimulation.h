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
#ifndef ___PHYSICS_SIMULATION_H___
#define ___PHYSICS_SIMULATION_H___

#include "Module_Physics.h"

#include "Module_Script.h"

#include <PreciseTimer.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export PhysicsSimulation : public named, noncopyable
		{
		private:
			BoundingRay * m_rayInstance;
			PhysicsObject * m_rayHolder;
			PhysicsMaterial * m_defaultMaterial;

			Space * m_dynamicSpace;
			Space * m_phantomSpace;
			Space * m_staticSpace;

			ScriptNode * m_scriptPerFrame;

			StepMode m_mode;
			PreciseTimer m_timer;

			dWorldID m_world;
			dJointGroupID m_contacts;
			Real m_erp;
			Real m_cfm;
			Real m_steptime;
			Real m_timescale;
			double m_steptimeLimit;
			double m_accumulatedTime;

			Vector3 m_gravity;

			PhysicsObjectMap m_objects;

			BoundingShapeMap m_bodylessBounds;

			SpaceMap m_spaces;

			bool m_autoUpdated;
			bool m_enabled;

			Real m_defaultAutoDisableTime;
			Real m_defaultAutoDisableLinear;
			Real m_defaultAutoDisableAngular;

			Real m_defaultDampingLinear;
			Real m_defaultDampingAngular;

		public:
			PhysicsSimulation( String const & p_name );
			~PhysicsSimulation();

		private:
			void _step( Real p_deltaTime );
			void _clearContacts();

			void _initialiseDefaultObjects();
			void _initialiseDefaultSpaces();

		public:
			void Update();
			void Render();
			PhysicsObject * CreateObject( Entity * p_entity, bool p_static = false, bool p_phantom = false );
			PhysicsObject * CreateObject( String const & p_name );
			void DestroyObject( String const & p_name );
			void DestroyObject( PhysicsObject * p_object );
			PhysicsObject * CloneObject( PhysicsObject * p_object, String const & p_clonedName, Space * p_newSpace = NULL );

			void SetCFM( Real p_cfm );
			void SetERP( Real p_erp );
			void SetGravity( const Vector3 & p_gravity );

			void ClearObjects( bool p_recreateDefaults = true ) noexcept;
			void ClearSpaces( bool p_recreateDefaults = true ) noexcept;

			Space * CreateSpace( String const & p_name, bool p_autoUpdated );
			bool DestroySpace( String const & p_name );

			void SetDefaultAutoDisable( Real p_time, Real p_linear, Real p_angular );
			void SetDefaultDamping( Real p_linear, Real p_angular );

			DistanceMap Raytrace( const Ray & p_ray, bool p_collideDynamic = true, bool p_collideStatic = true, bool p_collidePhandom = true, Space * p_collidePrecise = NULL );

			PhysicsObject * GetRay( const Ray & p_ray );

		public:
			inline void SetSteptime( Real p_step )
			{
				m_steptime = p_step;
			}
			inline void SetSteptimeLimit( double p_limit )
			{
				m_steptimeLimit = p_limit;
			}
			inline void SetTimescale( Real p_timescale )
			{
				m_timescale = p_timescale;
			}
			inline void SetAutoUpdated( bool p_updated )
			{
				m_autoUpdated = p_updated;
			}
			inline void SetEnabled( bool p_enabled )
			{
				m_enabled = p_enabled;
			}
			inline void SetUpdateScript( ScriptNode * p_script )
			{
				m_scriptPerFrame = p_script;
			}

			inline Real GetCFM()const
			{
				return m_cfm;
			}
			inline Real GetERP()const
			{
				return m_erp;
			}
			inline Real GetFPS()const
			{
				return m_steptime;
			}
			inline Real GetTimescale()const
			{
				return m_timescale;
			}
			inline Real GetSteptime()const
			{
				return m_steptime;
			}
			inline Real GetSteptimeLimit()const
			{
				return Real( m_steptimeLimit );
			}
			inline const Vector3 & GetGravity()const
			{
				return m_gravity;
			}
			inline bool IsAutoUpdated()const
			{
				return m_autoUpdated;
			}
			inline dWorldID GetWorldID()const
			{
				return m_world;
			}
			inline dJointGroupID GetContactGroupID()const
			{
				return m_contacts;
			}
			inline Space * GetDynamicSpace()const
			{
				return m_dynamicSpace;
			}
			inline Space * GetPhantomSpace()const
			{
				return m_phantomSpace;
			}
			inline Space * GetStaticSpace()const
			{
				return m_staticSpace;
			}
			inline bool IsEnabled()const
			{
				return m_enabled;
			}

			inline Real GetDefaultDampingLinear()const
			{
				return m_defaultDampingLinear;
			}
			inline Real GetDefaultDampingAngular()const
			{
				return m_defaultDampingAngular;
			}

			inline PhysicsObject * GetObjectByName( String const & p_name )const
			{
				return General::Utils::map::findOrNull( m_objects, p_name );
			}
			inline Space *	GetSpace( String const & p_name )const
			{
				return General::Utils::map::findOrNull( m_spaces, p_name );
			}
		};
	}
}

#endif
