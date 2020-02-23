/*
See LICENSE file in root folder
*/
#include <fstream>

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include "FileManager.h"

#include "StringConverter.h"
#include "StringUtils.h"
#pragma warning( pop )

namespace General::Files
{
	using namespace Utils;

	FileManager::FileManager()
	{
	}

	FileManager::~FileManager()
	{
	}

	FileError FileManager::OpenFile( Path const & p_path
		, size_t( *p_function )( void *, size_t, size_t, void * )
		, void * p_data
		, std::string const & p_postParams )
	{
		FILE * l_file = fopen( p_path.c_str(), "rb" );

		if ( !l_file )
		{
			return FileError::OPEN;
		}

		fseek( l_file, 0u, SEEK_END );
		auto l_size{ size_t( ftell( l_file ) ) };
		fseek( l_file, 0u, SEEK_SET );
		size_t l_remain = l_size;

		try
		{
			static constexpr size_t chunkSize = 16384;
			std::vector< uint8_t > l_buffer( std::min( l_remain, chunkSize ) );
			size_t l_read = 1u;

			while ( l_read && l_remain )
			{
				auto l_chunk = std::min( l_remain, chunkSize );
				l_read = fread( l_buffer.data(), 1u, l_chunk, l_file );

				if ( l_read )
				{
					p_function( l_buffer.data(), 1u, l_read, p_data );
				}

				l_remain -= l_chunk;
			}
		}
		catch ( ... )
		{
		}

		fclose( l_file );

		if ( l_remain )
		{
			return FileError::READ;
		}

		return FileError::SUCCESS;
	}

	FileError FileManager::GetStringFromFile( Path const & p_path, std::string & p_contents, std::string const & p_postParams )
	{
		return OpenFile( p_path, &FileManager::doGetString, &p_contents, p_postParams );
	}

	FileError FileManager::DownloadFile( Path const & p_path, std::string const & p_filename, std::string const & p_postParams )
	{
		std::ofstream l_file;
		l_file.open( p_filename.c_str(), std::ios::binary | std::ios::out );

		if ( l_file.is_open() )
		{
			return OpenFile( p_path, &FileManager::doDownload, &l_file, p_postParams );
		}

		return FileError::OPEN;
	}

	std::string FileManager::GetName( FileError p_errorCode )
	{
		switch ( p_errorCode )
		{
		case General::Files::FileError::SUCCESS:
			return "No error";
		case General::Files::FileError::OPEN:
			return "Can't open file";
		case General::Files::FileError::READ:
			return "File read error happened";
		case General::Files::FileError::COPY:
			return "File copy error happened";
		default:
			return "Unknown error";
		}
	}

	size_t FileManager::doGetString( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data )
	{
		std::string & l_string = *static_cast< std::string * >( p_data );
		l_string.append( static_cast< char * >( p_buffer ), p_size = p_numItems );
		return p_size = p_numItems;
	}

	size_t FileManager::doDownload( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data )
	{
		std::ofstream & l_out = *static_cast< std::ofstream * >( p_data );
		l_out.write( static_cast< char const * const >( p_buffer ), static_cast< std::streamsize >( p_size * p_numItems ) );
		return p_size = p_numItems;
	}

	size_t FileManager::doNull( void *, size_t, size_t, void * )
	{
		return 0;
	}
}
