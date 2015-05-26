#include "PrecompiledHeader.h"

#include "VideoManager.h"
#include "VideoObject.h"
#include "VideoTexture.h"
#include "VideoFactory.h"

#if ELYPSE_WINDOWS
#	include "VideoFactory_DirectShow.h"
#else
#	include "VideoFactory_GStreamer.h"
#endif


#include <OgreEntity.h>
#include <OgreLog.h>
#include <OgreStringConverter.h>


GENLIB_INIT_SINGLETON( VideoManager );

VideoManager :: VideoManager()
	:	m_currentFactory( NULL ),
		m_volume( 100 ),
		m_muted( false )
{
	GENLIB_SET_SINGLETON();
#if ELYPSE_WINDOWS
//	RegisterFactory( new VideoFactory_GStreamer);
	RegisterFactory( new VideoFactory_DirectShow );
#elif ELYPSE_LINUX
	RegisterFactory( new VideoFactory_GStreamer );
#endif
}

VideoManager :: ~VideoManager()
{
	Cleanup();
	General::Utils::map::deleteAll( m_factories );
}

void VideoManager :: RegisterFactory( VideoFactory * p_factory )
{
	genlib_assert( p_factory != NULL );
	m_factories.insert( VideoFactoryMap::value_type( p_factory->GetName(), p_factory ) );

	if ( m_currentFactory == NULL )
	{
		m_currentFactory = p_factory;
	}
}

void VideoManager :: SetCurrentFactory( const String & p_name )
{
	VideoFactory * l_factory = General::Utils::map::findOrNull( m_factories, p_name );

	if ( l_factory == NULL )
	{
		GENLIB_EXCEPTION( "VideoFactory named " + p_name + " does not exist" );
	}

	m_currentFactory = l_factory;
}

void VideoManager :: Update()
{
	General::Utils::map::cycle( m_objectList, & VideoObject::Update, static_cast <Real>( m_timer.Time() ) );
}

VideoObject * VideoManager :: CreateVideoObject( const String & p_name, EMusePlugin * p_plugin )
{
	VideoObject * l_object = General::Utils::map::insert( m_objectList, p_name, p_name, p_plugin );
	l_object->SetMaxVolume( m_volume );
	l_object->Mute( m_muted );
	return l_object;
}

void VideoManager :: DeleteVideoObject( const String & p_name )
{
	General::Utils::map::deleteValue( m_objectList, p_name );
}

bool VideoManager :: IsVideo( const String & p_entityName )
{
	//TODO.
	return false;
}

void VideoManager :: Cleanup()
{
	General::Utils::map::deleteAll( m_objectList );
}

void VideoManager :: StartAll()
{
	General::Utils::map::cycle( m_objectList, & VideoObject::Start );
}

void VideoManager :: StopAll()
{
	General::Utils::map::cycle( m_objectList, & VideoObject::Stop );
}

void VideoManager :: PauseAll()
{
	General::Utils::map::cycle( m_objectList, & VideoObject::Pause );
}

void VideoManager :: SetVolume( int p_volume )
{
	m_volume = p_volume;

	if ( m_volume < 0 )
	{
		m_volume = 0;
	}
	else if ( m_volume > 100 )
	{
		m_volume = 100;
	}

	General::Utils::map::cycle( m_objectList, & VideoObject::SetMaxVolume, m_volume );
}

void VideoManager :: Mute( bool p_muted )
{
	m_muted = p_muted;
	General::Utils::map::cycle( m_objectList, & VideoObject::Mute, m_muted );
}
/*
void VideoManager :: UpdateVolume( const Vector3 & p_position, Radian p_orientation)
{
	VideoObjectMap::iterator l_it = m_objectList.begin();

	while (l_it != m_objectList.end())
	{
		l_it->second->UpdateVolume( p_position, p_orientation);
		++ l_it;
	}
}

void VideoManager :: ReleaseAllZone( const String & p_zoneName)
{
	VideoObjectMap::iterator l_it = m_objectList.begin();
	StringArray l_toRemove;

	while (l_it != m_objectList.end())
	{
		l_it->second->ReleaseAllZone( p_zoneName);

		if (l_it->second->GetUse() <= 0)
		{
			l_toRemove.push_back( l_it->first);
		}

		++ l_it;
	}

	for (unsigned int i = 0 ; i < l_toRemove.size() ; ++ i)
	{
		General::Utils::map::deleteValue( m_objectList, l_toRemove[i]);
	}

	l_toRemove.clear();
}
*/
