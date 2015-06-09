#include "PrecompiledHeader.h"

#include "ElypseCtrl.h"

#include <OgreStringConverter.h>
#include <EMuseController.h>

#include "MainFrame.h"
#include "NewOverlayFrame.h"
#include "Project/Displayable.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/2D/Overlay.h"
#include "TrollLogs.h"

#if defined( __linux__ )
#	ifdef __WXGTK__
#		include <gdk/gdk.h>
#		include <gtk/gtk.h> // just this should suffice as it should include gdk.h itself
#		include <gdk/gdkx.h>
#		include <GL/glx.h>
#		if  wxCHECK_VERSION(2, 9, 0)
#			define piz(wxwin) WX_PIZZA((wxwin)->m_wxwindow)
#			define GetXWindow(wxwin) (wxwin)->m_wxwindow ? GDK_WINDOW_XWINDOW(((GtkWidget*)piz(wxwin))->window) : GDK_WINDOW_XWINDOW((wxwin)->m_widget->window)
#		else
#			define GetXWindow(wxwin) (wxwin)->m_wxwindow ? GDK_WINDOW_XWINDOW(GTK_PIZZA((wxwin)->m_wxwindow)->bin_window) : GDK_WINDOW_XWINDOW((wxwin)->m_widget->window)
#		endif
#		ifdef __WXX11__
#			include "wx/x11/privx.h"
#			define GetXWindow(wxwin)   ((Window)(wxwin)->GetHandle())
#		endif
#	endif //__WXGTK__
#endif //OGRE_PLATFORM_LINUX

const long ID_RENDERTIMER = wxNewId();

using namespace Troll;
using namespace Troll::Main;
using namespace Troll::GUI;
using namespace Troll::Objects2D;
using namespace EMuse::TEWidget;

extern MainFrame * g_mainFrame;

enum ElypseCtrlIDs
{
	tpIdDestroy,
	tpIdResize,
	tpIdReplace,
	tpIdNew,
	tpIdDelete
};

BEGIN_EVENT_TABLE( ElypseCtrl, wxControl )
	EVT_SET_FOCUS(	ElypseCtrl::OnSetFocus )
	EVT_KILL_FOCUS(	ElypseCtrl::OnKillFocus )
	EVT_SHOW(	ElypseCtrl::OnShow )
	EVT_ACTIVATE(	ElypseCtrl::OnActivate )
//	EVT_ICONIZE(				ElypseCtrl::OnIconise)

	EVT_KEY_DOWN(	ElypseCtrl::OnKeyDown )
	EVT_KEY_UP(	ElypseCtrl::OnKeyUp )
	EVT_CHAR(	ElypseCtrl::OnChar )
//	EVT_END_SESSION(			ElypseCtrl::_onClose)

	EVT_MOTION(	ElypseCtrl::OnMouseMove )
	EVT_MOUSEWHEEL(	ElypseCtrl::OnMouseWheel )
	EVT_MIDDLE_DOWN(	ElypseCtrl::OnButtonMiddleDown )
	EVT_MIDDLE_UP(	ElypseCtrl::OnButtonMiddleUp )
	EVT_LEFT_DOWN(	ElypseCtrl::OnButtonLeftDown )
	EVT_LEFT_UP(	ElypseCtrl::OnButtonLeftUp )
	EVT_RIGHT_DOWN(	ElypseCtrl::OnButtonRightDown )
	EVT_RIGHT_UP(	ElypseCtrl::OnButtonRightUp )
	EVT_LEFT_DCLICK(	ElypseCtrl::OnButtonLeftDown )
	EVT_MIDDLE_DCLICK(	ElypseCtrl::OnButtonMiddleDown )
	EVT_RIGHT_DCLICK(	ElypseCtrl::OnButtonRightDown )

	EVT_CLOSE(	ElypseCtrl::_onClose )

	EVT_PAINT(	ElypseCtrl::_onPaint )
	EVT_ERASE_BACKGROUND(	ElypseCtrl::_onEraseBackground )
	EVT_TIMER(	ID_RENDERTIMER,	ElypseCtrl::_onRenderTimer )

	EVT_MENU(	tpIdResize,		ElypseCtrl::_onResize )
	EVT_MENU(	tpIdReplace,	ElypseCtrl::_onReplace )
	EVT_MENU(	tpIdNew,		ElypseCtrl::_onNew )
	EVT_MENU(	tpIdDelete,		ElypseCtrl::_onDelete )
