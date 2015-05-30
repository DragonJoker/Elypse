#include "PrecompiledHeader.h"

#include "SoundInstance.h"
#include "SoundObject.h"

#include "EMuseLogs.h"

#include <OgreSceneNode.h>
#include <OgreVector3.h>

SoundInstance::SoundInstance( SoundObject * p_parent )
	:	owned_by <SoundObject>	( p_parent ),
		 m_node( NULL ),
		 m_status( EM_STATUS_INIT ),
		 m_looped( false ),
		 m_fadeIn( false ),
		 m_fadeOut( false ),
		 m_isInPlaylist( false ),
		 m_mute( false ),
		 m_fadeInTime( 0.0 ),
		 m_fadeOutTime( 0.0 ),
		 m_currentTime( 0.0 ),
		 m_timerTime( 0.0 ),
		 m_volumePercent( 1.0 ),
		 m_maxVolume( MAXVOLUME ),
		 m_volume( MAXVOLUME )
{
//	_logMessage( "SoundObject - Media loaded - " + m_soundName);
	unsigned int l_duration = 0;

	if ( ! CHECKFMODERROR( SoundManager::GetSingletonPtr()->GetFMODSystem()->playSound( FMOD_CHANNEL_FREE, m_owner->GetFMODSound(), true, & m_channel ) ) )
	{
		m_mediaTime = 0.0;
		m_status = EM_STATUS_NONE;
		EMUSE_MESSAGE_RELEASE( "Sound " + m_owner->GetName() + ", could not create the instance" );
		return;
	}

	CHECKFMODERROR( m_owner->GetFMODSound()->getLength( & l_duration, FMOD_TIMEUNIT_MS ) );
	m_mediaTime = static_cast <Real>( l_duration );
	m_status = EM_STATUS_READY;
	EMUSE_MESSAGE_DEBUG( "SoundInstance create : " + m_owner->GetName() + ", timed : " + StringConverter::toString( m_mediaTime ) );
}

SoundInstance::~SoundInstance()
{
	if ( m_status == EM_STATUS_PLAY || m_status == EM_STATUS_PAUSE )
	{
		CHECKFMODERROR( m_channel->stop() );
	}
}

void SoundInstance::_setVolume()
{
	m_volume = m_maxVolume * m_volumePercent;
}

void SoundInstance::SetNode( SceneNode * p_node )
{
	m_node = p_node;
	SetLastPosition();
	_set3D();
}

void SoundInstance::_set3D()
{
//	CHECKFMODERROR( m_owner->GetFMODSound()->setMode( FMOD_3D));
	CHECKFMODERROR( m_channel->setMode( FMOD_3D ) );
	FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
	CHECKFMODERROR( m_channel->set3DAttributes( & m_lastPosition, & l_velocity ) );
}

void SoundInstance::SetVelocity( const Vector3 & p_velocity )
{
	FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
	l_velocity.x = p_velocity.x;
	l_velocity.y = p_velocity.y;
	l_velocity.z = p_velocity.z;
	CHECKFMODERROR( m_channel->set3DAttributes( & m_lastPosition, & l_velocity ) );
}

void SoundInstance::SetLastPosition()
{
	if ( m_node == NULL )
	{
		return;
	}

	const Vector3 & l_lastPos = m_node->_getDerivedPosition();
	m_lastPosition.x = l_lastPos.x / 100;
	m_lastPosition.y = l_lastPos.y / 100;
	m_lastPosition.z = l_lastPos.z / 100;
}

void SoundInstance::Update3D( Real p_time )
{
	if ( ! Is3D() )
	{
		return;
	}

	FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
	Vector3 l_posSound = m_node->_getDerivedPosition();
	l_posSound.x /= 100;
	l_posSound.y /= 100;
	l_posSound.z /= 100;
	l_velocity.x = ( l_posSound.x - m_lastPosition.x ) / p_time;
	l_velocity.y = ( l_posSound.y - m_lastPosition.y ) / p_time;
	l_velocity.z = ( l_posSound.z - m_lastPosition.z ) / p_time;
	m_lastPosition.x = l_posSound.x;
	m_lastPosition.y = l_posSound.y;
	m_lastPosition.z = l_posSound.z;
	CHECKFMODERROR( m_channel->set3DAttributes( & m_lastPosition, & l_velocity ) );
}

void SoundInstance::Mute( bool p_mute )
{
	m_mute = p_mute;
	CHECKFMODERROR( m_channel->setMute( p_mute ) );
}

