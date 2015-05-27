
#include "PrecompiledHeader.h"

#include "GUI/Popup.h"
#include "GUI/MainFrame.h"

#include "xpm/troll_editor.xpm"

using namespace Troll::GUI;

extern MainFrame * g_mainFrame;

enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT,
	Minimal_TestDialog,
	Minimal_StartSimplePopup,
	Minimal_StartScrolledPopup,
	Minimal_LogWindow,
	Minimal_PopupButton,
	Minimal_PopupSpinctrl
};

BEGIN_EVENT_TABLE( SimpleTransientPopup, wxPopupTransientWindow )
	EVT_MOUSE_EVENTS(	SimpleTransientPopup::_onMouse )
	EVT_SIZE(	SimpleTransientPopup::_onSize )
	EVT_SET_FOCUS(	SimpleTransientPopup::_onSetFocus )
	EVT_KILL_FOCUS(	SimpleTransientPopup::_onKillFocus )
	EVT_BUTTON(	Minimal_PopupButton,	SimpleTransientPopup::_onButton )
	EVT_SPINCTRL(	Minimal_PopupSpinctrl,	SimpleTransientPopup::_onSpinCtrl )
END_EVENT_TABLE()


SimpleTransientPopup :: SimpleTransientPopup( wxWindow * p_parent )
	:	wxPopupTransientWindow( p_parent )
{
	m_panel = new wxScrolledWindow( this, wxID_ANY );
	m_panel->SetBackgroundColour( * wxLIGHT_GREY );
	// Keep this code to verify if mouse events work, they're required if
	// you're making a control like a combobox where the items are highlighted
	// under the cursor, the m_panel is set focus in the Popup() function
	m_panel->Connect( wxEVT_MOTION, wxMouseEventHandler( SimpleTransientPopup::_onMouse ), NULL, this );
	wxStaticText * text = new wxStaticText( m_panel, wxID_ANY,
											_T( "wx.PopupTransientWindow is a\n" )
											_T( "wx.PopupWindow which disappears\n" )
											_T( "automatically when the user\n" )
											_T( "clicks the mouse outside it or if it\n" )
											_T( "(or its first child) loses focus in \n" )
											_T( "any other way." ) );
	m_button = new wxButton( m_panel, Minimal_PopupButton, wxT( "Press Me" ) );
	m_spinCtrl = new wxSpinCtrl( m_panel, Minimal_PopupSpinctrl, wxT( "Hello" ) );
	m_mouseText = new wxStaticText( m_panel, wxID_ANY,
									wxT( "<- Test Mouse ->" ) );
	wxBoxSizer * topSizer = new wxBoxSizer( wxVERTICAL );
	topSizer->Add( text, 0, wxALL, 5 );
	topSizer->Add( m_button, 0, wxALL, 5 );
	topSizer->Add( m_spinCtrl, 0, wxALL, 5 );
	topSizer->Add( m_mouseText, 0, wxCENTRE | wxALL, 5 );
	m_panel->SetAutoLayout( true );
	m_panel->SetSizer( topSizer );
	topSizer->Fit( m_panel );
	topSizer->Fit( this );
}



SimpleTransientPopup :: ~SimpleTransientPopup()
{
}



void SimpleTransientPopup :: Popup( wxWindow * p_focus )
{
	g_mainFrame->LogDebugMessage( wxT( "0x%lx SimpleTransientPopup::Popup" ) );
	wxPopupTransientWindow::Popup( p_focus ? p_focus : m_panel );
}



void SimpleTransientPopup :: OnDismiss()
{
	g_mainFrame->LogDebugMessage( wxT( "0x%lx SimpleTransientPopup::OnDismiss" ) );
	wxPopupTransientWindow::OnDismiss();
}



bool SimpleTransientPopup :: ProcessLeftDown( wxMouseEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "0x%lx SimpleTransientPopup::ProcessLeftDown" ) );
	return wxPopupTransientWindow::ProcessLeftDown( p_event );
}



bool SimpleTransientPopup :: Show( bool show )
{
	g_mainFrame->LogDebugMessage( wxT( "0x%lx SimpleTransientPopup::Show" ) );
	return wxPopupTransientWindow::Show( show );
}



void SimpleTransientPopup :: _onSize( wxSizeEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "0x%lx SimpleTransientPopup::OnSize" ) );
	p_event.Skip();
}



void SimpleTransientPopup :: _onSetFocus( wxFocusEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "SimpleTransientPopup::OnSetFocus" ) );
	p_event.Skip();
}



void SimpleTransientPopup :: _onKillFocus( wxFocusEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "SimpleTransientPopup::OnKillFocus" ) );
	p_event.Skip();
}



void SimpleTransientPopup :: _onMouse( wxMouseEvent & p_event )
{
	wxRect rect( m_mouseText->GetRect() );
	rect.SetX( -100000 );
	rect.SetWidth( 1000000 );
	wxColour colour( *wxLIGHT_GREY );

	if ( rect.Contains( p_event.GetPosition() ) )
	{
		colour = wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT );
		g_mainFrame->LogDebugMessage( wxT( "SimpleTransientPopup::OnMouse" ) );
	}

	if ( colour != m_mouseText->GetBackgroundColour() )
	{
		m_mouseText->SetBackgroundColour( colour );
		m_mouseText->Refresh();
	}

	p_event.Skip();
}



void SimpleTransientPopup :: _onButton( wxCommandEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "SimpleTransientPopup::OnButton" ) );
	wxButton * button = wxDynamicCast( p_event.GetEventObject(), wxButton );

	if ( button->GetLabel() == wxT( "Press Me" ) )
	{
		button->SetLabel( wxT( "Pressed" ) );
	}
	else
	{
		button->SetLabel( wxT( "Press Me" ) );
	}

	p_event.Skip();
}



void SimpleTransientPopup :: _onSpinCtrl( wxSpinEvent & p_event )
{
	g_mainFrame->LogDebugMessage( wxT( "SimpleTransientPopup::OnSpinCtrl" ) );
	p_event.Skip();
}


