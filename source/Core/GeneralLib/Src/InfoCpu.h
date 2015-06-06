#ifndef ___INFO_CPU_H__
#define ___INFO_CPU_H__

#include <string.h>

#include "Config.h"

#if GENLIB_LINUX || GENLIB_MACOS
#	include <sys/sysinfo.h>
#endif

namespace General
{
	namespace Computer
	{
		struct cpuid_reg
		{
			unsigned int eax;
			unsigned int ebx;
			unsigned int ecx;
			unsigned int edx;
		};

#if defined( __ICL) || defined( _MSC_VER)

		inline cpuid_reg _cpuid( unsigned int a, unsigned int c = 0 )
		{
			cpuid_reg r;

			__try
			{
				_asm
				{
					mov eax, a
					mov ecx, c
					cpuid
					mov r.eax, eax
					mov r.ebx, ebx
					mov r.ecx, ecx
					mov r.edx, edx
				}
			}
			__except ( 1 )
			{
				r.eax = r.ebx = r.ecx = r.edx = 0;
			}

			return r;
		}

#elif defined( __GNUC__) && (defined( i386) || defined( __x86_64__))

		inline cpuid_reg _cpuid( unsigned int a, unsigned int c = 0 )
		{
			cpuid_reg r;
			__asm__( "cpuid" : "=a"( r.eax ), "=b"( r.ebx ), "=c"( r.ecx ), "=d"( r.edx ) : "a"( a ), "c"( c ) );
			return r;
		}

#else

		inline cpuid_reg _cpuid( unsigned int a, unsigned int c = 0 )
		{
			cpuid_reg r = { 0, 0, 0, 0 };
			return r;
		}

#endif

		inline cpuid_reg cpuid( unsigned int a, unsigned int c = 0 )
		{
			if ( a <= _cpuid( 0 ).eax )
			{
				return _cpuid( a, c );
			}

			cpuid_reg r = { 0, 0, 0, 0 };
			return r;
		}

		inline cpuid_reg cpuid_ext( unsigned int a, unsigned int c = 0 )
		{
			a |= 0x80000000;

			if ( a <= _cpuid( 0x80000000 ).eax )
			{
				return _cpuid( a, c );
			}

			cpuid_reg r = { 0, 0, 0, 0 };
			return r;
		}

		static char _cpu_vendor[13];
		static char _cpu_name[48];

		inline char * cpu_vendor()
		{
			unsigned int * p = reinterpret_cast <unsigned int *>( _cpu_vendor );
			cpuid_reg reg = cpuid( 0 );
			p[0] = reg.ebx;
			p[1] = reg.edx;
			p[2] = reg.ecx;
			_cpu_vendor[12] = '\0';
			return _cpu_vendor;
		}

		inline char * cpu_name()
		{
			unsigned int * p = reinterpret_cast <unsigned int *>( _cpu_name );
			cpuid_reg reg = cpuid_ext( 2 );
			p[0] = reg.eax;
			p[1] = reg.ebx;
			p[2] = reg.ecx;
			p[3] = reg.edx;
			reg = cpuid_ext( 3 );
			p[4] = reg.eax;
			p[5] = reg.ebx;
			p[6] = reg.ecx;
			p[7] = reg.edx;
			reg = cpuid_ext( 4 );
			p[8] = reg.eax;
			p[9] = reg.ebx;
			p[10] = reg.ecx;
			p[11] = reg.edx;
			return _cpu_name;
		}

		inline bool is_intel_cpu()
		{
			return ( strncmp( cpu_vendor(), "GenuineIntel", sizeof( "GenuineIntel" ) ) == 0 );
		}
		inline bool is_amd_cpu()
		{
			return ( strncmp( cpu_vendor(), "AuthenticAMD", sizeof( "AuthenticAMD" ) ) == 0 );
		}
		inline bool is_mmx_cpu()
		{
			return ( ( cpuid( 1 ).edx & 0x00800000 ) != 0 );
		}
		inline bool is_sse_cpu()
		{
			return ( ( cpuid( 1 ).edx & 0x02000000 ) != 0 );
		}
		inline bool is_sse2_cpu()
		{
			return ( ( cpuid( 1 ).edx & 0x04000000 ) != 0 );
		}
		inline bool is_sse3_cpu()
		{
			return ( ( cpuid( 1 ).ecx & 0x00000001 ) != 0 );
		}
		inline bool is_3dnow_cpu()
		{
			return ( ( cpuid_ext( 1 ).edx & 0x80000000 ) != 0 );
		}

		inline unsigned int num_l1_threads()
		{
			return ( ( cpuid( 4, 1 ).eax >> 14 ) & 0xFFF ) + 1;
		}
		inline unsigned int num_l2_threads()
		{
			return ( ( cpuid( 4, 2 ).eax >> 14 ) & 0xFFF ) + 1;
		}

		inline unsigned int l1_cache_size()
		{
			cpuid_reg reg = cpuid( 4, 1 );
			return	( ( ( reg.ebx >> 22 ) & 0x3FF ) + 1 )
					* ( ( ( reg.ebx >> 12 ) & 0x3FF ) + 1 )
					* ( ( reg.ebx & 0xFFF ) + 1 )
					* ( reg.ecx + 1 );
		}

		inline unsigned int l2_cache_size()
		{
			cpuid_reg reg = cpuid( 4, 2 );
			return	( ( ( reg.ebx >> 22 ) & 0x3FF ) + 1 )
					* ( ( ( reg.ebx >> 12 ) & 0x3FF ) + 1 )
					* ( ( reg.ebx & 0xFFF ) + 1 )
					* ( reg.ecx + 1 );
		}

		inline unsigned int num_logical_cores()
		{
			cpuid_reg reg = cpuid( 1 );
			return ( ( reg.edx & 0x10000000 ) ? ( reg.ebx >> 16 ) & 0xFF : 1 );
		}

		inline unsigned int num_physical_cores()
		{
			return ( ( ! is_amd_cpu() ) ? ( ( cpuid( 4, 0 ).eax >> 26 ) & 0x03F ) + 1 : num_logical_cores() );
		}
	}
}

#endif
