/*
See LICENSE file in root folder
*/
#include "ElypseCtrl.h"
#include "ElypseApp.h"
#include "ElypseFrame.h"

#include <ElypseFrameListener.h>
#include <ElypseInstance.h>
#include <ElypseLogs.h>

#include <OgreStringConverter.h>

#if defined( __linux__ )
#	include <gdk/gdkx.h>
#	include <gtk/gtk.h>
#endif

#define M_EXTRACT_COORDS( X ) Real( X.m_x ), Real( X.m_y )
#define M_CHECK_COORDS( X ) X.m_x < 0 || X.m_y < 0 || X.m_x > m_width || X.m_y > m_height

namespace ElypseBinary
{
	const long ID_RENDERTIMER = wxNewId();

	ElypseCtrl::ElypseCtrl( ElypseApp & p_owner, wxWindow * p_parent, std::shared_ptr< GuiCommon::wxElypsePlugin > p_plugin, std::shared_ptr< ElypseInstance > p_elypse )
		: wxControl( p_parent, wxID_ANY, wxPoint( 0, 0 ), GetClientSize( p_parent ), wxWANTS_CHARS | wxBORDER_NONE )
		, owned_by< ElypseApp >( p_owner )
		, m_elypse( p_elypse )
		, m_plugin( p_plugin )
	{
		int l_width = 0;
		int l_height = 0;

		GetSize( & l_width, & l_height );
		m_width = Real( l_width );
		m_height = Real( l_height );

		SetFocus();
	}

	ElypseCtrl::~ElypseCtrl()
	{
	}

	const String ElypseCtrl::GetWHandle()
	{
		SetBackgroundStyle( wxBG_STYLE_CUSTOM );
#ifdef __WXMSW__
		return StringConverter::toString( reinterpret_cast<size_t>( GetHWND() ) );
#elif defined( __WXGTK__)
		GtkWidget * l_gtkWidget = this->GetHandle();
		gtk_widget_realize( l_gtkWidget );
		Display * l_display = GDK_WINDOW_XDISPLAY( l_gtkWidget->window );
		Window l_wid = GDK_WINDOW_XWINDOW( l_gtkWidget->window );
		std::stringstream l_str;
		String l_screenStr = DisplayString( l_display );
		l_screenStr = l_screenStr.substr( l_screenStr.find( "." ) + 1, l_screenStr.size() );
		l_str << reinterpret_cast <unsigned long>( l_display ) << ':' << l_screenStr << ':' << l_wid;
		return String( l_str.str() );

		//GtkWidget * l_gtkWidget = this->GetHandle();
		//gtk_widget_realize( l_gtkWidget );
		//Display * l_display = GDK_WINDOW_XDISPLAY( l_gtkWidget->window );
		//std::stringstream l_str;
		//String l_screenStr = DisplayString( l_display );
		//l_screenStr = l_screenStr.substr( 1, ( l_screenStr.find( ".", 0 ) - 1 ) );
		//return l_screenStr;
#else
#	error Not supported on this platform.
#endif
	}

	BEGIN_EVENT_TABLE( ElypseCtrl, wxControl )
		EVT_KEY_DOWN( ElypseCtrl::OnKeyDown )
		EVT_KEY_UP( ElypseCtrl::OnKeyUp )
		EVT_CHAR( ElypseCtrl::OnChar )
		EVT_CLOSE( ElypseCtrl::OnClose )
		EVT_END_SESSION( ElypseCtrl::OnClose )

		EVT_KILL_FOCUS( ElypseCtrl::OnKillFocus )
		EVT_MOTION( ElypseCtrl::OnMouseMove )
		EVT_MOUSEWHEEL( ElypseCtrl::OnMouseWheel )
		EVT_MIDDLE_DOWN( ElypseCtrl::OnMButtonDown )
		EVT_MIDDLE_UP( ElypseCtrl::OnMButtonUp )
		EVT_LEFT_DOWN( ElypseCtrl::OnLButtonDown )
		EVT_LEFT_UP( ElypseCtrl::OnLButtonUp )
		EVT_RIGHT_DOWN( ElypseCtrl::OnRButtonDown )
		EVT_RIGHT_UP( ElypseCtrl::OnRButtonUp )
		EVT_LEFT_DCLICK( ElypseCtrl::OnLButtonDown )
		EVT_MIDDLE_DCLICK( ElypseCtrl::OnMButtonDown )
		EVT_RIGHT_DCLICK( ElypseCtrl::OnRButtonDown )

		EVT_PAINT( ElypseCtrl::OnPaint )
		EVT_ERASE_BACKGROUND( ElypseCtrl::OnEraseBackground )
		EVT_TIMER( ID_RENDERTIMER, ElypseCtrl::OnRenderTimer )
	END_EVENT_TABLE()

