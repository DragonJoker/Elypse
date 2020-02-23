/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_LOGS___
#define ___EMUSE_LOGS___

//#define ____MUSE_DEBUG____

#include "Elypse.h"

#include <Mutex.h>
#include <Singleton.h>

namespace Elypse
{
	namespace Debug
	{
		class d_dll_export ElypseLogs
		{
		protected:
			static ElypseLogs * sm_singleton;

		protected:
			std::mutex m_logMutex;
			std::mutex m_consoleMutex;

		public:
			ElypseLogs();
			virtual ~ElypseLogs();

		protected:
			virtual void DoLogMessage( String const & p_comment );
			virtual void DoConsoleMessage( String const & p_comment );

		public:
			static inline void Elypse_Message( String const & p_comment )
			{
				Elypse_LogMessage( p_comment );
				Elypse_ConsoleMessage( p_comment );
			}
			static inline void Elypse_LogMessage( String const & p_comment )
			{
				if ( sm_singleton != nullptr )
				{
					sm_singleton->DoLogMessage( p_comment );
				}
			}
			static inline void Elypse_ConsoleMessage( String const & p_comment )
			{
				if ( sm_singleton != nullptr )
				{
					sm_singleton->DoConsoleMessage( p_comment );
				}
				else
				{
					std::cout << p_comment << std::endl;
				}
			}

			GENLIB_SINGLETON_FOR_DLL( ElypseLogs );
		};
	}
}

#ifdef ____MUSE_DEBUG____

#define DETECT_MEMORY_LEAKS 0
#define MC_MULTY_THREAD_MODE 0
#define EMUSE_WARNING_LEVEL_FIVE 1

#define EMUSE_MESSAGE_DEBUG( p_comment ) Elypse::Debug::ElypseLogs::Muse_Message( p_comment)
#define EMUSE_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_Message( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )

#else

#ifndef ____MUSE_RELEASE____

#define MUSE_DEBUG_LEVEL EMML_Release

#define EMUSE_MESSAGE_DEBUG( p_comment )
#define EMUSE_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_Message( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )

#else

#define MUSE_DEBUG_LEVEL EMML_DebugNormal

#define DETECT_MEMORY_LEAKS 0
#define MC_MULTY_THREAD_MODE 0
#define EMUSE_WARNING_LEVEL_FIVE 0

#define EMUSE_MESSAGE_DEBUG( p_comment )
#define EMUSE_MESSAGE_NORMAL( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) Elypse::Debug::ElypseLogs::Elypse_LogMessage( p_comment )

#endif

#endif

#endif
