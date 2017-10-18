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
#include "Path.h"
#include "StringUtils.h"

namespace General
{
	namespace Utils
	{
		Path::Path()
		{
		}

		Path::Path( char const * const p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path::Path( std::string const & p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path::Path( const Path & p_data )
			: std::string{ Normalise( p_data ) }
		{
		}

		Path & Path::operator=( char const * const p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator=( std::string const & p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator=( const Path & p_path )
		{
			std::string::operator=( Normalise( p_path ) );
			return *this;
		}

		Path & Path::operator/=( const Path & p_path )
		{
			push_back( d_path_slash );
			append( p_path );
			return *this;
		}

		const Path Path::GetPath()const
		{
			size_t l_index = find_last_of( d_path_slash );

			if ( l_index != std::string::npos )
			{
				return substr( 0, l_index );
			}

			return Path();
		}

		const std::string Path::GetLeaf()const
		{
			size_t l_index = find_last_of( d_path_slash );

			if ( l_index != std::string::npos )
			{
				return substr( l_index + 1, std::string::npos );
			}

			return std::string( *this );
		}

		std::string Path::Normalise( std::string const & p_path )
		{
			std::string l_return = string::replace( p_path, '/', d_path_slash );
			l_return = string::replace( l_return, '\\', d_path_slash );
			return l_return;
		}

		Path operator/( const Path & p_a, const Path & p_b )
		{
			return Path( p_a ) /= p_b;
		}
	}
}
