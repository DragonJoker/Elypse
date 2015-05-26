#include "ElypseCtrl.h"
#include "ElypseApp.h"
#include "ElypseFrame.h"

#include "../EMuseCore/Main/EMuseFrameListener.h"
#include "../EMuseCore/Main/EMuseInstance.h"
#include "../EMuseCore/EMuseLogs.h"

#include <General/StringConverter.h>

const long ID_RENDERTIMER = wxNewId();

#define M_EXTRACT_COORDS( X) X.m_x , X.m_y
#define M_CHECK_COORDS( X) X.m_x < 0 || X.m_y < 0 || X.m_x > m_width || X.m_y > m_height


BEGIN_EVENT_TABLE( ElypseCtrl, wxControl)
	EVT_KEY_DOWN( ElypseCtrl::OnKeyDown)
	EVT_KEY_UP( ElypseCtrl::OnKeyUp)
	EVT_CHAR( ElypseCtrl::OnChar)
	EVT_CLOSE( ElypseCtrl::OnClose)
	EVT_END_SESSION( ElypseCtrl::OnClose)

	EVT_KILL_FOCUS( ElypseCtrl::OnKillFocus)
	EVT_MOTION( ElypseCtrl::OnMouseMove)
	EVT_MOUSEWHEEL( ElypseCtrl::OnMouseWheel)
	EVT_MIDDLE_DOWN( ElypseCtrl::OnMButtonDown)
	EVT_MIDDLE_UP( ElypseCtrl::OnMButtonUp)
	EVT_LEFT_DOWN( ElypseCtrl::OnLButtonDown)
	EVT_LEFT_UP( ElypseCtrl::OnLButtonUp)
	EVT_RIGHT_DOWN( ElypseCtrl::OnRButtonDown)
	EVT_RIGHT_UP( ElypseCtrl::OnRButtonUp)
	EVT_LEFT_DCLICK( ElypseCtrl::OnLButtonDown)
	EVT_MIDDLE_DCLICK( ElypseCtrl::OnMButtonDown)
	EVT_RIGHT_DCLICK( ElypseCtrl::OnRButtonDown)

	EVT_PAINT( ElypseCtrl::OnPaint)
	EVT_ERASE_BACKGROUND( ElypseCtrl::OnEraseBackground)
	EVT_TIMER( ID_RENDERTIMER, ElypseCtrl::OnRenderTimer)
END_EVENT_TABLE()

/*
BEGIN_EVENT_TABLE( ElypseFrame, wxFrame)
	EVT_CLOSE( ElypseFrame::OnClose)
END_EVENT_TABLE()
*/
ElypseCtrl :: ElypseCtrl(	ElypseApp * p_owner, 
							wxWindow * p_parent,
							EMusePlugin_Binary * p_plugin,
							EMuseInstance * p_ogre)
	:	wxControl( p_parent, wxID_ANY, wxPoint( 0, 0), GetClientSize( p_parent), wxWANTS_CHARS | wxBORDER_NONE),
		owned_by<ElypseApp>( p_owner),

#if ! defined( __WXMSW__) && defined( __WXGTK__)
		m_vi		(NULL),
#endif

		m_ogre		(p_ogre),
		m_plugin	(p_plugin)
{
	int l_width = 0;
	int l_height = 0;

	GetSize( & l_width, & l_height);
	m_width = static_cast <Real> ( l_width);
	m_height = static_cast <Real> ( l_height);

	SetFocus();
}

ElypseCtrl :: ~ElypseCtrl()
{
#if ! defined( __WXMSW__) && defined( __WXGTK__)
	if (m_vi != NULL)
	{
		XFree( m_vi);
	}
#endif
//	FreeConsole();
}

void ElypseCtrl :: OnClose( wxCloseEvent & p_event)
{
	if (p_event.CanVeto())
	{
		p_event.Veto();
	}
}

void ElypseCtrl :: OnKillFocus( wxFocusEvent &)
{
	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->ResetAll();
	}
}

