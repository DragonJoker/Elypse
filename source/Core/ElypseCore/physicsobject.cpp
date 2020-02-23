/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "PhysicsObject.h"
#include "Space.h"
#include "BoundingMesh.h"
#include "PhysicsSimulation.h"
#include "Mass.h"

#include <OgreEntity.h>
#include <OgreSceneNode.h>

#include <ode/objects.h>

PhysicsObject::PhysicsObject( PhysicsSimulation & p_parent, Entity * p_entity, Space * p_space )
	: named( p_entity->getName() )
	, owned_by< PhysicsSimulation >( p_parent )
	, m_entity( p_entity )
	, m_node( p_entity->getParentSceneNode() )
	, m_space( p_space )
	, m_enabled( true )
{
	genlib_assert( m_entity != nullptr );

	if ( m_node != nullptr )
	{
		m_node->_update( true, true );
	}

	m_entity->getUserObjectBindings().setUserAny( Ogre::Any( this ) );
	m_body = dBodyCreate( p_space->GetOwner()->GetWorldID() );
	dBodySetAutoDisableDefaults( m_body );
	m_mass = new Mass;
	dBodySetMass( m_body, m_mass->GetMassID() );
	SetLinearDamping( p_space->GetOwner()->GetDefaultDampingLinear() );
	SetAngularDamping( p_space->GetOwner()->GetDefaultDampingAngular() );
	ReassertSpatialState();
	InterpolateSpacialState( 0.0 );

	if ( p_space != nullptr )
	{
		p_space->AddObject( this );
	}

	SetEnabled( true );
}

PhysicsObject::PhysicsObject( PhysicsSimulation & p_parent, String const & p_name, Space * p_space )
	: named( p_name )
	, owned_by< PhysicsSimulation >( p_parent )
	, m_space( p_space )
	, m_static( true )
	, m_phantom( true )
{
	m_body = dBodyCreate( p_space->GetOwner()->GetWorldID() );
	dBodyDisable( m_body );
	dBodySetAutoDisableDefaults( m_body );
	SetAutoDisable( Real( 0.01 ), Real( 1.0 ), Real( 1.0 ) );
	SetLinearDamping( Real( 1.0 ) );
	SetAngularDamping( Real( 1.0 ) );
	SetEnabled( false );
}

PhysicsObject::~PhysicsObject()
{
	if ( m_space != nullptr )
	{
		m_space->RemoveObject( this );
	}

	General::Utils::vector::deleteAll( m_bounds );
	delete m_mass;
	dBodyDestroy( m_body );
}

void PhysicsObject::ApplyMoveOut()
{
	m_linearVelocity -= m_linearVelocity * m_linearVelocity.normalisedCopy().dotProduct( m_outTranslate.normalisedCopy() );
	SetPosition( GetActualPosition() + m_outTranslate * 1.00 );
	m_outTranslate = Vector3::ZERO;
}

void PhysicsObject::SetNode( SceneNode * p_node )
{
	m_node = p_node;
	m_node->_update( false, false );
	ReassertSpatialState();
	InterpolateSpacialState( 0.0 );
}

void PhysicsObject::UpdateNode()
{
	if ( m_node != nullptr )
	{
		if ( m_node->getParentSceneNode() != nullptr )
		{
			m_node->setPosition( m_drawState.position - m_node->getParentSceneNode()->_getDerivedPosition() );
			Quaternion q = m_drawState.orientation * m_node->getParentSceneNode()->_getDerivedOrientation().Inverse();
			q.normalise();
			m_node->setOrientation( q );
		}
		else
		{
			m_node->setPosition( m_drawState.position );
			m_drawState.orientation.normalise();
			m_node->setOrientation( m_drawState.orientation );
		}
	}
}

Vector3 PhysicsObject::GetActualPosition()
{
	return Vector3( dBodyGetPosition( m_body ) );
}

