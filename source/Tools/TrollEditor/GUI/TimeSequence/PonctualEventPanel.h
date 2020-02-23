/*
See LICENSE file in root folder
*/
#ifndef ___Troll_PonctualEventPanel___
#define ___Troll_PonctualEventPanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class PonctualEventPanel
				: public wxPanel
			{
			public:
				PonctualEventPanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~PonctualEventPanel();

				void SetPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event, Real p_time );
				void Replace( bool p_init = false );
				void SetTop( int p_top );

				void SetMouseLeftDown( bool p_leftDown )
				{
					m_mouseLeftDown = p_leftDown;
				}

			private:
				void _release( bool p_timeChanged );
				void _showContextMenu( wxPoint const & p_pos );

				DECLARE_EVENT_TABLE()
				void OnLeftMouseDown( wxMouseEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );
				void OnRightMouseUp( wxMouseEvent & p_event );
				void OnMouseEnter( wxMouseEvent & p_event );
				void OnMouseMove( wxMouseEvent & p_event );
				void OnMouseLeave( wxMouseEvent & p_event );
				void OnDeleteEvent( wxCommandEvent & p_event );
				void OnKillFocus( wxFocusEvent & p_event );

			protected:
				ProjectComponents::Temporal::TrollPonctualEvent * m_event;
				ArrowPanel * m_arrow;
				Real m_time;
				bool m_mouseLeftDown;
				int m_top;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
