/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_GUI_SCRIPT_DIALOG_H___
#define ___TROLL_GUI_SCRIPT_DIALOG_H___

#include "GUI/TrollEditorGuiPrerequisites.h"
#include "GUI/StcContext.h"
#include "GUI/StcTextEditor.h"

#include <wx/dialog.h>

namespace Troll
{
	namespace GUI
	{
		class ScriptDialog
			: public wxDialog
		{
		public:
			ScriptDialog( wxWindow * parent
				, wxString const & title
				, wxString const & caption
				, wxString const & value );

			inline wxString GetValue()const
			{
				return m_script->GetValue();
			}

		private:
			DECLARE_EVENT_TABLE()
			void OnOK( wxCommandEvent & event );
			void OnCancel( wxCommandEvent & event );

		private:
			wxStcTextEditor * m_script;
			StcContext m_stcContext{};
		};
	}
}

#endif
