
#ifndef ___SceneDependenciesFrame___
#define ___SceneDependenciesFrame___

#include <wx/dialog.h>
#include "Module_GUI.h"
#include "../Project/Module_Project.h"
#include <wx/frame.h>

namespace Troll
{
	namespace GUI
	{
		enum SceneDependenciesFrameIDs
		{
			sdfSceneSelector,
			sdfSceneDependencies,
			sdfOkBtn
		};

		class SceneDependenciesFrame : public wxFrame
		{
		private:
			wxComboBox * m_sceneSelector;
			wxCheckListBox * m_sceneDependencies;
			wxButton * m_okBtn;

			SceneMap m_scenes;
			TrollScene * m_selectedScene;

		public:
			SceneDependenciesFrame( wxWindow * p_parent, TrollScene * p_scene = NULL,
									const wxString & title = wxT( "Dépendances des scènes" ), const wxPoint & pos = wxDefaultPosition );
			~SceneDependenciesFrame();

			void SetScene( TrollScene * p_scene );

		private:
			void _initialise();
			void _populateSceneDependencies();

			void _onClose( wxCloseEvent & p_event );
			void _onCheckScene( wxCommandEvent & p_event );
			void _onSelectScene( wxCommandEvent & p_event );
			void _onOK( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

