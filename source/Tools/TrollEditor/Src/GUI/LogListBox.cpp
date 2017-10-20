/*
See LICENSE file in root folder
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
