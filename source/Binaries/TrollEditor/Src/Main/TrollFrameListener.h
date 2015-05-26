//********************************************************************************************************************
#ifndef ___TROLL_FRAME_LISTENER___
#define ___TROLL_FRAME_LISTENER___
//********************************************************************************************************************
#include <EMuseFrameListener.h>
#include "TrollInstance.h"
#include "../Script/TrollScriptEngine.h"
//********************************************************************************************************************
namespace Troll
{
	namespace Main
	{
		class TrollFrameListener : public EMuseFrameListener
		{
		protected:
			TrollInstance * m_ogre;

		protected:
			virtual void _setupInterfaces();
			virtual void _setupScript();

		public:
			TrollFrameListener( TrollInstance * p_oa , RenderWindow * m_window , const String & p_baseURL ,
								const String & p_installPath , const String & p_appStringIndex );

			TrollFrameListener( TrollInstance * p_oa , TrollFrameListener * p_ofl ,
								RenderWindow * p_window , const String & p_instanceNum );
			virtual ~TrollFrameListener();

		public:
			virtual void Initialise();
			bool frameStarted( Real p_timeSinceLastFrame );
		};
	}
}
//********************************************************************************************************************
#endif
//********************************************************************************************************************
