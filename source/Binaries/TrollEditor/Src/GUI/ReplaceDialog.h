//***********************************************************************************************************
#ifndef ___Troll_ReplaceDialog___
#define ___Troll_ReplaceDialog___
//***********************************************************************************************************
#include "Module_GUI.h"
#include <wx/frame.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class ReplaceDialog : public wxFrame
		{
		private:
			TextPanel * m_textPanel;
			wxTextCtrl * m_textToReplace;
			wxTextCtrl * m_replacementText;
			wxButton * m_find;
			wxButton * m_replace;
			wxButton * m_replaceAll;
			bool m_first;
			wxString m_text;

		public:
			ReplaceDialog( wxWindow * p_parent, TextPanel * p_textPanel );

		private:
			void _onFind( wxCommandEvent & p_event );
			void _onReplace( wxCommandEvent & p_event );
			void _onReplaceAll( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************