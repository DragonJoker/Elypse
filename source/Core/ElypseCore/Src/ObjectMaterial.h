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
#ifndef ___OBJECT_TEXTURE___
#define ___OBJECT_TEXTURE___

#include "Module_SpecialEffects.h"

#include <Manager.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export ObjectMaterial : public named
		{
		private:
			MaterialTarget * m_target;
			Material * m_material;
			bool m_created;

		public:
			ObjectMaterial( String const & p_name );
			~ObjectMaterial();
		private:
			void _drawTriangleHighPart( const PixelBox & p_box, const Ogre::TexturePtr & p_texture, const ColourValue & p_colour, const Vector2 & p_top, const Vector2 & p_lowLeft, const Vector2 & p_lowRight );
			void _drawTriangleLowPart( const PixelBox & p_box, const Ogre::TexturePtr & p_texture, const ColourValue & p_colour, const Vector2 & p_topLeft, const Vector2 & p_bottom, const Vector2 & p_topRight );

		public:
			void Create();
			void CreateCopyFrom( String const & p_name );

			void CopyTexture( uint32_t p_textureIndex, String const & p_textureName );

			void DrawRect( uint32_t p_textureIndex, const ColourValue & p_colour, const Vector2 & p_coords, const Vector2 & p_size );
			void DrawLine( uint32_t p_textureIndex, const ColourValue & p_colour, const Vector2 & p_start, const Vector2 & p_end, Real p_width );
			void DrawCircleEndedLine( uint32_t p_textureIndex, const ColourValue & p_colour, const Vector2 & p_start, const Vector2 & p_end, Real p_width );
			void DrawCircle( uint32_t p_textureIndex, const ColourValue & p_colour, const Vector2 & p_coords, Real p_radius );
			void DrawTriangle( uint32_t p_textureIndex, const ColourValue & p_colour, const Vector2 & p_a, const Vector2 & p_b, const Vector2 & p_c );
		};
	}
}

#endif
