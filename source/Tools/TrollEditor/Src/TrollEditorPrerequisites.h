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
#ifndef ___TROLL_EDITOR_PREREQUISITES_H___
#define ___TROLL_EDITOR_PREREQUISITES_H___

#include <GuiCommonPrerequisites.h>

#include <vector>
#include <map>

#define TROLL_NAMESPACE ELYPSE_NAMESPACE::Troll
#define BEGIN_TROLL_NAMESPACE BEGIN_ELYPSE_NAMESPACE { namespace Troll
#define END_TROLL_NAMESPACE } END_ELYPSE_NAMESPACE

class wxWindow;

BEGIN_ELYPSE_NAMESPACE
{
	namespace Troll
	{
		class Application;

		using GuiCommon::make_string;
		using GuiCommon::make_wxString;

		static const wxString SCENE_FILES = _( "Scene files" );
		static const wxString LOAD_SCRIPT_FILES = _( "Load script files" );
		static const wxString UNLOAD_SCRIPT_FILES = _( "Unload script files" );
		static const wxString GRAPHIC_DATA_FOLDERS = _( "Graphic (2D and 3D) data folders" );
		static const wxString OTHER_DATA_FOLDERS = _( "Non graphic data folders" );
		static const wxString TROLL_EDITOR_ERROR = _( "TrollEditor Error" );
		
		void LogDebug( wxString const & p_message );
		void LogMessage( wxString const & p_message );
		void LogError( wxString const & p_message );

		inline void LogDebug( String const & p_message )
		{
			LogDebug( make_wxString( p_message ) );
		}

		inline void LogMessage( String const & p_message )
		{
			LogMessage( make_wxString( p_message ) );
		}

		inline void LogError( String const & p_message )
		{
			LogError( make_wxString( p_message ) );
		}
	}
}
END_ELYPSE_NAMESPACE

#undef TROLL_HAS_MAKE_UNIQUE

#if defined( _MSC_VER)
#	if _MSC_VER >= 1800
#		define TROLL_HAS_MAKE_UNIQUE				1
#	elif _MSC_VER >= 1700
#		define TROLL_HAS_MAKE_UNIQUE				0
#	elif _MSC_VER >= 1600
#		define TROLL_HAS_MAKE_UNIQUE				0
#	elif _MSC_VER > 1500
#		define TROLL_HAS_MAKE_UNIQUE				0
#	else
#		define TROLL_HAS_MAKE_UNIQUE				0
#	endif
#elif defined( __clang__)
#	define TROLL_HAS_MAKE_UNIQUE					__has_feature(cxx_variadic_templates)
#elif defined( __GNUG__)
#	define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if GCC_VERSION >= 40900
#		if defined( _WIN32 )
#			define TROLL_HAS_MAKE_UNIQUE			1
#		elif GCC_VERSION >= 40500
#			define TROLL_HAS_MAKE_UNIQUE			0
#		else
#			define TROLL_HAS_MAKE_UNIQUE			0
#		endif
#	elif GCC_VERSION >= 40600
#		if defined( _WIN32 )
#			define TROLL_HAS_MAKE_UNIQUE			0
#		else
#			define TROLL_HAS_MAKE_UNIQUE			0
#		endif
#	elif GCC_VERSION >= 40400
#		define TROLL_HAS_MAKE_UNIQUE				0
#	elif GCC_VERSION >= 40300
#		define TROLL_HAS_MAKE_UNIQUE				0
#	else
#		define TROLL_HAS_MAKE_UNIQUE				0
#	endif
#elif defined( __BORLANDC__ )
#	warning "Theoretically supported compiler, but untested yet"
#	if __BORLANDC__ >= 0x621
#		define TROLL_HAS_MAKE_UNIQUE				0
#	endif
#else
#	error "Yet unsupported compiler"
#endif

namespace std
{
#if !TROLL_HAS_MAKE_UNIQUE
	template< class T >
	unique_ptr< T > make_unique()
	{
		return unique_ptr< T >( new T );
	}
	template< class T, typename Arg1 >
	unique_ptr< T > make_unique( Arg1 && arg1 )
	{
		return unique_ptr< T >( new T( arg1 ) );
	}
	template< class T, typename Arg1, typename Arg2 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2 )
	{
		return unique_ptr< T >( new T( arg1, arg2 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6, arg7 ) );
	}
	template< class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8 >
	unique_ptr< T > make_unique( Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5, Arg6 && arg6, Arg7 && arg7, Arg8 && arg8 )
	{
		return unique_ptr< T >( new T( arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8 ) );
	}
#endif
}

#endif
