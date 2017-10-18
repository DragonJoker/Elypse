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
#ifndef ___Troll_TimePanel___
#define ___Troll_TimePanel___

#include "Module_Time.h"

#include <wx/panel.h>

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			class TimePanel
				: public wxPanel
			{
			public:
				TimePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id = wxID_ANY, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
				~TimePanel();

				void IncrementTickPosition( int p_left );
				void UpdateTick( int p_left, bool p_updateSequence = false );

				inline void SetFirst( float p_first )
				{
					m_first = p_first;
					Refresh();
				}
				inline void IncrementFirst( float p_inc )
				{
					m_first += p_inc;
					Refresh();
				}
				inline void DecrementFirst( float p_dec )
				{
					m_first -= p_dec;
					Refresh();
				}
				inline void SetLeftDown( bool p_down )
				{
					m_leftDown = p_down;
				}
				inline float GetFirst()const
				{
					return m_first;
				}

			private:
				DECLARE_EVENT_TABLE()
				void OnPaint( wxPaintEvent & p_event );
				void OnLeftMouseDown( wxMouseEvent & p_event );
				void OnLeftMouseUp( wxMouseEvent & p_event );
				void OnMouseMove( wxMouseEvent & p_event );

			protected:
				int m_totalWidth;
				float m_scale;
				float m_first;
				TimeTick * m_tick;
				float m_currentTime;
				bool m_leftDown;
				ProjectFrame * m_projectFrame;
			};
		}
	}
}

#endif
