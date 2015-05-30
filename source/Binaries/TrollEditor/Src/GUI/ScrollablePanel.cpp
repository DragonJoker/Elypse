#include "PrecompiledHeader.h"

#include "GUI/ScrollablePanel.h"

using namespace Troll;
using namespace Troll::GUI;

BEGIN_EVENT_TABLE( TEScrollablePanel, wxPanel )
	EVT_SCROLL(	TEScrollablePanel::_onScroll )
END_EVENT_TABLE()


TEScrollablePanel::TEScrollablePanel( wxWindow * p_parent, wxWindowID p_id, wxPoint p_position, wxSize p_size )
	:	wxPanel( p_parent, p_id, p_position, p_size, wxVSCROLL | wxALWAYS_SHOW_SB )
{
}



TEScrollablePanel::~TEScrollablePanel()
{
}



void TEScrollablePanel::_onScroll( wxScrollEvent & p_event )
{
	std::cout << "TEScrollablePanel::_onScroll";
}
