/*
See LICENSE file in root folder
*/
#ifndef ___GENERAL_DYNAMIC_LIBRARY_H___
#define ___GENERAL_DYNAMIC_LIBRARY_H___

#include <string>
#ifndef __GNUG__
#	include <Windows.h>
#else
#	include <dlfcn.h>
#endif

namespace General
{
	namespace Utils
	{
		namespace
		{
#ifndef __GNUG__

			inline void * doLoadLibrary( char const * const p_name )
			{
				return ::LoadLibraryA( p_name );
			}

			inline void doFreeLibrary( void * p_libr )
			{
				::FreeLibrary( static_cast< HMODULE >( p_libr ) );
			}

			inline void * doGetSymbol( void * p_libr, char const * const p_name )
			{
				return ::GetProcAddress( static_cast< HMODULE >( p_libr ), p_name );
			}

#else

			inline void * doLoadLibrary( char const * const p_name )
			{
				return ::dlopen( name, RTLD_LAZY );
			}

			inline void doFreeLibrary( void * p_libr )
			{
				::dlclose( libr );
			}

			inline void * doGetSymbol( void * p_libr, char const * const p_name )
			{
				return ::dlsym( libr, name );
			}

#endif
		}

		class DynamicLibrary
		{
		public:
			inline ~DynamicLibrary()
			{
				Close();
			}

			inline bool Open( char const * const p_name )
			{
				Close();
				m_library = doLoadLibrary( p_name );
				return IsOpen();
			}

			inline bool Open( std::string const & p_name )
			{
				return Open( p_name.c_str() );
			}

			inline void Close()
			{
				if ( m_library )
				{
					doFreeLibrary( m_library );
				}
			}

			inline bool IsOpen()const
			{
				return m_library != nullptr;
			}

			inline void * GetFunction( char const * const p_name )
			{
				if ( m_library )
				{
					return doGetSymbol( m_library, p_name );
				}

				return nullptr;
			}

			inline void * GetFunction( std::string const & p_name )
			{
				return GetFunction( p_name.c_str() );
			}

			template< typename FuncType >
			inline bool GetFunction( std::string const & p_strName, FuncType & p_pfnFunction )
			{
				p_pfnFunction = reinterpret_cast< FuncType >( GetFunction( p_strName.c_str() ) );
				return p_pfnFunction != nullptr;
			}

			template< typename Ret, typename ... Arguments >
			inline bool GetFunction( std::string const & p_strName, std::function< Ret( Arguments... ) > & p_func )
			{
				typedef Ret( * PFNType )( Arguments... );
				PFNType l_pfnResult = nullptr;

				if ( GetFunction( p_strName, l_pfnResult ) )
				{
					p_func = l_pfnResult;
				}

				return l_pfnResult != nullptr;
			}

		private:
			void * m_library{ nullptr };
		};
	}
}

#undef doLoadLibrary
#undef doFreeLibrary
#undef doGetSymbol

#endif
