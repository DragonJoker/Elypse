/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Space.h"
#include "BoundingShape.h"
#include "PhysicsEngine.h"
#include "PhysicsObject.h"

#include <ode/collision_space.h>
#include <ode/collision.h>

Space::Space( String const & p_name, PhysicsSimulation & p_simulation, bool p_autoUpdated )
	: named( p_name )
	, owned_by< PhysicsSimulation >( p_simulation )
	, m_autoUpdated( p_autoUpdated )
	, m_internalCollisions( true )
{
	m_space = dSimpleSpaceCreate( 0 );
	dGeomSetData( reinterpret_cast <dGeomID>( m_space ), this );
}

Space::~Space()
{
	Clear();
	dSpaceDestroy( m_space );
}

void Space::Clear() noexcept
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
	m_box.setExtents( Real( aabb[0] ), Real( aabb[2] ),
					  Real( aabb[4] ), Real( aabb[1] ),
					  Real( aabb[3] ), Real( aabb[5] ) );
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
