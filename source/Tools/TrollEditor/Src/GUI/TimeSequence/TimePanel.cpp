/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TimePanel.h"

#include "TimeTick.h"
#include "TimeLinePanel.h"
#include "GUI/ProjectFrame.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			enum TimePanelIDs
			{
				tick
			};

			BEGIN_EVENT_TABLE( TimePanel, wxPanel )
				EVT_PAINT( TimePanel::OnPaint )
				EVT_LEFT_DOWN( TimePanel::OnLeftMouseDown )
				EVT_LEFT_UP( TimePanel::OnLeftMouseUp )
				EVT_MOTION( TimePanel::OnMouseMove )
			END_EVENT_TABLE()

			TimePanel::TimePanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
				, m_first( 0.0 )
				, m_leftDown( false )
				, m_currentTime( 0.0 )
				, m_projectFrame{ p_projectFrame }
			{
				m_tick = new TimeTick( this, m_projectFrame, tick, wxPoint( 0, 0 ), wxSize( 4, 20 ) );
				SetBackgroundColour( wxColour( 255, 255, 220 ) );
			}

			TimePanel::~TimePanel()
			{
			}

			void TimePanel::IncrementTickPosition( int p_left )
			{
				UpdateTick( m_tick->GetPosition().x + p_left );
			}

			void TimePanel::UpdateTick( int p_left, bool p_updateSequence )
			{
				m_tick->SetPosition( wxPoint( p_left, 0 ) );

				if ( p_updateSequence )
				{
					float l_tmpTime = m_first + ( m_tick->GetPosition().x / 100.0f );
					m_projectFrame->GetTimelineEditor()->UpdateSequences( l_tmpTime - m_currentTime );
					m_currentTime = l_tmpTime;
				}
			}

			void TimePanel::OnLeftMouseDown( wxMouseEvent & p_event )
			{
				m_leftDown = true;
			}

			void TimePanel::OnLeftMouseUp( wxMouseEvent & p_event )
			{
				m_leftDown = false;
				UpdateTick( p_event.GetX(), true );
			}

			void TimePanel::OnMouseMove( wxMouseEvent & p_event )
			{
				if ( m_leftDown )
				{
					//std::cout << "TimePanel::OnMouseMove - Left down\n";
					UpdateTick( p_event.GetX(), true );
				}
			}

			void TimePanel::OnPaint( wxPaintEvent & p_event )
			{
				wxPaintDC l_dc( this );
				wxFont l_font( 6, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false );
				l_dc.SetFont( l_font );
				wxBrush l_brush( wxColour( 255, 255, 220 ), wxSOLID );
				l_dc.SetBrush( l_brush );
				l_dc.SetBackground( l_brush );
				l_dc.SetBackgroundMode( wxTRANSPARENT );
				wxPen l_pen = *wxWHITE_PEN;
				l_dc.SetPen( l_pen );
				wxSize l_size = GetClientSize();
				l_dc.DrawRectangle( 0, 0, l_size.x, l_size.y );
				l_pen = *wxBLACK_PEN;
				l_dc.SetPen( l_pen );
				int l_current = int( m_first * 1000.0 + 50.0 );
				l_current /= 100;
				wxString l_tmp;

				for ( int i = 0; i < l_size.x; i += 10 )
				{
					if ( l_current % 5 == 0 )
					{
						l_dc.DrawLine( i, 0, i, l_size.y );
						l_tmp.Printf( wxT( "%.1f" ), l_current / 10.0 );
						l_dc.DrawText( l_tmp, i + 2, l_size.y / 2 );
						l_tmp.clear();
					}
					else
					{
						l_dc.DrawLine( i, 0, i, l_size.y / 2 );
					}

					l_current += 1;
				}
			}
		}
	}
}
