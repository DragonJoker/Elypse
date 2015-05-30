#ifndef ___MULTITHREAD_CONFIG_H___
#define ___MULTITHREAD_CONFIG_H___

#ifndef GENLIB_MT_USE_BOOST
#	define GENLIB_MT_USE_BOOST 1
#endif

#ifndef GENLIB_MT_USE_MFC
#	define GENLIB_MT_USE_MFC 0
#endif

#ifndef USE_MUTEX_MANAGER
#	define USE_MUTEX_MANAGER 0
#endif

#ifndef BOOST_FUNCTION_MAX_ARGS
#	define BOOST_FUNCTION_MAX_ARGS 2
#endif

#if ! GENLIB_MT_USE_MFC && ! GENLIB_MT_USE_BOOST
#	error Define either GENLIB_MT_USE_MFC or GENLIB_MT_USE_BOOST
#elif GENLIB_MT_USE_BOOST && GENLIB_MT_USE_MFC
#	error Definition of both GENLIB_MT_USE_MFC and GENLIB_MT_USE_BOOST. Choose only one
#elif GENLIB_MT_USE_BOOST
#	define RECURSIVE_MUTEXES_AVAILABLE 1
#else
#	define RECURSIVE_MUTEXES_AVAILABLE 0
#endif

#if USE_MUTEX_MANAGER
//#include "MutexManager.h"
#endif

#endif
