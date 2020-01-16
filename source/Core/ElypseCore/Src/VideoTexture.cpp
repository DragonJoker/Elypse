/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "VideoObject.h"
#include "VideoTexture.h"
#include "VideoImplementation.h"

#include "ElypseLogs.h"

#include <OgreRoot.h>
#include <OgreLog.h>
#include <OgreTexture.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreStringConverter.h>
#include <OgreTechnique.h>

using namespace Ogre;

VideoTexture::VideoTexture( VideoObject & p_owner )
	: owned_by< VideoObject >( p_owner )
	, m_bufferChange( false )
	, m_bufferSize( GetOwner()->m_width * GetOwner()->m_height * 4 )
	, m_tmpBuffer( new unsigned char[m_bufferSize] )
{
}

VideoTexture::~VideoTexture()
{
	delete [] m_tmpBuffer;
	MaterialManager::getSingletonPtr()->remove( GetOwner()->GetName() );
	TextureManager::getSingletonPtr()->remove( m_texture->getHandle() );
}

bool VideoTexture::initialise()
{
	createDefinedTexture( GetOwner()->GetName() );
	return true;
}

void VideoTexture::_createTexture( String const & p_materialName, String const & p_groupName, bool p_createMat )
{
	TextureUnitState * l_texUnitState;
	MaterialPtr l_pMaterial = MaterialManager::getSingleton().getByName( p_materialName );
	m_texture = TextureManager::getSingleton().createManual( p_materialName,
				p_groupName,
				TEX_TYPE_2D,
				GetOwner()->m_width,
				GetOwner()->m_height,
				0,
				PF_R8G8B8,
				TU_DYNAMIC_WRITE_ONLY_DISCARDABLE ).get();

	if ( !l_pMaterial )
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

void VideoTexture::createDefinedTexture( String const & p_materialName, String const & p_groupName )
{
	if ( !TextureManager::getSingleton().getByName( p_materialName ) )
	{
		_createTexture( p_materialName, p_groupName, true );
	}
}

void VideoTexture::CopyToBuffer( unsigned char * p_buffer, uint32_t p_bufferSize )
{
	auto l_lock = make_unique_lock( m_mutex );

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
	auto l_lock = make_unique_lock( m_mutex );

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
	auto l_lock = make_unique_lock( m_mutex );
	memset( m_tmpBuffer, 0, m_bufferSize );
	m_bufferChange = true;
}
