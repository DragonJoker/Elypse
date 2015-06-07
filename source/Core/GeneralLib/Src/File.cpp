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

using namespace boost::filesystem;

bool General::Files::FileExists( const char * p_filename )
{
	struct stat l_stat;
	return ( stat( p_filename, & l_stat ) == 0 );
}

bool General::Files::DirectoryCreate( const std::string & p_filename )
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
		return create_directory( p_filename );
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::DirectoryExists( const std::string & p_filename )
{
	try
	{
		return exists( p_filename ) && is_directory( p_filename );
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::FileDelete( const std::string & p_filename )
{
	try
	{
		remove( p_filename );
		return true;
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::FileCopy( const std::string & p_origin, const std::string & p_dest )
{
	try
	{
		copy_file( p_origin, p_dest );
		return true;
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::DirectoryDelete( const std::string & p_dirName )
{
	if ( ! exists( p_dirName ) )
	{
		return false;
	}

	try
	{
		directory_iterator iend;
		directory_iterator i( p_dirName );

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
			/*
			{
				//ERROR : could not delete a file ?
				return false;
			}
			*/
		}

		remove( p_dirName );
		return true;
	}
	catch ( ... )
	{
		//ERROR : could not delete a directory ?
		return false;
	}
}

unsigned int General::Files::GetFileSize( const char * p_filename )
{
	struct stat tagStat;
	stat( p_filename, & tagStat );
	return static_cast <unsigned int>( tagStat.st_size );
}

#if GENLIB_WINDOWS

bool General::Files::FileExists( const wchar_t * p_filename )
{
	struct _stat l_stat;
	return ( _wstat( p_filename, & l_stat ) == 0 );
}

bool General::Files::DirectoryCreate( const std::wstring & p_filename )
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
		return create_directory( p_filename );
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::DirectoryExists( const std::wstring & p_filename )
{
	try
	{
		return exists( p_filename ) && is_directory( p_filename );
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::FileDelete( const std::wstring & p_filename )
{
	try
	{
		remove( p_filename );
		return true;
	}
	catch ( std::exception & )
	{
		return false;
	}
}

bool General::Files::DirectoryDelete( const std::wstring & p_dirName )
{
	if ( ! exists( p_dirName ) )
	{
		return false;
	}

	try
	{
		//wdirectory_iterator iend;
		//wdirectory_iterator i( p_dirName);

		//for ( ;	i != iend ; ++ i)
		//{
		//	if (is_directory( i->status()))
		//	{
		//		if ( ! DirectoryDelete( i->path().string()))
		//		{
		//			return false;
		//		}
		//	}
		//	remove( i->path());
		//}

		//remove( p_dirName);
		return true;
	}
	catch ( ... )
	{
		//ERROR : could not delete a directory ?
		return false;
	}
}

void General::Files::ListDirectoryFiles( General::Utils::Path const & p_folderPath, std::function< void( General::Utils::Path const & ) > p_function, bool p_recursive )
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

#endif


