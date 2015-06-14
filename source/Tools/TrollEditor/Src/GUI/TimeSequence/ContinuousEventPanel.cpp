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

#include "ContinuousEventPanel.h"

#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/TimeSequence/ArrowPanel.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	enum ContinuousEventPanelIDs
	{
		newFrame,
		deleteEvent
	};

	BEGIN_EVENT_TABLE( ContinuousEventPanel, wxPanel )
		EVT_LEFT_DOWN( ContinuousEventPanel::OnLeftMouseDown )
		EVT_LEFT_UP( ContinuousEventPanel::OnLeftMouseUp )
		EVT_RIGHT_UP( ContinuousEventPanel::OnRightMouseUp )
		EVT_ENTER_WINDOW( ContinuousEventPanel::OnMouseEnter )
		EVT_MOTION( ContinuousEventPanel::OnMouseMove )
		EVT_LEAVE_WINDOW( ContinuousEventPanel::OnMouseLeave )
		EVT_KILL_FOCUS( ContinuousEventPanel::OnKillFocus )
		EVT_MENU( newFrame, ContinuousEventPanel::OnNewFrame )
		EVT_MENU( deleteEvent, ContinuousEventPanel::OnDeleteEvent )
	END_EVENT_TABLE()

	ContinuousEventPanel::ContinuousEventPanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position, const wxSize & p_size )
		: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_SIMPLE )
		, m_mouseLeftDown( false )
		, m_hasMoved( false )
		, m_top( 30 )
	{
		Hide();
		m_arrow = new ArrowPanel( p_parent, wxID_ANY, wxPoint( 0, 0 ), wxSize( 12, 20 ) );
		m_arrow->SetBoundPanel( this );
		m_arrow->Hide();
		SetBackgroundColour( wxColour( 0, 255, 0 ) );
	}

	ContinuousEventPanel::~ContinuousEventPanel()
	{
	}

	void ContinuousEventPanel::SetContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
		m_event = p_event;

		if ( p_event != NULL )
		{
			m_event->SetPanel( this );
			Replace( true );
		}
	}

	void ContinuousEventPanel::Replace( bool p_init )
	{
		if ( m_event != NULL )
		{
			m_arrow->Hide();
			Hide();

			if ( ! p_init )
			{
				GetParent()->RemoveChild( m_arrow );
				delete m_arrow;
				m_arrow = new ArrowPanel( GetParent(), wxID_ANY, wxPoint( int( m_event->GetStartTime() * 100 - 6 ), 20 ), wxSize( 12, m_top - 20 ) );
			}
			else
			{
				m_arrow->SetSize( int( m_event->GetStartTime() * 100 - 6 ), 20, 12, m_top - 20 );
			}

			std::cout << "ContinuousEventPanel::Replace - " << m_event->GetStartTime() << "\n";
			SetSize( int( m_event->GetStartTime() * 100 ), m_top, int( m_event->GetLength() > 0.0 ? m_event->GetLength() * 100 : 10 ), 20 );
			m_arrow->Show();
			Show();
		}
	}

	void ContinuousEventPanel::SetTop( int p_top )
	{
		m_top = p_top;
		m_arrow->SetSize( int( m_event->GetStartTime() * 100 - 6 ), 20, 12, m_top - 20 );
		SetSize( int( m_event->GetStartTime() * 100 ), m_top, int( m_event->GetLength() > 0.0 ? m_event->GetLength() * 100 : 10 ), 20 );
		m_arrow->Show();
		Show();
	}

	void ContinuousEventPanel::OnLeftMouseDown( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = true;
	}

	void ContinuousEventPanel::OnLeftMouseUp( wxMouseEvent & p_event )
	{
		_release( true );
		GUI::MainFrame::GetInstance()->ShowObjectInfos( m_event );
	}

	void ContinuousEventPanel::OnRightMouseUp( wxMouseEvent & p_event )
	{
		_showContextMenu( p_event.GetPosition() );
	}

	void ContinuousEventPanel::OnMouseEnter( wxMouseEvent & p_event )
	{
		wxClientDC l_dc( this );
		wxBrush l_brush( wxColour( 0, 255, 0 ), wxSOLID );
		l_dc.SetBrush( l_brush );
		l_dc.SetBackground( l_brush );
		l_dc.SetBackgroundMode( wxSOLID );
		wxPen l_pen( wxColour( 0, 0, 0 ), 1, wxSOLID );
		l_dc.SetPen( l_pen );
		l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
	}

	void ContinuousEventPanel::OnMouseMove( wxMouseEvent & p_event )
	{
		if ( m_mouseLeftDown && GetPosition().x != 0 )
		{
			int l_left = p_event.GetX() + GetPosition().x;
			SetPosition( wxPoint( l_left - GetClientSize().x / 2, GetPosition().y ) );
			m_arrow->SetPosition( wxPoint( l_left - 6 - ( GetClientSize().x / 2 ), m_arrow->GetPosition().y ) );
		}
	}

	void ContinuousEventPanel::OnMouseLeave( wxMouseEvent & p_event )
	{
		wxClientDC l_dc( this );
		wxBrush l_brush( wxColour( 0, 255, 0 ), wxSOLID );
		l_dc.SetBrush( l_brush );
		l_dc.SetBackground( l_brush );
		l_dc.SetBackgroundMode( wxSOLID );
		wxPen l_pen( wxColour( 0, 255, 0 ), 1, wxSOLID );
		l_dc.SetPen( l_pen );
		l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
		_release( false );
	}

	void ContinuousEventPanel::OnKillFocus( wxFocusEvent & p_event )
	{
		_release( false );
	}

	void ContinuousEventPanel::OnNewFrame( wxCommandEvent & p_event )
	{
		GUI::MainFrame::GetInstance()->CreateNewContinuousEventFrame( m_event );
	}

	void ContinuousEventPanel::OnDeleteEvent( wxCommandEvent & p_event )
	{
		GUI::MainFrame::GetInstance()->HideObjectInfos();
		m_event->GetSequence()->RemoveContinuousEvent( m_event );
		Hide();
		m_arrow->Hide();
	}

	void ContinuousEventPanel::_showContextMenu( const wxPoint & p_pos )
	{
		wxMenu l_menu;
		//l_menu.Append( newFrame, _( "New Frame" ));
		//l_menu.AppendSeparator();			//*****************************//
		l_menu.Append( deleteEvent, TE_REMOVE );
		PopupMenu( & l_menu, p_pos.x, p_pos.y );
	}

	void ContinuousEventPanel::_release( bool p_timeChanged )
	{
		if ( m_mouseLeftDown )
		{
			int l_left = GetPosition().x;
			int l_modulo = l_left % 10;

			if ( l_modulo >= 5 )
			{
				l_left += 10 - l_modulo;
			}
			else
			{
				l_left -= l_modulo;
			}

			SetPosition( wxPoint( l_left, GetPosition().y ) );
			m_arrow->SetPosition( wxPoint( l_left - 6, m_arrow->GetPosition().y ) );
		}

		m_mouseLeftDown = false;

		if ( p_timeChanged )
		{
			m_event->UpdateTime( GetPosition().x / 100.0f );
			m_event->GetSequence()->ChangeContinuousEventTime( m_event );
		}
	}
}
END_TROLL_GUI_TIME_NAMESPACE
