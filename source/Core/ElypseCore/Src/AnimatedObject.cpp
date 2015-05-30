#include "PrecompiledHeader.h"

#include "AnimatedObject.h"
#include "Animation.h"

#include <OgreEntity.h>
#include <OgreAnimationState.h>
#include <OgreSkeletonInstance.h>

#include <SetAlgo.h>

#include "EMuseLogs.h"

using namespace General::Utils;
using namespace Ogre;

AnimatedObject::AnimatedObject( AnimatedObjectGroup * p_group, Entity * p_entity )
	:	owned_by <AnimatedObjectGroup>	( p_group ),
		 m_entity( p_entity )
{
	genlib_assert( m_entity != NULL );
	_createAnimationMap();

	if ( m_entity->hasSkeleton() )
	{
		m_entity->getSkeleton()->setBlendMode( ANIMBLEND_AVERAGE );
//		m_entity->getSkeleton()->setBlendMode( ANIMBLEND_CUMULATIVE);
	}
}

AnimatedObject::~AnimatedObject()
{
	General::Utils::map::deleteAll( m_animations );
}

void AnimatedObject::_addAnimation( AnimationState * p_state )
{
	genlib_assert( p_state != NULL );
	General::Utils::map::insert( m_animations, p_state->getAnimationName(), this, p_state );
}

void AnimatedObject::Update( Real p_timeStep )
{
	genlib_assert( p_timeStep > 0.0 );
	General::Utils::map::cycle( m_animations, &EMAnimation::Update, p_timeStep );
}

void AnimatedObject::_createAnimationMap()
{
	if ( ! m_entity )
	{
		return;
	}

	AnimationStateSet * l_set = m_entity->getAllAnimationStates();

	if ( ! l_set )
	{
		return;
	}

	AnimationStateIterator l_iter = l_set->getAnimationStateIterator();

	while ( l_iter.hasMoreElements() )
	{
		AnimationState * l_animState = l_iter.getNext();
		_addAnimation( l_animState );
	}
}

RealMap AnimatedObject::GetAllAnimationsTimes()const
{
	RealMap l_map;
	AnimationMap::const_iterator i = m_animations.begin();
	const AnimationMap::const_iterator & iend = m_animations.end();

	for ( ; i != iend ; ++ i )
	{
		l_map.insert( l_map.end(), RealMap::value_type( i->first, i->second->GetLength() ) );
	}

	return l_map;
}

void AnimatedObject::StopAllAnimations()
{
	General::Utils::map::cycle( m_animations, & EMAnimation::Stop );
}

void AnimatedObject::SetPauseAllAnimations( bool p_pause )
{
	General::Utils::map::cycle( m_animations, & EMAnimation::SetPause, p_pause );
}

unsigned int AnimatedObject::GetNumPlayingAnimations()
{
	ConstEnabledAnimationStateIterator l_it = m_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
	AnimationState * l_state;
	unsigned int l_count = 0;

	while ( l_it.hasMoreElements() )
	{
		l_state = l_it.getNext();
		l_count++;
	}

	return l_count;
}

const String & AnimatedObject::GetPlayingAnimationName( unsigned int p_index )
{
	ConstEnabledAnimationStateIterator l_it = m_entity->getAllAnimationStates()->getEnabledAnimationStateIterator();
	AnimationState * l_state;
	unsigned int l_count = 0;

	while ( l_it.hasMoreElements() && l_count < p_index )
	{
		l_state = l_it.getNext();
		l_count++;
	}

	if ( l_it.hasMoreElements() )
	{
		l_state = l_it.getNext();
		return l_state->getAnimationName();
	}

	return EMPTY_STRING;
}