END_EVENT_TABLE()


String GetKeyFromReg( const char * p_registerName , const char * p_keyName )
{
	String l_returnValue;
#if defined( _WIN32 )
	LONG returnStatus;
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	char lszValue[255];
	returnStatus = RegOpenKeyExA( HKEY_LOCAL_MACHINE, p_registerName, 0L,  KEY_READ, &hKey );

	if ( returnStatus == ERROR_SUCCESS )
	{
		returnStatus = RegQueryValueExA( hKey, p_keyName, NULL, &dwType, ( LPBYTE )&lszValue, &dwSize );

		if ( returnStatus == ERROR_SUCCESS )
		{
			l_returnValue = lszValue;
		}
	}

#endif
	return l_returnValue;
}



ElypseCtrl::ElypseCtrl(	wxWindow * p_parent, Troll::Project * p_project,
						MainFrame * p_mainFrame, bool p_edit )
	:	wxControl( p_parent, wxID_ANY, wxPoint( 0, 0 ), GetClientSize( p_parent ), wxWANTS_CHARS | wxBORDER_NONE ),

#if ! defined( __WXMSW__) && defined( __WXGTK__)
		m_vi( NULL ),
#endif

		m_emuse( NULL ),
		m_plugin( NULL ),
		m_focused( false ),
		m_destroyed( false ),
		m_deleteAll( false ),
		m_oneTimePostSetFocus( true ),
		m_ogreActive( false ),
		m_fucked( false ),
		m_project( p_project ),
		m_mainFrame( p_mainFrame ),
		m_testedScene( m_project->GetMainScene() ),
		m_editionMode( p_edit ),
		m_newOverlayFrame( NULL )
{
	int l_width = 0;
	int l_height = 0;
	GetSize( & l_width, & l_height );
	m_width = static_cast <Real>( l_width );
	m_height = static_cast <Real>( l_height );
//	SetFocus();
	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::ElypseCtrl - Begin" ) );
	_initialise();
	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::ElypseCtrl - End" ) );
}



ElypseCtrl::~ElypseCtrl()
{
//	FreeConsole();
#if ! defined( __WXMSW__) && defined( __WXGTK__)
	if ( m_vi != NULL )
	{
		XFree( m_vi );
	}

#endif
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
	int l_attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 32, GLX_STENCIL_SIZE, 8, None };
	m_vi = glXChooseVisual( l_display, DefaultScreen( l_display ), l_attrlist );
	l_screenStr = l_screenStr.substr( l_screenStr.find( "." ) + 1, l_screenStr.size() );
	l_str << reinterpret_cast <unsigned long>( l_display ) << ':' << l_screenStr << ':' << l_wid << ':' << reinterpret_cast <unsigned long>( m_vi );
	return String( l_str.str() );
#else
#	error Not supported on this platform.
#endif
}



void ElypseCtrl::Init()
{
	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - Begin" ) );
	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - Path : " ) + wxString( m_path.c_str(), wxMBConvLibc() ) );
	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - File : " ) + wxString( m_museFile.c_str(), wxMBConvLibc() ) );
	String l_path = String( m_path );
	String l_fileName = String( m_museFile );

	try
	{
		m_emuse->SetMain( true );
		m_emuse->UseDirectX( true );
		m_emuse->ShowFPS( m_project->GetShowFPS() );
		m_emuse->SetAntialiasing( m_project->GetFSAA() );
		m_emuse->SetDownloadFiles( true );
		m_emuse->SetStartupScript( m_project->GetStartupScript().char_str().data() );
		m_plugin->SetSize( static_cast <unsigned int>( m_width ), static_cast <unsigned int>( m_height ) );
		m_plugin->SetHandle( this );

		if ( m_fucked )
		{
			return;
		}

		m_emuse->UseDirectX( true );
		m_emuse->SetHandle( GetWHandle() );
		m_primary = m_emuse->Init( static_cast <unsigned int>( m_width ), static_cast <unsigned int>( m_height ), "" );
		m_leftButton = false;
		m_rightButton = false;
		m_middleButton = false;
		wxString l_strUrl = wxString( wxT( "file://" ) ) + m_project->GetPath() + wxString( l_fileName.c_str(), wxMBConvLibc() );
		std::cout << "ElypseCtrl::Init - " << l_strUrl.char_str().data() << "\n";
		m_emuse->SetFileUrl( l_strUrl.char_str().data() );
		//	m_emuse->SetInstanceName( String( "TrollScene Test"));
		m_plugin->SetBaseUrl( l_path );
		m_emuse->SetCurrentDirectory( m_installPath );
		m_focused = true;
		m_emuse->SetFocus( true );
		m_oneTimePostSetFocus = true;
	}
	catch ( ... )
	{
		g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - Unable to initialise Elypse" ) );
		m_fucked = true;

		try
		{
			m_emuse->WaitForDeletion();
		}
		catch ( ... )
		{
			g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - Unable to stop Elypse cleanly" ) );
			return;
		}
	}

	g_mainFrame->LogDebugMessage( wxT( "ElypseCtrl::Init - End" ) );
}



