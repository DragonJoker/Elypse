/*
See LICENSE file in root folder
*/
#ifndef ___EMUSE_UTILS_H___
#define ___EMUSE_UTILS_H___

#include "Config.h"

#include <chrono>
#include <thread>

#if GENLIB_WINDOWS
//In case windows.h was already included ...
#	ifndef _WINDOWS_

extern "C"
{
	__declspec( dllimport ) uint32_t __stdcall GetWindowsDirectoryA( char * lpBuffer, uint32_t uSize );
	__declspec( dllimport ) int __stdcall GetSystemMetrics( int p_code );
}

#	endif

#endif

using namespace std::literals;

#endif
