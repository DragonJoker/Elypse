/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
