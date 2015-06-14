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

#include "Space.h"
#include "BoundingShape.h"
#include "PhysicsEngine.h"
#include "PhysicsObject.h"

#include <ode/collision_space.h>
#include <ode/collision.h>

Space::Space( const String & p_name, PhysicsSimulation * p_simulation, bool p_autoUpdated )
	: named( p_name ),
		owned_by<PhysicsSimulation>	( p_simulation ),
		m_autoUpdated( p_autoUpdated ),
		m_internalCollisions( true )
{
	genlib_assert( m_owner != NULL );
	m_space = dSimpleSpaceCreate( 0 );
	dGeomSetData( reinterpret_cast <dGeomID>( m_space ), this );
}

Space::~Space()
{
	Clear();
	dSpaceDestroy( m_space );
}

void Space::Clear() d_no_throw
{
	dSpaceClean( m_space );
	m_objects.clear();
}

void Space::AddObject( PhysicsObject * p_object )
{
	genlib_assert( p_object != NULL );
	Space * l_space = p_object->GetSpace();

	if ( l_space != NULL && l_space != this )
	{
		l_space->RemoveObject( p_object );
	}

	p_object->SetSpace( this );
	m_objects.insert( p_object );
}

void Space::RemoveObject( PhysicsObject * p_object )
{
	genlib_assert( p_object != NULL );
	p_object->SetSpace( NULL );
	General::Utils::set::eraseValue( m_objects, p_object );
}

const AxisAlignedBox & Space::GetBoundingBox()
{
	dReal aabb[6];
	dGeomGetAABB( GetSpaceAsGeomID(), aabb );
	m_box.setExtents( static_cast <Real>( aabb[0] ), static_cast <Real>( aabb[2] ),
						static_cast <Real>( aabb[4] ), static_cast <Real>( aabb[1] ),
						static_cast <Real>( aabb[3] ), static_cast <Real>( aabb[5] ) );
	return m_box;
}

void Space::CollideInternal()
{
	if ( m_internalCollisions )
	{
		dSpaceCollide( m_space, NULL, PhysicsEngine::CollisionCallback );
	}
}

void Space::CollideSpace( Space * p_collidesWith )
{
	genlib_assert( p_collidesWith != NULL );
	dSpaceCollide2( reinterpret_cast <dGeomID>( m_space ), reinterpret_cast <dGeomID>( p_collidesWith->m_space ), NULL, PhysicsEngine::CollisionCallback );
}

void Space::CollideShape( BoundingShape * p_collidesWith )
{
	genlib_assert( p_collidesWith != NULL );
	dSpaceCollide2( reinterpret_cast <dGeomID>( m_space ), p_collidesWith->GetGeomID(), NULL, PhysicsEngine::CollisionCallback );
}

void Space::CollideObject( PhysicsObject * p_collidesWith )
{
	genlib_assert( p_collidesWith != NULL );
	size_t imax = p_collidesWith->GetNumBounds();

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		dSpaceCollide2( reinterpret_cast <dGeomID>( m_space ), p_collidesWith->GetBoundIndexed( i )->GetGeomID(), NULL, PhysicsEngine::CollisionCallback );
	}
}

void Space::Render( Real p_factor )
{
	if ( m_autoUpdated )
	{
		General::Utils::set::cycle( m_objects, & PhysicsObject::InterpolateSpacialState, p_factor );
	}
}

void Space::UpdateState()
{
	if ( m_autoUpdated )
	{
		General::Utils::set::cycle( m_objects, & PhysicsObject::UpdateState );
	}
}

void Space::ApplyDamping()
{
	if ( m_autoUpdated )
	{
		General::Utils::set::cycle( m_objects, & PhysicsObject::ApplyDamping );
	}
}
