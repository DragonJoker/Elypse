/*
See LICENSE file in root folder
*/

#ifndef _NewSceneFrame_H
#define _NewSceneFrame_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"

#include <wx/dialog.h>

namespace Troll
{
	namespace GUI
	{
		enum NewSceneFrameIDs
		{
			ID_clBtn,
			ID_okBtn,
			ID_snETxt,
		};

		class NewSceneFrame : public wxDialog
		{
		private:
			// GUI controls
			wxTextCtrl * m_sceneName;

		public:
			NewSceneFrame( ProjectFrame * p_parent, wxString const & title = wxT( "Nouvelle Scène" ), wxPoint const & pos = wxDefaultPosition );
			~NewSceneFrame();

			wxString GetSceneName();

		private:
			void _onSceneChange( wxCommandEvent & p_event );
			void _onOK( wxCommandEvent & p_event );
			void _onCancel( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()

		private:
			ProjectFrame * m_projectFrame;
		};
	}
}

#endif

