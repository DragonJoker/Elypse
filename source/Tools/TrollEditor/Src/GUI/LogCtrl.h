/*
See LICENSE file in root folder
*/
#ifndef ___Troll_LogCtrl___
#define ___Troll_LogCtrl___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/listbox.h>

namespace Troll
{
	namespace GUI
	{
		class LogCtrl
			: public wxListBox
		{
		public:
			LogCtrl( wxWindow * parent, wxWindowID id, wxPoint const & pos, wxSize const & size, long style );

		private:
			DECLARE_EVENT_TABLE()
			void OnDoubleClick( wxMouseEvent & p_event );
		};
	}
}

#endif
