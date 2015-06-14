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
#include "ObjectMaterial.h"
#include <OgrePixelFormat.h>
#include <OgreTexture.h>
#include <OgreColourValue.h>

ObjectMaterial::ObjectMaterial( const String & p_name )
	: named( p_name ),
		m_material( NULL ),
		m_created( false )
{
	Ogre::ResourcePtr l_ptr = MaterialManager::getSingletonPtr()->getByName( m_name );

	if ( ! l_ptr.isNull() )
	{
		m_material = static_cast <Material *>( l_ptr.getPointer() );
	}
}

ObjectMaterial::~ObjectMaterial()
{
	if ( m_material != NULL && m_created )
	{
		MaterialManager::getSingletonPtr()->remove( m_material->getName() );
		m_material = NULL;
	}
}

void ObjectMaterial::Create()
{
	m_created = true;
	m_material = static_cast <Material *>( MaterialManager::getSingletonPtr()->create( m_name, "Internal", true ).getPointer() );
}

void ObjectMaterial::CreateCopyFrom( const String & p_name )
{
	m_created = true;
	MaterialPtr l_ptr = MaterialManager::getSingletonPtr()->getByName( p_name );

	if ( l_ptr.isNull() )
	{
		return;
	}

	m_material = l_ptr->clone( m_name ).getPointer();
	Pass * l_pass = m_material->getTechnique( 0 )->getPass( 0 );

	for ( unsigned short i = 0 ; i < l_pass->getNumTextureUnitStates() ; i ++ )
	{
		TextureUnitState * l_tus = l_pass->getTextureUnitState( i );
		Ogre::TexturePtr l_srcPtr = l_tus->_getTexturePtr();
		Ogre::TexturePtr l_ptr = TextureManager::getSingletonPtr()->createManual( "__" + m_name + "_texture_" + StringConverter::toString( i ), "__INTERNAL__",
								 l_srcPtr->getTextureType(), Ogre::uint( l_srcPtr->getWidth() ), Ogre::uint( l_srcPtr->getHeight() ),
								 Ogre::uint( l_srcPtr->getDepth() ), int( l_srcPtr->getNumMipmaps() ), l_srcPtr->getFormat() );
		l_ptr->getBuffer()->blit( l_srcPtr->getBuffer() );
		l_tus->setTextureName( l_ptr->getName() );
	}
}

void ObjectMaterial::DrawRect( unsigned int p_textureIndex, const ColourValue & p_colour, const Vector2 & p_coords, const Vector2 & p_size )
{
	genlib_assert( m_material != NULL );
	TextureUnitState * l_tus = m_material->getTechnique( 0 )->getPass( 0 )->getTextureUnitState( p_textureIndex );
	Ogre::TexturePtr l_ptr = l_tus->_getTexturePtr();
	const PixelBox & l_box = l_ptr->getBuffer()->lock( Box( 0, 0, l_ptr->getWidth(), l_ptr->getHeight() ), HardwareBuffer::HBL_NORMAL );
	size_t l_startX = size_t( p_coords.x * l_ptr->getWidth() );
	size_t l_startY = size_t( p_coords.y * l_ptr->getHeight() );
	size_t l_endX = size_t( l_startX + p_size.x * l_ptr->getWidth() );
	size_t l_endY = size_t( l_startY + p_size.y * l_ptr->getHeight() );

	for ( size_t i = l_startX ; i < l_endX ; i ++ )
	{
		for ( size_t j = l_startY ; j < l_endY ; j ++ )
		{
			size_t l_index = ( i * 4 ) + ( j * 4 * l_box.rowPitch );
			Ogre::uint8 * l_dest = static_cast <Ogre::uint8 *>( l_box.data ) + l_index;
			PixelUtil::packColour( p_colour, l_ptr->getFormat(), l_dest );
		}
	}

	l_ptr->getBuffer()->unlock();
}

void ObjectMaterial::DrawCircle( unsigned int p_textureIndex, const ColourValue & p_colour, const Vector2 & p_center, Real p_radius )
{
//	std::cout << "DrawCircle : " << p_center << " // radius : " << p_radius << std::endl;
	genlib_assert( m_material != NULL );
	TextureUnitState * l_tus = m_material->getTechnique( 0 )->getPass( 0 )->getTextureUnitState( p_textureIndex );
	Ogre::TexturePtr l_ptr = l_tus->_getTexturePtr();
	const PixelBox & l_box = l_ptr->getBuffer()->lock( Box( 0, 0, l_ptr->getWidth(), l_ptr->getHeight() ), HardwareBuffer::HBL_NORMAL );
	size_t l_startX = size_t( p_center.x - p_radius ) * l_ptr->getWidth();
	size_t l_startY = size_t( p_center.y - p_radius ) * l_ptr->getHeight();
	size_t l_endX = size_t( p_center.x + p_radius ) * l_ptr->getWidth();
	size_t l_endY = size_t( p_center.y + p_radius ) * l_ptr->getHeight();
	size_t l_square = size_t( p_radius * l_ptr->getWidth() * p_radius * l_ptr->getWidth() );
	Vector2 l_center( Real( 0.5 * ( l_startX + l_endX ) ), Real( 0.5 * ( l_startY + l_endY ) ) );

	for ( size_t i = l_startX ; i < l_endX ; i ++ )
	{
		for ( size_t j = l_startY ; j < l_endY ; j ++ )
		{
			if ( l_square > ( ( i - l_center.x ) * ( i - l_center.x ) + ( j - l_center.y ) * ( j - l_center.y ) ) )
			{
				size_t l_index = ( i * 4 ) + ( j * 4 * l_box.rowPitch );
				Ogre::uint8 * l_dest = static_cast <Ogre::uint8 *>( l_box.data ) + l_index;
				PixelUtil::packColour( p_colour, l_ptr->getFormat(), l_dest );
			}
		}
	}

	l_ptr->getBuffer()->unlock();
}

