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

#include "GUI/LogListBox.h"

namespace Troll
{
	namespace GUI
	{
		wxLogListBox::wxLogListBox( wxListBox * p_list )
			: m_list( p_list )
		{
		}

		wxLogListBox::~wxLogListBox()
		{
		}

		void wxLogListBox::DoLogTextAtLevel( wxLogLevel p_level, wxString const & p_msg )
		{
			// don't put trace messages into listbox or we can get into infinite
			// recursion
			if ( p_level != wxLOG_Trace )
			{
				wxLog::DoLogTextAtLevel( p_level, p_msg );
			}
		}

		void wxLogListBox::DoLogText( wxString const & p_msg )
		{
			wxString l_msg;
			TimeStamp( &l_msg );
			wxString l_line = p_msg;
			size_t l_index = p_msg.find( wxT( "Compiler Error" ) );

			if ( l_index != wxString::npos )
			{
				l_line = l_line.substr( l_index );
			}

			l_msg += l_line;
			m_list->Append( l_msg );
			m_list->SetFirstItem( m_list->GetCount() - 1 );
		}
	}
}
