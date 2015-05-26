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

#	define genlib_assert( X) if ( ! (X)){ GENLIB_EXCEPTION( std::string( "Assert not respected : (") + #X + std::string( ") file ") + std::string( __FILE__) + std::string( " @ L# ") + General::Utils::ToString<unsigned int>( __LINE__));}

#else

#	define genlib_assert( X)

#endif

#endif
