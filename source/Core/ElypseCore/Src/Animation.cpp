#include "PrecompiledHeader.h"

#include "Animation.h"

#include <OgreAnimationState.h>

#include "EMuseLogs.h"

EMAnimation::EMAnimation( AnimatedObject * p_owner, AnimationState * p_animation )
	:	owned_by<AnimatedObject>	( p_owner ),
		   named( p_animation->getAnimationName() ),
		   m_animation( p_animation ),
		   m_nextAnimation( NULL ),
		   m_status( AS_STOPPED ),
		   m_time( 0.0 ),
		   m_length( 0.0 ),
		   m_timescale( 1.0 ),
		   m_maxTimeOut( 0.0 ),
		   m_maxTimeIn( 0.0 ),
		   m_timeOut( 0.0 ),
		   m_weight( 0.0 ),
		   m_reversed( false ),
		   m_looped( false ),
		   m_useTimeOut( false ),
		   m_useTimeIn( false )
{
	genlib_assert( m_animation );
	m_animation->setEnabled( false );
	m_animation->setLoop( false );
	m_animation->setTimePosition( 0.0 );
	m_animation->setWeight( 1.0 );
	m_length = m_animation->getLength();
}

EMAnimation::~EMAnimation()
{
}

void EMAnimation::_stop( bool p_continueChain )
{
	m_time = 0.0;
	m_weight = 0.0;
	m_timeOut = 0.0;
	m_status = AS_STOPPED;
	m_animation->setEnabled( false );
	m_animation->setWeight( m_weight );
	_setTime( m_time );

	if ( p_continueChain && m_nextAnimation != NULL )
	{
		m_nextAnimation->Play();
	}
}

void EMAnimation::Update( Real p_delta )
{
	p_delta *= m_timescale;

	if ( m_status == AS_PLAYING )
	{
		EMUSE_CONSOLE_MESSAGE_DEBUG( "Animation playing : " + m_animation->getAnimationName() + " / time : " + StringConverter::toString( m_time ) );

		if ( m_weight < 1.0 )
		{
			if ( p_delta > 0.0 )
			{
				m_weight += ( p_delta / m_maxTimeIn );
			}
			else
			{
				m_weight -= ( p_delta / m_maxTimeOut );
			}

			if ( m_weight > 1.0 )
			{
				m_weight = 1.0;
			}

			m_animation->setWeight( m_weight );
		}

		m_time += p_delta;

		if ( m_time > m_length )
		{
			if ( m_looped )
			{
				m_time = 0.0;
				_setTime( m_time );
			}
			else
			{
				_stop( true );
			}
		}
		else
		{
			if ( m_time < 0.0 && p_delta < 0.0 )
			{
				if ( m_looped )
				{
					m_time = m_length;
					_setTime( m_time );
				}
				else
				{
					_stop( true );
				}
			}
			else
			{
				_setTime( m_time );
//				m_animation->addTime( p_delta);
			}
		}
	}
	else if ( m_status == AS_TIMEOUT )
	{
		if ( p_delta > 0.0 )
		{
			m_timeOut += p_delta;
		}
		else
		{
			m_timeOut -= p_delta;
		}

		if ( m_timeOut >= m_maxTimeOut )
		{
			_stop( true );
		}
		else
		{
			m_weight =  1 - ( m_timeOut / m_maxTimeOut );
			m_animation->setWeight( m_weight );
		}
	}
	else if ( m_status == AS_PAUSED && m_useTimeIn )
	{
		if ( m_weight < 1.0 )
		{
			if ( p_delta > 0.0 )
			{
				m_weight += ( p_delta / m_maxTimeIn );
			}
			else
			{
				m_weight -= ( p_delta / m_maxTimeIn );
			}

			m_animation->setWeight( m_weight );
		}
	}
}

void EMAnimation::Stop()
{
	if ( m_status == AS_PLAYING || m_status == AS_PAUSED )
	{
		if ( m_useTimeOut )
		{
			m_status = AS_TIMEOUT;
			m_timeOut = m_maxTimeOut * ( 1 - m_weight );
		}
		else
		{
			_stop( false );
		}
	}
	else if ( m_status == AS_TIMEOUT )
	{
		_stop( false );
	}
}

void EMAnimation::Pause()
{
	if ( m_status == AS_PLAYING )
	{
		m_status = AS_PAUSED;
	}
}

void EMAnimation::SetPause( bool p_paused )
{
	if ( p_paused && m_status == AS_PLAYING )
	{
		m_status = AS_PAUSED;
	}
	else if ( m_status == AS_PAUSED && ! p_paused )
	{
		m_status = AS_PLAYING;
	}
}

void EMAnimation::Play()
{
	if ( m_status == AS_STOPPED )
	{
		m_time = 0.0;
		m_timeOut = 0.0;
		m_animation->setEnabled( true );
		_setTime( m_time );

		if ( m_useTimeIn )
		{
			m_weight = 0.0;
			m_animation->setWeight( 0.0 );
		}
		else
		{
			m_weight = 1.0;
			m_animation->setWeight( 1.0 );
		}
	}
	else if ( m_status == AS_TIMEOUT )
	{
		m_timeOut = 0.0;
	}

	m_status = AS_PLAYING;
}

void EMAnimation::_setTime( Real p_time )
{
	if ( m_reversed )
	{
		m_animation->setTimePosition( m_length - p_time );
	}
	else
	{
		m_animation->setTimePosition( p_time );
	}
}

void EMAnimation::SetTime( Real p_time )
{
	m_time = p_time;
	m_animation->setTimePosition( m_time );
}

void EMAnimation::SetLooped( bool p_looped )
{
	m_looped = p_looped;
	m_animation->setLoop( p_looped );
}

void EMAnimation::SetWeight( Real p_weight )
{
	m_weight = p_weight;
	m_animation->setWeight( p_weight );
}
