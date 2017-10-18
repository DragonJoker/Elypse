/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
		l_parser.AddSwitch(	wxT( "h" ), wxT( "help" ), _( "Displays this help" ) );
		l_parser.AddSwitch(	wxT( "d" ), wxT( "debug" ), _( "Displays debug logs in console log" ) );
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
