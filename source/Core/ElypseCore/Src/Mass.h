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
#ifndef ___MASS_H___
#define ___MASS_H___

#include "Module_Physics.h"

#include <ode/mass.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export Mass
		{
		private:
			Ode::dMass m_mass;

		public:
			Mass();
			Mass( Real p_mass, const Ogre::Vector3 & p_center, const Ogre::Matrix3 & p_inertia );
			Mass( const Mass & p_other );

		public:
			Mass & operator = ( const Mass & p_other );
			Mass & operator = ( Ode::dMass * p_other );

			void Adjust( Real p_mass );
			void Translate( const Ogre::Vector3 & p_offset );
			void Rotate( const Ogre::Quaternion & p_orientation );
			void Add( const Mass & p_other );

			void SetSphereMass( Real p_mass, Real p_radius );
			void SetSphereDensity( Real p_density, Real p_radius );

			void SetTrimesh( Real p_density, BoundingMesh * p_mesh );

			void SetBox( Real p_density, const Ogre::Vector3 & p_extends );

			Ogre::Matrix3 GetLocalInertiaTensor() const;

		public:
			inline const Ode::dMass * GetMassID()const
			{
				return & m_mass;
			}
		};
	}
}

#endif
