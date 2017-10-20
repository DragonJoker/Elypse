/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"
#include "RawImage.h"

#include <OgreHardwareBuffer.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

RawImage::RawImage( String const & p_name )
	: named{ p_name }
{
	TextureManager::getSingletonPtr()->load( p_name, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	Ogre::TexturePtr l_ptr = TextureManager::getSingletonPtr()->getByName( p_name );
	m_width = l_ptr->getWidth();
	m_height = l_ptr->getHeight();
	m_data.resize( m_width * m_height );
	const PixelBox & l_box = l_ptr->getBuffer()->lock( Box( 0, 0, m_width, m_height ), HardwareBuffer::HBL_READ_ONLY );
	char * l_rawData = static_cast< char * >( l_box.data );
	size_t imax = m_width * m_height;

	for ( auto & l_colour : m_data )
	{
		PixelUtil::unpackColour( &l_colour, l_ptr->getFormat(), l_rawData );
		l_rawData += PixelUtil::getComponentCount( l_ptr->getFormat() );
	}

	l_ptr->getBuffer()->unlock();
}

RawImage::~RawImage()
{
	RawImageManager::GetSingletonPtr()->RemoveElement( GetName() );
}
