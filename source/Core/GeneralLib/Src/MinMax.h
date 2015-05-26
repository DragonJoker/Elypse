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

#define min min
#define max max
#define abs abs
#define nabs nabs

namespace General
{
	namespace Utils
	{
		template <typename T>
		inline const T & min( const T & p_a, const T & p_b )
		{
			return ( ( p_a < p_b ) ? p_a : p_b );
		}

		template <typename T>
		inline const T & max( const T & p_a, const T & p_b )
		{
			return ( ( p_a > p_b ) ? p_a : p_b );
		}

		template <typename T>
		inline const T & minmax( const T & p_min, const T & p_value, const T & p_max )
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

		template <typename T>
		inline void clamp( const T & p_min, T & p_value, const T & p_max )
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

		template <typename T>
		inline T abs( const T & p_value )
		{
			return ( ( p_value < T() ) ? -p_value : p_value );
		}

		template <typename T>
		inline T nabs( const T & p_value )
		{
			return ( ( p_value < T() ) ? p_value : -p_value );
		}
	}
}

#endif