void SoundInstance::UpdateFade( Real p_time, bool p_muted )
{
	if ( m_status != EM_STATUS_PLAY
			&& m_status != EM_STATUS_PAUSE
			&& m_status != EM_STATUS_LOOP )
	{
		return;
	}

	m_currentTime += p_time;

	if ( m_status != EM_STATUS_LOOP && m_currentTime < m_mediaTime )
	{
		unsigned int l_time = 0;

		if ( ! CHECKFMODERROR( m_channel->getPosition( & l_time, FMOD_TIMEUNIT_MS ) ) )
		{
			m_status = EM_STATUS_NONE;
			return;
		}

		m_currentTime = static_cast <Real>( l_time );

		if ( l_time == 0 && ! m_looped )
		{
			m_status = EM_STATUS_STOP;
			return;
		}
	}

	if ( m_currentTime >= m_mediaTime )
	{
		if ( ! m_looped )
		{
			if ( CHECKFMODERROR( m_channel->stop() ) )
			{
				m_status = EM_STATUS_STOP;
			}

			return;
		}

		if ( m_timerTime > 0.0 )
		{
			m_status = EM_STATUS_LOOP;

			if ( m_currentTime < m_mediaTime + m_timerTime )
			{
				return;
			}

			if ( ! CHECKFMODERROR( SoundManager::GetSingletonPtr()->GetFMODSystem()->playSound( FMOD_CHANNEL_REUSE, m_owner->GetFMODSound(), false, & m_channel ) ) )
			{
				m_status = EM_STATUS_NONE;
				return;
			}

			CHECKFMODERROR( m_channel->setMute( m_mute ) );

			if ( Is3D() )
			{
				_set3D();
			}

			m_status = EM_STATUS_PLAY;
			m_currentTime = 0;
		}
	}

	if ( m_currentTime <= m_fadeInTime )
	{
		if ( m_fadeInTime == 0 )
		{
			CHECKFMODERROR( m_channel->setVolume( m_volume ) );
			return;
		}

		CHECKFMODERROR( m_channel->setVolume( m_volume * m_currentTime / m_fadeInTime ) );
		return;
	}

	if ( m_currentTime >= m_mediaTime - m_fadeOutTime )
	{
		if ( m_fadeOutTime == 0 )
		{
			return;
		}

		CHECKFMODERROR( m_channel->setVolume( m_volume * ( m_mediaTime - m_currentTime ) / m_fadeOutTime ) );
		return;
	}

	CHECKFMODERROR( m_channel->setVolume( m_volume ) );
}

void SoundInstance::Pause()
{
	if ( m_status != EM_STATUS_PLAY )
	{
		return;
	}

	if ( CHECKFMODERROR( m_channel->setPaused( true ) ) )
	{
		m_status = EM_STATUS_PAUSE;
	}
}

void SoundInstance::Stop()
{
	if ( m_status != EM_STATUS_PLAY && m_status != EM_STATUS_PAUSE )
	{
		return;
	}

	if ( CHECKFMODERROR( m_channel->setPaused( true ) ) )
	{
		CHECKFMODERROR( m_channel->setPosition( 0, FMOD_TIMEUNIT_MS ) );
		m_status = EM_STATUS_STOP;
		m_currentTime = 0;
	}
}

void SoundInstance::Play()
{
	if ( m_status < EM_STATUS_READY )
	{
		return;
	}

	if ( m_looped )
	{
		if ( ! CHECKFMODERROR( m_channel->setPaused( false ) ) )
		{
			SoundManager::GetSingletonPtr()->LogMessage( String( "SoundObject::Play - can't play " ) + m_owner->GetName() );
			return;
		}
	}
	else
	{
		if ( ! CHECKFMODERROR( SoundManager::GetSingletonPtr()->GetFMODSystem()->playSound( FMOD_CHANNEL_FREE, m_owner->GetFMODSound(), false, & m_channel ) ) )
		{
			SoundManager::GetSingletonPtr()->LogMessage( String( "SoundObject::Play - can't play " ) + m_owner->GetName() );
			return;
		}

		CHECKFMODERROR( m_channel->setVolume( m_volume ) );
		m_currentTime = 0.0;

		if ( Is3D() )
		{
			_set3D();
			SetLastPosition();
			FMOD_VECTOR l_velocity = { 0.0f, 0.0f, 0.0f };
			CHECKFMODERROR( m_channel->set3DAttributes( & m_lastPosition, & l_velocity ) );
		}
	}

	m_status = EM_STATUS_PLAY;
}

void SoundInstance::SetLooped( bool p_looped )
{
	m_looped = p_looped;

	if ( p_looped && m_timerTime == 0 )
	{
		CHECKFMODERROR( m_channel->setMode( FMOD_LOOP_NORMAL ) );
	}
	else
	{
		CHECKFMODERROR( m_channel->setMode( FMOD_LOOP_OFF ) );
	}
}
