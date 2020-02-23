#include "AuiDockArt.h"

namespace Troll
{
	namespace GUI
	{
		AuiDockArt::AuiDockArt()
			: wxAuiDefaultDockArt()
		{
			wxAuiDefaultDockArt::SetMetric( wxAuiPaneDockArtSetting::wxAUI_DOCKART_PANE_BORDER_SIZE, 0 );
			wxAuiDefaultDockArt::SetMetric( wxAuiPaneDockArtSetting::wxAUI_DOCKART_GRADIENT_TYPE, wxAUI_GRADIENT_NONE );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_BACKGROUND_COLOUR, GuiCommon::InactiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_BORDER_COLOUR, GuiCommon::BorderColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_GRIPPER_COLOUR, wxColour( 127, 127, 127 ) );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_SASH_COLOUR, GuiCommon::InactiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, GuiCommon::InactiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, GuiCommon::InactiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR, GuiCommon::ActiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR, GuiCommon::ActiveTabColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, GuiCommon::InactiveTextColour );
			wxAuiDefaultDockArt::SetColour( wxAuiPaneDockArtSetting::wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR, GuiCommon::ActiveTextColour );
		}

		void AuiDockArt::DrawBackground( wxDC & p_dc, wxWindow * p_window, wxRect const & p_rect )
		{
#if wxCHECK_VERSION( 2, 9, 0 )

			p_dc.SetPen( wxPen( GuiCommon::PanelBackgroundColour, 1, wxPENSTYLE_SOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::PanelBackgroundColour, wxBRUSHSTYLE_SOLID ) );

#else

			p_dc.SetPen( wxPen( GuiCommon::PanelBackgroundColour, 1, wxSOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::PanelBackgroundColour, wxSOLID ) );

#endif

			p_dc.DrawRectangle( p_rect );
		}
	}
}
