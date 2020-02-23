/*
See LICENSE file in root folder
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
