#ifndef ___GENLIB_CONFIG_H___
#define ___GENLIB_CONFIG_H___

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 1
#elif defined(linux) || defined(__linux) || defined(__linux__)
#	define GENLIB_LINUX 1
#	define GENLIB_MACOS 0
#	define GENLIB_WINDOWS 0
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#	define GENLIB_LINUX 0
#	define GENLIB_MACOS 1
#	define GENLIB_WINDOWS 0
#endif

#if defined( _MSC_VER)
#	if _MSC_VER >= 1800
#		define GENLIB_HAS_STDTHREAD					1
#		define GENLIB_HAS_STDFUNCTIONAL				1
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER >= 1700
#		define GENLIB_HAS_STDTHREAD					1
#		define GENLIB_HAS_STDFUNCTIONAL				1
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER >= 1600
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	elif _MSC_VER > 1500
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	else
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	endif
#elif defined( __GNUG__)
#	define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if GCC_VERSION >= 40600
#		if defined( _WIN32 )
#			define GENLIB_HAS_STDTHREAD				0
#			define GENLIB_HAS_STDFUNCTIONAL			1
#			define GENLIB_HAS_TR1FUNCTIONAL			1
#		else
#			define GENLIB_HAS_STDTHREAD				1
#			define GENLIB_HAS_STDFUNCTIONAL			1
#			define GENLIB_HAS_TR1FUNCTIONAL			1
#		endif
#	elif GCC_VERSION >= 40300
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				1
#	else
#		define GENLIB_HAS_STDTHREAD					0
#		define GENLIB_HAS_STDFUNCTIONAL				0
#		define GENLIB_HAS_TR1FUNCTIONAL				0
#	endif
#else
#	error "Yet unsupported compiler"
#endif

#endif
