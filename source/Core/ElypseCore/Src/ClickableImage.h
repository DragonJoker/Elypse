#ifndef ___CLICKABLE_IMAGE_H___
#define ___CLICKABLE_IMAGE_H___

#include "ClickableShape.h"

namespace EMuse
{
	namespace Gui
	{
		class d_dll_export ClickableImage : public ClickableShape
		{
		protected:
			RawImage * m_raw;

		public:
			ClickableImage( const String & p_textureName, Real p_x, Real p_y );
			virtual ~ClickableImage();

		public:
			virtual bool TestClick( Real p_x, Real p_y );
		};
	}
}

#endif
