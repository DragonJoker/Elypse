/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "LinePanel.h"

#include "GUI/TimeSequence/TimeLinePanel.h"
#include "Project/Temporal/Sequence.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			enum LinePanelIDs
			{
				addPonctualEvent,
				addContinuousEvent
			};

			BEGIN_EVENT_TABLE( LinePanel, wxPanel )
				EVT_LEFT_UP( LinePanel::OnLeftMouseUp )
				EVT_RIGHT_UP( LinePanel::OnRightMouseUp )
				EVT_ENTER_WINDOW( LinePanel::OnMouseEnter )
				EVT_LEAVE_WINDOW( LinePanel::OnMouseLeave )
				EVT_MENU( addPonctualEvent, LinePanel::OnAddPonctualEvent )
				EVT_MENU( addContinuousEvent, LinePanel::OnAddContinuousEvent )
			END_EVENT_TABLE()

			LinePanel::LinePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_SIMPLE )
				, m_currentLeft( 20 )
				, m_projectFrame{ p_projectFrame }
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
				m_projectFrame->GetTimelineEditor()->UpdateSequence( m_sequence );
			}

			void LinePanel::SetSequence( ProjectComponents::Temporal::TrollSequence * p_sequence )
			{
				m_sequence = p_sequence;
				p_sequence->SetPanel( this );
			}

			void LinePanel::_showContextMenu( wxPoint const & p_pos )
			{
				wxMenu l_menu;
				l_menu.Append( addPonctualEvent, TE_ADD_PONCTUAL_EVENT );
				l_menu.Append( addContinuousEvent, TE_ADD_CONTINUOUS_EVENT );
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

				PopupMenu( &l_menu, p_pos.x, p_pos.y );
			}

			void LinePanel::OnAddPonctualEvent( wxCommandEvent & p_event )
			{
				m_projectFrame->AddPonctualEvent( m_sequence, m_currentLeft / 100.0f );
			}

			void LinePanel::OnAddContinuousEvent( wxCommandEvent & p_event )
			{
				m_projectFrame->AddContinuousEvent( m_sequence, m_currentLeft / 100.0f );
			}

			void LinePanel::OnLeftMouseUp( wxMouseEvent & p_event )
			{
				m_projectFrame->ShowObjectInfos( m_sequence );
			}

			void LinePanel::OnRightMouseUp( wxMouseEvent & p_event )
			{
				_showContextMenu( p_event.GetPosition() );
			}

			void LinePanel::OnMouseEnter( wxMouseEvent & p_event )
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

			void LinePanel::OnMouseLeave( wxMouseEvent & p_event )
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
		}
	}
}
