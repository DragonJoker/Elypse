/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/ScrollablePanel.h"

namespace Troll
{
	namespace GUI
	{
		BEGIN_EVENT_TABLE( TEScrollablePanel, wxPanel )
			EVT_SCROLL( TEScrollablePanel::OnScroll )
		END_EVENT_TABLE()

		TEScrollablePanel::TEScrollablePanel( wxWindow * p_parent, wxWindowID p_id, wxPoint p_position, wxSize p_size )
			: wxPanel( p_parent, p_id, p_position, p_size, wxVSCROLL | wxALWAYS_SHOW_SB )
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );
		}

		TEScrollablePanel::~TEScrollablePanel()
		{
		}

		void TEScrollablePanel::OnScroll( wxScrollEvent & p_event )
		{
			std::cout << "TEScrollablePanel::_onScroll";
		}
	}
}
