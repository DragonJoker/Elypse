/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ContinuousEventPanel___
#define ___Troll_ContinuousEventPanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class ContinuousEventPanel
				: public wxPanel
			{
			public:
				ContinuousEventPanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~ContinuousEventPanel();

				void SetContinuousEvent( ProjectComponents::Temporal::TrollContinuousEvent * p_event );
				void Replace( bool p_init = false );
				void SetTop( int p_top );

			private:
				void _showContextMenu( wxPoint const & p_pos );
				void _release( bool p_timeChanged );

				DECLARE_EVENT_TABLE()
				void OnLeftMouseDown( wxMouseEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );
				void OnRightMouseUp( wxMouseEvent & p_event );
				void OnMouseEnter( wxMouseEvent & p_event );
				void OnMouseLeave( wxMouseEvent & p_event );
				void OnMouseMove( wxMouseEvent & p_event );
				void OnNewFrame( wxCommandEvent & p_event );
				void OnDeleteEvent( wxCommandEvent & p_event );
				void OnKillFocus( wxFocusEvent & p_event );

			protected:
				ProjectComponents::Temporal::TrollContinuousEvent * m_event;
				ArrowPanel * m_arrow;
				Real m_time;
				bool m_mouseLeftDown;
				bool m_hasMoved;
				int m_top;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
