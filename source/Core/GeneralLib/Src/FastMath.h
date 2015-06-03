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
			* i  = 0x5f3759df - ( * i >> 1 );
			float * l_tempY  = reinterpret_cast <float *>( i );
			y = ( * l_tempY );
			y  = y * ( f - ( x * y * y ) );
			y  = y * ( f - ( x * y * y ) );
			return p_number * y;
		}

		inline unsigned int NextPowOfTwo( unsigned int x )
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
			long long * j = reinterpret_cast <long long *>( & p_float );
			( * j ) = ( * j ) & 0x7FFFFFFFFFFFFFFFLL;
			return ( * ( reinterpret_cast <double *>( j ) ) );
		}

		inline unsigned int uabs( int p_value )
		{
			return static_cast <unsigned int>( p_value & 0x7FFFFFFF );
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



		/*
		//Fast rand from http://burtleburtle.net/bob/rand/smallprng.html
		typedef unsigned long int  u4;
		typedef struct ranctx { u4 a; u4 b; u4 c; u4 d; } ranctx;

		#define rot(x,k) (((x)<<(k))|((x)>>(32-(k))))
		u4 ranval( ranctx *x ) {
		u4 e = x->a - rot(x->b, 27);
		x->a = x->b ^ rot(x->c, 17);
		x->b = x->c + x->d;
		x->c = x->d + e;
		x->d = e + x->a;
		return x->d;
		}

		void raninit( ranctx *x, u4 seed ) {
		u4 i;
		x->a = 0xf1ea5eed, x->b = x->c = x->d = seed;
		for (i=0; i<20; ++i) {
		    (void)ranval(x);
		}
		}
		*/

	}
}

#endif
