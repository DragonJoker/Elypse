#include "PrecompiledHeader.h"

#include "ArrowPanel.h"

#include "GUI/MainFrame.h"
#include "GUI/TimeSequence/TimePanel.h"
/*
#include <Sequences/BaseEvent.h>
*/

using namespace Troll;
using namespace Troll::GUI;

BEGIN_EVENT_TABLE( ArrowPanel, wxPanel )
	EVT_PAINT(	ArrowPanel::_onPaint )
	EVT_LEFT_DOWN(	ArrowPanel::_onMouseLeftDown )
	EVT_LEFT_UP(	ArrowPanel::_onMouseLeftUp )
	EVT_MOTION(	ArrowPanel::_onMouseMove )
	EVT_LEAVE_WINDOW(	ArrowPanel::_onMouseLeave )
	EVT_KILL_FOCUS(	ArrowPanel::_onKillFocus )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


ArrowPanel::ArrowPanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
						const wxSize & p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE ),
		m_mouseLeftDown( false ),
		m_boundPanel( NULL )
{
	SetBackgroundColour( wxColour( 0, 255, 0 ) );
}



ArrowPanel::~ArrowPanel()
{
}



void ArrowPanel::_onPaint( wxPaintEvent & p_event )
{
	wxPaintDC l_dc( this );
	wxBrush l_brush( wxColour( 255, 255, 255 ), wxSOLID );
	l_dc.SetBrush( l_brush );
	l_dc.SetBackground( l_brush );
	l_dc.SetBackgroundMode( wxTRANSPARENT );
	wxPen l_pen = *wxWHITE_PEN;
	l_dc.SetPen( l_pen );
	wxSize l_size = GetClientSize();
	l_dc.DrawRectangle( 0, 0, l_size.x, l_size.y );
	l_pen = *wxBLACK_PEN;
	l_dc.SetPen( l_pen );
	l_dc.DrawLine( l_size.x / 2, 0, l_size.x / 2, l_size.y );
	l_dc.DrawLine( l_size.x / 2 - 5, l_size.y - 5, l_size.x / 2, l_size.y );
	l_dc.DrawLine( l_size.x / 2 + 5, l_size.y - 5, l_size.x / 2, l_size.y );
}



void ArrowPanel::_onMouseLeftDown( wxMouseEvent & p_event )
{
	m_mouseLeftDown = true;
//	m_boundPanel->SetMouseLeftDown( true);
}



void ArrowPanel::_onMouseLeftUp( wxMouseEvent & p_event )
{
	m_mouseLeftDown = false;
//	m_boundPanel->SetMouseLeftDown( false);
}



void ArrowPanel::_onMouseMove( wxMouseEvent & p_event )
{
	/*
		if (m_mouseLeftDown && m_boundPanel != NULL && GetPosition().x != -6)
		{
			int l_left;
			l_left = p_event.GetX() + GetPosition().x;

			SetPosition( wxPoint( l_left - 6, GetPosition().y));
			m_boundPanel->SetPosition(wxPoint( l_left, m_boundPanel->GetPosition().y));
		}
	*/
}



void ArrowPanel::_onMouseLeave( wxMouseEvent & p_event )
{
	m_mouseLeftDown = false;
//	m_boundPanel->SetMouseLeftDown( false);
}



void ArrowPanel::_onKillFocus( wxFocusEvent & p_event )
{
	m_mouseLeftDown = false;
//	m_boundPanel->SetMouseLeftDown( false);
}


