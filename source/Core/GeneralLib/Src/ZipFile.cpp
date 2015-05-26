#include <fstream>

#include "ZipFile.h"
#include "ZipDirectory.h"
#include "ZipArchive.h"
#include "StringConverter.h"
#include "Macros.h"

namespace
{
	const unsigned int c_bufferSize = 2048;
}

using namespace General::Files;
using namespace General::Utils;

bool ZipFile :: Extract( const std::string & p_newPath, ZipArchiveBase <std::string> * p_archive )
{
//	std::cout << "ZipFile :: EXTRACT : " << p_newPath << "\n";
	ZZIP_FILE * l_zip = zzip_file_open( p_archive->GetArchive(), m_fullname.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS );

	if ( l_zip == NULL )
	{
		return false;
	}

	char l_buffer[c_bufferSize];
	unsigned int l_size = 0;
	std::ofstream l_outFile;
	l_outFile.open( ( p_newPath + m_fullname ).c_str(), std::ios::out | std::ios::binary );

	if ( ! l_outFile.is_open() )
	{
		zzip_file_close( l_zip );
		return false;
	}

	do
	{
		l_size = zzip_file_read( l_zip, & l_buffer[0], c_bufferSize );
		l_outFile.write( & l_buffer[0], l_size );
	}
	while ( l_size > 0 );

	zzip_file_close( l_zip );
	l_outFile.close();
	return true;
}

#if GENLIB_WINDOWS

bool ZipWFile :: Extract( const std::wstring & p_newPath, ZipArchiveBase <std::wstring> * p_archive )
{
//	std::wcout << "ZipWFile :: EXTRACT : " << p_newPath << "\n";
	ZZIP_FILE * l_zip = zzip_file_open( p_archive->GetArchive(), ToString( m_fullname ).c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS );

	if ( l_zip == NULL )
	{
//		std::cout << "ZipWFile : l_zip est null" << std::endl;
		return false;
	}

	char l_buffer[c_bufferSize];
	unsigned int l_size = 0;
	std::ofstream l_outFile;
	l_outFile.open( ToString( p_newPath + m_fullname ).c_str(), std::ios::out | std::ios::binary );

	if ( ! l_outFile.is_open() )
	{
		zzip_file_close( l_zip );
//		std::cout << "ZipWFile : not opened" << std::endl;
		return false;
	}

	do
	{
		l_size = zzip_file_read( l_zip, & l_buffer[0], c_bufferSize );
//		std::cout << "l_size : " << l_size << "\n";
		l_outFile.write( & l_buffer[0], l_size );
	}
	while ( l_size > 0 );

	zzip_file_close( l_zip );
	l_outFile.close();
//	std::cout << "ZipWFile : ok " << std::endl;
	return true;
}

#endif
