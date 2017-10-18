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

#ifndef ___SceneDependenciesFrame___
#define ___SceneDependenciesFrame___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"

#include <wx/frame.h>

namespace Troll
{
	namespace GUI
	{
		class SceneDependenciesFrame
			: public wxDialog
		{
		public:
			SceneDependenciesFrame( wxWindow * p_parent
				, ProjectComponents::ScenePtr p_scene = nullptr
				, wxString const & title = _( "Scenes dependencies" )
				, wxPoint const & pos = wxDefaultPosition );
			~SceneDependenciesFrame();

			void SetScene( ProjectComponents::ScenePtr p_scene );

		private:
			void DoInitialise();
			void DoPopulateSceneDependencies();

			DECLARE_EVENT_TABLE()
			void OnClose( wxCloseEvent & p_event );
			void OnCheckScene( wxCommandEvent & p_event );
			void OnSelectScene( wxCommandEvent & p_event );
			void OnOK( wxCommandEvent & p_event );

		private:
			wxComboBox * m_sceneSelector{ nullptr };
			wxCheckListBox * m_sceneDependencies{ nullptr };

			ProjectComponents::SceneMap m_scenes;
			ProjectComponents::ScenePtr m_selectedScene{ nullptr };
		};
	}
}

#endif
