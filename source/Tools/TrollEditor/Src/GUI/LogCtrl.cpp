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

#include "GUI/LogCtrl.h"

#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
#include "GUI/StcTextEditor.hpp"

#include "Project/Scene.h"

#include <StringUtils.h>

#include <regex>

BEGIN_TROLL_GUI_NAMESPACE
{
	BEGIN_EVENT_TABLE( LogCtrl, wxListBox )
		EVT_LEFT_DCLICK( LogCtrl::OnDoubleClick )
	END_EVENT_TABLE()

	LogCtrl::LogCtrl( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, long style )
		: wxListBox( parent, id, pos, size, wxArrayString(), style )
	{
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
			TROLL_PROJECT_NAMESPACE::File * l_file = NULL;
			int l_lineIndex = 1;

			if ( l_results.size() == 3 )
			{
				std::string l_fileName = l_results[1].str();
				General::Utils::string::trim( l_fileName );
				l_lineIndex = atol( l_results[2].str().c_str() );
				l_file = GUI::MainFrame::GetInstance()->GetFile( make_wxString( l_fileName ) );
			}

			if ( l_file )
			{
				GUI::MainFrame::GetInstance()->SelectFileTab( l_file->IdOnglet );
				std::clog << "LogCtrl::OnDoubleClick - " << l_file->IdOnglet << "\n";
				wxStcTextEditor * l_edit = l_file->EditPanel;
				l_edit->SetFocus();
				l_edit->GotoLine( l_lineIndex - 1 );
			}
		}

		p_event.Skip();
	}
}
END_TROLL_GUI_NAMESPACE