void ElypseCtrl::_setCustomFocus( bool p_hasFocus )
{
	if ( p_hasFocus )
	{
#if defined( _WIN32 )
		SetCapture( ( HWND )GetHWND() );
#endif
		m_focused = true;
		m_emuse->SetFocus( true );
		m_oneTimePostSetFocus = true;
	}
	else
	{
		if ( m_emuse->IsActive() )
		{
			m_emuse->GetFL()->ResetAll();
		}

		m_emuse->SetFocus( false );
		m_focused = false;
#if defined( _WIN32 )
		ReleaseCapture();
#endif
	}
}



void ElypseCtrl::OnSetFocus( wxFocusEvent & p_event )
{
	if ( ! m_destroyed )
	{
		_setCustomFocus( true );
	}
}



void ElypseCtrl::OnKillFocus( wxFocusEvent & )
{
	if ( ! m_destroyed )
	{
		_setCustomFocus( false );
	}
}



void ElypseCtrl::OnShow( wxShowEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	_setCustomFocus( p_event.IsShown() );
}



void ElypseCtrl::OnActivate( wxActivateEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	_setCustomFocus( p_event.GetActive() );
}



void ElypseCtrl::OnIconise( wxIconizeEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	_setCustomFocus( false );
}



void ElypseCtrl::OnMouseMove( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed || ! m_emuse->IsActive() )
	{
//		std::cout << "ElypseCtrl::OnMouseMove - Not focused\n";
		return;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( m_oneTimePostSetFocus )
	{
		m_oneTimePostSetFocus = false;
		m_emuse->GetFL()->SetMouse( Real( l_point.x ), Real( l_point.y ) );
	}

	m_emuse->GetFL()->MouseMove( Real( l_point.x ), Real( l_point.y ) );
}



void ElypseCtrl::OnMouseWheel( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	wxPoint l_point = p_event.GetPosition();
	short l_delta = p_event.GetWheelDelta();

	if ( p_event.GetWheelRotation() < 0 )
	{
		l_delta = -1 * l_delta;
	}

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseWheel( l_delta );
	}
}



void ElypseCtrl::OnButtonMiddleDown( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( l_point.x < 0 || l_point.y < 0 || l_point.x > m_width || l_point.y > m_height )
	{
		if ( m_rightButton == false && m_leftButton == false )
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS );
			wxPostEvent( this, l_event );
		}
	}
	else
	{
		m_middleButton = true;
#if defined( _WIN32 )

		if ( GetFocus() != GetHWND() )
		{
			SetFocus();
		}

#endif
	}

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseMDown( Real( l_point.x ), Real( l_point.y ) );
	}
}



void ElypseCtrl::OnButtonMiddleUp( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( m_rightButton == false && m_leftButton == false )
	{
#if defined( _WIN32 )
		ReleaseCapture();
#endif
	}

	m_middleButton = false;

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseMUp( Real( l_point.x ), Real( l_point.y ) );
	}
}



