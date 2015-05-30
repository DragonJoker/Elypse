/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		VideoTexture.h - VideoTexture.cpp

	Desc :		Class used to put a video in a texture.

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "VideoObject.h"
#include "VideoTexture.h"
#include "VideoImplementation.h"

#include "EMuseLogs.h"

#include <OgreRoot.h>
#include <OgreLog.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreStringConverter.h>

using namespace Ogre;

VideoTexture::VideoTexture( VideoObject * p_owner )
	:	owned_by <VideoObject>	( p_owner ),
		 m_bufferChange( false ),
		 m_bufferSize( m_owner->m_width * m_owner->m_height * 4 ),
		 m_tmpBuffer( new unsigned char[m_bufferSize] )
{
}

VideoTexture::~VideoTexture()
{
	delete [] m_tmpBuffer;
	MaterialManager::getSingletonPtr()->remove( m_owner->GetName() );
	TextureManager::getSingletonPtr()->remove( m_texture->getHandle() );
}

bool VideoTexture::initialise()
{
	createDefinedTexture( m_owner->GetName() );
	return true;
}

void VideoTexture::_createTexture( const String & p_materialName, const String & p_groupName, bool p_createMat )
{
	TextureUnitState * l_texUnitState;
	MaterialPtr l_pMaterial = MaterialManager::getSingleton().getByName( p_materialName );
	m_texture = TextureManager::getSingleton().createManual(	p_materialName,
				p_groupName,
				TEX_TYPE_2D,
				m_owner->m_width,
				m_owner->m_height,
				0,
				PF_R8G8B8,
				TU_DYNAMIC_WRITE_ONLY_DISCARDABLE ).getPointer();

	if ( l_pMaterial.isNull() )
	{
		l_pMaterial = MaterialManager::getSingleton().create( p_materialName, p_groupName );
		l_texUnitState = l_pMaterial->getTechnique( 0 )->getPass( 0 )->createTextureUnitState( p_materialName );
	}
	else
	{
		l_texUnitState = l_pMaterial->getTechnique( 0 )->getPass( 0 )->getTextureUnitState( 0 );
	}

	l_texUnitState->setTextureName( p_materialName, TEX_TYPE_2D );
	l_texUnitState->setTextureFiltering( FO_LINEAR, FO_LINEAR, FO_NONE );
	l_texUnitState->setTextureAddressingMode( TextureUnitState::TAM_WRAP );
}

void VideoTexture::createDefinedTexture( const String & p_materialName, const String & p_groupName )
{
	if ( TextureManager::getSingleton().getByName( p_materialName ).isNull() )
	{
		_createTexture( p_materialName, p_groupName, true );
	}
}

void VideoTexture::CopyToBuffer( unsigned char * p_buffer, unsigned int p_bufferSize )
{
	GENLIB_AUTO_SCOPED_LOCK();

	if ( m_bufferSize == p_bufferSize )
	{
		memcpy( m_tmpBuffer, p_buffer, p_bufferSize );
		m_bufferChange = true;
	}
	else
	{
		EMUSE_MESSAGE_NORMAL( "VideoTexture::CopyToBuffer : decrepancy in buffer sizes : " + StringConverter::toString( m_bufferSize ) + " against : " + StringConverter::toString( p_bufferSize ) );
	}
}

void VideoTexture::Update()
{
	GENLIB_AUTO_SCOPED_LOCK();

	if ( ! m_bufferChange )
	{
		return;
	}

	HardwarePixelBufferSharedPtr l_pixelBuffer = m_texture->getBuffer();
	unsigned char * l_dest = reinterpret_cast <unsigned char *>( l_pixelBuffer->lock( HardwareBuffer::HBL_DISCARD ) );
	memcpy( l_dest, m_tmpBuffer, m_bufferSize );
	m_bufferChange = false;
	l_pixelBuffer->unlock();
}

void VideoTexture::RedrawInBlack()
{
	GENLIB_AUTO_SCOPED_LOCK();
	memset( m_tmpBuffer, 0, m_bufferSize );
	m_bufferChange = true;
}
