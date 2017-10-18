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
	sm_singleton = NULL;
}

void Elypse::Debug::ElypseLogs::DoLogMessage( String const & p_comment )
{
	if ( ElypseController::GetSingletonPtr() != NULL )
	{
		Log * l_log = ElypseController::GetSingletonPtr()->GetLog();

		if ( NULL != l_log )
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
