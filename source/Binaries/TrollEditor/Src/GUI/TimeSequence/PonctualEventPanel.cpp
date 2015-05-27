
#include "PrecompiledHeader.h"

#include "GUI/MainFrame.h"
#include "GUI/TimeSequence/PonctualEventPanel.h"
#include "GUI/TimeSequence/ArrowPanel.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/Sequence.h"
/*
#include <Sequences/BaseEvent.h>
*/

using namespace Troll;
using namespace Troll::Temporal;
using namespace Troll::GUI;

enum PonctualEventPanelIDs
{
	deleteEvent
};

BEGIN_EVENT_TABLE( PonctualEventPanel, wxPanel )
	EVT_LEFT_DOWN(	PonctualEventPanel::_onLeftMouseDown )
	EVT_LEFT_UP(	PonctualEventPanel::_onLeftMouseUp )
	EVT_RIGHT_UP(	PonctualEventPanel::_onRightMouseUp )
	EVT_ENTER_WINDOW(	PonctualEventPanel::_onMouseEnter )
	EVT_MOTION(	PonctualEventPanel::_onMouseMove )
	EVT_LEAVE_WINDOW(	PonctualEventPanel::_onMouseLeave )
	EVT_KILL_FOCUS(	PonctualEventPanel::_onKillFocus )
	EVT_MENU( deleteEvent,	PonctualEventPanel::_onDeleteEvent )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


PonctualEventPanel :: PonctualEventPanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
		const wxSize & p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_SIMPLE ),
		m_mouseLeftDown( false ),
		m_top( 30 )
{
	Hide();
	m_arrow = new ArrowPanel( p_parent, wxID_ANY, wxPoint( 0, 0 ), wxSize( 12, 20 ) );
	m_arrow->Hide();
	m_arrow->SetBoundPanel( this );
	SetBackgroundColour( wxColour( 255, 127, 0 ) );
}



PonctualEventPanel :: ~PonctualEventPanel()
{
}



void PonctualEventPanel :: SetPonctualEvent( TrollPonctualEvent * p_event, Real p_time )
{
	m_time = p_time;
	m_event = p_event;

	if ( p_event != NULL )
	{
//		std::cout << "PonctualEventPanel :: SetPonctualEvent\n";
		m_event->SetPanel( this );
		Replace( true );
	}
}



void PonctualEventPanel :: Replace( bool p_init )
{
	m_time = m_event->GetFireTime();
	m_arrow->Hide();
	Hide();

	if ( ! p_init )
	{
		GetParent()->RemoveChild( m_arrow );
		delete m_arrow;
		m_arrow = new ArrowPanel( GetParent(), wxID_ANY, wxPoint( int( m_time * 100 - 6 ), 20 ), wxSize( 12, m_top - 20 ) );
	}
	else
	{
		m_arrow->SetSize( int( m_time * 100 - 6 ), 20, 12, m_top - 20 );
	}

	SetSize( int( m_time * 100 ), m_top, 20, 20 );
	m_arrow->Show();
	Show();
}



void PonctualEventPanel :: SetTop( int p_top )
{
	m_top = p_top;
	m_arrow->SetSize( int( m_time * 100 - 6 ), 20, 12, m_top - 20 );
	SetSize( int( m_time * 100 ), m_top, 20, 20 );
	m_arrow->Show();
	Show();
}



void PonctualEventPanel :: _showContextMenu( const wxPoint & p_pos )
{
	wxMenu l_menu;
	l_menu.Append( deleteEvent, _T( "Supprimer" ) );
	PopupMenu( & l_menu, p_pos.x, p_pos.y );
}



void PonctualEventPanel :: _onDeleteEvent( wxCommandEvent & p_event )
{
	g_mainFrame->HideObjectInfos();
	m_event->GetSequence()->RemovePonctualEvent( m_event );
	Hide();
	m_arrow->Hide();
}



void PonctualEventPanel :: _onLeftMouseDown( wxMouseEvent & p_event )
{
	m_mouseLeftDown = true;
}



void PonctualEventPanel :: _release( bool p_timeChanged )
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
		m_event->SetFireTime( GetPosition().x / 100.0f );
		m_event->GetSequence()->ChangePonctualEventTime( m_event );
	}
}



void PonctualEventPanel :: _onLeftMouseUp( wxMouseEvent & p_event )
{
	_release( true );
	g_mainFrame->ShowObjectInfos( m_event );
}



void PonctualEventPanel :: _onRightMouseUp( wxMouseEvent & p_event )
{
	_showContextMenu( p_event.GetPosition() );
}



void PonctualEventPanel :: _onMouseEnter( wxMouseEvent & p_event )
{
	wxClientDC l_dc( this );
	wxBrush l_brush( wxColour( 255, 127, 0 ), wxSOLID );
	l_dc.SetBrush( l_brush );
	l_dc.SetBackground( l_brush );
	l_dc.SetBackgroundMode( wxSOLID );
	wxPen l_pen( wxColour( 0, 0, 0 ), 1, wxSOLID );
	l_dc.SetPen( l_pen );
	l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
}



void PonctualEventPanel :: _onMouseMove( wxMouseEvent & p_event )
{
	if ( m_mouseLeftDown && GetPosition().x != 0 )
	{
		int l_left = p_event.GetX() + GetPosition().x;
		SetPosition( wxPoint( l_left - 10, GetPosition().y ) );
		m_arrow->SetPosition( wxPoint( l_left - 16, m_arrow->GetPosition().y ) );
	}
}



void PonctualEventPanel :: _onMouseLeave( wxMouseEvent & p_event )
{
	wxClientDC l_dc( this );
	wxBrush l_brush( wxColour( 255, 127, 0 ), wxSOLID );
	l_dc.SetBrush( l_brush );
	l_dc.SetBackground( l_brush );
	l_dc.SetBackgroundMode( wxSOLID );
	wxPen l_pen( wxColour( 255, 127, 0 ), 1, wxSOLID );
	l_dc.SetPen( l_pen );
	l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
	_release( false );
}



void PonctualEventPanel :: _onKillFocus( wxFocusEvent & p_event )
{
	_release( false );
}


