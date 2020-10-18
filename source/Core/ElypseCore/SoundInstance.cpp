/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SoundInstance.h"
#include "SoundObject.h"

#include "ElypseLogs.h"

#include <OgreSceneNode.h>
#include <OgreVector.h>

SoundInstance::SoundInstance( SoundObject & p_parent )
	: owned_by< SoundObject >( p_parent )
	, m_node( nullptr )
	, m_status( EM_STATUS_INIT )
	, m_looped( false )
	, m_fadeIn( false )
	, m_fadeOut( false )
	, m_isInPlaylist( false )
	, m_mute( false )
	, m_fadeInTime( 0.0 )
	, m_fadeOutTime( 0.0 )
	, m_currentTime( 0.0 )
	, m_timerTime( 0.0 )
	, m_volumePercent( 1.0 )
	, m_maxVolume( MAXVOLUME )
	, m_volume( MAXVOLUME )
	, m_channel( SoundManager::GetSingletonPtr()->CreateChannel( *GetOwner() ) )
{
	//_logMessage( "SoundObject - Media loaded - " + m_soundName);
	if ( !m_channel )
	{
		m_mediaTime = 0.0;
		m_status = EM_STATUS_NONE;
		return;
	}
	
	uint32_t duration = GetOwner()->GetLength();
	m_mediaTime = Real( duration );
	m_status = EM_STATUS_READY;
	EMUSE_MESSAGE_DEBUG( "SoundInstance create : " + GetOwner()->GetName() + ", timed : " + StringConverter::toString( m_mediaTime ) );
}

SoundInstance::~SoundInstance()
{
	if ( m_status == EM_STATUS_PLAY || m_status == EM_STATUS_PAUSE )
	{
		m_channel->Stop();
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
	m_channel->Enable3D();
}

void SoundInstance::SetVelocity( const Vector3 & p_velocity )
{
	m_channel->SetVelocity( p_velocity );
}

void SoundInstance::SetLastPosition()
{
	if ( m_node == nullptr )
	{
		return;
	}

	const Vector3 & lastPos = m_node->_getDerivedPosition();
	m_channel->SetPosition( lastPos / 100 );
}

void SoundInstance::Update3D( Real time )
{
	if ( ! Is3D() )
	{
		return;
	}

	Vector3 posSound = m_node->_getDerivedPosition();
	posSound.x /= 100;
	posSound.y /= 100;
	posSound.z /= 100;
	m_channel->Update3D( posSound / 100, time );
}

void SoundInstance::Mute( bool mute )
{
	m_mute = mute;
	m_channel->Mute( mute );
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
		uint32_t time = 0;

		if ( !m_channel->GetTimePosition( time ) )
		{
			m_status = EM_STATUS_NONE;
			return;
		}

		m_currentTime = Real( time );

		if ( time == 0 && ! m_looped )
		{
			m_status = EM_STATUS_STOP;
			return;
		}
	}

	if ( m_currentTime >= m_mediaTime )
	{
		if ( ! m_looped )
		{
			if ( m_channel->Stop() )
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

			if ( !m_channel->Restart() )
			{
				m_status = EM_STATUS_NONE;
				return;
			}

			m_channel->Mute( m_mute );

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
			m_channel->SetVolume( m_volume );
			return;
		}

		m_channel->SetVolume( m_volume * m_currentTime / m_fadeInTime );
		return;
	}

	if ( m_currentTime >= m_mediaTime - m_fadeOutTime )
	{
		if ( m_fadeOutTime == 0 )
		{
			return;
		}

		m_channel->SetVolume( m_volume * ( m_mediaTime - m_currentTime ) / m_fadeOutTime );
		return;
	}

	m_channel->SetVolume( m_volume );
}

void SoundInstance::Pause()
{
	if ( m_status != EM_STATUS_PLAY )
	{
		return;
	}

	if ( m_channel->Pause( true ) )
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

	if ( m_channel->Pause( true ) )
	{
		m_channel->SetTimePosition( 0 );
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
		if ( !m_channel->Pause( false ) )
		{
			SoundManager::GetSingletonPtr()->LogMessage( String( "SoundObject::Play - can't play " ) + GetOwner()->GetName() );
			return;
		}
	}
	else
	{
		if ( !m_channel->Restart() )
		{
			SoundManager::GetSingletonPtr()->LogMessage( String( "SoundObject::Play - can't play " ) + GetOwner()->GetName() );
			return;
		}

		m_channel->SetVolume( m_volume );
		m_currentTime = 0.0;

		if ( Is3D() )
		{
			SetLastPosition();
			_set3D();
		}
	}

	m_status = EM_STATUS_PLAY;
}

void SoundInstance::SetLooped( bool p_looped )
{
	m_looped = p_looped;
	m_channel->Loop( m_looped && m_timerTime == 0 );
}
