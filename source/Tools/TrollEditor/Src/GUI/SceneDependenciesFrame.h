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

BEGIN_TROLL_GUI_NAMESPACE
{
	class SceneDependenciesFrame
		: public wxFrame
	{
	public:
		SceneDependenciesFrame( wxWindow * p_parent, TROLL_PROJECT_NAMESPACE::Scene * p_scene = NULL, const wxString & title = _( "Scenes dependencies" ), const wxPoint & pos = wxDefaultPosition );
		~SceneDependenciesFrame();

		void SetScene( TROLL_PROJECT_NAMESPACE::Scene * p_scene );

	private:
		void _initialise();
		void _populateSceneDependencies();

		DECLARE_EVENT_TABLE()
		void OnClose( wxCloseEvent & p_event );
		void OnCheckScene( wxCommandEvent & p_event );
		void OnSelectScene( wxCommandEvent & p_event );
		void OnOK( wxCommandEvent & p_event );

	private:
		wxComboBox * m_sceneSelector;
		wxCheckListBox * m_sceneDependencies;
		wxButton * m_okBtn;

		TROLL_PROJECT_NAMESPACE::SceneMap m_scenes;
		TROLL_PROJECT_NAMESPACE::Scene * m_selectedScene;
	};
}
END_TROLL_GUI_NAMESPACE

#endif

