/*
See LICENSE file in root folder
*/
#ifndef ___Troll_Application___
#define ___Troll_Application___

#include <wx/app.h>

#include "TrollEditorPrerequisites.h"

#include "GUI/TrollEditorGuiPrerequisites.h"

namespace Troll
{
	class Application
		: public wxApp
	{
		friend class ProjectFrame;

	public:
		Application();
		~Application();

		virtual bool OnInit();
		virtual int OnExit();

		GUI::MainFrame * GetMainFrame()const
		{
			return m_mainFrame;
		}

		GUI::ProjectFrame * GetProjectFrame()const
		{
			return m_projectFrame;
		}

	public:
		GUI::MainFrame * m_mainFrame{ nullptr };
		GUI::ProjectFrame * m_projectFrame{ nullptr };
	};
}

wxDECLARE_APP( Troll::Application );

#endif