void ElypseCtrl::OnButtonLeftDown( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	if ( m_mainFrame->m_propertyDialog )
	{
		m_mainFrame->m_propertyDialog->Close();
		delete m_mainFrame->m_propertyDialog;
		m_mainFrame->m_propertyDialog = NULL;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( l_point.x < 0 || l_point.y < 0 || l_point.x > m_width || l_point.y > m_height )
	{
		if ( m_rightButton == false && m_middleButton == false )
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS );
			wxPostEvent( this, l_event );
		}
	}
	else
	{
		m_leftButton = true;
#if defined( _WIN32 )

		if ( GetFocus() != GetHWND() )
		{
			SetFocus();
		}

#endif
	}

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseLDown( Real( l_point.x ), Real( l_point.y ) );
	}

	/*
			RaySceneQuery * mRaySceneQuery;
			SceneManager * l_manager = m_emuse->GetSceneManager();
			mRaySceneQuery = l_manager->createRayQuery(Ray());
			Ray mouseRay = l_manager->getCamera( "RootCamera")->getCameraToViewportRay( l_point.x / m_width, l_point.y / m_height);
			mRaySceneQuery->setRay( mouseRay);
			mRaySceneQuery->setSortByDistance( true);

			RaySceneQueryResult & result = mRaySceneQuery->execute();

			bool l_found = false;
			RaySceneQueryResult::iterator itr = result.begin();
			if (itr != result.end())
			{
				while (! l_found)
				{
					if (itr == result.end())
					{
						l_found = true;
					}
					else
					{
						if (itr->movable->isVisible())
						{
							if (itr->movable != NULL && itr->movable->getParentSceneNode() != l_manager->getRootSceneNode())
							{
								Entity * l_entity = static_cast <Entity*>( itr->movable);
								BoundingMesh * l_box = new BoundingMesh( l_entity->getMesh().getPointer());
	//
								l_box->SetBoundingSphere( l_entity->getWorldBoundingSphere().getRadius());
								l_box->SetBoundingBox( l_entity->getWorldBoundingBox());
								l_box->RequestTransform( l_entity->getParentSceneNode()->_getDerivedPosition(), l_entity->getParentSceneNode()->_getDerivedOrientation(),
														 l_entity->getParentSceneNode()->_getDerivedScale());
								l_box->UpdateVertices();
								l_box->RecalcBoundingBox();
								if (l_box->RayTrace( mouseRay).first)
								{
									l_found = true;
								}
								else
								{
									itr++;
								}
	//
								delete l_box;
								itr++;
							}
							else
							{
								itr++;
							}
						}
						else
						{
							itr++;
						}
					}
				}
				if (itr != result.end())
				{
					wxString l_name = itr->movable->getName().c_str();
					m_displayedObject = reinterpret_cast <TEDisplayable *>( m_testedScene->GetEntity( l_name));
					int l_height = m_displayedObject->BuildPanel( m_mainFrame->m_propertyDialog, m_mainFrame->m_propertyDialog->GetClientSize().x);
				}
			}
			l_manager->destroyQuery(mRaySceneQuery);
		}
	*/
}



void ElypseCtrl::OnButtonLeftUp( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( m_rightButton == false && m_middleButton == false )
	{
#if defined( _WIN32 )
		ReleaseCapture();
#endif
	}

	m_leftButton = false;

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseLUp( Real( l_point.x ), Real( l_point.y ) );
	}
}



void ElypseCtrl::OnButtonRightDown( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	wxPoint l_point = p_event.GetPosition();

	if ( l_point.x < 0 || l_point.y < 0 || l_point.x > m_width || l_point.y > m_height )
	{
		if ( m_leftButton == false && m_middleButton == false )
		{
			wxFocusEvent l_event( wxEVT_KILL_FOCUS );
			wxPostEvent( this, l_event );
		}
	}
	else
	{
		m_rightButton = true;
#if defined( _WIN32 )

		if ( GetFocus() != GetHWND() )
		{
			SetFocus();
		}

#endif
	}

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->MouseRDown( Real( l_point.x ), Real( l_point.y ) );
	}
}



void ElypseCtrl::OnButtonRightUp( wxMouseEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	if ( g_mainFrame->IsEditingOverlays() )
	{
		_showContextMenu( p_event.GetPosition() );
	}
	else
	{
		wxPoint l_point = p_event.GetPosition();

		if ( m_leftButton == false && m_middleButton == false )
		{
#if defined( _WIN32 )
			ReleaseCapture();
#endif
		}

		m_rightButton = false;

		if ( m_emuse->IsActive() )
		{
			m_emuse->GetFL()->MouseRUp( Real( l_point.x ), Real( l_point.y ) );
		}
	}
}



