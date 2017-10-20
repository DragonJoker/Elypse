/*
See LICENSE file in root folder
*/
#include <sys/stat.h>
//#include <sys/types.h>
#include <boost/filesystem.hpp>

#include "File.h"
#include "Path.h"

#if defined( _WIN32 )
#	include <direct.h>
#	include <Shlobj.h>
#	if defined( _MSC_VER )
#		pragma warning( push )
#		pragma warning( disable:4311 )
#		pragma warning( disable:4312 )
#	endif
#	include <windows.h>
#	if defined( _MSC_VER )
#		pragma warning( pop )
#	endif
#	define GetCurrentDir _getcwd
#	undef CopyFile
#	undef DeleteFile
#else
#	include <unistd.h>
#	include <sys/types.h>
#	include <dirent.h>
#	include <errno.h>
#	include <pwd.h>
#	define GetCurrentDir getcwd
#endif

#include <iostream>

namespace General
{
	namespace Files
	{
		bool FileExists( char const * const p_filename )
		{
			struct stat l_stat;
			return ( stat( p_filename, & l_stat ) == 0 );
		}

		bool DirectoryCreate( std::string const & p_filename )
		{
			if ( p_filename.empty() )
			{
				return false;
			}

			std::string l_filename = p_filename;

			if ( l_filename[l_filename.size() - 1] == d_path_slash )
			{
				l_filename.erase( l_filename.size() - 1, 1 );
			}

			l_filename = l_filename.substr( 0, l_filename.find_last_of( d_path_slash ) );

			if ( !DirectoryExists( l_filename ) )
			{
				if ( !DirectoryCreate( l_filename ) )
				{
					return false;
				}
			}

			try
			{
				return boost::filesystem::create_directory( p_filename );
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool DirectoryExists( std::string const & p_filename )
		{
			try
			{
				return boost::filesystem::exists( p_filename ) && boost::filesystem::is_directory( p_filename );
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool FileDelete( std::string const & p_filename )
		{
			try
			{
				boost::filesystem::remove( p_filename );
				return true;
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool FileCopy( std::string const & p_origin, std::string const & p_dest )
		{
			try
			{
				boost::filesystem::copy_file( p_origin, p_dest );
				return true;
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool DirectoryDelete( std::string const & p_dirName )
		{
			if ( !boost::filesystem::exists( p_dirName ) )
			{
				return false;
			}

			try
			{
				boost::filesystem::directory_iterator iend;
				boost::filesystem::directory_iterator i( p_dirName );

				for ( ;	i != iend ; ++ i )
				{
					if ( is_directory( i->status() ) )
					{
						if ( !DirectoryDelete( i->path().string() ) )
						{
							return false;
						}
					}

					remove( i->path() );
				}

				boost::filesystem::remove( p_dirName );
				return true;
			}
			catch ( ... )
			{
				return false;
			}
		}

		uint32_t GetFileSize( char const * const p_filename )
		{
			struct stat tagStat;
			stat( p_filename, & tagStat );
			return static_cast< uint32_t >( tagStat.st_size );
		}

#if GENLIB_WINDOWS

		bool FileExists( wchar_t const * const p_filename )
		{
			struct _stat l_stat;
			return ( _wstat( p_filename, & l_stat ) == 0 );
		}

		bool DirectoryCreate( std::wstring const & p_filename )
		{
			if ( p_filename.empty() )
			{
				return false;
			}

			std::wstring l_filename = p_filename;

			if ( l_filename[l_filename.size() - 1] == d_path_slash )
			{
				l_filename.erase( l_filename.size() - 1, 1 );
			}

			l_filename = l_filename.substr( 0, l_filename.find_last_of( d_path_slash ) );

			if ( !DirectoryExists( l_filename ) )
			{
				if ( !DirectoryCreate( l_filename ) )
				{
					return false;
				}
			}

			try
			{
				return boost::filesystem::create_directory( p_filename );
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool DirectoryExists( std::wstring const & p_filename )
		{
			try
			{
				return boost::filesystem::exists( p_filename ) && boost::filesystem::is_directory( p_filename );
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool FileDelete( std::wstring const & p_filename )
		{
			try
			{
				boost::filesystem::remove( p_filename );
				return true;
			}
			catch ( std::exception & )
			{
				return false;
			}
		}

		bool DirectoryDelete( std::wstring const & p_dirName )
		{
			if ( !boost::filesystem::exists( p_dirName ) )
			{
				return false;
			}

			try
			{
				boost::filesystem::directory_iterator iend;
				boost::filesystem::directory_iterator i( p_dirName );

				for ( ; i != iend ; ++ i )
				{
					if ( boost::filesystem::is_directory( i->status() ) )
					{
						if ( !DirectoryDelete( i->path().string() ) )
						{
							return false;
						}
					}

					remove( i->path() );
				}

				boost::filesystem::remove( p_dirName );
				return true;
			}
			catch ( ... )
			{
				return false;
			}
		}

#endif

		void ListDirectoryFiles( Utils::Path const & p_folderPath, std::function< void( Utils::Path const & ) > p_function, bool p_recursive )
		{
#if defined( _WIN32 )

			WIN32_FIND_DATAA l_findData;
			HANDLE l_hHandle = ::FindFirstFileA( ( p_folderPath + d_path_slash + std::string( "*.*" ) ).c_str(), &l_findData );
			std::string l_strBuffer;

			if ( l_hHandle != INVALID_HANDLE_VALUE )
			{
				l_strBuffer = l_findData.cFileName;

				if ( l_strBuffer != "." && l_strBuffer != ".." )
				{
					p_function( l_strBuffer );
				}

				while ( ::FindNextFileA( l_hHandle, &l_findData ) )
				{
					if ( l_findData.cFileName != l_strBuffer )
					{
						l_strBuffer = l_findData.cFileName;

						if ( ( l_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY )
						{
							if ( p_recursive )
							{
								ListDirectoryFiles( General::Utils::Path( p_folderPath + d_path_slash + l_strBuffer ), p_function, p_recursive );
							}
						}
						else
						{
							if ( l_strBuffer != "." && l_strBuffer != ".." )
							{
								p_function( p_folderPath + d_path_slash + l_strBuffer );
							}
						}
					}
				}
			}

#elif defined( __linux__ )

			DIR * l_pDir;

			if ( !( l_pDir = opendir( p_folderPath.c_str() ) ) )
			{
				switch ( errno )
				{
				case EACCES:
					std::cerr << "Can't open dir : Permission denied - Directory : " << p_folderPath;
					break;

				case EBADF:
					std::cerr << "Can't open dir : Invalid file descriptor - Directory : " << p_folderPath;
					break;

				case EMFILE:
					std::cerr << "Can't open dir : Too many file descriptor in use - Directory : " << p_folderPath;
					break;

				case ENFILE:
					std::cerr << "Can't open dir : Too many files currently open - Directory : " << p_folderPath;
					break;

				case ENOENT:
					std::cerr << "Can't open dir : Directory doesn't exist - Directory : " << p_folderPath;
					break;

				case ENOMEM:
					std::cerr << "Can't open dir : Insufficient memory - Directory : " << p_folderPath;
					break;

				case ENOTDIR:
					std::cerr << "Can't open dir : <name> is not a directory - Directory : " << p_folderPath;
					break;

				default:
					std::cerr << "Can't open dir : Unknown error - Directory : " << p_folderPath;
					break;
				}
			}
			else
			{
				unsigned char const l_isFile = 0x8;
				dirent * l_pDirent;

				while ( ( l_pDirent = readdir( l_pDir ) ) != NULL )
				{
					if ( strcmp( l_pDirent->d_name, "." ) && strcmp( l_pDirent->d_name, ".." ) )
					{
						if ( l_pDirent->d_type == l_isFile )
						{
							if ( p_recursive )
							{
								ListDirectoryFiles( General::Utils::Path( p_folderPath + d_path_slash + std::string( l_pDirent->d_name ) ), p_function, p_recursive );
							}
						}
						else
						{
							p_function( p_folderPath + d_path_slash + std::string( l_pDirent->d_name ) );
						}
					}
				}

				closedir( l_pDir );
			}

#else

#	error "Unsupported platform"

#endif
		}
	}
}


