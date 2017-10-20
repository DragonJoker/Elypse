/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_FRAME_LISTENER___
#define ___TROLL_FRAME_LISTENER___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <ElypseFrameListener.h>

namespace Troll
{
	namespace ElypseRW
	{
		class TrollFrameListener
			: public ElypseFrameListener
		{
		public:
			TrollFrameListener( TrollInstance & p_instance, RenderWindow * m_window, String const & p_baseURL, String const & p_installPath, String const & p_appStringIndex );
			TrollFrameListener( TrollInstance & p_instance, TrollFrameListener * p_ofl, RenderWindow * p_window, String const & p_instanceNum );
			virtual ~TrollFrameListener();

			virtual void Initialise();
			bool frameStarted( Real p_timeSinceLastFrame );

		protected:
			virtual void _setupInterfaces();
			virtual void _setupScript();

		protected:
			TrollInstance * m_ogre;
		};
	}
}

#endif
