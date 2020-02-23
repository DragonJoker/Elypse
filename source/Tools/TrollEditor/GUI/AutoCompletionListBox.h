/*
See LICENSE file in root folder
*/
#ifndef ___MYLISTBOX_H___
#define ___MYLISTBOX_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/listbox.h>

namespace Troll
{
	namespace GUI
	{
		class AutoCompletionListBox
			: public wxListBox
		{
		public:
			AutoCompletionListBox( wxWindow * parent, wxWindowID id, wxPoint const & pos, wxSize const & size, const wxArrayString & choices );

		private:
			DECLARE_EVENT_TABLE()
			void OnMouseLeftDown( wxMouseEvent & p_event );
			void OnKeyDown( wxKeyEvent & p_event );
		};
	}
}

#endif

