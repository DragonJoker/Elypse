/*
See LICENSE file in root folder
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
