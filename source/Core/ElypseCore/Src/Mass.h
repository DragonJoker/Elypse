/*
See LICENSE file in root folder
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
