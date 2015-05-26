//***********************************************************************************************************
#ifndef ___MYLISTBOX_H___
#define ___MYLISTBOX_H___
//***********************************************************************************************************
#include <wx/listbox.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		class AutoCompletionListBox : public wxListBox
		{
		public:

			AutoCompletionListBox( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size,
								   const wxArrayString & choices );

		private:
			void _onMouseLeftDown( wxMouseEvent & p_event );
			void _onKeyDown( wxKeyEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************