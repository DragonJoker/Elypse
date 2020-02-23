/*
See LICENSE file in root folder
*/

#ifndef ___Troll_ReplaceDialog___
#define ___Troll_ReplaceDialog___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/frame.h>

namespace Troll
{
	namespace GUI
	{
		class ReplaceDialog
			: public wxFrame
		{
		public:
			ReplaceDialog( wxWindow * p_parent, wxStcTextEditor * p_textPanel );

		private:
			DECLARE_EVENT_TABLE()
			void OnFind( wxCommandEvent & p_event );
			void OnReplace( wxCommandEvent & p_event );
			void OnReplaceAll( wxCommandEvent & p_event );

		private:
			wxStcTextEditor * m_textPanel;
			wxTextCtrl * m_textToReplace;
			wxTextCtrl * m_replacementText;
			bool m_first;
			wxString m_text;
		};
	}
}

#endif