void ElypseCtrl::OnMouseEnter( wxMouseEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	_setCustomFocus( true );
}



void ElypseCtrl::OnMouseLeave( wxMouseEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	_setCustomFocus( false );
}



void ElypseCtrl::OnKeyUp( wxKeyEvent & p_event )
{
	if ( ! m_focused || m_destroyed )
	{
		return;
	}

	if ( m_emuse->IsActive() )
	{
		m_emuse->GetFL()->KeyUp( p_event.GetRawKeyCode() );
	}
}



void ElypseCtrl::OnKeyDown( wxKeyEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	if ( p_event.GetRawKeyCode() == WXK_F9 )
	{
		wxMenuEvent l_event( wxEVT_COMMAND_MENU_SELECTED, IdEditerScene );
		wxPostEvent( g_mainFrame, l_event );
		return;
	}

	if ( p_event.GetRawKeyCode() == WXK_F10 )
	{
		wxMenuEvent l_event( wxEVT_COMMAND_MENU_SELECTED, IdEditerOverlays );
		wxPostEvent( g_mainFrame, l_event );
		return;
	}

	if ( p_event.GetRawKeyCode() == WXK_F11 )
	{
		wxMenuEvent l_event( wxEVT_COMMAND_MENU_SELECTED, IdTester );
		wxPostEvent( g_mainFrame, l_event );
		return;
	}

	if ( ! m_focused )
	{
		return;
	}

	p_event.Skip();

	if ( ! m_emuse->IsActive() )
	{
		return;
	}

#if defined( _WIN32 )
	unsigned short l_flags = HIWORD( p_event.GetRawKeyFlags() );

	if ( ( l_flags & KF_REPEAT ) == KF_REPEAT )
	{
		m_emuse->GetFL()->KeyRepeat( p_event.GetRawKeyCode(), LOWORD( p_event.GetRawKeyFlags() ) );
		return;
	}

#endif
	m_emuse->GetFL()->KeyDown( p_event.GetRawKeyCode() );
}



void ElypseCtrl::OnChar( wxKeyEvent & p_event )
{
	if ( ! m_emuse->IsActive() || m_destroyed )
	{
		return;
	}

	m_emuse->GetFL()->OnChar( p_event.GetRawKeyCode() );
}



void ElypseCtrl::SetCtrlFocus()
{
	if ( ! m_destroyed )
	{
		_setCustomFocus( true );
	}
}



void ElypseCtrl::KillCtrlFocus()
{
	if ( ! m_destroyed )
	{
		_setCustomFocus( false );
	}
}



void ElypseCtrl::_onClose( wxCloseEvent & p_event )
{
	if ( m_project->GetShowDebug() )
	{
#if defined( _WIN32 )
		FreeConsole();
#endif
	}

	m_destroyed = true;

	try
	{
		m_emuse->WaitForDeletion();

		if ( EMuseController::GetSingletonPtr() != NULL && m_emuse->IsMain() )
		{
			EMuseController::GetSingletonPtr()->WaitForThreadEnded();
			EMuseController::Destroy();
		}

		delete m_emuse;
		m_emuse = NULL;
	}
	catch ( ... )
	{
		m_emuse = NULL;
	}

	delete m_plugin;
	m_plugin = NULL;
}



void ElypseCtrl::_onResize( wxCommandEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	String l_script = "editor_currentWidth = editor_currentWidth - (MOUSE_DIFFERENCE_X * GENERAL_SCREEN_WIDTH);";
	l_script += "editor_currentHeight = editor_currentHeight - (MOUSE_DIFFERENCE_Y * GENERAL_SCREEN_HEIGHT);";
	ScriptNode * l_node = ScriptEngine::GetContext()->scriptEngine->CompileScript( l_script );

	if ( l_node != NULL )
	{
		l_node->Use();
		l_node->Execute();
		delete l_node;
	}

	ScriptEngine::GetContext()->scriptEngine->GetVariable( "editor_resizeOverlay" )->set<bool>( true );
}