void PhysicsObject::UpdateState()
{
	if ( m_static || m_phantom )
	{
		//TODO : hack, bad idea, temporal coherence needed;
		ReassertSpatialState();
	}
	else
	{
		m_linearVelocity = Vector3( dBodyGetLinearVel( m_body ) );

		if ( m_applyMoveOut )
		{
			ApplyMoveOut();
		}

		const dReal * l_position = dBodyGetPosition( m_body );
		m_currentState.position.x = l_position[0];
		m_currentState.position.y = l_position[1];
		m_currentState.position.z = l_position[2];
		const dReal * l_orientation = dBodyGetQuaternion( m_body );
		m_currentState.orientation.w = l_orientation[0];
		m_currentState.orientation.x = l_orientation[1];
		m_currentState.orientation.y = l_orientation[2];
		m_currentState.orientation.z = l_orientation[3];
		UpdateNode();
	}
}

void PhysicsObject::SetEnabled( bool p_enabled )
{
	m_enabled = p_enabled;

	if ( ! p_enabled )
	{
		dBodyDisable( m_body );
	}
	else
	{
		dBodyEnable( m_body );
	}
}

void PhysicsObject::SetAutoDisable( Real p_time, Real p_angular, Real p_linear )
{
	m_disableThresold_time = p_time;
	m_disableThresold_angular = p_angular;
	m_disableThresold_linear = p_linear;
	dBodySetAutoDisableFlag( m_body, 1 );
	dBodySetAutoDisableLinearThreshold( m_body, p_linear );
	dBodySetAutoDisableAngularThreshold( m_body, p_angular );
	dBodySetAutoDisableTime( m_body, p_time );
}

void PhysicsObject::SetMass( Real p_mass )
{
	m_mass->SetSphereMass( p_mass, 1.0 );
}

Vector3 PhysicsObject::GetSpeed()
{
	const dReal * l_speed = dBodyGetLinearVel( m_body );
	return Vector3( l_speed[0], l_speed[1], l_speed[2] );
}

void PhysicsObject::SetSpeed( const Vector3 & p_speed )
{
	m_enabled = true;
	dBodySetLinearVel( m_body, p_speed.x, p_speed.y, p_speed.z );
}

Vector3 PhysicsObject::GetAngularSpeed()
{
	const dReal * l_speed = dBodyGetAngularVel( m_body );
	return Vector3( l_speed[0], l_speed[1], l_speed[2] );
}

void PhysicsObject::SetAngularSpeed( const Vector3 & p_speed )
{
	dBodySetAngularVel( m_body, p_speed.x, p_speed.y, p_speed.z );
}

void PhysicsObject::AddForce( const Vector3 & p_force )
{
	dBodyAddForce( m_body, p_force.x, p_force.y, p_force.z );
}

void PhysicsObject::AddTorque( const Vector3 & p_torque )
{
	dBodyAddTorque( m_body, p_torque.x, p_torque.y, p_torque.z );
}

void PhysicsObject::AddRelativeForce( const Vector3 & p_force )
{
	dBodyAddRelForce( m_body, p_force.x, p_force.y, p_force.z );
}

void PhysicsObject::AddRelativeTorque( const Vector3 & p_torque )
{
	dBodyAddRelTorque( m_body, p_torque.x, p_torque.y, p_torque.z );
}

void PhysicsObject::AddForceAt( const Vector3 & p_force, const Vector3 & p_position )
{
	dBodyAddForceAtPos( m_body, p_force.x, p_force.y, p_force.z, p_position.x, p_position.y, p_position.z );
}

void PhysicsObject::AddForceAtRelative( const Vector3 & p_force, const Vector3 & p_position )
{
	dBodyAddForceAtRelPos( m_body, p_force.x, p_force.y, p_force.z, p_position.x, p_position.y, p_position.z );
}

