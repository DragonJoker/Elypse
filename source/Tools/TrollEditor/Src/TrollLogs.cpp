/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "TrollLogs.h"

#include "GUI/MainFrame.h"

namespace Troll
{
	TrollLogs::TrollLogs()
	{
		GENLIB_SET_SINGLETON_FOR_DLL();
	}

	void TrollLogs::DoLogMessage( String const & p_comment )
	{
		if ( sm_singleton )
		{
			auto l_lock = make_unique_lock( m_logMutex );
			LogMessage( make_wxString( p_comment ) );
		}
	}

	void TrollLogs::DoConsoleMessage( String const & p_comment )
	{
		if ( sm_singleton )
		{
			auto l_lock = make_unique_lock( m_consoleMutex );
			std::clog << p_comment << std::endl;
		}
	}
}
