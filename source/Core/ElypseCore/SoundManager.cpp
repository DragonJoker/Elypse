/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "SoundManager.h"
#include "SoundObject.h"
#include "SoundPlaylist.h"
#include "SoundInstance.h"

#include "ElypseLogs.h"

#include <OgreEntity.h>
#include <OgreLog.h>
#include <StringConverter.h>

GENLIB_INIT_SINGLETON( SoundManager );

#if ELYPSE_USE_FMOD
#	include "FmodApi.h"
#elif ELYPSE_USE_OPENAL
#	include "OpenALApi.h"
#else
#	pragma warning( "No Sound !" )
#endif

namespace Elypse::Media
{
	SoundManager::SoundManager( const String & p_basePath )
		: m_basePath( p_basePath )
		, m_musicVolume( 0.0 )
		, m_SFXVolume( 0.0 )
		, m_musicMuted( false )
		, m_SFXMuted( false )
#if ELYPSE_USE_FMOD
		, m_system( std::make_unique< fmod::SoundSystem >() )
#elif ELYPSE_USE_OPENAL
		, m_system( std::make_unique< openal::SoundSystem >() )
#endif
	{
		GENLIB_SET_SINGLETON();
		srand( uint32_t( time( nullptr ) ) );
	}

	SoundManager::~SoundManager()
	{
		Cleanup();
	}

	void SoundManager::AddPlaylist( SoundPlaylist * p_playlist )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		if ( p_playlist == nullptr )
		{
			return;
		}

		if ( General::Utils::map::has( m_playlists, p_playlist->GetName() ) )
		{
			_logMessage( "SoundManager::AddPlaylist - Can't add the playlist " + p_playlist->GetName() + " because it already exists" );
			return;
		}

