/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TimeTick.h"

#include "TimePanel.h"

#include "TimeLinePanel.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			TimeTick::TimeTick( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
				, m_projectFrame{ p_projectFrame }
			{
				m_panel = static_cast < TimePanel * >( p_parent );
				SetBackgroundColour( wxColour( 0, 0, 0 ) );
			}

			BEGIN_EVENT_TABLE( TimeTick, wxPanel )
				EVT_LEFT_DOWN( TimeTick::OnLeftMouseDown )
				EVT_LEFT_UP( TimeTick::OnLeftMouseUp )
			END_EVENT_TABLE()

			void TimeTick::OnLeftMouseDown( wxMouseEvent & p_event )
			{
				m_panel->SetLeftDown( true );
			}

			void TimeTick::OnLeftMouseUp( wxMouseEvent & p_event )
			{
				m_panel->SetLeftDown( false );
			}
		}
	}
}

