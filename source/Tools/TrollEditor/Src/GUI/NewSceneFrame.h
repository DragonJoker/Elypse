
#ifndef _NewSceneFrame_H
#define _NewSceneFrame_H

#include <wx/dialog.h>

#include "Module_GUI.h"
#include "Project/Module_Project.h"

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
			wxButton * m_okBtn;
			wxButton * m_cancelBtn;

		public:
			NewSceneFrame( wxWindow * p_parent, const wxString & title = wxT( "Nouvelle Scène" ), const wxPoint & pos = wxDefaultPosition );
			~NewSceneFrame();

			wxString GetSceneName();

		private:
			void _onSceneChange( wxCommandEvent & p_event );
			void _onOK( wxCommandEvent & p_event );
			void _onCancel( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

