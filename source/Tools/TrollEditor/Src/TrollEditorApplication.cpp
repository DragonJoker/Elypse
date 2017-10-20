/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TrollEditorApplication.h"

#include "GUI/MainFrame.h"

#include <wx/cmdline.h>

wxIMPLEMENT_APP( Troll::Application );

namespace Troll
{
	Application::Application()
		: wxApp{}
	{
	}

	Application::~Application()
	{
	}

	bool Application::OnInit()
	{
		bool l_return = true;
		wxAppConsole::SetAppName( wxT( "troll_editor" ) );
		wxAppConsole::SetVendorName( wxT( "dragonjoker" ) );
#if wxCHECK_VERSION( 2, 9, 0 )
		wxAppConsole::SetAppDisplayName( wxT( "Troll Editor" ) );
		wxAppConsole::SetVendorDisplayName( wxT( "DragonJoker" ) );
#endif
		wxCmdLineParser l_parser( argc, argv );
		l_parser.AddSwitch(	wxT( "h" ), _( "help" ), _( "Displays this help" ) );
		l_parser.AddSwitch(	wxT( "d" ), _( "debug" ), _( "Displays debug logs in console log" ) );
		l_parser.AddParam( wxT( "project file" ), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL );
		l_parser.Parse();

		// S'il y avait des erreurs ou "-h" ou "--help", on affiche l'aide et on sort
		if ( l_parser.Found( wxT( 'h' ) ) )
		{
			l_parser.Usage();
			l_return = false;
		}

		if ( l_return )
		{
#if defined( NDEBUG )
			bool l_debug( l_parser.Found( wxT( 'd' ) ) );
#else
			bool l_debug( true );
#endif
			m_mainFrame = new GUI::MainFrame { l_debug, wxT( "Troll Editor" ), 50, 50, 800, 600 };
			m_mainFrame->Show();
			m_mainFrame->Maximize();

			if ( l_parser.GetParamCount() )
			{
				m_mainFrame->OpenProjectFile( l_parser.GetParam() );
			}

			SetTopWindow( m_mainFrame );
			SetExitOnFrameDelete( true );
		}

		return l_return;
	}

	int Application::OnExit()
	{
		return wxApp::OnExit();
	}

	void LogDebug( wxString const & p_message )
	{
		wxGetApp().GetMainFrame()->LogDebug( p_message );
	}

	void LogMessage( wxString const & p_message )
	{
		wxGetApp().GetMainFrame()->LogMessage( p_message );
	}

	void LogError( wxString const & p_message )
	{
		wxGetApp().GetMainFrame()->LogError( p_message );
	}
}
