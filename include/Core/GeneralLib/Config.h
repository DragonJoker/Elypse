/*
See LICENSE file in root folder
*/
#ifndef ___GENLIB_CONFIG_H___
#define ___GENLIB_CONFIG_H___

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 1
#	include <sdkddkver.h>
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define GENLIB_LINUX 1
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 0
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 1
#	define GENLIB_WINDOWS 0
#endif

#if !defined( _MSC_VER) && !defined( __GNUG__)
#	error "Yet unsupported compiler"
#endif

#endif