void ElypseCtrl::_onReplace( wxCommandEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	ScriptEngine::GetContext()->scriptEngine->GetVariable( "editor_moveOverlay" )->set<bool>( true );
}



void ElypseCtrl::_onNew( wxCommandEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	EMOverlay * l_overlay = ScriptEngine::GetContext()->scriptEngine->GetVariable( "editor_currentOverlay" )->get<EMOverlay *>();
	wxTextEntryDialog * l_dialog = new wxTextEntryDialog( this, wxT( "Choisissez un nom pour votre overlay" ), wxT( "Nom de l'overlay" ), wxT( "NewOverlay" ) );

	if ( l_dialog->ShowModal() == wxID_OK )
	{
		wxString l_name = l_dialog->GetValue();
		m_newOverlayFrame = new NewOverlayFrame( wxPoint( 200, 200 ), l_overlay, l_name );

		if ( m_newOverlayFrame->MustBeClosed() )
		{
			m_newOverlayFrame->Close();
			m_newOverlayFrame = NULL;
		}
		else
		{
			m_newOverlayFrame->Show();
		}
	}

	l_dialog->Destroy();
}



void ElypseCtrl::_onDelete( wxCommandEvent & p_event )
{
	if ( m_destroyed )
	{
		return;
	}

	EMOverlay * l_overlay = ScriptEngine::GetContext()->scriptEngine->GetVariable( "editor_currentOverlay" )->get<EMOverlay *>();

	if ( l_overlay != NULL )
	{
		TrollOverlay * l_teov = g_mainFrame->GetCurrentProject()->GetMainScene()->GetOverlay( wxString( l_overlay->GetName().c_str(), wxMBConvLibc() ) );
		l_overlay->GetOwner()->DestroyElement( l_overlay->GetName() );

		if ( l_teov != NULL )
		{
			delete l_teov;
		}
	}
}



void ElypseCtrl::_initialise()
{
	m_path = m_project->GetPath().char_str().data();
	m_museFile = ( m_project->GetName() + wxT( ".muse" ) ).char_str().data();
	m_fsaa = static_cast <unsigned int>( m_project->GetFSAA() ) * 2;
	std::cout << "ElypseCtrl::_initialise - Path : " << m_path << " - Muse file : " << m_museFile << "\n";
//	SetBackgroundColour( wxColour( 212, 208, 200));
	m_plugin = new EMusePlugin_wxWidget;
#if defined( _WIN32 )
	wxDynamicLibrary * l_pLibrary = new wxDynamicLibrary( wxT( "ieframe.dll" ) );
	String l_tempPath;
	m_plugin->SetDllInstance( l_pLibrary );
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;
	ZeroMemory( & osvi, sizeof( OSVERSIONINFO ) );
	osvi.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &osvi );
	bIsWindowsXPorLater = ( ( osvi.dwMajorVersion > 5 )
							|| ( ( osvi.dwMajorVersion == 5 ) && ( osvi.dwMinorVersion >= 1 ) ) );

	if ( osvi.dwMajorVersion >= 6 )
	{
		m_isVista = true;
		m_plugin->UseProtectedMode();
		char * l_userProfile = std::getenv( "userprofile" );
		m_installPath = l_userProfile;

		if ( m_installPath.empty() )
		{
			m_plugin->SetGraphicalStatus( StatusErrorDirectories );
			m_fucked = true;
		}

		l_tempPath = m_installPath + String( "\\AppData\\Roaming\\" );
		m_installPath += "\\AppData\\Roaming\\FDSSoftMedia\\Elypse\\";
	}
	else if ( osvi.dwMajorVersion > 5 )
	{
		m_isVista = true;
		m_plugin->UseProtectedMode();
		char * l_userProfile = std::getenv( "userprofile" );
		m_installPath = l_userProfile;

		if ( m_installPath.empty() )
		{
			m_plugin->SetGraphicalStatus( StatusErrorDirectories );
			m_fucked = true;
		}

		l_tempPath = m_installPath + String( "\\AppData\\LocalLow\\" );
		m_installPath += "\\AppData\\LocalLow\\FDSSoftMedia\\Elypse\\";
	}
	else
	{
		m_isVista = false;
		m_installPath = GetKeyFromReg( "SOFTWARE\\MICROSOFT\\WINDOWS\\CURRENTVERSION", "ProgramFilesDir" );
		m_plugin->SetProgramFilesDir( m_installPath );

		if ( m_installPath.empty() )
		{
			m_plugin->SetGraphicalStatus( StatusErrorDirectories );
			m_fucked = true;
		}

		l_tempPath = m_installPath;
		m_installPath += String( "\\FDSSoftMedia\\Elypse\\" );
	}

	if ( ! m_fucked )
	{
		String l_tempString;
		l_tempString = m_installPath;
		BOOL l_res;

		if ( _access( l_tempString.c_str(), 0 ) == -1 )
		{
			l_tempPath += "\\FDSSoftMedia\\";
			l_res = CreateDirectoryA( l_tempPath.c_str(), NULL );
			l_res = CreateDirectoryA( l_tempString.c_str(), NULL );
		}

		l_tempString = m_installPath + "rsc\\";

		if ( _access( l_tempString.c_str(), 0 ) == -1 )
		{
			l_res = CreateDirectoryA( l_tempString.c_str(), NULL );
		}

		l_tempString = m_installPath + "cfg\\";

		if ( _access( l_tempString.c_str(), 0 ) == -1 )
		{
			l_res = CreateDirectoryA( l_tempString.c_str(), NULL );
		}

		try
		{
			m_emuse = new TrollInstance( m_installPath , m_plugin );
		}
		catch ( ... )
		{
			m_plugin->SetGraphicalStatus( StatusErrorOgre );
		}
	}

