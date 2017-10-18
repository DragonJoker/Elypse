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
	General::Utils::map::deleteAll( m_zones );
	ClearScene();
	m_manager->destroyCamera( m_camera );
	Root::getSingletonPtr()->destroySceneManager( m_manager );
}

void Universe::AddZone( Zone * p_zone )
{
	genlib_assert( p_zone != nullptr );

	if ( ! General::Utils::map::has( m_zones, p_zone->GetName() ) )
	{
		m_zones.insert( std::make_pair( p_zone->GetName(), p_zone ) );
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
