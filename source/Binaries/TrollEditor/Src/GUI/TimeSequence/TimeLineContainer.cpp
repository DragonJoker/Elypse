//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/TimeSequence/TimeLineContainer.h"
#include "GUI/TimeSequence/TimePanel.h"
#include "GUI/MainFrame.h"
//***********************************************************************************************************
using namespace Troll;
using namespace Troll::GUI;
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( TimeLineContainer, wxScrolledWindow )
	EVT_SCROLLWIN( TimeLineContainer::_onScroll )
END_EVENT_TABLE()
//***********************************************************************************************************

TimeLineContainer :: TimeLineContainer( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position,
										const wxSize & p_size )
	:	wxScrolledWindow( p_parent, p_id, p_position, p_size, wxBORDER_NONE ),
		m_precPosition( 0 )
{
}

//***********************************************************************************************************

TimeLineContainer :: ~TimeLineContainer()
{
}

//***********************************************************************************************************

void TimeLineContainer :: _onScroll( wxScrollWinEvent & p_event )
{
	wxScrolledWindow::HandleOnScroll( p_event );
	int l_position = GetScrollPos( wxHORIZONTAL );
	int l_diff = l_position - m_precPosition;

	if ( l_diff != 0 )
	{
		g_mainFrame->m_timePanel->IncrementFirst( 0.2f * l_diff );
		g_mainFrame->m_timePanel->IncrementTickPosition( ( 0 - l_diff ) * 20 );
		m_precPosition = l_position;
	}
}

//***********************************************************************************************************
