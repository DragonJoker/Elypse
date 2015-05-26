#include "ZipArchive.h"
#include "ZipDirectory.h"
#include "ZipFile.h"
#include "STLMacros.h"
#include "StringConverter.h"

#include <iostream>

using namespace General::Files;
using namespace General::Utils;

bool ZipArchive :: _load()
{
	ZZIP_DIRENT l_entry;
	zzip_error_t l_error;
	ZipDirectoryBase <std::string> * l_currentDirectory = m_rootDirectory;
	std::cout << "ZipArchive :: load : " << m_fullPath << std::endl;
	m_internalArchive = zzip_dir_open( m_fullPath.c_str(), & l_error );

	if ( l_error != ZZIP_NO_ERROR )
	{
		std::cout << "ERROR : " << GetErrorDesc( l_error ) << std::endl;
		return false;
	}

	while ( zzip_dir_read( m_internalArchive, & l_entry ) )
	{
		std::string l_fullPath = l_entry.d_name;
		size_t l_index = l_fullPath.find_last_of( '/' );

		if ( l_index == std::string::npos )
		{
			l_index = l_fullPath.find_last_of( '\\' );
		}

		if ( l_index == std::string::npos )
		{
			l_index = -1;
		}

		std::string l_path = l_fullPath.substr( 0, l_index + 1 );
		std::string l_name = l_fullPath.substr( l_index + 1, l_fullPath.length() - l_index );

		if ( l_name.empty() )
		{
			l_path.erase( l_index );
			size_t l_index = l_path.find_last_not_of( '/' );
			l_path = l_path.substr( 0, l_index + 1 );
			l_index = l_path.find_last_of( '/' );
			l_name = l_path.substr( l_index + 1, l_path.length() - l_index );
			l_path = l_path.substr( 0, l_index + 1 );
			l_currentDirectory = GetDirectory( l_name );

			if ( l_currentDirectory )
			{
				l_currentDirectory = l_currentDirectory->AddDirectory( l_name );
			}
			else
			{
				l_currentDirectory = m_rootDirectory->AddDirectory( l_name );
			}

			l_fullPath.erase( l_fullPath.size() - 1 );
			m_directories.insert( ZipDirectoryMap::value_type( l_fullPath, l_currentDirectory ) );
		}
		else
		{
			if ( ! l_path.empty() )
			{
				l_path.erase( l_path.size() - 1 );

				if ( l_currentDirectory->GetName() != l_path
						&& ! l_path.empty() )
				{
					l_currentDirectory = GetDirectory( l_path );
				}
			}
			else
			{
				l_currentDirectory = m_rootDirectory;
			}

			ZipFile * l_file = l_currentDirectory->AddFile( l_name, l_fullPath );
			l_file->SetSize( l_entry.d_csize, l_entry.st_size );
			m_files.insert( ZipFileMap::value_type( l_fullPath, l_file ) );
		}
	}

	m_rootDirectory->CheckDirectorySize();
	return true;
}

#if GENLIB_WINDOWS

bool ZipWArchive :: _load()
{
	ZZIP_DIRENT l_entry;
	zzip_error_t l_error;
	ZipDirectoryBase <std::wstring> * l_currentDirectory = m_rootDirectory;
	m_internalArchive = zzip_dir_open( ToString( m_fullPath ).c_str(), & l_error );

	if ( l_error != ZZIP_NO_ERROR )
	{
		std::cout << "ERROR : " << GetErrorDesc( l_error ) << std::endl;
		return false;
	}

	while ( zzip_dir_read( m_internalArchive, & l_entry ) )
	{
		std::wstring l_fullPath = ToWString( l_entry.d_name );
		size_t l_index = l_fullPath.find_last_of( '/' );

		if ( l_index == std::wstring::npos )
		{
			l_index = l_fullPath.find_last_of( '\\' );
		}

		if ( l_index == std::wstring::npos )
		{
			l_index = -1;
		}

		std::wstring l_path = l_fullPath.substr( 0, l_index + 1 );
		std::wstring l_name = l_fullPath.substr( l_index + 1, l_fullPath.length() - l_index );

		if ( l_name.empty() )
		{
			l_path.erase( l_index );
			size_t l_index = l_path.find_last_not_of( '/' );
			l_path = l_path.substr( 0, l_index + 1 );
			l_index = l_path.find_last_of( '/' );
			l_name = l_path.substr( l_index + 1, l_path.length() - l_index );
			l_path = l_path.substr( 0, l_index + 1 );
			l_currentDirectory = GetDirectory( l_name );

			if ( l_currentDirectory )
			{
				l_currentDirectory = l_currentDirectory->AddDirectory( l_name );
			}
			else
			{
				l_currentDirectory = m_rootDirectory->AddDirectory( l_name );
			}

			l_fullPath.erase( l_fullPath.size() - 1 );
			m_directories.insert( ZipDirectoryMapType::value_type( ToWString( l_fullPath ), l_currentDirectory ) );
		}
		else
		{
			if ( ! l_path.empty() )
			{
				l_path.erase( l_path.size() - 1 );

				if ( l_currentDirectory->GetName() != l_path
						&& ! l_path.empty() )
				{
					l_currentDirectory = GetDirectory( l_path );
				}
			}
			else
			{
				l_currentDirectory = m_rootDirectory;
			}

			ZipFileBase <std::wstring> * l_file = l_currentDirectory->AddFile( l_name, l_fullPath );
			l_file->SetSize( l_entry.d_csize, l_entry.st_size );
			m_files.insert( ZipFileMapType::value_type( l_fullPath, l_file ) );
		}
	}

	m_rootDirectory->CheckDirectorySize();
	return true;
}

#endif
