/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ArrowPanel___
#define ___Troll_ArrowPanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class ArrowPanel
				: public wxPanel
			{
			public:
				ArrowPanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~ArrowPanel();

				inline void SetBoundPanel( wxPanel * p_panel )
				{
					m_boundPanel = p_panel;
				}
				inline void SetMouseLeftDown( bool p_leftDown )
				{
					m_mouseLeftDown = p_leftDown;
				}

			private:
				DECLARE_EVENT_TABLE()
				void OnPaint( wxPaintEvent & p_event );
				void OnMouseLeftDown( wxMouseEvent & p_event );
				void OnMouseLeftUp( wxMouseEvent & p_event );
				void OnMouseMove( wxMouseEvent & p_event );
				void OnMouseLeave( wxMouseEvent & p_event );
				void OnKillFocus( wxFocusEvent & p_event );

			private:
				bool m_mouseLeftDown;
				wxPanel * m_boundPanel;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
