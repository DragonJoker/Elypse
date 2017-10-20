/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/LogCtrl.h"

#include "TrollEditorApplication.h"

#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
#include "GUI/StcTextEditor.h"

#include "Project/Scene.h"

#include <StringUtils.h>

#include <regex>

namespace Troll
{
	namespace GUI
	{
		BEGIN_EVENT_TABLE( LogCtrl, wxListBox )
			EVT_LEFT_DCLICK( LogCtrl::OnDoubleClick )
		END_EVENT_TABLE()

		LogCtrl::LogCtrl( wxWindow * parent, wxWindowID id, wxPoint const & pos, wxSize const & size, long style )
			: wxListBox( parent, id, pos, size, wxArrayString(), style )
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );
		}

		void LogCtrl::OnDoubleClick( wxMouseEvent & p_event )
		{
			int l_selection = GetSelection();

			if ( l_selection >= 0 )
			{
				std::string l_line = make_string( GetString( l_selection ) );
				const std::regex l_regexCompileError( "[^[]*\\[[^\\\\]*[\\\\]*([^@]*)@ L# ([0-9]*) \\][^\n]*" );
				std::smatch l_results;
				std::regex_match( l_line, l_results, l_regexCompileError );

				if ( l_results.size() == 3 )
				{
					std::string l_fileName = l_results[1].str();
					General::Utils::string::trim( l_fileName );
					auto l_lineIndex = atol( l_results[2].str().c_str() );
					wxGetApp().GetMainFrame()->GotoFileLine( make_wxString( l_fileName ), uint32_t( l_lineIndex - 1 ) );
				}
			}

			p_event.Skip();
		}
	}
}
