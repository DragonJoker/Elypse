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

#include "ClickableImage.h"
#include "RawImage.h"

#include <OgreTextureManager.h>
#include <OgreTexture.h>

ClickableImage::ClickableImage( String const & p_name, Real p_width, Real p_height )
{
	genlib_assert( ! p_name.empty() );
	m_raw = RawImageManager::GetSingletonPtr()->GetElementByName( p_name );

	if ( !m_raw )
	{
		m_raw = RawImageManager::GetSingletonPtr()->CreateElement( p_name );
	}

	genlib_assert( m_raw != nullptr );
	m_raw->Use();
	m_x = p_width;
	m_y = p_height;
}

ClickableImage::~ClickableImage()
{
	m_raw->Release();
}

bool ClickableImage::TestClick( Real p_x, Real p_y )
{
	p_x /= m_x;
	p_y /= m_y;
	return m_raw->GetPixelAt( p_x, p_y ).a > 0.5;
}
