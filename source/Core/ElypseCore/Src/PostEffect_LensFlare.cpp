/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "PostEffect_LensFlare.h"

#include <OgreViewport.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreRoot.h>

PostEffect_LensFlare::PostEffect_LensFlare( String const & p_name, Viewport * p_viewport )
	: PostEffect( p_name, p_viewport )
	, m_camera( p_viewport->getCamera() )
{
	OverlayManager::getSingletonPtr()->getByName( "Core/LensFlare" )->show();
	m_lensflareElements[0] = OverlayManager::getSingletonPtr()->getOverlayElement( "Core/LensFlare/1" );
	m_lensflareElements[1] = OverlayManager::getSingletonPtr()->getOverlayElement( "Core/LensFlare/2" );
	m_lensflareElements[2] = OverlayManager::getSingletonPtr()->getOverlayElement( "Core/LensFlare/3" );
	m_lensflareElements[3] = OverlayManager::getSingletonPtr()->getOverlayElement( "Core/LensFlare/4" );
	SceneManager::MovableObjectIterator l_iter = m_camera->getSceneManager()->getMovableObjectIterator( "Light" );

	if ( l_iter.hasMoreElements() )
	{
		Light * l_light = static_cast <Light *>( l_iter.getNext() );
		m_direction = l_light->getDerivedDirection();
	}
	else
	{
		m_direction = Vector3( 0.5, -0.5, 0.5 );
	}

	m_direction.normalise();
	m_listener = new LensFlareListener( *this );
}

PostEffect_LensFlare::~PostEffect_LensFlare()
{
	delete m_listener;
}

void PostEffect_LensFlare::SetEnabled( bool p_enable )
{
	if ( p_enable )
	{
		OverlayManager::getSingletonPtr()->getByName( "Core/LensFlare" )->show();
	}
	else
	{
		OverlayManager::getSingletonPtr()->getByName( "Core/LensFlare" )->hide();
	}
}

void PostEffect_LensFlare::Update()
{
	m_lastDirection = m_camera->getProjectionMatrix() * m_camera->getViewMatrix() * ( m_camera->getDerivedPosition() + 1000.0 * m_direction );
	m_lastDirection.normalise();
	Real l_cosAngle = m_direction.dotProduct( m_camera->getParentSceneNode()->_getDerivedOrientation() * Vector3::NEGATIVE_UNIT_Z );

	if ( l_cosAngle > 0.0 )
	{
		m_lastDirection /= 2.0;
		m_lensflareElements[0]->setPosition( m_lastDirection.x * 0.5f, m_lastDirection.y * 0.5f );
		m_lensflareElements[1]->setPosition( -m_lastDirection.x * 0.5f, -m_lastDirection.y * 0.5f );
		m_lensflareElements[2]->setPosition( -m_lastDirection.x * 0.9f, -m_lastDirection.y * 0.9f );
		m_lensflareElements[3]->setPosition( -m_lastDirection.x * 2.0f, -m_lastDirection.y * 2.0f );
		//	m_lensflareElements[1]->setPosition( m_lastDirection.x / 2.0, m_lastDirection.y/2.0);
		//	m_lensflareElements[2]->setPosition( m_lastDirection.x*2.0, m_lastDirection.y*2.0);
		//	m_lensflareElements[3]->setPosition( m_lastDirection.x*(-3.0), m_lastDirection.y*(-3.0));
	}
	else
	{
		m_lensflareElements[0]->setPosition( 100.0f, 100.0f );
		m_lensflareElements[1]->setPosition( 100.0f, 100.0f );
		m_lensflareElements[2]->setPosition( 100.0f, 100.0f );
		m_lensflareElements[3]->setPosition( 100.0f, 100.0f );
	}
}

LensFlareListener::LensFlareListener( PostEffect_LensFlare & p_owner )
	: owned_by( p_owner )
{
	Root::getSingletonPtr()->addFrameListener( this );
}

LensFlareListener::~LensFlareListener()
{
	Root::getSingletonPtr()->removeFrameListener( this );
}

bool LensFlareListener::frameStarted( const FrameEvent & )
{
	GetOwner()->Update();
	return true;
}
