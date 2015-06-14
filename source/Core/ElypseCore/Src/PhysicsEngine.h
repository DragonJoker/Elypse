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
#ifndef ___PHYSICS_ENGINE_H___
#define ___PHYSICS_ENGINE_H___

#include "Module_Physics.h"

#include <PreciseTimer.h>

#include <Manager.h>
#include <ode/contact.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export PhysicsEngine : d_noncopyable
		{
		private:
			PhysicsSimulationMap m_universes;
			PhysicsMaterialMap m_materials;

		public:
			PhysicsEngine();
			~PhysicsEngine();

		public:
			void ClearAllUniverses() d_no_throw;
			void Update();
			void Render();

			PhysicsSimulation * CreateSimulation( const String & p_name );
			bool DestroySimulation( const String & p_name );

			PhysicsMaterial * CreateMaterial( const String & p_name );
			bool DestroyMaterial( const String & p_name );

			static void CollisionCallback( void * p_data, Ode::dGeomID p_geomA, Ode::dGeomID p_geomB );
			static void RollingFrictionCallback( PhysicsObject * p_objA, PhysicsObject * p_objB, Ode::dContact * p_contacts, unsigned int p_numContacts );

			inline PhysicsSimulation * GetSimulation( const String & p_name ) const
			{
				return General::Utils::map::findOrNull( m_universes, p_name );
			}
			inline PhysicsMaterial * GetMaterial( const String & p_name ) const
			{
				return General::Utils::map::findOrNull( m_materials, p_name );
			}
		};
	}
}

#endif