#else
	char l_pPath[FILENAME_MAX];
	char l_szTmp[32];
	sprintf( l_szTmp, "/proc/%d/exe", getpid() );
	int l_iBytes = std::min< size_t >( readlink( l_szTmp, l_pPath, sizeof( l_pPath ) ), sizeof( l_pPath ) - 1 );

	if ( l_iBytes >= 0 )
	{
		l_pPath[l_iBytes] = '\0';
		m_installPath = l_pPath;
	}

	std::size_t l_uiIndex = m_installPath.find_last_of( "/" );
	m_installPath = m_installPath.substr( 0, l_uiIndex );
	l_uiIndex = m_installPath.find_last_of( "/" );
	m_installPath = m_installPath.substr( 0, l_uiIndex );

	try
	{
		m_emuse = new TrollInstance( m_installPath, m_plugin );
	}
	catch ( ... )
	{
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}

#endif
	m_plugin->InitGDI();
	wxRect l_rect = GetClientRect();
	m_height = Real( l_rect.height );
	m_width = Real( l_rect.width );

	if ( m_project->GetShowDebug() )
	{
		EMuseController::GetSingletonPtr()->ShowConsole();
#if defined( _WIN32 )
		AllocConsole();
		freopen( "CONOUT$", "w", stdout );
#endif
	}
}



void ElypseCtrl::_showContextMenu( const wxPoint & p_position )
{
	if ( m_destroyed )
	{
		return;
	}

	wxMenu l_menu;
	String l_script = "editor_resizeOverlay = false;";
	l_script += "editor_moveOverlay = false;";
	l_script += "editor_currentOverlay = OverlayElement_MousePickup( MOUSE_POSITION_X, MOUSE_POSITION_Y);";
	l_script += "editor_currentWidth = editor_currentOverlay->GetWidth();";
	l_script += "editor_currentHeight = editor_currentOverlay->GetHeight();";
	ScriptNode * l_node = ScriptEngine::GetContext()->scriptEngine->CompileScript( l_script );

	if ( l_node != NULL )
	{
		l_node->Use();
		l_node->Execute();
		delete l_node;
	}

	if ( ScriptEngine::GetContext()->scriptEngine->GetVariable( "editor_currentOverlay" )->get<EMOverlay *>() != NULL )
	{
		l_menu.Append( tpIdResize, wxT( "Redimensionner" ) );
		l_menu.Append( tpIdReplace, wxT( "Déplacer" ) );
//		l_menu.Append( tpIdDelete, wxT( "Supprimer"));
	}

	l_menu.Append( tpIdNew, wxT( "Nouveau" ) );
	PopupMenu( & l_menu, p_position.x, p_position.y );
}


