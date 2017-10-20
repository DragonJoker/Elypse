/*
See LICENSE file in root folder
*/
#ifndef ___MULTITHREAD_CONFIG_H___
#define ___MULTITHREAD_CONFIG_H___

#include "Config.h"

#ifndef USE_MUTEX_MANAGER
#	define USE_MUTEX_MANAGER 0
#endif

#define GENLIB_MT_USE_STL 1
#define RECURSIVE_MUTEXES_AVAILABLE 1

#ifndef BOOST_FUNCTION_MAX_ARGS
#	define BOOST_FUNCTION_MAX_ARGS 2
#endif

#if USE_MUTEX_MANAGER
#	include "MutexManager.h"
#endif

#endif
