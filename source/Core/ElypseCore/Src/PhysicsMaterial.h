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
#ifndef ___PHYSICS_MATERIAL_H___
#define ___PHYSICS_MATERIAL_H___

#include "Module_Physics.h"

#include <ode/contact.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export PhysicsMaterial : public named, noncopyable
		{
		public:
			Real m_friction;
			Real m_addFriction;
			Real m_bouncyness;
			Real m_velocityThreshold;
			Real m_erp;
			Real m_cfm;
			Real m_independentVelocity;
			Real m_addVelocity;
			Real m_forceDependentSlip;
			Real m_addForceDependentSlip;
			Real m_rollFrictionCoeff1;
			Real m_rollFrictionDamping;

		public:
			PhysicsMaterial( String const & p_name );
			~PhysicsMaterial();

		public:
			void SetContactProperties( PhysicsMaterial * p_otherMaterial, Ode::dContact * p_contactPoint );

		public:
			inline void SetFriction( Real p_friction )
			{
				m_friction = p_friction;
			}
			inline void SetAddFriction( Real p_addFriction )
			{
				m_addFriction = p_addFriction;
			}
			inline void SetBouncyness( Real p_bouncyness )
			{
				m_bouncyness = p_bouncyness;
			}
			inline void SetVelocityThreshold( Real p_velocityThreshold )
			{
				m_velocityThreshold = p_velocityThreshold;
			}
			inline void SetErp( Real p_erp )
			{
				m_erp = p_erp;
			}
			inline void SetCfm( Real p_cfm )
			{
				m_cfm = p_cfm;
			}
			inline void SetIndependantVelocity( Real p_independentVelocity )
			{
				m_independentVelocity = p_independentVelocity;
			}
			inline void SetAddVelocity( Real p_addVelocity )
			{
				m_addVelocity = p_addVelocity;
			}
			inline void SetForceDependantSlip( Real p_forceDependentSlip )
			{
				m_forceDependentSlip = p_forceDependentSlip;
			}
			inline void SetAddForceDependantSlip( Real p_addForceDependentSlip )
			{
				m_addForceDependentSlip = p_addForceDependentSlip;
			}

			inline void SetRollFrictionCoeff( Real p_factor )
			{
				m_rollFrictionCoeff1 = p_factor;
			}
			inline void SetRollFrictionDampen( Real p_factor )
			{
				m_rollFrictionDamping = p_factor;
			}
		};
	}
}

#endif