	void ElypseCtrl::OnKeyDown( wxKeyEvent & p_keyEvent )
	{
		p_keyEvent.Skip();
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
#if ELYPSE_WINDOWS
			uint16_t l_flags = HIWORD( p_keyEvent.GetRawKeyFlags() );

			if ( ( l_flags & KF_REPEAT ) == KF_REPEAT )
			{
				l_elypse->GetFL()->KeyRepeat( p_keyEvent.GetRawKeyCode(), LOWORD( p_keyEvent.GetRawKeyFlags() ) );
			}
			else
#endif
			{
				if ( p_keyEvent.GetRawKeyCode() == 116 )
				{
					wxMutexGuiLeave();
					//Do not touch that, *this gets destroyed in the OnExit, so we need to store a local of the m_owner
					ElypseApp * l_owner = GetOwner();
					l_owner->OnExit();
					l_owner->OnInit();
				}
				else
				{
					l_elypse->GetFL()->KeyDown( p_keyEvent.GetRawKeyCode() );
				}
			}
		}
	}

	void ElypseCtrl::OnKeyUp( wxKeyEvent & p_keyEvent )
	{
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->KeyUp( p_keyEvent.GetRawKeyCode() );
		}
	}

	void ElypseCtrl::OnChar( wxKeyEvent & p_keyEvent )
	{
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->OnChar( p_keyEvent.GetRawKeyCode() );
		}
	}

	void ElypseCtrl::OnKillFocus( wxFocusEvent & )
	{
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->ResetAll();
		}
	}

	void ElypseCtrl::OnMouseMove( wxMouseEvent & p_mouseEvent )
	{
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseMove( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnMouseWheel( wxMouseEvent & p_mouseEvent )
	{
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseWheel( p_mouseEvent.m_wheelRotation );
		}
	}

	void ElypseCtrl::OnMButtonDown( wxMouseEvent & p_mouseEvent )
	{
		if ( M_CHECK_COORDS( p_mouseEvent ) )
		{
			if ( !m_rightButton && !m_leftButton )
			{
				wxFocusEvent l_event( wxEVT_KILL_FOCUS );
				wxPostEvent( this, l_event );
			}
		}
		else
		{
			m_middleButton = true;

			if ( wxWindow::FindFocus() != this )
			{
				SetFocus();
			}

			CaptureMouse();
		}

		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseMDown( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnMButtonUp( wxMouseEvent & p_mouseEvent )
	{
		if ( !m_rightButton && !m_leftButton )
		{
			ReleaseMouse();
		}

		m_middleButton = false;
		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseMUp( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnLButtonDown( wxMouseEvent & p_mouseEvent )
	{
		if ( M_CHECK_COORDS( p_mouseEvent ) )
		{
			if ( !m_rightButton && !m_middleButton )
			{
				wxFocusEvent l_event( wxEVT_KILL_FOCUS );
				wxPostEvent( this, l_event );
			}
		}
		else
		{
			m_leftButton = true;

			if ( wxWindow::FindFocus() != this )
			{
				SetFocus();
			}

			if ( !m_rightButton && !m_middleButton )
			{
				CaptureMouse();
			}
		}

		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseLDown( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnLButtonUp( wxMouseEvent & p_mouseEvent )
	{
		if ( !m_rightButton && !m_middleButton )
		{
			ReleaseMouse();
		}

		m_leftButton = false;

		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseLUp( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnRButtonDown( wxMouseEvent & p_mouseEvent )
	{
		if ( M_CHECK_COORDS( p_mouseEvent ) )
		{
			if ( !m_leftButton && !m_middleButton )
			{
				wxFocusEvent l_event( wxEVT_KILL_FOCUS );
				wxPostEvent( this, l_event );
			}
		}
		else
		{
			m_rightButton = true;

			if ( wxWindow::FindFocus() != this )
			{
				SetFocus();
			}

			CaptureMouse();
		}

		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseRDown( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnRButtonUp( wxMouseEvent & p_mouseEvent )
	{
		if ( !m_leftButton && !m_middleButton )
		{
			ReleaseMouse();
		}

		m_rightButton = false;

		auto && l_elypse = GetElypse();

		if ( l_elypse && l_elypse->IsActive() )
		{
			l_elypse->GetFL()->MouseRUp( M_EXTRACT_COORDS( p_mouseEvent ) );
		}
	}

	void ElypseCtrl::OnClose( wxCloseEvent & p_event )
	{
		if ( p_event.CanVeto() )
		{
			p_event.Veto();
		}
	}

	void ElypseCtrl::OnPaint( wxPaintEvent & p_event )
	{
		p_event.Skip();
	}

	void ElypseCtrl::OnEraseBackground( wxEraseEvent & p_event )
	{
		p_event.Skip();
	}

	void ElypseCtrl::OnRenderTimer( wxTimerEvent & p_event )
	{
		p_event.Skip();
	}
}
