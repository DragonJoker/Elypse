#include "PrecompiledHeader.h"

#include "AnimatedObjectGroup.h"
#include "AnimatedObject.h"
#include "Animation.h"
#include "AnimationManager.h"

#include <OgreEntity.h>

using namespace General::Theory;
using namespace General::Utils;

AnimatedObjectGroup::AnimatedObjectGroup( const String & p_name, AnimationManager * p_owner )
	:	owned_by <AnimationManager> ( p_owner ),
		named( p_name )
{
}

AnimatedObjectGroup::~AnimatedObjectGroup()
{
	General::Utils::map::deleteAll( m_objects );
}

void AnimatedObjectGroup::Update( Real p_delta )
{
	General::Utils::map::cycle( m_objects, & AnimatedObject::Update, p_delta );
}

AnimatedObject * AnimatedObjectGroup::AddEntity( Entity * p_entity )
{
	genlib_assert( p_entity != NULL );
	AnimatedObject * l_object = General::Utils::map::insert( m_objects, p_entity->getName(), this, p_entity );
	const RealMap & l_map = l_object->GetAllAnimationsTimes();

	if ( m_objects.size() == 1 )
	{
		m_commonAnimationsTime = l_map;
	}
	else
	{
		m_commonAnimationsTime = General::Utils::map::intersects( l_map, m_commonAnimationsTime );
	}

	return l_object;
}

void AnimatedObjectGroup::StartAnimation( const String & p_animationName )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->Play();
		}
	}
}

void AnimatedObjectGroup::StopAnimation( const String & p_animationName )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->Stop();
		}
	}
}

void AnimatedObjectGroup::PauseAnimation( const String & p_animationName )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->Pause();
		}
	}
}

void AnimatedObjectGroup::SetTimeAnimation( const String & p_animationName, Real p_time )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetTime( p_time );
		}
	}
}

void AnimatedObjectGroup::SetTimeScaleAnimation( const String & p_animationName, Real p_timescale )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetTimescale( p_timescale );
		}
	}
}

void AnimatedObjectGroup::RemoveObject( const String & p_objectName )
{
	General::Utils::map::deleteValue( m_objects, p_objectName );
}

void AnimatedObjectGroup::SetWeightAnimation( const String & p_animationName, Real p_weight )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetWeight( p_weight );
		}
	}
}

void AnimatedObjectGroup::SetAnimationTimeOut( const String & p_animationName, Real p_timeOut )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetTimeOut( p_timeOut );
		}
	}
}

void AnimatedObjectGroup::SetAnimationTimeIn( const String & p_animationName, Real p_timeIn )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetTimeIn( p_timeIn );
		}
	}
}

void AnimatedObjectGroup::SetAnimationLooped( const String & p_animationName, bool p_looped )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetLooped( p_looped );
		}
	}
}

void AnimatedObjectGroup::SetAnimationReversed( const String & p_animationName, bool p_reversed )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animationName );

		if ( l_anim != NULL )
		{
			l_anim->SetReversed( p_reversed );
		}
	}
}

Real AnimatedObjectGroup::GetAnimationTime( const String & p_animationName )const
{
	return General::Utils::map::findOrNull( m_commonAnimationsTime, p_animationName );
}

void AnimatedObjectGroup::ChainAnimation( const String & p_animName, const String & p_chainedAnimationName )
{
	AnimatedObjectMap::iterator i = m_objects.begin();
	const AnimatedObjectMap::iterator & iend = m_objects.end();

	for ( ; i != iend ; ++ i )
	{
		EMAnimation * l_anim = i->second->GetAnimation( p_animName );

		if ( l_anim )
		{
			l_anim->SetNextAnimation( i->second->GetAnimation( p_chainedAnimationName ) );
		}
	}
}

void AnimatedObjectGroup::Clear()
{
	m_objects.clear();
	m_commonAnimationsTime.clear();
}

void AnimatedObjectGroup::StopAllAnimations()
{
	General::Utils::map::cycle( m_objects, & AnimatedObject::StopAllAnimations );
}

void AnimatedObjectGroup::SetPauseAll( bool p_pause )
{
	General::Utils::map::cycle( m_objects, & AnimatedObject::SetPauseAllAnimations, p_pause );
}

void AnimatedObjectGroup::ChainAllAnimations( bool p_looped )
{
	if ( m_commonAnimationsTime.size() < 2 )
	{
		if ( m_commonAnimationsTime.empty() )
		{
			return;
		}
		else
		{
			SetAnimationLooped( m_commonAnimationsTime.begin()->first, p_looped );
		}
	}

	RealMap::iterator l_it = m_commonAnimationsTime.begin();
	const RealMap::const_iterator & l_end = m_commonAnimationsTime.end();
	String l_animName;
	l_animName = l_it->first;
	++ l_it;

	while ( l_it != l_end )
	{
		ChainAnimation( l_animName, l_it->first );
		l_animName = l_it->first;
		++ l_it;
	}

	if ( p_looped )
	{
		ChainAnimation( l_animName, m_commonAnimationsTime.begin()->first );
	}
}
