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

#include "PhysicsSimulation.h"
#include "Space.h"
#include "PhysicsObject.h"
#include "BoundingShape.h"
#include "SimpleBoundingShapes.h"
#include "PhysicsMaterial.h"

#include "ScriptNode.h"

#include <OgreEntity.h>

#include <ode/objects.h>
#include <ode/collision.h>

namespace
{
	const uint32_t c_fastStepIterationCount = 40;
}

PhysicsSimulation::PhysicsSimulation( String const & p_name )
	: named( p_name )
	, m_dynamicSpace( NULL )
	, m_phantomSpace( NULL )
	, m_staticSpace( NULL )
	, m_scriptPerFrame( NULL )
	, m_mode( Step_Quick )
	, m_erp( Real( 1.0 ) )
	, m_cfm( Real( 0.0 ) )
	, m_steptime( Real( 0.01 ) )
	, m_timescale( Real( 1.0 ) )
	, m_steptimeLimit( 0.05 )
	, m_accumulatedTime( 0.0 )
	, m_autoUpdated( false )
	, m_enabled( true )
{
	m_defaultMaterial = new PhysicsMaterial( "Default" );
	m_world = dWorldCreate();
	dWorldSetAutoDisableFlag( m_world, 0 );
	SetDefaultAutoDisable( Real( 10.0 ), Real( 0.01 ), Real( 0.01 ) );
	SetGravity( Vector3( Real( 0.0 ), Real( -98.1 ), Real( 0.0 ) ) );
	m_contacts = dJointGroupCreate( 0 );
	_initialiseDefaultSpaces();
	_initialiseDefaultObjects();
}

PhysicsSimulation::~PhysicsSimulation()
{
	ClearObjects( false );
	ClearSpaces( false );
	delete m_defaultMaterial;
	dJointGroupDestroy( m_contacts );
	dWorldDestroy( m_world );
}

void PhysicsSimulation::_initialiseDefaultObjects()
{
	m_rayHolder = CreateObject( "RayHolder" );
	m_phantomSpace->AddObject( m_rayHolder );
	m_rayInstance = new BoundingRay();
	m_rayHolder->AddBound( m_rayInstance );
}

void PhysicsSimulation::_initialiseDefaultSpaces()
{
	m_dynamicSpace = CreateSpace( "default_dynamic", true );
	m_phantomSpace = CreateSpace( "default_phantom", false );
	m_staticSpace = CreateSpace( "default_static", false );
	m_dynamicSpace->SetInternalCollisions( true );
	m_phantomSpace->SetInternalCollisions( false );
	m_staticSpace->SetInternalCollisions( false );
}

void PhysicsSimulation::ClearObjects( bool p_recreateDefaults ) noexcept
{
	_clearContacts();

	General::Utils::map::deleteAll( m_objects );
	General::Utils::map::deleteAll( m_bodylessBounds );
	General::Utils::map::cycle( m_spaces, & Space::Clear );

	m_rayHolder = NULL;
	m_rayInstance = NULL;

	if ( p_recreateDefaults )
	{
		_initialiseDefaultObjects();
	}
}

void PhysicsSimulation::ClearSpaces( bool p_recreateDefaults ) noexcept
{
	General::Utils::map::deleteAll( m_spaces );

	if ( p_recreateDefaults )
	{
		_initialiseDefaultSpaces();
	}
}

void PhysicsSimulation::SetDefaultDamping( Real p_linear, Real p_angular )
{
	m_defaultDampingLinear = p_linear;
	m_defaultDampingAngular = p_angular;
}

void PhysicsSimulation::SetDefaultAutoDisable( Real p_time, Real p_linear, Real p_angular )
{
	m_defaultAutoDisableTime = p_time;
	m_defaultAutoDisableLinear = p_linear;
	m_defaultAutoDisableAngular = p_angular;
	dWorldSetAutoDisableTime( m_world, p_time );
	dWorldSetAutoDisableLinearThreshold( m_world, p_time );
	dWorldSetAutoDisableAngularThreshold( m_world, p_time );
}

void PhysicsSimulation::SetCFM( Real p_cfm )
{
	m_cfm = p_cfm;
	dWorldSetCFM( m_world, p_cfm );
}

void PhysicsSimulation::SetERP( Real p_erp )
{
	m_erp = p_erp;
	dWorldSetERP( m_world, p_erp );
}

void PhysicsSimulation::SetGravity( const Vector3 & p_gravity )
{
	dWorldSetGravity( m_world, p_gravity.x, p_gravity.y, p_gravity.z );
	m_gravity = p_gravity;
}

PhysicsObject * PhysicsSimulation::CreateObject( Entity * p_entity, bool p_static, bool p_phantom )
{
	genlib_assert( p_entity != NULL );
	const PhysicsObjectMap::iterator & ifind = m_objects.find( p_entity->getName() );

	if ( ifind != m_objects.end() )
	{
		return ifind->second;
	}

	Space * l_space = ( p_static ? ( p_phantom ? m_phantomSpace : m_staticSpace ) : m_dynamicSpace );
	PhysicsObject * l_object = new PhysicsObject( *this, p_entity, l_space );

	if ( p_static )
	{
		l_object->SetStatic( true );
	}

	if ( p_phantom )
	{
		l_object->SetPhantom( true );
	}

	m_objects.insert( std::make_pair( p_entity->getName(), l_object ) );
	l_object->SetMaterial( m_defaultMaterial );
	return l_object;
}

