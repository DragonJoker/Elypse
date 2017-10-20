/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TimeLineContainer.h"

#include "TimePanel.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			TimeLineContainer::TimeLineContainer( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxScrolledWindow( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
				, m_precPosition( 0 )
				, m_projectFrame{ p_projectFrame }
			{
			}

			TimeLineContainer::~TimeLineContainer()
			{
			}

			BEGIN_EVENT_TABLE( TimeLineContainer, wxScrolledWindow )
				EVT_SCROLLWIN( TimeLineContainer::OnScroll )
			END_EVENT_TABLE()

			void TimeLineContainer::OnScroll( wxScrollWinEvent & p_event )
			{
				wxScrolledWindow::HandleOnScroll( p_event );
				int l_position = GetScrollPos( wxHORIZONTAL );
				int l_diff = l_position - m_precPosition;

				if ( l_diff != 0 )
				{
					m_projectFrame->IncrementTimePanel( l_diff );
					m_precPosition = l_position;
				}
			}
		}
	}
}
