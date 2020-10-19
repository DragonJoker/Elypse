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
			SceneDependenciesFrame( wxWindow * parent
				, ProjectComponents::ScenePtr scene = nullptr
				, wxString const & title = _( "Scenes dependencies" )
				, wxPoint const & pos = wxDefaultPosition );
			~SceneDependenciesFrame();

			void SetScene( ProjectComponents::ScenePtr scene );

		private:
			void DoInitialise();
			void DoPopulateSceneDependencies();

			DECLARE_EVENT_TABLE()
			void OnClose( wxCloseEvent & event );
			void OnCheckScene( wxCommandEvent & event );
			void OnSelectScene( wxCommandEvent & event );
			void OnOK( wxCommandEvent & event );

		private:
			wxComboBox * m_sceneSelector{};
			wxListBox * m_sceneDependencies{};

			ProjectComponents::SceneMap m_scenes;
			ProjectComponents::ScenePtr m_selectedScene;
		};
	}
}

#endif
