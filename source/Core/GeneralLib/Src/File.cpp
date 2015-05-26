#include <sys/stat.h>
//#include <sys/types.h>
#include <boost/filesystem.hpp>

#include "File.h"
#include "Path.h"

using namespace boost::filesystem;

bool General :: Files :: FileExists( const char * p_filename )
{
	struct stat l_stat;
	return ( stat( p_filename, & l_stat ) == 0 );
}

bool General :: Files :: DirectoryCreate( const std::string & p_filename )
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

	if ( ! DirectoryExists( l_filename ) )
	{
		if ( ! DirectoryCreate( l_filename ) )
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

bool General :: Files :: DirectoryExists( const std::string & p_filename )
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

bool General :: Files :: FileDelete( const std::string & p_filename )
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

bool General :: Files :: FileCopy( const std::string & p_origin, const std::string & p_dest )
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

bool General :: Files :: DirectoryDelete( const std::string & p_dirName )
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
				if ( ! DirectoryDelete( i->path().string() ) )
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

unsigned int General :: Files :: GetFileSize( const char * p_filename )
{
	struct stat tagStat;
	stat( p_filename, & tagStat );
	return static_cast <unsigned int>( tagStat.st_size );
}

#if GENLIB_WINDOWS

bool General :: Files :: FileExists( const wchar_t * p_filename )
{
	struct _stat l_stat;
	return ( _wstat( p_filename, & l_stat ) == 0 );
}

bool General :: Files :: DirectoryCreate( const std::wstring & p_filename )
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

	if ( ! DirectoryExists( l_filename ) )
	{
		if ( ! DirectoryCreate( l_filename ) )
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

bool General :: Files :: DirectoryExists( const std::wstring & p_filename )
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

bool General :: Files :: FileDelete( const std::wstring & p_filename )
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

bool General :: Files :: DirectoryDelete( const std::wstring & p_dirName )
{
	if ( ! exists( p_dirName ) )
	{
		return false;
	}

	try
	{
		/*
				wdirectory_iterator iend;
				wdirectory_iterator i( p_dirName);

				for ( ;	i != iend ; ++ i)
				{
					if (is_directory( i->status()))
					{
						if ( ! DirectoryDelete( i->path().string()))
						{
							return false;
						}
					}
					remove( i->path());
				}

				remove( p_dirName);
		*/
		return true;
	}
	catch ( ... )
	{
		//ERROR : could not delete a directory ?
		return false;
	}
}

#endif


