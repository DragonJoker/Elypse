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
#ifndef ___UTILS_MIN_MAX_H___
#define ___UTILS_MIN_MAX_H___

//In case someone tries to create macros for these -_- template versions are better...
#ifdef min
#	undef min
#endif

#ifdef max
#	undef max
#endif

#ifdef abs
#	undef abs
#endif

#ifdef nabs
#	undef nabs
#endif

namespace General
{
	namespace Utils
	{
		template< typename T >
		inline T const & min( T const & p_a, T const & p_b )
		{
			return ( ( p_a < p_b ) ? p_a : p_b );
		}

		template< typename T >
		inline T const & max( T const & p_a, T const & p_b )
		{
			return ( ( p_a > p_b ) ? p_a : p_b );
		}

		template< typename T >
		inline T const & minmax( T const & p_min, T const & p_value, T const & p_max )
		{
			if ( p_value < p_min )
			{
				return p_min;
			}

			if ( p_value > p_max )
			{
				return p_max;
			}

			return p_value;
		}

		template< typename T >
		inline void clamp( T const & p_min, T & p_value, T const & p_max )
		{
			if ( p_value < p_min )
			{
				p_value = p_min;
				return;
			}

			if ( p_value > p_max )
			{
				p_value = p_max;
			}
		}

		template< typename T >
		inline T abs( T const & p_value )
		{
			return ( ( p_value < T{} ) ? -p_value : p_value );
		}

		template< typename T >
		inline T nabs( T const & p_value )
		{
			return ( ( p_value < T{} ) ? p_value : -p_value );
		}
	}
}

#endif
