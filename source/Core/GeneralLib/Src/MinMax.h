/*
See LICENSE file in root folder
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
