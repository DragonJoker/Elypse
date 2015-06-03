#include "PrecompiledHeader.h"

#include "EMuseLogs.h"

#include "EMuseController.h"
#include <Mutex.h>
#include <ThreadId.h>
#include <OgreLog.h>

String EMPTY_STRING = String();

GENLIB_INIT_SINGLETON_FOR_DLL( EMuse::Debug::EMuseLogs );

EMuse::Debug::EMuseLogs::EMuseLogs()
{
	GENLIB_SET_SINGLETON_FOR_DLL();
}

EMuse::Debug::EMuseLogs::~EMuseLogs()
{
	sm_singleton = NULL;
}

void EMuse::Debug::EMuseLogs::_logMessage( const String & p_comment )
{
	if ( EMuseController::GetSingletonPtr() != NULL )
	{
		Log * l_log = EMuseController::GetSingletonPtr()->GetLog();

		if ( NULL != l_log )
		{
			GENLIB_SCOPED_LOCK( m_logMutex );
			l_log->logMessage( p_comment );
		}

#if defined( _MSC_VER )

		if ( IsDebuggerPresent() )
		{
			OutputDebugStringA( ( p_comment + "\n" ).c_str() );
		}

#endif
	}
}

void EMuse::Debug::EMuseLogs::_consoleMessage( const String & p_comment )
{
	GENLIB_SCOPED_LOCK( m_consoleMutex );
	std::cout << p_comment << std::endl;
#if defined( _MSC_VER )

	if ( IsDebuggerPresent() )
	{
		OutputDebugStringA( ( p_comment + "\n" ).c_str() );
	}

#endif
}
