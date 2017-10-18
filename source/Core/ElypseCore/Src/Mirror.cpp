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

#include "Mirror.h"
#include "MirrorMaterial.h"

#include <OgreMovablePlane.h>
#include <OgreMaterialManager.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>

#include "ElypseLogs.h"

using namespace Ogre;

Mirror::Mirror( String const & p_name )
	: named( p_name )
	, m_camera( NULL )
	, m_texture( NULL )
	, m_planeNode( NULL )
	, m_mainCamera( NULL )
	, m_plane( NULL )
	, m_render( NULL )
	, m_viewport( NULL )
	, m_needUpdate( true )
	, m_autoUpdate( false )
	, m_alwaysRedraw( false )
	, m_position( Vector3::ZERO )
	, m_normal( Vector3::UNIT_Y )
	, m_background( ColourValue::Black )
	, m_texWidth( 512 )
	, m_texHeight( 512 )
{
}

Mirror::~Mirror()
{
	if ( m_planeNode != NULL )
	{
		m_manager->destroySceneNode( m_planeNode->getName() );
	}

	if ( m_plane != NULL )
	{
		delete m_plane;
	}

	if ( m_camera )
	{
		m_manager->destroyCamera( m_camera );
	}

	if ( m_render != NULL )
	{
		m_render->removeAllViewports();
	}

	General::Utils::map::deleteAll( m_materials );

	if ( m_texture != NULL )
	{
		TextureManager::getSingletonPtr()->remove( m_texture->getHandle() );
	}
}

void Mirror::Initialise( String const & p_instanceKey, SceneManager * p_sceneManager )
{
	m_manager = p_sceneManager;
	m_planeNode = m_manager->getRootSceneNode()->createChildSceneNode( "___INTERNAL_MirrorPlaneNode_" + m_name );
	m_plane = new MovablePlane( m_normal, m_position );
	m_planeNode->attachObject( m_plane );
	m_texture = TextureManager::getSingleton().createManual( "__INTERNAL_MirrorTexture_" + m_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, m_texWidth, m_texHeight, 0, PF_R8G8B8, TU_RENDERTARGET ).getPointer();
	m_render = m_texture->getBuffer()->getRenderTarget();
	m_render->setAutoUpdated( m_autoUpdate );
	m_camera = m_manager->createCamera( "__INTERNAL_MirrorCamera_" + m_name );
	m_viewport = m_render->addViewport( m_camera );
	m_viewport->setClearEveryFrame( true );
	_setBackground();
	m_camera->enableReflection( m_plane );
	m_camera->enableCustomNearClipPlane( m_plane );
	_linkCameras();
	General::Utils::map::cycle( m_materials, & MirrorMaterial::Apply );
}

void Mirror::_setBackground()
{
	if ( m_viewport != NULL )
	{
		m_viewport->setBackgroundColour( m_background );
	}
}

void Mirror::_linkCameras()
{
	if ( m_mainCamera == NULL || m_camera == NULL )
	{
		return;
	}

	m_camera->setNearClipDistance( m_mainCamera->getNearClipDistance() );
	m_camera->setFarClipDistance( m_mainCamera->getFarClipDistance() );
	m_camera->setAspectRatio( m_mainCamera->getAspectRatio() );
}

void Mirror::Update()
{
	if ( m_camera == NULL || m_render == NULL )
	{
		return;
	}

	if ( m_camera->getOrientation() != m_mainCamera->getDerivedOrientation() )
	{
		m_camera->setOrientation( m_mainCamera->getDerivedOrientation() );
		m_needUpdate = true;
	}

	if ( m_camera->getPosition() != m_mainCamera->getDerivedPosition() )
	{
		m_camera->setPosition( m_mainCamera->getDerivedPosition() );
		m_needUpdate = true;
	}

	if ( ! m_alwaysRedraw && m_needUpdate && IsVisible() )
	{
		m_render->update();
	}

	if ( ! m_autoUpdate )
	{
		m_needUpdate = false;
	}
}

void Mirror::AddMaterial( String const & p_materialName, Real p_opacity )
{
	EMUSE_MESSAGE_DEBUG( "Mirror::AddMaterial : " + m_name + String( " -> " ) + p_materialName );
	MirrorMaterial * l_mat = General::Utils::map::insert( m_materials, p_materialName, p_materialName, *this );
	l_mat->SetOpacity( p_opacity );
}

void Mirror::RemoveMaterial( String const & p_materialName )
{
	General::Utils::map::deleteValue( m_materials, p_materialName );
}

void Mirror::ChangeMaterialAlpha( String const & p_materialName, Real p_opacity )
{
	MirrorMaterial * l_mat = General::Utils::map::findOrNull( m_materials, p_materialName );

	if ( l_mat != NULL )
	{
		l_mat->SetOpacity( p_opacity );
	}
}

bool Mirror::IsVisible()const
{
	if ( m_plane == NULL )
	{
		return false;
	}

	if ( m_plane->getSide( m_mainCamera->getDerivedPosition() ) == Plane::NEGATIVE_SIDE )
	{
		return false;
	}

	if ( m_mainCamera->getDerivedDirection().dotProduct( m_plane->normal ) > 0.0 )
	{
		return false;
	}

	return true;
}

void Mirror::SetAutoUpdate( bool p_redrawWhenCameraMoves, bool p_redrawEveryFrame )
{
	m_autoUpdate = p_redrawWhenCameraMoves;
	m_alwaysRedraw = p_redrawEveryFrame;

	if ( m_autoUpdate )
	{
		m_needUpdate = true;
	}

	if ( m_render != NULL )
	{
		m_render->setAutoUpdated( m_alwaysRedraw );
	}
}
