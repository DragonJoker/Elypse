/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "BoundingShape.h"
#include "PhysicsEngine.h"
#include "Space.h"
#include "PhysicsSimulation.h"
#include "PhysicsMaterial.h"

#include <ode/objects.h>

BoundingShape::BoundingShape( BoundingShapeClass p_class )
	: m_class( p_class )
	, m_enabled( true )
	, m_maxContacts( 0 )
	, m_geom( nullptr )
	, m_space( nullptr )
	, m_parentObject( nullptr )
	, m_contacts( nullptr )
{
	SetMaxContacts( 12 );
}

BoundingShape::~BoundingShape()
{
	if ( m_geom != nullptr )
	{
		dGeomDestroy( m_geom );
	}

	delete [] m_contacts;
}

void BoundingShape::_register()
{
	dGeomSetData( m_geom, this );

	if ( m_space != nullptr )
	{
		dSpaceAdd( m_space->GetSpaceID(), m_geom );
	}
}

void BoundingShape::SetParent( PhysicsObject * p_parent )
{
	genlib_assert( p_parent != nullptr );
	m_parentObject = p_parent;
	dGeomSetBody( m_geom, p_parent->GetBodyId() );
}

void BoundingShape::SetMaxContacts( uint32_t p_maxContacts )
{
	if ( m_contacts != nullptr )
	{
		delete [] m_contacts;
	}

	m_contacts = new dContact[p_maxContacts];
	m_maxContacts = p_maxContacts;
}

const AxisAlignedBox &	BoundingShape::GetBoundingBox()
{
	dReal aabb[6];
	dGeomGetAABB( m_geom, aabb );
	m_box.setExtents( aabb[0], aabb[2], aabb[4], aabb[1], aabb[3], aabb[5] );
	return m_box;
}

void BoundingShape::SetPosition( const Vector3 & p_position )
{
	dGeomSetOffsetPosition( m_geom, p_position.x, p_position.y, p_position.z );
}

void BoundingShape::SetOrientation( const Quaternion & p_orientation )
{
	dReal data[4]
	{
		p_orientation.w,
		p_orientation.x,
		p_orientation.y,
		p_orientation.z,
	};
	dGeomSetOffsetRotation( m_geom, data );
}

void BoundingShape::SetSpace( Space * p_space )
{
	if ( p_space == m_space )
	{
		return;
	}

	if ( m_geom == nullptr )
	{
		m_space = p_space;
		return;
	}

	if ( m_space != nullptr )
	{
		dSpaceRemove( m_space->GetSpaceID(), m_geom );
	}

	m_space = p_space;

	if ( m_space != nullptr )
	{
		dSpaceAdd( m_space->GetSpaceID(), m_geom );
	}
}

bool BoundingShape::Collide( BoundingShape * p_collideWith )
{
	genlib_assert( p_collideWith != nullptr );
	int l_numContacts = dCollide( m_geom, p_collideWith->m_geom, m_maxContacts, & m_contacts[0].geom, sizeof( dContact ) );

	if ( l_numContacts <= 0 )
	{
		return false;
	}

	dWorldID l_world = nullptr;
	dJointGroupID l_group = nullptr;

	if ( m_space )
	{
		PhysicsSimulation * l_sim = m_space->GetOwner();
		l_world = l_sim->GetWorldID();
		l_group = l_sim->GetContactGroupID();
	}

	//If one of the shapes is pass-through( no body), the test pass, but there is no need to continue
	if ( m_parentObject == nullptr || p_collideWith->m_parentObject == nullptr )
	{
		return true;
	}

	dBodyID l_bodyA = m_parentObject->GetBodyId();
	dBodyID l_bodyB = p_collideWith->m_parentObject->GetBodyId();
	Vector3 l_v3 = Vector3::ZERO;

	//Static bodies need no contact points. Ode still needs to push the dynamic object far away, so we continue anyways
	if ( m_parentObject->IsStatic() ||  ! m_parentObject->IsEnabled() )
	{
		l_bodyA = nullptr;
	}

	if ( p_collideWith->m_parentObject->IsStatic() ||  ! p_collideWith->m_parentObject->IsEnabled() )
	{
		l_bodyB = nullptr;
	}

	PhysicsMaterial * l_matA = m_parentObject->GetMaterial();
	PhysicsMaterial * l_matB = p_collideWith->m_parentObject->GetMaterial();

	//TODO : temporal coherence
	//This logs contacts between 2 objects for collision lookup later.

	for ( int i = 0 ; i < l_numContacts ; i ++ )
	{
		m_parentObject->AddContact( p_collideWith->m_parentObject , &m_contacts[i] );
		p_collideWith->m_parentObject->AddContact( m_parentObject , &m_contacts[i] );
		//Sets the various contact properties : friction, bounce, etc.

		//Tells ode to add the contact joint in the world.
		if ( l_bodyA != nullptr || l_bodyB != nullptr )
		{
			l_matA->SetContactProperties( l_matB, &m_contacts[i] );
			dJointAttach( dJointCreateContact( l_world, l_group, & m_contacts[i] ), l_bodyA, l_bodyB );
		}

		l_v3 += m_contacts[0].geom.depth * Vector3( m_contacts[0].geom.normal );
	}

	if ( l_bodyA != nullptr || l_bodyB != nullptr )
	{
		PhysicsEngine::RollingFrictionCallback( m_parentObject, p_collideWith->m_parentObject, m_contacts, l_numContacts );
	}

	l_v3 /= Real( l_numContacts );
	m_parentObject->SetMoveOut( l_v3 );
	p_collideWith->m_parentObject->SetMoveOut( -l_v3 );
	return true;
}
