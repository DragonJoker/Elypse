#ifndef ___GENERAL_FILE_H___
#define ___GENERAL_FILE_H___

#include <string>

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
		
		bool ListDirectoryFiles( Utils::Path const & p_folderPath, std::vector< Utils::Path > & p_files, bool p_recursive = false );
	}
}

#endif
