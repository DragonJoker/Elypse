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

#include "CamTex.h"

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreTextureManager.h>
#include <OgreRenderTexture.h>
#include <OgreMaterialManager.h>
#include <OgreHardwarePixelBuffer.h>

using namespace Ogre;

CamTex::CamTex( const String & p_name, SceneManager * p_sceneManager, Camera * p_camera, unsigned int p_textureWidth, unsigned int p_textureHeight )
	: named( p_name ),
		m_camera( p_camera ),
		m_sceneManager( p_sceneManager )
{
	m_texture = TextureManager::getSingleton().createManual( m_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, p_textureWidth, p_textureHeight, 0, PF_A8R8G8B8, TU_RENDERTARGET | TU_AUTOMIPMAP );
	m_renderTarget = m_texture->getBuffer()->getRenderTarget();
	m_renderTarget->setAutoUpdated( false );
	m_viewport = m_renderTarget->addViewport( m_camera );
	m_viewport->setClearEveryFrame( true );
	m_viewport->setBackgroundColour( ColourValue::Black );
	m_viewport->setOverlaysEnabled( false );
	p_camera->setAspectRatio( p_textureWidth / static_cast <Real>( p_textureHeight ) );
	p_camera->setNearClipDistance( 5.0 );
	MaterialPtr l_material;
	l_material = MaterialManager::getSingletonPtr()->getByName( m_name );

	if ( ! l_material.isNull() )
	{
		m_ownsMaterial = false;
		m_textureUnitState = l_material->getTechnique( 0 )->getPass( 0 )->createTextureUnitState( m_name );
		m_textureUnitState->setTextureAddressingMode( TextureUnitState::TAM_WRAP );
		m_textureUnitState->setProjectiveTexturing( false, m_camera );
		l_material->getTechnique( 0 )->getPass( 0 )->setSceneBlending( SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA );
	}
	else
	{
		m_ownsMaterial = true;
		l_material = MaterialManager::getSingletonPtr()->create( m_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		m_textureUnitState = l_material->getTechnique( 0 )->getPass( 0 )->createTextureUnitState( m_name );
		m_textureUnitState->setTextureAddressingMode( TextureUnitState::TAM_WRAP );
		m_textureUnitState->setProjectiveTexturing( false, m_camera );
		l_material->getTechnique( 0 )->getPass( 0 )->setSceneBlending( SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA );
	}
}

CamTex::~CamTex()
{
	if ( m_ownsMaterial )
	{
		MaterialManager::getSingletonPtr()->remove( m_name );
	}

	m_renderTarget->removeAllViewports();
	TextureManager::getSingletonPtr()->remove( m_texture->getHandle() );
}

void CamTex::SetCamera( Camera * p_camera )
{
	m_textureUnitState->setProjectiveTexturing( false, p_camera );
	m_viewport->setCamera( p_camera );
	p_camera->setAspectRatio( static_cast <Real>( m_viewport->getActualWidth() ) / static_cast <Real>( m_viewport->getActualHeight() ) );
	m_camera = p_camera;
}

void CamTex::Update()const
{
	m_renderTarget->update();
}
