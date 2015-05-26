#ifndef ___RAW_IMAGE_H___
#define ___RAW_IMAGE_H___

#include "Module_Gui.h"

#include <OgreColourValue.h>

#include <AutoSingleton.h>
#include <Manager.h>
#include <ReferenceCounted.h>

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export RawImageManager : public AutoSingleton<RawImageManager>, public General::Templates::Manager<RawImage>
		{
		};

		class d_dll_export RawImage : public ReferenceCounted<RawImage>, public General::Theory::named
		{
		protected:
			size_t m_width;
			size_t m_height;

			Ogre::ColourValue * m_data;

			size_t m_refCount;

		public:
			RawImage( const String & p_name );
			~RawImage();

		public:
			inline const Ogre::ColourValue & GetPixelAt( size_t p_x, size_t p_y )const
			{
				return m_data[p_x + m_width * p_y];
			}

			inline const Ogre::ColourValue & GetPixelAt( Real p_x, Real p_y )const
			{
				return GetPixelAt( static_cast<size_t>( p_x * ( m_width - 1 ) ),
								   static_cast<size_t>( p_y * ( m_height - 1 ) ) );
			}
		};
	}
}

#endif
