
#ifndef ___ProjectProperties___
#define ___ProjectProperties___

#include <wx/frame.h>
#include "../Module_GUI.h"
#include "../../Project/Module_Project.h"

namespace Troll
{
	namespace GUI
	{
		class ProjectProperties : public wxFrame
		{
		protected:
			Project * m_project;

			wxStaticText * m_projectName;
			wxComboBox * m_backgroundType;
			wxButton * m_background;
			wxCheckBox * m_shadows;
			wxComboBox * m_fsaa;
			wxCheckBox * m_showDebugConsole;
			wxCheckBox * m_showFPS;
			wxTextCtrl * m_startupScript;
			wxButton * m_back;

		public:
			ProjectProperties( Project * p_project, wxWindow * parent, const wxPoint & pos = wxDefaultPosition );
			~ProjectProperties();

			void UpdateProperties();

		private:
			void _onSelectBackgroundType( wxCommandEvent & p_event );
			void _onBackground( wxCommandEvent & p_event );
			void _onShadowsChange( wxCommandEvent & p_event );
			void _onSelectFSAA( wxCommandEvent & p_event );
			void _onShowConsoleChange( wxCommandEvent & p_event );
			void _onShowFPSChange( wxCommandEvent & p_event );
			void _onStartupScriptChange( wxCommandEvent & p_event );
			void _onBack( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

