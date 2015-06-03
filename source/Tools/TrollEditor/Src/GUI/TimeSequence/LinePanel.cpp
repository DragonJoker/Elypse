#include "PrecompiledHeader.h"

#include "LinePanel.h"

#include "GUI/TimeSequence/TimeLinePanel.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/MainFrame.h"
/*
#include <Main/Context.h>
*/

using namespace Troll;
using namespace Troll::Temporal;
using namespace Troll::GUI;

enum LinePanelIDs
{
	addPonctualEvent,
	addContinuousEvent
};

BEGIN_EVENT_TABLE( LinePanel, wxPanel )
	EVT_LEFT_UP(	LinePanel::_onLeftMouseUp )
	EVT_RIGHT_UP(	LinePanel::_onRightMouseUp )
	EVT_ENTER_WINDOW(	LinePanel::_onMouseEnter )
	EVT_LEAVE_WINDOW(	LinePanel::_onMouseLeave )
	EVT_MENU( addPonctualEvent,		LinePanel::_onAddPonctualEvent )
	EVT_MENU( addContinuousEvent,	LinePanel::_onAddContinuousEvent )
END_EVENT_TABLE()

extern MainFrame * g_mainFrame;


LinePanel::LinePanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
					  const wxSize & p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_SIMPLE ),
		m_currentLeft( 20 )
{
	Hide();
	SetBackgroundColour( wxColour( 0, 255, 255 ) );
}



LinePanel::~LinePanel()
{
}



void LinePanel::Replace()
{
	SetSize( int( m_sequence->GetTotalLength() * 100 ), 20 );
	g_mainFrame->m_timelineEditor->UpdateSequence( m_sequence );
}



void LinePanel::SetSequence( TrollSequence * p_sequence )
{
	m_sequence = p_sequence;
	p_sequence->SetPanel( this );
}



void LinePanel::_showContextMenu( const wxPoint & p_pos )
{
	wxMenu l_menu;
	l_menu.Append( addPonctualEvent, _T( "Ajouter Evènement Ponctuel" ) );
	l_menu.Append( addContinuousEvent, _T( "Ajouter Evènement Continu" ) );
	m_currentLeft = p_pos.x;
	int l_modulo = m_currentLeft % 10;

	if ( l_modulo >= 5 )
	{
		m_currentLeft += 10 - l_modulo;
	}
	else
	{
		m_currentLeft -= l_modulo;
	}

	PopupMenu( & l_menu, p_pos.x, p_pos.y );
}



void LinePanel::_onAddPonctualEvent( wxCommandEvent & p_event )
{
	g_mainFrame->AddPonctualEvent( m_sequence, m_currentLeft / 100.0f );
}



void LinePanel::_onAddContinuousEvent( wxCommandEvent & p_event )
{
	g_mainFrame->AddContinuousEvent( m_sequence, m_currentLeft / 100.0f );
}



void LinePanel::_onLeftMouseUp( wxMouseEvent & p_event )
{
	g_mainFrame->ShowObjectInfos( m_sequence );
}



void LinePanel::_onRightMouseUp( wxMouseEvent & p_event )
{
	_showContextMenu( p_event.GetPosition() );
}



void LinePanel::_onMouseEnter( wxMouseEvent & p_event )
{
	wxClientDC l_dc( this );
	wxBrush l_brush( wxColour( 0, 255, 255 ), wxSOLID );
	l_dc.SetBrush( l_brush );
	l_dc.SetBackground( l_brush );
	l_dc.SetBackgroundMode( wxSOLID );
	wxPen l_pen( wxColour( 0, 0, 0 ), 1, wxSOLID );
	l_dc.SetPen( l_pen );
	l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
}



void LinePanel::_onMouseLeave( wxMouseEvent & p_event )
{
	wxClientDC l_dc( this );
	wxBrush l_brush( wxColour( 0, 255, 255 ), wxSOLID );
	l_dc.SetBrush( l_brush );
	l_dc.SetBackground( l_brush );
	l_dc.SetBackgroundMode( wxSOLID );
	wxPen l_pen( wxColour( 0, 255, 255 ), 1, wxSOLID );
	l_dc.SetPen( l_pen );
	l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
}


