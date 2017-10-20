/*
See LICENSE file in root folder
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