void ObjectMaterial::_drawTriangleHighPart( const PixelBox & p_box, const Ogre::TexturePtr & p_texture, const ColourValue & p_colour, const Vector2 & p_top, const Vector2 & p_lowLeft, const Vector2 & p_lowRight )
{
	Real l_startX = p_lowLeft.x * p_box.getWidth();
	Real l_endX = p_lowRight.x * p_box.getWidth();
	size_t l_startY = Math::IFloor( p_lowLeft.y * p_box.getHeight() );
	size_t l_endY = Math::IFloor( p_top.y * p_box.getHeight() );
	Real l_ratioStart = p_box.getWidth() * ( p_top.x - p_lowLeft.x ) / static_cast<Real>( l_endY - l_startY );
	Real l_ratioEnd = p_box.getWidth() * ( p_top.x - p_lowRight.x ) /  static_cast<Real>( l_endY - l_startY );

	for ( size_t i = l_startY ; i < l_endY ; i ++ )
	{
		for ( Real j = l_startX - Real( 0.5 ) ; j <= l_endX ; j += Real( 1.0 ) )
		{
			size_t l_index = ( Math::IFloor( j ) * 4 ) + ( i * 4 * p_box.rowPitch );
			Ogre::uint8 * l_dest = static_cast <Ogre::uint8 *>( p_box.data ) + l_index;
			PixelUtil::packColour( p_colour, p_texture->getFormat(), l_dest );
		}

		l_startX += l_ratioStart;
		l_endX += l_ratioEnd;
	}
}

void ObjectMaterial::_drawTriangleLowPart( const PixelBox & p_box, const Ogre::TexturePtr & p_texture,
		const ColourValue & p_colour, const Vector2 & p_topLeft,
		const Vector2 & p_bottom, const Vector2 & p_topRight )
{
	Real l_startX = p_topLeft.x * p_box.getWidth();
	Real l_endX = p_topRight.x * p_box.getWidth();
	size_t l_startY = Math::IFloor( p_topLeft.y * p_box.getHeight() );
	size_t l_endY = Math::IFloor( p_bottom.y * p_box.getHeight() );
	Real l_ratioStart = p_box.getWidth() * ( p_bottom.x - p_topLeft.x ) / static_cast<Real>( l_startY - l_endY );
	Real l_ratioEnd = p_box.getWidth() * ( p_bottom.x - p_topRight.x ) /  static_cast<Real>( l_startY - l_endY );

	for ( size_t i = l_startY ; i > l_endY ; i -- )
	{
		for ( Real j = l_startX - Real( 0.5 ) ; j <= l_endX ; j += Real( 1.0 ) )
		{
			size_t l_index = ( Math::IFloor( j ) * 4 ) + ( i * 4 * p_box.rowPitch );
			Ogre::uint8 * l_dest = static_cast <Ogre::uint8 *>( p_box.data ) + l_index;
			PixelUtil::packColour( p_colour, p_texture->getFormat(), l_dest );
		}

		l_startX += l_ratioStart;
		l_endX += l_ratioEnd;
	}
}

