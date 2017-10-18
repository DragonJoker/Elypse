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
#ifndef ___GENERAL_LIB_ZIP_H___
#define ___GENERAL_LIB_ZIP_H___

#include <string>
#include <vector>
#include <map>

#include "Config.h"
#include "Exception.h"

#include <zip.h>
#include <memory>

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
