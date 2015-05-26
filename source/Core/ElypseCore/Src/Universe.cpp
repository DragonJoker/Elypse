#include "PrecompiledHeader.h"

#include "Universe.h"
#include "Zone.h"

#include <OgreSceneManager.h>
#include <OgreRenderTarget.h>
#include <OgreViewport.h>
#include <OgreRoot.h>
#include <OgreCamera.h>

#include "EMuseController.h"

#include "EMuseLogs.h"

Universe :: Universe( const String & p_universeName, const String & p_instanceKey )
	:	named( p_universeName ),
		m_manager( NULL ),
		m_renderTarget( NULL ),
		m_viewport( NULL )
{
	genlib_assert( ! m_name.empty() );
	m_manager = EMuseController::GetSingletonPtr()->CreateSceneManager( "EMusePlayer" + p_instanceKey );
	genlib_assert( m_manager != NULL );
	m_camera = m_manager->createCamera( "RootCamera" );
	genlib_assert( m_camera != NULL );
	EMUSE_LOG_MESSAGE_RELEASE( "Universe create : " + m_name );
}

Universe :: ~Universe()
{
	EMUSE_LOG_MESSAGE_RELEASE( "Universe delete : " + m_name );
	General::Utils::map::deleteAll( m_zones );
	ClearScene();
	m_manager->destroyCamera( m_camera );
	Root::getSingletonPtr()->destroySceneManager( m_manager );
}

void Universe :: AddZone( Zone * p_zone )
{
	genlib_assert( p_zone != NULL );

	if ( ! General::Utils::map::has( m_zones, p_zone->GetName() ) )
	{
		m_zones.insert( ZoneMap::value_type( p_zone->GetName(), p_zone ) );
	}
}

void Universe :: SetRenderTarget( RenderTarget * p_rt )
{
	genlib_assert( p_rt != NULL );
	EMUSE_MESSAGE_DEBUG( "Universe :: SetRenderTarget -> " + m_name + " // " + p_rt->getName() );
	m_renderTarget = p_rt;
	m_viewport = m_renderTarget->addViewport( m_camera );
	genlib_assert( m_viewport != NULL );
	m_camera->setAspectRatio( static_cast <Real>( m_viewport->getActualWidth() ) / static_cast <Real>( m_viewport->getActualHeight() ) );
	m_viewport->setBackgroundColour( ColourValue::White );
	m_renderTarget->update();
}

void Universe :: ClearScene() d_no_throw
{
	m_manager->clearScene();
}
