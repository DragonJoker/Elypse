#ifndef ___GENERAL_LIB_ZIP_H___
#define ___GENERAL_LIB_ZIP_H___

#include <string>
#include <vector>
#include <map>

#include "Config.h"
/*
#if GENLIB_WINDOWS
#	if defined(_MT) || defined(__MT__)
#		pragma comment( lib, "zziplib_mt.lib")
#	else
#		pragma comment( lib, "zziplib_md.lib")
#	endif
#	pragma comment( lib, "zlib.lib")
#endif
*/
#include <zzip/zzip.h>

namespace General
{
	namespace Files
	{
		template <typename T>
		class ZipFileBase;

		template <typename T>
		class ZipDirectoryBase;

		template <typename T>
		class ZipArchiveBase;

		typedef ZipFileBase <std::string> ZipFile;
		typedef ZipDirectoryBase <std::string> ZipDirectory;
		typedef ZipArchiveBase <std::string> ZipArchive;

#if GENLIB_WINDOWS

		typedef ZipFileBase <std::wstring> ZipWFile;
		typedef ZipDirectoryBase <std::wstring> ZipWDirectory;
		typedef ZipArchiveBase <std::wstring> ZipWArchive;

#endif

		typedef std::vector	<ZipFile *>			ZipFileArray;
		typedef std::vector	<ZipDirectory *>	ZipDirectoryArray;

		typedef std::map	<std::string, ZipFile *>		ZipFileMap;
		typedef std::map	<std::string, ZipDirectory *>	ZipDirectoryMap;

		typedef std::vector	<std::string>		StringArray;
	}
}

#endif