void ObjectMaterial::DrawTriangle( unsigned int p_textureIndex, const ColourValue & p_colour, const Vector2 & p_a, const Vector2 & p_b, const Vector2 & p_c )
{
	genlib_assert( m_material != NULL );
	TextureUnitState * l_tus = m_material->getTechnique( 0 )->getPass( 0 )->getTextureUnitState( p_textureIndex );
	Ogre::TexturePtr l_ptr = l_tus->_getTexturePtr();
	const PixelBox & l_box = l_ptr->getBuffer()->lock( Box( 0, 0, l_ptr->getWidth(), l_ptr->getHeight() ), HardwareBuffer::HBL_NORMAL );
	Vector2 l_mid;
	Vector2 l_top;
	Vector2 l_bottom;

	if ( p_a.y >= p_b.y && p_a.y <= p_c.y )
	{
		l_mid = p_a;
		l_top = p_c;
		l_bottom = p_b;
	}
	else if ( p_a.y >= p_c.y && p_a.y <= p_b.y )
	{
		l_mid = p_a;
		l_top = p_b;
		l_bottom = p_c;
	}
	else if ( p_b.y >= p_a.y && p_b.y <= p_c.y )
	{
		l_mid = p_b;
		l_top = p_c;
		l_bottom = p_a;
	}
	else if ( p_b.y >= p_c.y && p_b.y <= p_a.y )
	{
		l_mid = p_b;
		l_top = p_a;
		l_bottom = p_c;
	}
	else if ( p_c.y >= p_a.y && p_c.y <= p_b.y )
	{
		l_mid = p_c;
		l_top = p_b;
		l_bottom = p_a;
	}
	else if ( p_c.y >= p_b.y && p_c.y <= p_a.y )
	{
		l_mid = p_c;
		l_top = p_a;
		l_bottom = p_b;
	}

	if ( l_mid.y == l_top.y )
	{
		if ( l_mid.x > l_top.x )
		{
			_drawTriangleLowPart( l_box, l_ptr, p_colour, l_top, l_bottom, l_mid );
		}
		else
		{
			_drawTriangleLowPart( l_box, l_ptr, p_colour, l_mid, l_bottom, l_top );
		}

		l_ptr->getBuffer()->unlock();
		return;
	}

	if ( l_mid.y == l_bottom.y )
	{
		if ( l_mid.x > l_bottom.x )
		{
			_drawTriangleHighPart( l_box, l_ptr, p_colour, l_top, l_bottom, l_mid );
		}
		else
		{
			_drawTriangleHighPart( l_box, l_ptr, p_colour, l_top, l_mid, l_bottom );
		}

		l_ptr->getBuffer()->unlock();
		return;
	}

	Real l_prorata = ( l_top.y - l_mid.y ) / ( l_bottom.y - l_top.y );
	Vector2 l_tempPoint = l_top - ( l_bottom - l_top ) * l_prorata;

	if ( l_tempPoint.x > l_mid.x )
	{
		std::swap( l_tempPoint, l_mid );
	}

	_drawTriangleLowPart( l_box, l_ptr, p_colour, l_tempPoint, l_bottom, l_mid );
	_drawTriangleHighPart( l_box, l_ptr, p_colour, l_top, l_tempPoint, l_mid );
	l_ptr->getBuffer()->unlock();
}

void ObjectMaterial::DrawLine( unsigned int p_textureIndex, const ColourValue & p_colour, const Vector2 & p_start, const Vector2 & p_end, Real p_width )
{
	Vector2 l_a, l_b, l_c, l_d;
	Vector2 l_diff = p_end - p_start;
	l_diff.normalise();

	if ( l_diff.x < 0.0 )
	{
		l_diff *= ( -1.0 );
	}

	l_diff *= p_width;
	l_a = p_start + Vector2( -l_diff.y, l_diff.x );
	l_b = p_start - Vector2( -l_diff.y, l_diff.x );
	l_c = p_end + Vector2( -l_diff.y, l_diff.x );
	l_d = p_end - Vector2( -l_diff.y, l_diff.x );
	DrawTriangle( p_textureIndex, p_colour, l_a, l_b, l_c );
	DrawTriangle( p_textureIndex, p_colour, l_b, l_c, l_d );
}

void ObjectMaterial::DrawCircleEndedLine( unsigned int p_textureIndex, const ColourValue & p_colour, const Vector2 & p_start, const Vector2 & p_end, Real p_width )
{
	DrawCircle( p_textureIndex, p_colour, p_start, p_width );
	DrawCircle( p_textureIndex, p_colour, p_end, p_width );
	Vector2 l_a, l_b, l_c, l_d;
	Vector2 l_diff = p_end - p_start;
	l_diff.normalise();

	if ( l_diff.x < 0.0 )
	{
		l_diff *= ( -1.0 );
	}

	l_diff *= p_width;
	l_a = p_start + Vector2( -l_diff.y, l_diff.x );
	l_b = p_start - Vector2( -l_diff.y, l_diff.x );
	l_c = p_end + Vector2( -l_diff.y, l_diff.x );
	l_d = p_end - Vector2( -l_diff.y, l_diff.x );
	DrawTriangle( p_textureIndex, p_colour, l_a, l_b, l_c );
	DrawTriangle( p_textureIndex, p_colour, l_b, l_c, l_d );
}

void ObjectMaterial::CopyTexture( unsigned int p_textureIndex, const String & p_textureName )
{
	TextureUnitState * l_tus = m_material->getTechnique( 0 )->getPass( 0 )->getTextureUnitState( p_textureIndex );
	Ogre::TexturePtr l_ptr = l_tus->_getTexturePtr();
	Ogre::TexturePtr l_tempPtr = TextureManager::getSingletonPtr()->getByName( p_textureName );

	if ( l_tempPtr.isNull() || l_ptr.isNull() )
	{
		return;
	}

	l_tempPtr->load();
	l_ptr->load();
	l_ptr->getBuffer()->blit( l_tempPtr->getBuffer() );
}

