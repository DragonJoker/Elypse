#ifndef ___GENERAL_LIB_ZIP_H___
#define ___GENERAL_LIB_ZIP_H___

#include <string>
#include <vector>
#include <map>

#include "Config.h"
#include "Exception.h"

#include <zip.h>

namespace General
{
	namespace Files
	{
		template< typename CharType >
		class ZipFileBase;

		template< typename CharType >
		class ZipDirectoryBase;

		template< typename CharType >
		class ZipArchiveBase;

		typedef ZipFileBase< char > ZipFile;
		typedef ZipDirectoryBase< char > ZipDirectory;
		typedef ZipArchiveBase< char > ZipArchive;

#if GENLIB_WINDOWS

		typedef ZipFileBase< wchar_t > ZipWFile;
		typedef ZipDirectoryBase< wchar_t > ZipWDirectory;
		typedef ZipArchiveBase< wchar_t > ZipWArchive;

#endif

		typedef std::vector< ZipFile * >ZipFileArray;
		typedef std::vector< ZipDirectory * >ZipDirectoryArray;

		typedef std::map< std::string, ZipFile * > ZipFileMap;
		typedef std::map< std::string, ZipDirectory * > ZipDirectoryMap;

		typedef std::vector< std::string > StringArray;

		namespace Zip
		{
			static std::string GetErrorDesc( int p_errorCode )
			{
				char l_buffer[1024] = { 0 };
				zip_error_to_str( l_buffer, 1024, errno, p_errorCode );
				return "(code " + std::to_string( p_errorCode ) + ") " + std::string( l_buffer );
			}
		}
	}
}

#endif
