/*
See LICENSE file in root folder
*/
#ifndef ___MACROS_H___
#define ___MACROS_H___

#include "Config.h"

#ifndef ___MUSE_DEBUG___
#	define d_debug_only
#	define d_debug_assert( X)
#else
#	define d_debug_only if( 0)
#	define d_debug_assert( X) Assert( X)
#endif

#ifndef __FUNCTION__
#	define __FUNCTION__ ""
#endif

#ifndef __COUNTER__
#	define __COUNTER__ ""
#endif

#ifndef ___GENLIB_DLL___
#	define d_dll
#else
#	define d_dll d_dllexport
#endif

#define d_coucou std::cout << "Coucou ~ " << __FILE__ << "::" << __FUNCTION__ << " @ line " << __LINE__ << std::endl

#endif
