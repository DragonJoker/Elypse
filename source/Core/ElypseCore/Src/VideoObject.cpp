/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ElypseLogs.h"

#include "VideoObject.h"

#include "VideoTexture.h"
#include "VideoManager.h"
#include "VideoOverlay.h"
#include "VideoInstance.h"
#include "VideoImplementation.h"
#include "VideoFactory.h"

#include <NeoCurl.h>

#include "ElypsePlugin.h"

VideoObject::VideoObject( String const & p_name, ElypsePlugin * p_plugin )
	: named( p_name )
	, m_isFinished( false )
	, m_isPlaying( false )
	, m_looped( false )
	, m_tokenUrl( false )
	, m_autoInitialised( true )
	, m_width( 0 )
	, m_height( 0 )
	, m_videoTexture( NULL )
	, m_plugin( p_plugin )
	, m_impl( NULL )
{
	genlib_assert( m_plugin != NULL );
}

VideoObject::~VideoObject()
{
	if ( m_impl != NULL )
	{
		delete m_impl;
	}

	if ( m_videoTexture != NULL )
	{
		delete m_videoTexture;
		m_videoTexture = NULL;
	}

	General::Utils::map::deleteAll( m_overlayMap );
	General::Utils::map::deleteAll( m_instanceMap );
}

void VideoObject::RemoveFromZone( Zone * p_zone )
{
	VideoManager::GetSingletonPtr()->DeleteVideoObject( m_name );
}

VideoOverlay * VideoObject::CreateOverlay( EMOverlay * p_overlay )
{
	VideoOverlay * l_vidOverlay = General::Utils::map::findOrNull( m_overlayMap, p_overlay );

	if ( l_vidOverlay != NULL )
	{
		return l_vidOverlay;
	}

	l_vidOverlay = new VideoOverlay( *this, p_overlay );
	m_overlayMap.insert( std::make_pair( p_overlay, l_vidOverlay ) );
	return l_vidOverlay;
}

VideoInstance * VideoObject::CreateInstance( Entity * p_entity )
{
	VideoInstance * l_vidInstance = General::Utils::map::findOrNull( m_instanceMap, p_entity );

	if ( l_vidInstance != NULL )
	{
		return l_vidInstance;
	}

	l_vidInstance = new VideoInstance( *this, p_entity );
	m_instanceMap.insert( std::make_pair( p_entity, l_vidInstance ) );
	return l_vidInstance;
}

VideoOverlay * VideoObject::GetOverlay( EMOverlay * p_overlay )
{
	return General::Utils::map::findOrNull( m_overlayMap, p_overlay );
}

VideoInstance * VideoObject::GetInstance( Entity * p_instance )
{
	return General::Utils::map::findOrNull( m_instanceMap, p_instance );
}

void VideoObject::DeleteOverlay( EMOverlay * p_overlay )
{
	General::Utils::map::deleteValue( m_overlayMap, p_overlay );
}

void VideoObject::DeleteInstance( Entity * p_instance )
{
	General::Utils::map::deleteValue( m_instanceMap, p_instance );
}

void VideoObject::Update( Real p_time )
{
	if ( ! m_isPlaying )
	{
		return;
	}

	if ( m_impl != NULL )
	{
		m_impl->Update( p_time );
	}
}

void VideoObject::Initialise()
{
	genlib_assert( ! m_basePath.empty() );

	if ( m_mediaUrl.empty() )
	{
		GENLIB_EXCEPTION( "A video must have an Url." );
	}

	if ( m_width == 0 )
	{
		m_width = 512;
	}

	if ( m_height == 0 )
	{
		m_height = 512;
	}

	if ( m_tokenUrl )
	{
		CURLManager l_manager;
		std::string l_url;
		l_manager.GetStringFromUrl( m_mediaUrl, l_url );
		m_mediaUrl = l_url;
	}

	if ( m_mediaUrl.find( "./" ) == 0 )
	{
		Url l_url( "file://" + m_basePath );
		l_url.GetProtocol();
		l_url /= m_mediaUrl.substr( 2, String::npos );
		m_mediaUrl = l_url;
	}

	m_plugin->LockGui();

	if ( m_impl == NULL )
	{
		if ( VideoManager::GetSingletonPtr()->GetCurrentFactory() != NULL )
		{
			m_impl = VideoManager::GetSingletonPtr()->GetCurrentFactory()->Create( *this );
			m_impl->SetMaxVolume( m_maxVolume );
			m_impl->SetVolumePercent( m_volumePercent );
			m_impl->Mute( m_muted );
			m_impl->Initialise( m_mediaUrl );
		}
	}

	m_plugin->UnlockGui();
}

void VideoObject::InitialiseVideoTexture()
{
	if ( m_videoTexture == NULL )
	{
		m_videoTexture = new VideoTexture( *this );
		m_videoTexture->initialise();
	}

	VideoOverlayMap::iterator l_it = m_overlayMap.begin();

	for ( ; l_it != m_overlayMap.end() ; ++l_it )
	{
		if ( ! l_it->second->IsLoadDelayed() )
		{
			l_it->second->Initialise( m_name );
		}
	}

	VideoInstanceMap::iterator l_it2 = m_instanceMap.begin();

	for ( ; l_it2 != m_instanceMap.end() ; ++l_it2 )
	{
		if ( ! l_it2->second->IsLoadDelayed() )
		{
			l_it2->second->Initialise( m_name );
		}
	}
}

void VideoObject::Reset()
{
	General::Utils::map::cycle( m_overlayMap, & VideoOverlay::Reset );
	General::Utils::map::cycle( m_instanceMap, & VideoInstance::Reset );

	if ( m_impl != NULL )
	{
		delete m_impl;
		m_impl = NULL;
	}

	m_videoTexture->RedrawInBlack();
	m_videoTexture->Update();
	m_isPlaying = false;
}

void VideoObject::Start()
{
	if ( m_isPlaying || m_impl == NULL )
	{
		return;
	}

	m_isPlaying = m_impl->Start();
}

void VideoObject::Pause()
{
	if ( ! m_isPlaying || m_impl == NULL )
	{
		return;
	}

	m_impl->Pause();
	m_isPlaying = false;
}

void VideoObject::Stop()
{
	if ( m_impl != NULL )
	{
		m_impl->Stop();
	}

	/*
	if (m_videoTexture != NULL)
	{
		m_videoTexture->RedrawInBlack();
		m_videoTexture->Update();
	}
	*/
	m_isPlaying = false;
}

void VideoObject::SetMaxVolume( int p_maxVolume )
{
	m_maxVolume = p_maxVolume;

	if ( m_impl != NULL )
	{
		m_impl->SetMaxVolume( m_maxVolume );
	}
}

void VideoObject::SetVolumePercent( Real p_percent )
{
	m_volumePercent = p_percent;

	if ( m_impl != NULL )
	{
		m_impl->SetVolumePercent( m_volumePercent );
	}
}

void VideoObject::Mute( bool p_muted )
{
	m_muted = p_muted;

	if ( m_impl != NULL )
	{
		m_impl->Mute( p_muted );
	}
}
