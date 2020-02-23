/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "CamTex.h"

#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreTextureManager.h>
#include <OgreRenderTexture.h>
#include <OgreMaterialManager.h>
#include <OgreTechnique.h>
#include <OgreHardwarePixelBuffer.h>

using namespace Ogre;

CamTex::CamTex( String const & p_name, SceneManager * p_sceneManager, Camera * p_camera, uint32_t p_textureWidth, uint32_t p_textureHeight )
	: named( p_name )
	, m_camera( p_camera )
	, m_sceneManager( p_sceneManager )
{
	m_texture = TextureManager::getSingleton().createManual( m_name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D, p_textureWidth, p_textureHeight, 0, PF_A8R8G8B8, TU_RENDERTARGET | TU_AUTOMIPMAP );
	m_renderTarget = m_texture->getBuffer()->getRenderTarget();
	m_renderTarget->setAutoUpdated( false );
	m_viewport = m_renderTarget->addViewport( m_camera );
	m_viewport->setClearEveryFrame( true );
	m_viewport->setBackgroundColour( ColourValue::Black );
	m_viewport->setOverlaysEnabled( false );
	p_camera->setAspectRatio( p_textureWidth / Real( p_textureHeight ) );
	p_camera->setNearClipDistance( 5.0 );
	MaterialPtr l_material;
	l_material = MaterialManager::getSingletonPtr()->getByName( m_name );

	if ( l_material )
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
	p_camera->setAspectRatio( Real( m_viewport->getActualWidth() ) / Real( m_viewport->getActualHeight() ) );
	m_camera = p_camera;
}

void CamTex::Update()const
{
	m_renderTarget->update();
}