void PhysicsObject::AddRelativeForceAt( const Vector3 & p_force, const Vector3 & p_position )
{
	dBodyAddRelForceAtPos( m_body, p_force.x, p_force.y, p_force.z, p_position.x, p_position.y, p_position.z );
}

void PhysicsObject::AddRelativeForceAtRelative( const Vector3 & p_force, const Vector3 & p_position )
{
	dBodyAddRelForceAtRelPos( m_body, p_force.x, p_force.y, p_force.z, p_position.x, p_position.y, p_position.z );
}

const Vector3 & PhysicsObject::GetForce()
{
	const dReal * l_force = dBodyGetForce( m_body );
	m_forces.x = l_force[0];
	m_forces.y = l_force[1];
	m_forces.z = l_force[2];
	return m_forces;
}

const Vector3 & PhysicsObject::GetTorque()
{
	const dReal * l_torque = dBodyGetTorque( m_body );
	m_torque.x = l_torque[0];
	m_torque.y = l_torque[1];
	m_torque.z = l_torque[2];
	return m_torque;
}

void PhysicsObject::SetForce( const Vector3 & p_force )
{
	dBodySetForce( m_body, p_force.x, p_force.y, p_force.z );
}

void PhysicsObject::SetTorque( const Vector3 & p_torque )
{
	dBodySetTorque( m_body, p_torque.x, p_torque.y, p_torque.z );
}

Vector3 PhysicsObject::GetLocalVelocity( const Vector3 & p_position )
{
	dVector3 l_vel;
	dBodyGetPointVel( m_body, p_position.x, p_position.y, p_position.z, l_vel );
	return Vector3( l_vel[0], l_vel[1], l_vel[2] );
}

void PhysicsObject::AddBound( BoundingShape * p_shape )
{
	genlib_assert( p_shape != nullptr );
	m_bounds.push_back( p_shape );
	p_shape->SetParent( this );
	p_shape->SetSpace( m_space );
}

void PhysicsObject::SetFloating( bool p_floating )
{
	dBodySetGravityMode( m_body, p_floating ? 0 : 1 );
}

bool PhysicsObject::GetFloating()
{
	return dBodyGetGravityMode( m_body ) != 0;
}

void PhysicsObject::SetLinearDamping( Real p_linearDamping )
{
	m_linearDamping = -p_linearDamping;
	m_isLinearDamped = m_linearDamping < 0.0;
	m_isDamped = m_isLinearDamped || m_isAngularDamped;
}

void PhysicsObject::SetAngularDamping( Real p_angularDamping )
{
	m_angularDamping = -p_angularDamping;
	m_isAngularDamped = m_angularDamping < 0.0;
	m_isDamped = m_isLinearDamped || m_isAngularDamped;
}

Real PhysicsObject::GetLinearDamping()
{
	return -m_linearDamping;
}

Real PhysicsObject::GetAngularDamping()
{
	return -m_angularDamping;
}

void PhysicsObject::ApplyDamping()
{
	if ( m_isLinearDamped )
	{
		const dReal * v = dBodyGetLinearVel( m_body );
		dBodyAddForce( m_body, v[0] * m_linearDamping, v[1] * m_linearDamping, v[2] * m_linearDamping );
	}

	if ( m_isAngularDamped )
	{
		const dReal * v = dBodyGetAngularVel( m_body );
		dBodyAddTorque( m_body, v[0] * m_angularDamping, v[1] * m_angularDamping, v[2] * m_angularDamping );
	}
}

void PhysicsObject::InterpolateSpacialState( Real p_factor )
{
	m_previousState = m_currentState;
	m_drawState.position = m_currentState.position * p_factor + m_previousState.position * ( 1.0f - p_factor );
	m_drawState.orientation = Quaternion::Slerp( p_factor, m_previousState.orientation, m_currentState.orientation, true );
}

void PhysicsObject::ReassertSpatialState()
{
	if ( m_node != nullptr )
	{
		SetPosition( m_node->_getDerivedPosition() );
		SetOrientation( m_node->_getDerivedOrientation() );
	}
}