PhysicsObject * PhysicsSimulation::CreateObject( String const & p_name )
{
	PhysicsObject * l_object = General::Utils::map::insert( m_objects, p_name, *this, p_name, m_staticSpace );
	l_object->SetStatic( true );
	l_object->SetMaterial( m_defaultMaterial );
	return l_object;
}

void PhysicsSimulation::DestroyObject( String const & p_name )
{
	General::Utils::map::deleteValue( m_objects, p_name );
}

void PhysicsSimulation::DestroyObject( PhysicsObject * p_object )
{
	DestroyObject( p_object->GetName() );
}

void PhysicsSimulation::Update()
{
	m_accumulatedTime += m_timer.Time() * m_timescale;
	clamp( 0.0, m_accumulatedTime, m_steptimeLimit );

	while ( m_accumulatedTime >= m_steptime )
	{
		m_accumulatedTime -= m_steptime;
		_step( m_steptime );
	}

	Render();
}

void PhysicsSimulation::_clearContacts()
{
	dJointGroupEmpty( m_contacts );
}

void PhysicsSimulation::_step( Real p_deltaTime )
{
	_clearContacts();
	General::Utils::map::cycle( m_objects, & PhysicsObject::ClearContacts );
	m_dynamicSpace->CollideInternal();
	m_staticSpace->CollideSpace( m_dynamicSpace );
	General::Utils::map::cycle( m_spaces, & Space::ApplyDamping );

	switch ( m_mode )
	{
	case Step_Basic:
		dWorldStep( m_world, p_deltaTime );
		break;

	case Step_Quick:
		dWorldQuickStep( m_world, p_deltaTime );
		break;
//		case Step_Fast1: dWorldStepFast1( m_world, p_deltaTime, c_fastStepIterationCount);break;
	}

	General::Utils::map::cycle( m_spaces, & Space::UpdateState );

	if ( m_scriptPerFrame )
	{
		m_scriptPerFrame->Execute();
	}
}

void PhysicsSimulation::Render()
{
	Real l_factor = Real( m_accumulatedTime / m_steptime );
	General::Utils::map::cycle( m_spaces, & Space::Render, l_factor );
}

Space * PhysicsSimulation::CreateSpace( String const & p_name, bool p_autoUpdated )
{
	const SpaceMap::iterator & ifind = m_spaces.find( p_name );

	if ( ifind != m_spaces.end() )
	{
		return ifind->second;
	}

	Space * l_space = new Space( p_name, *this, p_autoUpdated );
	m_spaces.insert( std::make_pair( p_name, l_space ) );
	return l_space;
}

bool PhysicsSimulation::DestroySpace( String const & p_name )
{
	return General::Utils::map::deleteValue( m_spaces, p_name );
}

PhysicsObject * PhysicsSimulation::GetRay( const Ray & p_ray )
{
	m_rayInstance->SetRay( p_ray );
	return m_rayHolder;
}

DistanceMap PhysicsSimulation::Raytrace( const Ray & p_ray, bool p_collideDynamic, bool p_collideStatic, bool p_collidePhandom, Space * p_collidePrecise )
{
	PhysicsObjectArray l_raytraceArray;
	DistanceMap l_tempMap;
	m_rayHolder->ClearContacts();
	m_rayInstance->SetRay( p_ray );

	if ( p_collideDynamic )
	{
		m_dynamicSpace->CollideShape( m_rayInstance );
	}

	if ( p_collideStatic )
	{
		m_staticSpace->CollideShape( m_rayInstance );
	}

	if ( p_collidePhandom )
	{
		m_phantomSpace->CollideShape( m_rayInstance );
	}

	if ( p_collidePrecise )
	{
		p_collidePrecise->CollideShape( m_rayInstance );
	}

	const ContactMap & l_contacts = m_rayHolder->GetAllContacts();
	const ContactMap::const_iterator & iend = l_contacts.end();
	ContactMap::const_iterator i = l_contacts.begin();

	for ( ; i != iend ; ++ i )
	{
		Real l_min = Math::POS_INFINITY;

		for ( uint32_t j = 0 ; j < i->second.size() ; j ++ )
		{
			l_min = min( l_min, i->second[j].distance( m_rayInstance->GetOrigin() ) );
		}

		l_tempMap.insert( std::make_pair( l_min, i->first ) );
	}

	return l_tempMap;
}

PhysicsObject * PhysicsSimulation::CloneObject( PhysicsObject * p_object, String const & p_clonedName, Space * p_newSpace )
{
	if ( p_newSpace == NULL )
	{
		return NULL;
	}

	PhysicsObject * l_object = p_object->Clone( p_clonedName, p_newSpace );

	if ( l_object != NULL )
	{
		p_newSpace->AddObject( l_object );
		m_objects.insert( std::make_pair( p_clonedName, l_object ) );
	}

	return l_object;
}
