#ifndef ___ELYPSE_APP_H___
#define ___ELYPSE_APP_H___

#include "Module_Binary.h"

#include <Module_Main.h>

namespace EMuse
{
	namespace Binary
	{
		class ElypseApp : public wxApp
		{
		private:
			ElypseFrame * m_frame;
			ElypseCtrl * m_control;
			EMusePlugin_Binary * m_plugin;
			EMuseInstance * m_ogre;

			unsigned int m_width;
			unsigned int m_height;

		private:
			void _readConfigFile( const String & p_filepath );

		public:
			virtual bool OnInit();
			virtual bool InitParams( const String & p_filepath );
			virtual int OnExit();

			void Close();
		};
	}
}

#endif
