
#ifndef ___Troll_LogCtrl___
#define ___Troll_LogCtrl___

#include <wx/listbox.h>

#include "Module_GUI.h"

namespace Troll
{
	namespace GUI
	{
		class LogCtrl
			: public wxListBox
		{
		public:
			LogCtrl( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style );

		private:
			DECLARE_EVENT_TABLE()
			void OnDoubleClick( wxMouseEvent & p_event );
		};
	}
}

#endif

