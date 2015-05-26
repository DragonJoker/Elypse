#include "PrecompiledHeader.h"
#include "RawImage.h"

#include <OgreHardwareBuffer.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

RawImage :: RawImage( const String & p_name )
	:	named( p_name ),
		m_refCount( 0 )
{
	TextureManager::getSingletonPtr()->load( p_name, ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	Ogre::TexturePtr l_ptr = TextureManager::getSingletonPtr()->getByName( p_name );
	m_width = l_ptr->getWidth();
	m_height = l_ptr->getHeight();
	m_data = new ColourValue[ m_width * m_height];
	const PixelBox & l_box = l_ptr->getBuffer()->lock( Box( 0, 0, m_width, m_height ), HardwareBuffer::HBL_READ_ONLY );
	ColourValue * l_data = m_data;
	char * l_rawData = static_cast <char *>( l_box.data );
	size_t imax = m_width * m_height;

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		PixelUtil::unpackColour( l_data, l_ptr->getFormat(), l_rawData );
		l_data ++;
		l_rawData += PixelUtil::getComponentCount( l_ptr->getFormat() );
	}

	l_ptr->getBuffer()->unlock();
}

RawImage :: ~RawImage()
{
	delete [] m_data;
	RawImageManager::GetSingletonPtr()->RemoveElement( this );
}
