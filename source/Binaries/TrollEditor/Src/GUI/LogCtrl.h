//***********************************************************************************************************
#ifndef ___Troll_LogCtrl___
#define ___Troll_LogCtrl___
//***********************************************************************************************************
#include <wx/textctrl.h>
#include "Module_GUI.h"
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class LogCtrl : public wxTextCtrl
		{
		public:
			LogCtrl( wxWindow * parent, wxWindowID id,
					 const wxString & value, const wxPoint & pos,
					 const wxSize & size, long style );

		private:
			void _onDoubleClick( wxMouseEvent & p_event );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************