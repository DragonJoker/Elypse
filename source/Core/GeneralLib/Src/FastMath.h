/*
See LICENSE file in root folder
*/
#ifndef ___FAST_MATH_H___
#define ___FAST_MATH_H___

namespace General
{
	namespace Math
	{
		inline float FastSQRT( float p_number )
		{
			long * i;
			float x, y;
			const float f = 1.5F;
			x = p_number * 0.5F;
			y = p_number;
			i = reinterpret_cast <long *>( & y );
			* i = 0x5f3759df - ( * i >> 1 );
			float * l_tempY = reinterpret_cast <float *>( i );
			y = ( * l_tempY );
			y = y * ( f - ( x * y * y ) );
			y = y * ( f - ( x * y * y ) );
			return p_number * y;
		}

		inline uint32_t NextPowOfTwo( uint32_t x )
		{
			-- x;
			x |= ( x >> 1 );
			x |= ( x >> 2 );
			x |= ( x >> 4 );
			x |= ( x >> 8 );
			x |= ( x >> 16 );
			return ++ x;
		}

		inline float FastAbs( float p_float )
		{
			long * j = reinterpret_cast <long *>( & p_float );
			( * j ) = ( * j ) & 0x7FFFFFFF;
			return ( * ( reinterpret_cast <float *>( j ) ) );
		}

		inline double FastAbs( double p_float )
		{
			int64_t * j = reinterpret_cast <int64_t *>( & p_float );
			( * j ) = ( * j ) & 0x7FFFFFFFFFFFFFFFLL;
			return ( * ( reinterpret_cast <double *>( j ) ) );
		}

		inline uint32_t uabs( int p_value )
		{
			return static_cast <uint32_t>( p_value & 0x7FFFFFFF );
		}

		template< typename T >
		inline T modulo( T p_value, T p_modulo )
		{
			T l_v = p_value / p_modulo;

			if ( p_value < T( 0 ) )
			{
				p_value += p_modulo;
			}

			return p_value - p_modulo * static_cast< int >( l_v );
		}
	}
}

#endif
