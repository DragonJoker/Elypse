/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "Universe.h"
#include "Zone.h"

#include <OgreSceneManager.h>
#include <OgreRenderTarget.h>
#include <OgreViewport.h>
#include <OgreRoot.h>
#include <OgreCamera.h>

#include "ElypseController.h"

#include "ElypseLogs.h"

Universe::Universe( String const & p_universeName, String const & p_instanceKey )
	: named{ p_universeName }
{
	genlib_assert( !m_name.empty() );
	m_manager = ElypseController::GetSingletonPtr()->CreateSceneManager( "ElypsePlayer" + p_instanceKey );
	genlib_assert( m_manager != nullptr );
	m_camera = m_manager->createCamera( "RootCamera" );
	genlib_assert( m_camera != nullptr );
	EMUSE_LOG_MESSAGE_RELEASE( "Universe create : " + m_name );
}

Universe::~Universe()
{
	EMUSE_LOG_MESSAGE_RELEASE( "Universe delete : " + m_name );
	m_zones.clear();
	ClearScene();
	m_manager->destroyCamera( m_camera );
	Root::getSingletonPtr()->destroySceneManager( m_manager );
}

void Universe::AddZone( ZonePtr p_zone )
{
	genlib_assert( p_zone );

	if ( ! General::Utils::map::has( m_zones, p_zone->GetName() ) )
	{
		auto l_name = p_zone->GetName();
		m_zones.emplace( l_name, std::move( p_zone ) );
	}
}

void Universe::SetRenderTarget( RenderTarget * p_rt )
{
	genlib_assert( p_rt != nullptr );
	EMUSE_MESSAGE_DEBUG( "Universe::SetRenderTarget -> " + m_name + " // " + p_rt->getName() );
	m_renderTarget = p_rt;
	m_viewport = m_renderTarget->addViewport( m_camera );
	genlib_assert( m_viewport != nullptr );
	m_camera->setAspectRatio( Real( m_viewport->getActualWidth() ) / Real( m_viewport->getActualHeight() ) );
	m_viewport->setBackgroundColour( ColourValue::White );
	m_renderTarget->update();
}

void Universe::ClearScene() noexcept
{
	m_manager->clearScene();
}
