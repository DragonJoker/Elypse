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
#ifndef ___RAW_IMAGE_H___
#define ___RAW_IMAGE_H___

#include "Module_Gui.h"

#include <OgreColourValue.h>

#include <AutoSingleton.h>
#include <Manager.h>
#include <ReferenceCounted.h>

namespace Elypse
{
	namespace Gui
	{
		class d_dll_export RawImageManager
			: public AutoSingleton< RawImageManager >
			, public General::Templates::Manager< RawImage >
		{
		};

		class d_dll_export RawImage
			: public ReferenceCounted< RawImage >
			, public General::Theory::named
		{
		public:
			RawImage( String const & p_name );
			~RawImage();

		public:
			inline const Ogre::ColourValue & GetPixelAt( size_t p_x, size_t p_y )const
			{
				return m_data[p_x + m_width * p_y];
			}

			inline const Ogre::ColourValue & GetPixelAt( Real p_x, Real p_y )const
			{
				return GetPixelAt( size_t( p_x * ( m_width - 1 ) ), size_t( p_y * ( m_height - 1 ) ) );
			}

		protected:
			uint32_t m_width{ 0 };
			uint32_t m_height{ 0 };
			std::vector< Ogre::ColourValue > m_data;
			size_t m_refCount{ 0 };
		};
	}
}

#endif
