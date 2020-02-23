/*
See LICENSE file in root folder
*/
#ifndef ___GENLIB_ASSERTIONS_H___
#define ___GENLIB_ASSERTIONS_H___

#ifndef GENLIB_USE_ASSERT
#	define GENLIB_USE_ASSERT 0
#endif

#if GENLIB_USE_ASSERT

#include "Exception.h"
#include "StringConverter.h"

#	ifdef genlib_assert
#		undef genlib_assert
#	endif

#	define genlib_assert( X ) assert( X )

#else

#	define genlib_assert( X )

#endif

#endif
