/*
See LICENSE file in root folder
*/
#ifndef ___FILE_VERSION_H___
#define ___FILE_VERSION_H___

#include <string>

#include "Macros.h"

namespace General
{
	namespace Computer
	{
#if GENLIB_WINDOWS
#	pragma comment( lib, "version.lib")
#endif
		d_dll bool GetFileVersion( std::string const & p_fileName, std::string & p_version );
		d_dll bool GetFileVersion( std::wstring const & p_fileName, std::wstring & p_version );
	}
}

#endif
