/*
See LICENSE file in root folder
*/
#ifndef ___Troll_TimeLineContainer___
#define ___Troll_TimeLineContainer___

#include "Module_Time.h"

#include <wx/scrolwin.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class TimeLineContainer
				: public wxScrolledWindow
			{
			public:
				TimeLineContainer( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~TimeLineContainer();

			private:
				DECLARE_EVENT_TABLE()
				void OnScroll( wxScrollWinEvent & p_event );

			protected:
				int m_precPosition;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
