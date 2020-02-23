/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "PonctualEventPanel.h"

#include "GUI/ProjectFrame.h"
#include "GUI/TimeSequence/ArrowPanel.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/Sequence.h"

namespace Troll
{
	namespace GUI
	{
		namespace Time
		{
			enum PonctualEventPanelIDs
			{
				deleteEvent
			};

			BEGIN_EVENT_TABLE( PonctualEventPanel, wxPanel )
				EVT_LEFT_DOWN( PonctualEventPanel::OnLeftMouseDown )
				EVT_LEFT_UP( PonctualEventPanel::OnLeftMouseUp )
				EVT_RIGHT_UP( PonctualEventPanel::OnRightMouseUp )
				EVT_ENTER_WINDOW( PonctualEventPanel::OnMouseEnter )
				EVT_MOTION( PonctualEventPanel::OnMouseMove )
				EVT_LEAVE_WINDOW( PonctualEventPanel::OnMouseLeave )
				EVT_KILL_FOCUS( PonctualEventPanel::OnKillFocus )
				EVT_MENU( deleteEvent, PonctualEventPanel::OnDeleteEvent )
			END_EVENT_TABLE()

			PonctualEventPanel::PonctualEventPanel( wxWindow * p_parent, ProjectFrame * p_projectFrame, wxWindowID p_id, wxPoint const & p_position, wxSize const & p_size )
				: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_SIMPLE )
				, m_mouseLeftDown( false )
				, m_top( 30 )
				, m_projectFrame{ p_projectFrame }
			{
				Hide();
				m_arrow = new ArrowPanel( p_parent, m_projectFrame, wxID_ANY, wxPoint( 0, 0 ), wxSize( 12, 20 ) );
				m_arrow->Hide();
				m_arrow->SetBoundPanel( this );
				SetBackgroundColour( wxColour( 255, 127, 0 ) );
			}

			PonctualEventPanel::~PonctualEventPanel()
			{
			}

			void PonctualEventPanel::SetPonctualEvent( ProjectComponents::Temporal::TrollPonctualEvent * p_event, Real p_time )
			{
				m_time = p_time;
				m_event = p_event;

				if ( p_event != nullptr )
				{
					m_event->SetPanel( this );
					Replace( true );
				}
			}

			void PonctualEventPanel::Replace( bool p_init )
			{
				m_time = m_event->GetFireTime();
				m_arrow->Hide();
				Hide();

				if ( !p_init )
				{
					GetParent()->RemoveChild( m_arrow );
					delete m_arrow;
					m_arrow = new ArrowPanel( GetParent(), m_projectFrame, wxID_ANY, wxPoint( int( m_time * 100 - 6 ), 20 ), wxSize( 12, m_top - 20 ) );
				}
				else
				{
					m_arrow->SetSize( int( m_time * 100 - 6 ), 20, 12, m_top - 20 );
				}

				SetSize( int( m_time * 100 ), m_top, 20, 20 );
				m_arrow->Show();
				Show();
			}

			void PonctualEventPanel::SetTop( int p_top )
			{
				m_top = p_top;
				m_arrow->SetSize( int( m_time * 100 - 6 ), 20, 12, m_top - 20 );
				SetSize( int( m_time * 100 ), m_top, 20, 20 );
				m_arrow->Show();
				Show();
			}

			void PonctualEventPanel::_showContextMenu( wxPoint const & p_pos )
			{
				wxMenu l_menu;
				l_menu.Append( deleteEvent, TE_REMOVE );
				PopupMenu( &l_menu, p_pos.x, p_pos.y );
			}

			void PonctualEventPanel::OnDeleteEvent( wxCommandEvent & p_event )
			{
				m_projectFrame->HideObjectInfos();
				m_event->GetSequence()->RemovePonctualEvent( m_event );
				Hide();
				m_arrow->Hide();
			}

			void PonctualEventPanel::OnLeftMouseDown( wxMouseEvent & p_event )
			{
				m_mouseLeftDown = true;
			}

			void PonctualEventPanel::_release( bool p_timeChanged )
			{
				if ( m_mouseLeftDown )
				{
					int l_left = GetPosition().x;
					int l_modulo = l_left % 10;

					if ( l_modulo >= 5 )
					{
						l_left += 10 - l_modulo;
					}
					else
					{
						l_left -= l_modulo;
					}

					SetPosition( wxPoint( l_left, GetPosition().y ) );
					m_arrow->SetPosition( wxPoint( l_left - 6, m_arrow->GetPosition().y ) );
				}

				m_mouseLeftDown = false;

				if ( p_timeChanged )
				{
					m_event->SetFireTime( GetPosition().x / 100.0f );
					m_event->GetSequence()->ChangePonctualEventTime( m_event );
				}
			}

			void PonctualEventPanel::OnLeftMouseUp( wxMouseEvent & p_event )
			{
				_release( true );
				m_projectFrame->ShowObjectInfos( m_event );
			}

			void PonctualEventPanel::OnRightMouseUp( wxMouseEvent & p_event )
			{
				_showContextMenu( p_event.GetPosition() );
			}

			void PonctualEventPanel::OnMouseEnter( wxMouseEvent & p_event )
			{
				wxClientDC l_dc( this );
				wxBrush l_brush( wxColour( 255, 127, 0 ), wxSOLID );
				l_dc.SetBrush( l_brush );
				l_dc.SetBackground( l_brush );
				l_dc.SetBackgroundMode( wxSOLID );
				wxPen l_pen( wxColour( 0, 0, 0 ), 1, wxSOLID );
				l_dc.SetPen( l_pen );
				l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
			}

			void PonctualEventPanel::OnMouseMove( wxMouseEvent & p_event )
			{
				if ( m_mouseLeftDown && GetPosition().x != 0 )
				{
					int l_left = p_event.GetX() + GetPosition().x;
					SetPosition( wxPoint( l_left - 10, GetPosition().y ) );
					m_arrow->SetPosition( wxPoint( l_left - 16, m_arrow->GetPosition().y ) );
				}
			}

			void PonctualEventPanel::OnMouseLeave( wxMouseEvent & p_event )
			{
				wxClientDC l_dc( this );
				wxBrush l_brush( wxColour( 255, 127, 0 ), wxSOLID );
				l_dc.SetBrush( l_brush );
				l_dc.SetBackground( l_brush );
				l_dc.SetBackgroundMode( wxSOLID );
				wxPen l_pen( wxColour( 255, 127, 0 ), 1, wxSOLID );
				l_dc.SetPen( l_pen );
				l_dc.DrawRectangle( 0, 0, GetClientSize().x, GetClientSize().y );
				_release( false );
			}

			void PonctualEventPanel::OnKillFocus( wxFocusEvent & p_event )
			{
				_release( false );
			}
		}
	}
}
