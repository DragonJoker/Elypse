/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include <OgreVector.h>
#include <OgreLog.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreStringConverter.h>

#include "SoundObject.h"
#include "SoundManager.h"
#include "SoundInstance.h"

#include "Zone.h"
#include "ElypseLogs.h"

namespace Elypse::Media
{
	SoundObject::SoundObject( String const & p_name )
		: named( p_name )
		, m_globalInstance( nullptr )
		, m_useCount( 0 )
		, m_type( EM_NONE )
		, m_looped( false )
		, m_muted( false )
		, m_global( true )
		, m_volumePercent( MAXVOLUME )
	{
		EMUSE_MESSAGE_DEBUG( "SoundObject::SoundObject( " + m_name + ")" );
	}

	SoundObject::~SoundObject()
	{
		General::Utils::map::deleteAll( m_instances );

		if ( m_globalInstance )
		{
			delete m_globalInstance;
		}
	}

	void SoundObject::AddToZone( Zone * p_zone )
	{
	//	p_zone->AddZoneObject( this);
	}

	void SoundObject::RemoveFromZone( Zone * p_zone )
	{
		SoundManager::GetSingletonPtr()->RemoveSoundObject( m_name );
	}

	uint32_t SoundObject::GetLength()const
	{
		uint32_t result = 0u;
		GetSoundApi().GetLength( result );
		return result;
	}

	void SoundObject::Mute( bool p_mute )
	{
		if ( IsGlobal() )
		{
			m_globalInstance->Mute( p_mute );
		}
		else
		{
			General::Utils::map::cycle( m_instances, &SoundInstance::Mute, p_mute );
		}
	}

	void SoundObject::SetSoundUrl( String const & p_soundURL )
	{
		m_soundURL = p_soundURL;
	}

	void SoundObject::Initialise()
	{
		m_sound = SoundManager::GetSingletonPtr()->GetSystem().CreateStream( m_soundURL, m_global );

		if ( m_global )
		{
			m_globalInstance = new SoundInstance( *this );

			if ( m_type == EM_MUSIC )
			{
				SoundManager::AddToMusic( m_globalInstance->GetChannel() );
			}
			else
			{
				SoundManager::AddToSFX( m_globalInstance->GetChannel() );
			}

			m_globalInstance->SetVolumePercent( m_volumePercent );
			m_useCount++;
			m_globalInstance->SetLooped( m_looped );
		}
	}

	SoundInstance * SoundObject::CreateInstance( SceneNode * p_node )
	{
		if ( p_node == nullptr )
		{
			return nullptr;
		}

		SoundInstance * l_instance = General::Utils::map::findOrNull( m_instances, p_node->getName() );

		if ( l_instance != nullptr )
		{
			return l_instance;
		}

		l_instance = new SoundInstance( *this );

		if ( m_type == EM_MUSIC )
		{
			SoundManager::AddToMusic( l_instance->GetChannel() );
		}
		else
		{
			SoundManager::AddToSFX( l_instance->GetChannel() );
		}

		m_instances.insert( std::make_pair( p_node->getName(), l_instance ) );
		m_useCount++;
		SoundManager::GetSingletonPtr()->RegisterInstance( l_instance );
		return l_instance;
	}

	void SoundObject::RemoveNode( String const & p_nodeName )
	{
		General::Utils::map::deleteValue( m_instances, p_nodeName );
		m_useCount--;
	}

	void SoundObject::SetMaxVolume( Real p_volume )
	{
		m_maxVolume = p_volume;

		if ( IsGlobal() )
		{
			m_globalInstance->SetMaxVolume( p_volume );
		}
		else
		{
			General::Utils::map::cycle( m_instances, &SoundInstance::SetMaxVolume, p_volume );
		}
	}

	void SoundObject::PlayAll()
	{
		if ( IsGlobal() )
		{
			m_globalInstance->Play();
		}
		else
		{
			General::Utils::map::cycle( m_instances, &SoundInstance::Play );
		}
	}

	void SoundObject::PauseAll()
	{
		if ( IsGlobal() )
		{
			m_globalInstance->Pause();
		}
		else
		{
			General::Utils::map::cycle( m_instances, &SoundInstance::Pause );
		}
	}

	void SoundObject::StopAll()
	{
		if ( IsGlobal() )
		{
			m_globalInstance->Stop();
		}
		else
		{
			General::Utils::map::cycle( m_instances, &SoundInstance::Stop );
		}
	}

	void SoundObject::Update3D( Real p_time )
	{
		if ( IsGlobal() )
		{
			return;
		}

		SoundInstanceMap::iterator l_it = m_instances.begin();

		while ( l_it != m_instances.end() )
		{
			SoundInstance * l_instance = l_it->second;

			if ( l_instance->IsInPlaylist() )
			{
				++l_it;
				continue;
			}

			l_instance->Update3D( p_time );
			++l_it;
		}
	}

	void SoundObject::UpdateFade( Real p_time )
	{
		if ( IsGlobal() )
		{
			m_globalInstance->UpdateFade( p_time, m_muted );
			return;
		}

		SoundInstanceMap::iterator l_it = m_instances.begin();

		while ( l_it != m_instances.end() )
		{
			SoundInstance * l_instance = l_it->second;

			if ( l_instance->IsInPlaylist() )
			{
				++l_it;
				continue;
			}

			l_instance->UpdateFade( p_time, m_muted );
			++l_it;
		}
	}

	void SoundObject::SetLocal( String const & p_zone )
	{
		m_zone = p_zone;
		m_global = false;
	}
}
