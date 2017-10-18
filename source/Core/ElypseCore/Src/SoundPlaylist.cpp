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

#include "SoundPlaylist.h"
#include "SoundObject.h"
#include "SoundInstance.h"

#include "ElypseLogs.h"

#include <OgreLog.h>
#include <OgreMath.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>

#include <random>

SoundPlaylist::SoundPlaylist( String const & p_name, Real p_SFXVolume, Real p_musicVolume, String const & p_zoneName )
	: m_name( p_name )
	, m_zoneName( p_zoneName )
	, m_timerType( TT_Fixed )
	, m_random( false )
	, m_looped( false )
	, m_waiting( false )
	, m_nbUse( 0 )
	, m_currentPlaying( -1 )
	, m_type( EM_NONE )
	, m_timerTime( 0.0 )
	, m_timerMin( 0.0 )
	, m_timerMax( 0.0 )
	, m_musicVolume( minmax <Real> ( 0, p_musicVolume, 1 ) )
	, m_SFXVolume( minmax <Real> ( 0, p_SFXVolume, 1 ) )
	, m_volumePercent( 1.0 )
{
	EMUSE_MESSAGE_DEBUG( "SoundObject::SoundPlaylist - " + p_name );
}

SoundPlaylist::~SoundPlaylist()
{
	m_objectList.clear();
	m_positions.clear();
}

void SoundPlaylist::AddSoundObject( SoundObject * p_soundObject )
{
	if ( p_soundObject == NULL || General::Utils::map::has( m_objectList, p_soundObject->GetName() ) )
	{
		return;
	}

	_logMessage( "SoundPlaylist::AddSoundObject - " + p_soundObject->GetName() + " to playlist " + m_name );
	m_objectList.insert( std::make_pair( p_soundObject->GetName(), p_soundObject ) );
	m_positions.push_back( p_soundObject );

	if ( m_type == EM_NONE )
	{
		m_type = p_soundObject->GetSoundType();
	}

	if ( m_type == EM_SFX )
	{
		m_volume = m_SFXVolume;
		return;
	}

	m_volume = m_musicVolume;
}

void SoundPlaylist::SetNode( SceneNode * p_node, String const & p_zone )
{
	SoundObjectMap::iterator l_it = m_objectList.begin();
	const SoundObjectMap::iterator & l_end = m_objectList.end();

	for ( ; l_it != l_end ; ++ l_it )
	{
		SoundInstance * l_instance = l_it->second->CreateInstance( p_node );
		l_instance->SetInPlaylist( true );
	}

	m_refName = p_node->getName();
	m_nbUse ++;
}

void SoundPlaylist::_randomise()
{
	EMSoundsPosition l_positions;
	std::random_device l_device;

	while ( !m_positions.empty() )
	{
		auto l_rand = std::uniform_int_distribution< size_t >( 0, m_positions.size() - 1 )( l_device );
		l_positions.push_back( m_positions[l_rand] );
		m_positions.erase( m_positions.begin() + l_rand );
	}

	m_positions = l_positions;
}

void SoundPlaylist::Play()
{
	_logMessage( "SoundPlaylist::Play - " + m_name );

	if ( m_random )
	{
		_randomise();
	}

	m_currentPlaying = 0;
	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_instance = l_sound->GetSoundInstance( m_refName );

	if ( l_instance != NULL )
	{
		l_instance->Play();
		return;
	}

	m_currentPlaying = -1;
}

void SoundPlaylist::Pause()
{
	if ( m_currentPlaying == -1 )
	{
		return;
	}

	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_instance = l_sound->GetSoundInstance( m_refName );

	if ( l_instance != NULL )
	{
		l_instance->Pause();
	}
}

void SoundPlaylist::Stop()
{
	if ( m_currentPlaying == -1 )
	{
		return;
	}

	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_instance = l_sound->GetSoundInstance( m_refName );

	if ( l_instance != NULL )
	{
		l_instance->Stop();
		return;
	}

	m_currentPlaying = -1;
}

void SoundPlaylist::Next()
{
	m_currentPlaying ++;

	if ( m_currentPlaying == static_cast< int >( m_positions.size() ) )
	{
		if ( m_looped )
		{
			Play();
		}

		return;
	}

	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_instance = l_sound->GetSoundInstance( m_refName );

	if ( l_instance != NULL )
	{
		l_instance->Play();
		return;
	}

	m_currentPlaying = -1;
}

void SoundPlaylist::ReleaseAllZone( String const & p_zoneName )
{
	_logMessage( "SoundPlaylist::ReleaseAllZone - Zone : " + p_zoneName + " Playlist Name : " + m_name );

	if ( m_zoneName != p_zoneName )
	{
		return;
	}

//	_logMessage("SoundPlaylist::ReleaseAllZone - My Zone");
	m_objectList.clear();

	if ( m_nbUse > 0 )
	{
		m_nbUse --;
	}
}

void SoundPlaylist::Update( Real p_tslf )
{
	_updateTime( p_tslf );
	_update3D( p_tslf );
}

void SoundPlaylist::_updateTime( Real p_tslf )
{
	if ( m_currentPlaying == -1 )
	{
		return;
	}

	/*
		Real l_time = p_tslf;

		if (p_tslf >= 1.0)
		{
			l_time = 0.25;
		}
	*/
	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_sInstance = l_sound->GetSoundInstance( m_refName );

	if ( l_sInstance == NULL )
	{
		return;
	}

	l_sInstance->SetMaxVolume( m_volume );
	l_sInstance->SetVolumePercent( m_volumePercent );

	if ( m_waiting )
	{
		m_currentTime += p_tslf;
		//m_log->logMessage("SoundPlaylist::UpdateTime - " + m_name + " - m_currentTime : "+StringConverter::toString(m_currentTime));

		if ( ( m_timerType == TT_Fixed && ( m_timerTime <= 0.0 || m_currentTime >= m_timerTime ) )
				|| ( m_timerType == TT_Random && ( m_randomTimerTime <= 0.0 || m_currentTime >= m_randomTimerTime ) ) )
		{
			m_waiting = false;
			Next();
		}

		return;
	}

	l_sInstance->UpdateFade( p_tslf, l_sound->IsMuted() );

	if ( l_sInstance->IsPlaying() )
	{
		return;
	}

//	l_it1->second->Stop( m_refName);
	m_waiting = true;
	m_currentTime = 0;

	if ( m_timerType == TT_Random )
	{
		m_randomTimerTime = Ogre::Math::RangeRandom( m_timerMin, m_timerMax );
	}
}

void SoundPlaylist::_update3D( Real p_tslf )
{
	if ( m_currentPlaying == -1 )
	{
		return;
	}

	SoundObject * l_sound = m_positions[m_currentPlaying];
	SoundInstance * l_sInstance = l_sound->GetSoundInstance( m_refName );

	if ( l_sInstance == NULL || ! l_sInstance->IsPlaying() )
	{
		return;
	}

	l_sInstance->Update3D( p_tslf );
}

void SoundPlaylist::_logMessage( String const & p_msg )
{
	EMUSE_MESSAGE_DEBUG( p_msg );
}
