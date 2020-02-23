/*
See LICENSE file in root folder
*/
#ifndef ___VIDEO_FACTORY_DIRECTSHOW_H___
#define ___VIDEO_FACTORY_DIRECTSHOW_H___

#include "VideoFactory.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoFactory_DirectShow : public VideoFactory
		{

		public:
			VideoFactory_DirectShow();
			virtual ~VideoFactory_DirectShow();

		private:
			virtual void Initialise();
			virtual void Update();
			virtual VideoImplementation * Create( VideoObject & p_owner );
		};
	}
}

#endif