void ElypseCtrl :: OnMouseMove( wxMouseEvent & p_mouseEvent)
{
	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseMove( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnMouseWheel( wxMouseEvent & p_mouseEvent)
{
	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseWheel( p_mouseEvent.m_wheelRotation);
	}
}

void ElypseCtrl :: OnMButtonDown( wxMouseEvent & p_mouseEvent)
{
	if (M_CHECK_COORDS( p_mouseEvent))
	{
		if ( ! m_rightButton && ! m_leftButton)
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS);
			wxPostEvent( this, l_event);
		}
	}
	else
	{
		m_middleButton = true;

		if (wxWindow::FindFocus() != this)
		{
			SetFocus();
		}

		CaptureMouse();
	}

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseMDown( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnMButtonUp( wxMouseEvent & p_mouseEvent)
{
	if ( ! m_rightButton && ! m_leftButton)
	{
		ReleaseMouse();
	}

	m_middleButton = false;

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseMUp( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnLButtonDown( wxMouseEvent & p_mouseEvent)
{
	if (M_CHECK_COORDS( p_mouseEvent))
	{
		if ( ! m_rightButton && ! m_middleButton)
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS);
			wxPostEvent( this, l_event);
		}
	}
	else
	{
		m_leftButton = true;

		if (wxWindow::FindFocus() != this)
		{
			SetFocus();
		}

		CaptureMouse();
	}

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseLDown( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnLButtonUp( wxMouseEvent & p_mouseEvent)
{
	if ( ! m_rightButton && ! m_middleButton)
	{
		ReleaseMouse();
	}

	m_leftButton = false;

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseLUp( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnRButtonDown( wxMouseEvent & p_mouseEvent)
{
	if (M_CHECK_COORDS( p_mouseEvent))
	{
		if ( ! m_leftButton && ! m_middleButton)
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS);
			wxPostEvent( this, l_event);
		}
	}
	else
	{
		m_rightButton = true;

		if (wxWindow::FindFocus() != this)
		{
			SetFocus();
		}

		CaptureMouse();
	}

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseRDown( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnRButtonUp( wxMouseEvent & p_mouseEvent)
{
	if ( ! m_leftButton && ! m_middleButton)
	{
		ReleaseMouse();
	}

	m_rightButton = false;

	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->MouseRUp( M_EXTRACT_COORDS( p_mouseEvent));
	}
}

void ElypseCtrl :: OnKeyUp( wxKeyEvent & p_keyEvent)
{
	if (m_ogre->IsActive())
	{
		m_ogre->GetFL()->KeyUp( p_keyEvent.GetRawKeyCode());
	}
}

void ElypseCtrl :: OnKeyDown( wxKeyEvent & p_keyEvent)
{
	p_keyEvent.Skip();
	if ( ! m_ogre->IsActive())
	{
		return;
	}

	#if ELYPSE_WINDOWS
	unsigned short l_flags = HIWORD( p_keyEvent.GetRawKeyFlags());
	if ((l_flags & KF_REPEAT) == KF_REPEAT)
	{
		m_ogre->GetFL()->KeyRepeat( p_keyEvent.GetRawKeyCode(), LOWORD( p_keyEvent.GetRawKeyFlags()));
		return;
	}
	#endif


	if (p_keyEvent.GetRawKeyCode() == 116)
	{
		wxMutexGuiLeave();
		//Do not touch that, *this gets destroyed in the OnExit, so we need to store a local of the m_owner
		ElypseApp * l_owner = m_owner;
		l_owner->OnExit();
		l_owner->OnInit();
	}
	else
	{
		m_ogre->GetFL()->KeyDown( p_keyEvent.GetRawKeyCode());
	}
}

void ElypseCtrl :: OnChar( wxKeyEvent & p_keyEvent)
{
	if ( ! m_ogre->IsActive())
	{
		return;
	}
	m_ogre->GetFL()->OnChar( p_keyEvent.GetRawKeyCode());
}

const String ElypseCtrl :: GetWHandle()
{
	SetBackgroundStyle( wxBG_STYLE_CUSTOM);

#ifdef __WXMSW__

	return ToString( reinterpret_cast<size_t>( GetHWND()));

#elif defined( __WXGTK__)

	gtk_widget_set_double_buffered( m_wxwindow, FALSE);
	GdkWindow * l_gdkWin = GTK_PIZZA( m_wxwindow)->bin_window;
	Display * l_display = GDK_WINDOW_XDISPLAY( l_gdkWin);
	Window l_wid = GDK_WINDOW_XWINDOW( l_gdkWin);

	std::stringstream l_str;
	String l_screenStr = DisplayString( l_display);
	int l_attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 32, GLX_STENCIL_SIZE, 8, None };

	m_vi = glXChooseVisual( l_display, DefaultScreen( l_display), l_attrlist);
	l_screenStr = l_screenStr.substr( l_screenStr.find( ".") + 1, l_screenStr.size());

	l_str << reinterpret_cast <unsigned long> ( l_display) << ':' << l_screenStr << ':' << l_wid << ':' << reinterpret_cast <unsigned long> ( m_vi);
	return String( l_str.str());

#else
#	error Not supported on this platform.
#endif
}
