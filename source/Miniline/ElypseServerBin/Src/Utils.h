/*****************************************************************************

	Author : Marc BILLON
	Compagny : Fordev Studio - Copyright 2007 - 2008

*****************************************************************************/
#ifndef ___EMUSE_UTILS_H___
#define ___EMUSE_UTILS_H___

/*
	Purpose : predeclaration of various utility functions present in standard headers, without the need to include most of them.
	Also standardises the Sleep function.
*/

#ifdef WIN32
	//In case windows.h was already included ...
#	ifndef _WINDOWS_

		extern "C"
		{
			__declspec( dllimport) void			__stdcall Sleep						( unsigned long);
			__declspec( dllimport) unsigned int	__stdcall GetWindowsDirectoryA		( char * lpBuffer, unsigned int uSize);
			__declspec( dllimport) void			__stdcall QueryPerformanceCounter	( long long * p_perfCount);
			__declspec( dllimport) void			__stdcall QueryPerformanceFrequency	( long long * p_perfFreq);
			__declspec( dllimport) int			__stdcall GetSystemMetrics			( int p_code);
		}

#	endif

#	define GENLIB_SLEEP( time) Sleep( time)

#elif defined( __GNUG__)

#	include <iostream>
#	include <time.h>
#	include <errno.h>
#	include <string.h>

namespace General
{ namespace Utils
{
	inline void Sleep( double p_time)
	{
		timespec l_timeToWait;
		timespec l_timeRemaining;
	
		l_timeToWait.tv_sec = static_cast <long> ( p_time);
		l_timeToWait.tv_nsec = static_cast <long> ( (p_time - l_timeToWait.tv_sec) * 1000000000);

		while (true)
		{
			if (nanosleep( & l_timeToWait, & l_timeRemaining) == 0)
			{
				return;
			}

			if (errno != EINTR)
			{	/*	2 types d'erreur possibles :
					- EFAULT (les paramètres ne sont pas dans l'espace d'adressage du programme),
					- EINVAL (le champ tv_nsec n'est pas dans l'intervalle [0, 999 999 999]).
				*/
				std::cerr << "Trouble encountered when sleeping : [" << strerror( errno) << "]" << std::endl;
				return;
			}

			l_timeToWait.tv_sec = l_timeRemaining.tv_sec;
			l_timeToWait.tv_nsec = l_timeRemaining.tv_nsec;
		}
	}
}
}

#	define GENLIB_SLEEP( time) General::Utils::Sleep( time / 1000.0)

#endif
#endif
