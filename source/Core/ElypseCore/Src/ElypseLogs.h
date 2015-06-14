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
			Mutex m_logMutex;
			Mutex m_consoleMutex;

		public:
			ElypseLogs();
			virtual ~ElypseLogs();

		protected:
			virtual void _logMessage( const String & p_comment );
			virtual void _consoleMessage( const String & p_comment );

		public:
			static inline void Elypse_Message( const String & p_comment )
			{
				Elypse_LogMessage( p_comment );
				Elypse_ConsoleMessage( p_comment );
			}
			static inline void Elypse_LogMessage( const String & p_comment )
			{
				if ( sm_singleton != NULL )
				{
					sm_singleton->_logMessage( p_comment );
				}
			}
			static inline void Elypse_ConsoleMessage( const String & p_comment )
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
