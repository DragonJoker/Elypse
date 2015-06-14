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
		bool FileExists( const char * p_filename );
		bool DirectoryCreate( const std::string & p_filename );
		bool DirectoryExists( const std::string & p_filename );
		bool DirectoryDelete( const std::string & p_dirName );
		bool FileDelete( const std::string & p_filename );
		bool FileCopy( const std::string & p_origin, const std::string & p_dest );

		unsigned int GetFileSize( const char * p_filename );

		inline unsigned int GetFileSize( const std::string & p_filename )
		{
			return GetFileSize( p_filename.c_str() );
		}
		inline bool FileExists( const std::string & p_filename )
		{
			return FileExists( p_filename.c_str() );
		}

		bool FileExists( const wchar_t * p_filename );
		bool FileDelete( const std::wstring & p_filename );
		bool DirectoryCreate( const std::wstring & p_filename );
		bool DirectoryExists( const std::wstring & p_filename );
		bool DirectoryDelete( const std::wstring & p_dirName );
		inline bool FileExists( const std::wstring & p_filename )
		{
			return FileExists( p_filename.c_str() );
		}

		void ListDirectoryFiles( Utils::Path const & p_folderPath, std::function< void( Utils::Path const & ) > p_function, bool p_recursive = false );
	}
}

#endif
