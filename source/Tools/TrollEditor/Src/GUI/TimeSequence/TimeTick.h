/*
See LICENSE file in root folder
*/
#ifndef ___Troll_TimeTick___
#define ___Troll_TimeTick___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class TimeTick
				: public wxPanel
			{
			public:
				TimeTick( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );

			private:
				void OnLeftMouseDown( wxMouseEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );

				DECLARE_EVENT_TABLE()

			protected:
				TimePanel * m_panel;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
