#ifndef ___EMUSE_LOGS___
#define ___EMUSE_LOGS___

//#define ____MUSE_DEBUG____

#include "EMuse.h"

#include <Mutex.h>
#include <Singleton.h>

namespace EMuse
{
	namespace Debug
	{
		class d_dll_export EMuseLogs
		{
		protected:
			static EMuseLogs * sm_singleton;

		protected:
			Mutex m_logMutex;
			Mutex m_consoleMutex;

		public:
			EMuseLogs();
			virtual ~EMuseLogs();

		protected:
			virtual void _logMessage( const String & p_comment );
			virtual void _consoleMessage( const String & p_comment );

		public:
			static inline void EMuse_Message( const String & p_comment )
			{
				EMuse_LogMessage( p_comment );
				EMuse_ConsoleMessage( p_comment );
			}
			static inline void EMuse_LogMessage( const String & p_comment )
			{
				if ( sm_singleton != NULL )
				{
					sm_singleton->_logMessage( p_comment );
				}
			}
			static inline void EMuse_ConsoleMessage( const String & p_comment )
			{
				if ( sm_singleton != NULL )
				{
					sm_singleton->_consoleMessage( p_comment );
				}
				else
				{
					std::cout << p_comment << std::endl;
				}
			}

			GENLIB_SINGLETON_FOR_DLL( EMuseLogs );
		};
	}
}

#ifdef ____MUSE_DEBUG____

#define DETECT_MEMORY_LEAKS 0
#define MC_MULTY_THREAD_MODE 0
#define EMUSE_WARNING_LEVEL_FIVE 1

#define EMUSE_MESSAGE_DEBUG( p_comment ) EMuse::Debug::EMuseLogs::Muse_Message( p_comment)
#define EMUSE_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_Message( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )

#else

#ifndef ____MUSE_RELEASE____

#define MUSE_DEBUG_LEVEL EMML_Release

#define EMUSE_MESSAGE_DEBUG( p_comment )
#define EMUSE_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_Message( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )

#else

#define MUSE_DEBUG_LEVEL EMML_DebugNormal

#define DETECT_MEMORY_LEAKS 0
#define MC_MULTY_THREAD_MODE 0
#define EMUSE_WARNING_LEVEL_FIVE 0

#define EMUSE_MESSAGE_DEBUG( p_comment )
#define EMUSE_MESSAGE_NORMAL( p_comment )
#define EMUSE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_Message( p_comment )

#define EMUSE_CONSOLE_MESSAGE_DEBUG( p_comment )
#define EMUSE_CONSOLE_MESSAGE_NORMAL( p_comment )
#define EMUSE_CONSOLE_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_ConsoleMessage( p_comment )

#define EMUSE_LOG_MESSAGE_DEBUG( p_comment )
#define EMUSE_LOG_MESSAGE_NORMAL( p_comment )
#define EMUSE_LOG_MESSAGE_RELEASE( p_comment ) EMuse::Debug::EMuseLogs::EMuse_LogMessage( p_comment )

#endif

#endif

#endif
