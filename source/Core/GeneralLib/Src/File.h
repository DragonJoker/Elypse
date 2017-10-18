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
#ifndef ___GENERAL_FILE_H___
#define ___GENERAL_FILE_H___

#include <string>
#include <functional>

#include "Config.h"
#include "Path.h"

namespace General
{
	namespace Files
	{
		/*
			Tests on linux :
			existence test x 100000:
			not exists:

			iostream 750 ms
			fopen 419ms
			boost 524 ms
			stat 230 ms

			exists

			iostream 1.5 sec
			fopen 900 msec
			boost 870 msec
			stat 450 msec
		*/
		bool FileExists( char const * const p_filename );
		bool DirectoryCreate( std::string const & p_filename );
		bool DirectoryExists( std::string const & p_filename );
		bool DirectoryDelete( std::string const & p_dirName );
		bool FileDelete( std::string const & p_filename );
		bool FileCopy( std::string const & p_origin, std::string const & p_dest );

		uint32_t GetFileSize( char const * const p_filename );

		inline uint32_t GetFileSize( std::string const & p_filename )
		{
			return GetFileSize( p_filename.c_str() );
		}
		inline bool FileExists( std::string const & p_filename )
		{
			return FileExists( p_filename.c_str() );
		}

		bool FileExists( wchar_t const * const p_filename );
		bool FileDelete( std::wstring const & p_filename );
		bool DirectoryCreate( std::wstring const & p_filename );
		bool DirectoryExists( std::wstring const & p_filename );
		bool DirectoryDelete( std::wstring const & p_dirName );
		inline bool FileExists( std::wstring const & p_filename )
		{
			return FileExists( p_filename.c_str() );
		}

		void ListDirectoryFiles( Utils::Path const & p_folderPath, std::function< void( Utils::Path const & ) > p_function, bool p_recursive = false );
	}
}

#endif
