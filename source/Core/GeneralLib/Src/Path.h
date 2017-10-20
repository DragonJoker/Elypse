/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_PATH_H___
#define ___GENERAL_PATH_H___

#include <string>
#include "Macros.h"

#if GENLIB_WINDOWS
#	define d_path_slash	'\\'
#else
#	define d_path_slash '/'
#endif

namespace General
{
	namespace Utils
	{
		class Path
			: public std::string
		{
		public:
			Path();
			Path( char const * const p_data );
			Path( std::string const & p_data );
			Path( const Path & p_data );
			Path & operator=( char const * const p_path );
			Path & operator=( std::string const & p_path );
			Path & operator=( const Path & p_path );
			Path & operator/=( const Path & p_path );

			const Path GetPath()const;
			const std::string GetLeaf()const;

			static std::string Normalise( std::string const & p_path );
		};

		Path operator/( const Path & p_a, const Path & p_b );
	}
}

#endif
