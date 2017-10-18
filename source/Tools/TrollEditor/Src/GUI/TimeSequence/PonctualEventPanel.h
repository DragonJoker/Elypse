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
