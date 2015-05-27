
#ifndef ___Troll_TimeLineContainer___
#define ___Troll_TimeLineContainer___

#include "../Module_GUI.h"
#include <wx/scrolwin.h>

namespace Troll
{
	namespace GUI
	{
		class TimeLineContainer : public wxScrolledWindow
		{
		protected:
			int m_precPosition;
		public:
			TimeLineContainer( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition,
							   const wxSize & p_size = wxDefaultSize );
			~TimeLineContainer();

		private:
			void _onScroll( wxScrollWinEvent & p_event );
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

