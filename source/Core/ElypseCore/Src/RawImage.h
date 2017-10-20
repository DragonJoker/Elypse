/*
See LICENSE file in root folder
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