		m_playlists.emplace( p_playlist->GetName(), p_playlist );
	}

	SoundObject * SoundManager::CreateSound( String const & p_name )
	{
		if ( !m_system->IsInitialised() )
		{
			return nullptr;
		}

		SoundObject * l_sound = General::Utils::map::findOrNull( m_sounds, p_name );

		if ( l_sound != nullptr )
		{
			_logMessage( "SoundManager::AddPlaylist - Can't add the sound " + p_name + " because it already exists" );
			return l_sound;
		}

		l_sound = new SoundObject( p_name );
		m_sounds.emplace( p_name, l_sound );
		return l_sound;
	}

	SoundChannelPtr SoundManager::CreateChannel( SoundObject const & object )
	{
		return m_system->CreateChannel( object );
	}

	void SoundManager::RemoveSoundObject( String const & p_name )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		General::Utils::map::deleteValue( m_sounds, p_name );
	}

	void SoundManager::RegisterInstance( SoundInstance * p_instance )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		if ( p_instance == nullptr )
		{
			return;
		}

		SceneNode * l_node = p_instance->GetNode();

		if ( l_node == nullptr )
		{
			return;
		}

		SoundNodeMap::iterator ifind = m_nodeInstances.find( l_node->getName() );

		if ( ifind == m_nodeInstances.end() )
		{
			ifind = m_nodeInstances.insert( std::make_pair( l_node->getName(), SoundInstanceSet() ) ).first;
		}

		ifind->second.insert( p_instance );
	}

	void SoundManager::RemoveNode( String const & p_nodeName )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		const SoundNodeMap::iterator & ifind = m_nodeInstances.find( p_nodeName );

		if ( ifind == m_nodeInstances.end() )
		{
			return;
		}

		const SoundInstanceSet & l_set = ifind->second;
		SoundInstanceSet::const_iterator l_iter = l_set.begin();
		const SoundInstanceSet::const_iterator & l_end = l_set.end();

		for ( ; l_iter != l_end; ++l_iter )
		{
			( *l_iter )->GetOwner()->RemoveNode( p_nodeName );
		}

		m_nodeInstances.erase( ifind );
	}


	void SoundManager::SetVolumePercent( String const & p_nodeName, Real p_percent )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		p_percent = minmax <Real>( 0.0, p_percent, 1.0 );
		const SoundNodeMap::iterator & ifind = m_nodeInstances.find( p_nodeName );

		if ( ifind == m_nodeInstances.end() )
		{
			return;
		}

		const SoundInstanceSet & l_set = ifind->second;
		SoundInstanceSet::const_iterator l_iter = l_set.begin();
		const SoundInstanceSet::const_iterator & l_end = l_set.end();

		for ( ; l_iter != l_end; ++l_iter )
		{
			( *l_iter )->SetVolumePercent( p_percent );
		}
	}

	/*
	bool SoundManager::IsSoundObject( String const &  p_name)
	{
		SoundObjectMap::iterator l_it = m_objectList.begin();
		const SoundObjectMap::iterator & l_end = m_objectList.end();

		for( ; l_it != l_end ; ++l_it)
		{
			if (l_it->second->GetMediaEntity( p_name) != nullptr)
			{
				return true;
			}
		}

		return false;
	}
	*/

	void SoundManager::Cleanup()
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		General::Utils::map::deleteAll( m_sounds );
		General::Utils::map::deleteAll( m_playlists );
	}

	void SoundManager::StartAll()
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		General::Utils::map::cycle( m_sounds, &SoundObject::PlayAll );
	}

	void SoundManager::StopAll()
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		General::Utils::map::cycle( m_sounds, &SoundObject::StopAll );
	}

	void SoundManager::PauseAll()
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		General::Utils::map::cycle( m_sounds, &SoundObject::PauseAll );
	}

	void SoundManager::SetVolume( Real p_volume, SoundType p_type )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		p_volume = minmax<Real>( MINVOLUME, p_volume, MAXVOLUME );
		SoundObjectMap::iterator l_it = m_sounds.begin();
		const SoundObjectMap::iterator & l_end = m_sounds.end();

		if ( p_type == EM_MUSIC )
		{
			m_musicVolume = p_volume;
		}
		else
		{
			m_SFXVolume = p_volume;
		}

		for ( ; l_it != l_end; ++l_it )
		{
			if ( l_it->second->GetSoundType() == p_type )
			{
				l_it->second->SetMaxVolume( p_volume );
			}
		}
	}

	void SoundManager::Mute( bool p_mute, SoundType p_type )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		if ( p_type == EM_SFX )
		{
			m_SFXMuted = p_mute;
		}
		else if ( p_type == EM_MUSIC )
		{
			m_musicMuted = p_mute;
		}

		SoundObjectMap::iterator l_it = m_sounds.begin();
		const SoundObjectMap::iterator & l_end = m_sounds.end();

		for ( ; l_it != l_end; ++l_it )
		{
			if ( l_it->second->GetSoundType() == p_type )
			{
				l_it->second->Mute( p_mute );
			}
		}
	}

	void SoundManager::Update( const Vector3 & p_position,
		const Real * p_up,
		const Real * p_forward,
		Real p_updateTime )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		m_system->Update3D( p_position / 100, p_up, p_forward, p_updateTime );
		_update( p_updateTime );
		m_system->Update();
	}

	void SoundManager::_update( Real p_time )
	{
		if ( !m_system->IsInitialised() )
		{
			return;
		}

		SoundObjectMap::iterator l_it = m_sounds.begin();
		const SoundObjectMap::iterator & l_end = m_sounds.end();

		for ( ; l_it != l_end; ++l_it )
		{
			l_it->second->UpdateFade( p_time );
			l_it->second->Update3D( p_time );
		}

		General::Utils::map::cycle( m_playlists, &SoundPlaylist::Update, p_time );
	}

	void SoundManager::_logMessage( String const & p_msg )
	{
		EMUSE_MESSAGE_RELEASE( p_msg );
	}
}