/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ElypseLogs.h"

#include "ElypseController.h"
#include <Mutex.h>
#include <OgreLog.h>

String EMPTY_STRING = String();

GENLIB_INIT_SINGLETON_FOR_DLL( Elypse::Debug::ElypseLogs );

Elypse::Debug::ElypseLogs::ElypseLogs()
{
	GENLIB_SET_SINGLETON_FOR_DLL();
}

Elypse::Debug::ElypseLogs::~ElypseLogs()
{
	sm_singleton = nullptr;
}

void Elypse::Debug::ElypseLogs::DoLogMessage( String const & p_comment )
{
	if ( ElypseController::GetSingletonPtr() != nullptr )
	{
		Log * l_log = ElypseController::GetSingletonPtr()->GetLog();

		if ( nullptr != l_log )
		{
			auto l_lock = make_unique_lock( m_logMutex );
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

void Elypse::Debug::ElypseLogs::DoConsoleMessage( String const & p_comment )
{
	auto l_lock = make_unique_lock( m_consoleMutex );
	std::cout << p_comment << std::endl;
#if defined( _MSC_VER )

	if ( IsDebuggerPresent() )
	{
		OutputDebugStringA( ( p_comment + "\n" ).c_str() );
	}

#endif
}
