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
#ifndef ___GENERAL_DYNAMIC_LIBRARY_H___
#define ___GENERAL_DYNAMIC_LIBRARY_H___

#if defined( _WIN32 )
#	include <Windows.h>
#else
#	include <dlfcn.h>
#endif

#include <string>

namespace General
{
	namespace Utils
	{
		class DynamicLibrary
		{
		public:
			DynamicLibrary()
				: m_library( NULL )
			{
			}

			~DynamicLibrary()
			{
				DoClose();
			}

		private:
			void DoClose()
			{
				if ( m_library )
				{
#ifndef __GNUG__
					FreeLibrary( static_cast< HMODULE >( m_library ) );
#else
					dlclose( m_library );
#endif
				}
			}
		public:

			bool Open( const char * p_name )
			{
				DoClose();
#ifndef __GNUG__
				m_library = LoadLibraryA( p_name );
#else
				m_library = dlopen( p_name, RTLD_LAZY );
#endif
				return IsOpen();
			}

			bool Open( const std::string & p_name )
			{
				return Open( p_name.c_str() );
			}

			inline bool IsOpen()const
			{
				return m_library != NULL;
			}

			void * GetFunction( const char * p_name )
			{
				if ( m_library )
				{
#ifndef __GNUG__
					return GetProcAddress( static_cast< HMODULE >( m_library ), p_name );
#else
					return dlsym( m_library, p_name );
#endif
				}

				return NULL;
			}

			void * GetFunction( const std::string & p_name )
			{
				return GetFunction( p_name.c_str() );
			}

			template< typename FuncType >
			bool GetFunction( std::string const & p_strName, FuncType & p_pfnFunction )
			{
				p_pfnFunction = reinterpret_cast< FuncType >( GetFunction( p_strName.c_str() ) );
				return p_pfnFunction != NULL;
			}

			template< typename Ret, typename ... Arguments >
			bool GetFunction( std::string const & p_strName, std::function< Ret( Arguments... ) > & p_func )
			{
				typedef Ret( * PFNType )( Arguments... );
				PFNType l_pfnResult = NULL;

				if ( GetFunction( p_strName, l_pfnResult ) )
				{
					p_func = l_pfnResult;
				}

				return l_pfnResult != NULL;
			}

		private:
			void * m_library;
		};
	}
}

#endif