void PhysicsObject::SetPosition( const Vector3 & p_position )
{
	if ( m_drawState.position == p_position )
	{
		return;
	}

	dBodySetPosition( m_body, p_position.x, p_position.y, p_position.z );
	m_currentState.position = p_position;
	m_previousState.position = p_position;
	m_drawState.position = p_position;

	if ( m_node != nullptr )
	{
		m_node->setPosition( p_position );
	}
}

void PhysicsObject::SetOrientation( const Quaternion & p_orientation )
{
	if ( m_drawState.orientation == p_orientation )
	{
		return;
	}

	dQuaternion q;
	q[0] = p_orientation.w;
	q[1] = p_orientation.x;
	q[2] = p_orientation.y;
	q[3] = p_orientation.z;
	dBodySetQuaternion( m_body, q );
	m_currentState.orientation = p_orientation;
	m_previousState.orientation = p_orientation;
	m_drawState.orientation = p_orientation;

	if ( m_node != nullptr )
	{
		m_node->setOrientation( p_orientation );
	}
}

void PhysicsObject::SetSpace( Space * p_space )
{
	m_space = p_space;
	General::Utils::vector::cycle( m_bounds, &BoundingShape::SetSpace, p_space );
}

void PhysicsObject::ClearContacts()
{
	m_currentContacts.clear();
}

void PhysicsObject::AddContact( PhysicsObject * p_object, dContact * p_contact )
{
	genlib_assert( p_contact != nullptr );

	if ( p_object != this )
	{
		m_currentContacts[p_object].push_back( Vector3( p_contact->geom.pos ) );
	}
}

bool PhysicsObject::CollisionTest_Fast( PhysicsObject * p_collideWith )
{
	return m_currentContacts.find( p_collideWith ) != m_currentContacts.end();
}

int PhysicsObject::NumCollisionTest_Fast( PhysicsObject * p_collideWith )
{
	ContactMap::iterator l_it = m_currentContacts.find( p_collideWith );

	if ( l_it != m_currentContacts.end() )
	{
		return static_cast< int >( l_it->second.size() );
	}

	return 0;
}

bool PhysicsObject::CollisionTest_Complete( PhysicsObject * p_collideWith )
{
	genlib_assert( p_collideWith != nullptr );
	size_t imax = m_bounds.size();
	size_t jmax = p_collideWith->m_bounds.size();

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		for ( size_t j = 0 ; j < jmax ; j ++ )
		{
			if ( m_bounds[i]->Collide( p_collideWith->m_bounds[j] ) )
			{
				return true;
			}
		}
	}

	return false;
}

int PhysicsObject::NumCollisionTest_Complete( PhysicsObject * p_collideWith )
{
	genlib_assert( p_collideWith != nullptr );
	size_t imax = m_bounds.size();
	size_t jmax = p_collideWith->m_bounds.size();
	int l_total = 0;

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		for ( size_t j = 0 ; j < jmax ; j ++ )
		{
			if ( m_bounds[i]->Collide( p_collideWith->m_bounds[j] ) )
			{
				l_total += 1;
			}
		}
	}

	return l_total;
}


PhysicsObject * PhysicsObject::Clone( String const & p_name, Space * p_space )
{
	genlib_assert( ! p_name.empty() );

	if ( p_space == nullptr )
	{
		p_space = m_space;
	}

	PhysicsObject * l_object = new PhysicsObject( *GetOwner(), p_name, p_space );
	l_object->SetPosition( GetPosition() );
	l_object->SetOrientation( GetOrientation() );
	size_t imax = m_bounds.size();

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		l_object->AddBound( m_bounds[i]->Clone() );
	}

	return l_object;
}

void PhysicsObject::SetMoveOut( const Vector3 & p_translate )
{
	m_outTranslate += p_translate;
	m_outTranslate /= 2.0;
}
