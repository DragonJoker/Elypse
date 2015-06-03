#include "PrecompiledHeader.h"

#include "TrollLogs.h"

#include "GUI/MainFrame.h"

using namespace Troll;
using namespace Troll::GUI;

TrollLogs::TrollLogs()
{
	GENLIB_SET_SINGLETON_FOR_DLL();
}

void TrollLogs::_logMessage( const String & p_comment )
{
	if ( sm_singleton != NULL )
	{
		GENLIB_SCOPED_LOCK( m_logMutex );
		g_mainFrame->LogOutMessage( p_comment );
	}
}

void TrollLogs::_consoleMessage( const String & p_comment )
{
	if ( sm_singleton != NULL )
	{
		GENLIB_SCOPED_LOCK( m_consoleMutex );
		std::cout << p_comment << std::endl;
	}
}
