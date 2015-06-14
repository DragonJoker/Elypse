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

#include "TimeTick.h"

#include "TimePanel.h"

#include "TimeLinePanel.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	BEGIN_EVENT_TABLE( TimeTick, wxPanel )
		EVT_LEFT_DOWN( TimeTick::OnLeftMouseDown )
		EVT_LEFT_UP( TimeTick::OnLeftMouseUp )
	END_EVENT_TABLE()

	TimeTick::TimeTick( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position, const wxSize & p_size )
		: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
	{
		m_panel = static_cast <TimePanel *>( p_parent );
		SetBackgroundColour( wxColour( 0, 0, 0 ) );
	}

	void TimeTick::OnLeftMouseDown( wxMouseEvent & p_event )
	{
		m_panel->SetLeftDown( true );
	}

	void TimeTick::OnLeftMouseUp( wxMouseEvent & p_event )
	{
		m_panel->SetLeftDown( false );
	}
}
END_TROLL_GUI_TIME_NAMESPACE

