/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
