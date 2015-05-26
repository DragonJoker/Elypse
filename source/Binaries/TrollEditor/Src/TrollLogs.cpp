#include "PrecompiledHeader.h"

#include "TrollLogs.h"
#include "GUI/MainFrame.h"
/*
#include <General/Mutex.h>
*/
using namespace Troll;
using namespace Troll::GUI;

extern MainFrame * g_mainFrame;

#ifndef ELYPSE_STATIC_LIB
//String EMPTY_STRING = String();
#endif

TrollLogs :: TrollLogs()
{
	GENLIB_SET_SINGLETON();
}

void TrollLogs :: _logMessage( const String & p_comment )
{
	if ( sm_singleton != NULL )
	{
		GENLIB_SCOPED_LOCK( m_logMutex );
		g_mainFrame->LogOutMessage( p_comment );
	}
}

void TrollLogs :: _consoleMessage( const String & p_comment )
{
	if ( sm_singleton != NULL )
	{
		GENLIB_SCOPED_LOCK( m_consoleMutex );
		std::cout << p_comment << std::endl;
	}
}
