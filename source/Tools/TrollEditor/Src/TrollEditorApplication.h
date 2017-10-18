/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
