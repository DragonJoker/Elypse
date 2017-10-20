/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_GUI_SCRIPT_DIALOG_H___
#define ___TROLL_GUI_SCRIPT_DIALOG_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/dialog.h>

namespace Troll
{
	namespace GUI
	{
		class ScriptDialog
			: public wxDialog
		{
		public:
			ScriptDialog( wxWindow * p_parent, wxString const & p_title, wxString const & p_caption, wxString const & p_value );

			inline wxString GetValue()const
			{
				return m_script->GetValue();
			}

		private:
			DECLARE_EVENT_TABLE()
			void OnOK( wxCommandEvent & p_event );
			void OnCancel( wxCommandEvent & p_event );

		private:
			wxTextCtrl * m_script;
		};
	}
}

#endif
