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
#include "PrecompiledHeader.h"

#include "PhysicsMaterial.h"


PhysicsMaterial::PhysicsMaterial( const String & p_name )
	: named( p_name ),
		m_friction( 0.0 ),
		m_addFriction( 0.0 ),
		m_bouncyness( 0.0 ),
		m_velocityThreshold( 0.0 ),
		m_erp( 0.0 ),
		m_cfm( 0.0 ),
		m_independentVelocity( 0.0 ),
		m_addVelocity( 0.0 ),
		m_forceDependentSlip( 0.0 ),
		m_addForceDependentSlip( 0.0 ),
		m_rollFrictionCoeff1( 0.0 ),
		m_rollFrictionDamping( 0.0 )
{
}

PhysicsMaterial::~PhysicsMaterial()
{
}

void PhysicsMaterial::SetContactProperties( PhysicsMaterial * p_otherMaterial, dContact * p_contactPoint )
{
	if ( this == NULL || p_otherMaterial == NULL || p_contactPoint == NULL )
	{
		return;
	}

	p_contactPoint->surface.mode = dContactApprox1;

	if ( m_friction > 0.0 || p_otherMaterial->m_friction > 0.0 )
	{
		p_contactPoint->surface.mu = m_friction + p_otherMaterial->m_friction - ( p_otherMaterial->m_friction * m_friction );
	}

	if ( m_addFriction > 0.0 || p_otherMaterial->m_addFriction > 0.0 )
	{
		p_contactPoint->surface.mu2 = m_addFriction + p_otherMaterial->m_addFriction - ( p_otherMaterial->m_addFriction * m_addFriction );
		p_contactPoint->surface.mode |= dContactMu2;
	}

	if ( m_bouncyness > 0.0 || p_otherMaterial->m_bouncyness > 0.0 )
	{
		p_contactPoint->surface.bounce = m_bouncyness + p_otherMaterial->m_bouncyness - ( p_otherMaterial->m_bouncyness * m_bouncyness );
		p_contactPoint->surface.mode |= dContactBounce;
	}

	if ( m_velocityThreshold > 0.0 || p_otherMaterial->m_velocityThreshold > 0.0 )
	{
		p_contactPoint->surface.bounce_vel = m_velocityThreshold + p_otherMaterial->m_velocityThreshold - ( p_otherMaterial->m_velocityThreshold * m_velocityThreshold );
		p_contactPoint->surface.mode |= dContactBounce;
	}

	if ( m_erp > 0.0 || p_otherMaterial->m_erp > 0.0 )
	{
		p_contactPoint->surface.soft_erp = m_erp + p_otherMaterial->m_erp - ( p_otherMaterial->m_erp * m_erp );
		p_contactPoint->surface.mode |= dContactSoftERP;
	}

	if ( m_cfm > 0.0 || p_otherMaterial->m_cfm > 0.0 )
	{
		p_contactPoint->surface.soft_cfm = m_cfm + p_otherMaterial->m_cfm - ( p_otherMaterial->m_cfm * m_cfm );
		p_contactPoint->surface.mode |= dContactSoftCFM;
	}

	if ( m_independentVelocity > 0.0 || p_otherMaterial->m_independentVelocity > 0.0 )
	{
		p_contactPoint->surface.motion1 = m_independentVelocity + p_otherMaterial->m_independentVelocity - ( p_otherMaterial->m_independentVelocity * m_independentVelocity );
		p_contactPoint->surface.mode |= dContactMotion1;
	}

	if ( m_addVelocity > 0.0 || p_otherMaterial->m_addVelocity > 0.0 )
	{
		p_contactPoint->surface.motion2 = m_addVelocity + p_otherMaterial->m_addVelocity - ( p_otherMaterial->m_addVelocity * m_addVelocity );
		p_contactPoint->surface.mode |= dContactMotion2;
	}

	if ( m_forceDependentSlip > 0.0 || p_otherMaterial->m_forceDependentSlip > 0.0 )
	{
		p_contactPoint->surface.slip1 = m_forceDependentSlip + p_otherMaterial->m_forceDependentSlip - ( p_otherMaterial->m_forceDependentSlip * m_forceDependentSlip );
		p_contactPoint->surface.mode |= dContactSlip1;
	}

	if ( m_addVelocity > 0.0 || p_otherMaterial->m_addForceDependentSlip > 0.0 )
	{
		p_contactPoint->surface.slip2 = m_addForceDependentSlip + p_otherMaterial->m_addForceDependentSlip - ( p_otherMaterial->m_addForceDependentSlip * m_addForceDependentSlip );
		p_contactPoint->surface.mode |= dContactSlip2;
	}
}
