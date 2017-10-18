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
#include "TrollEditorPch.h"

#include "TimeLineContainer.h"

#include "TimePanel.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			TimeLineContainer::TimeLineContainer( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxScrolledWindow( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
				, m_precPosition( 0 )
				, m_projectFrame{ p_projectFrame }
			{
			}

			TimeLineContainer::~TimeLineContainer()
			{
			}

			BEGIN_EVENT_TABLE( TimeLineContainer, wxScrolledWindow )
				EVT_SCROLLWIN( TimeLineContainer::OnScroll )
			END_EVENT_TABLE()

			void TimeLineContainer::OnScroll( wxScrollWinEvent & p_event )
			{
				wxScrolledWindow::HandleOnScroll( p_event );
				int l_position = GetScrollPos( wxHORIZONTAL );
				int l_diff = l_position - m_precPosition;

				if ( l_diff != 0 )
				{
					m_projectFrame->IncrementTimePanel( l_diff );
					m_precPosition = l_position;
				}
			}
		}
	}
}
