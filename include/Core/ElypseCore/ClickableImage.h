/*
See LICENSE file in root folder
*/
#ifndef ___CLICKABLE_IMAGE_H___
#define ___CLICKABLE_IMAGE_H___

#include "ClickableShape.h"

namespace Elypse
{
	namespace Gui
	{
		class d_dll_export ClickableImage
			: public ClickableShape
		{
		public:
			ClickableImage( String const & p_textureName, Real p_x, Real p_y );
			virtual ~ClickableImage();

		public:
			virtual bool TestClick( Real p_x, Real p_y );

		protected:
			std::shared_ptr< RawImage > m_raw;
		};
	}
}

#endif
