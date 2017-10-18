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
