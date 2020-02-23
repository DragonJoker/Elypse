/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSE_APP_H___
#define ___ELYPSE_APP_H___

#include "ElypseBinaryPrerequisites.h"

#include <Module_Main.h>

namespace ElypseBinary
{
	class ElypseApp
		: public wxApp
	{
	private:
		ElypseFrame * m_frame;
		ElypseCtrl * m_control;
		GuiCommon::wxElypsePluginSPtr m_plugin;
		std::shared_ptr< ElypseInstance > m_elypse;

		uint32_t m_width;
		uint32_t m_height;

	private:
		void _readConfigFile( String const & p_filepath );

	public:
		virtual bool OnInit();
		virtual bool InitParams( String const & p_filepath );
		virtual int OnExit();

		void Close();
	};
}

#endif
