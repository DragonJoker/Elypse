#include "PrecompiledHeader.h"

#include "LogListBox.h"

wxLogListBox::wxLogListBox( wxListBox * p_list )
	: m_list( p_list )
{
}

wxLogListBox::~wxLogListBox()
{
}

void wxLogListBox::DoLog( wxLogLevel p_level, const wxChar * p_line, time_t p_time )
{
	// don't put trace messages into listbox or we can get into infinite
	// recursion
	if ( p_level != wxLOG_Trace )
	{
		wxLog::DoLog( p_level, p_line, p_time );
	}
}

void wxLogListBox::DoLogString( const wxChar * p_line, time_t p_time )
{
	wxString msg;
	TimeStamp( &msg );
	wxString l_line( p_line );
	size_t l_index = l_line.find( wxT( "Compiler Error" ) );

	if ( l_index != wxString::npos )
	{
		l_line = l_line.substr( l_index );
	}

	msg += l_line;

#ifdef __WXUNIVERSAL__
	m_list->AppendAndEnsureVisible( msg );
#else // other ports don't have this method yet
	m_list->Append( msg );
	m_list->SetFirstItem( m_list->GetCount() - 1 );
#endif
}