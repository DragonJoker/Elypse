/*
See LICENSE file in root folder
*/
#ifndef ___MODULE_DOWNLOAD_H___
#define ___MODULE_DOWNLOAD_H___

#include "Elypse.h"

#include <Path.h>
#include <Url.h>

namespace Elypse
{
	namespace Download
	{
		class CMD5Checksum;
		class MuseDownloader;
		class MusePack;
		class MuseFilePack;
		class DownloadManager;

		struct EM_SndHeader;

		typedef std::map< String, MuseDownloader * > DownloaderMap;
		typedef std::vector< MuseFilePack * > MuseFileArray;
		typedef std::vector< EM_SndHeader * > SndHeaderArray;
		typedef std::vector< char > DataArray;

		enum DataBlockType
		{
			EM_BLOCK_UNKNOWN = 0,
			EM_BLOCK_ZIPDATA = 1, /* .zip files */
			EM_BLOCK_GZIPDATA = 2, /* unused */
			EM_BLOCK_SCENE = 3, /* .emscene files */
			EM_BLOCK_INTERACTIONS = 4, /* .emscript files */
			EM_BLOCK_SNDDATA = 5, /* unused */
			EM_BLOCK_CONFIG = 6, /* unused */
			EM_BLOCK_ZIPSNDDATA = 7, /* .emsnd files */
			EM_BLOCK_EMCONFIG = 8, /* unused */
			EM_NB_BLOCK_TYPES = 8,
			EM_BLOCK_ERROR = 9,
		};

		enum SessionType
		{
			EM_TYPE_UNKNOWN,
			EM_TYPE_LOCAL,
			EM_TYPE_HTTP,
			EM_TYPE_FTP,
			EM_TYPE_ERROR
		};

		enum HeaderStatus
		{
			HEADEREMPTY = 0,
			TYPEISOK = 1,
			FILENAMESIZEISOK = 2,
			FILENAMEISOK = 3,
			FILESIZEISOK = 4,
			HEADERCOMPLETE = 5,
			HEADERERROR = 6
		};

		static const std::string  DataTypeExtension[] =
		{
			"",
			".zip",			// EM_BLOCK_ZIPDATA			= 1,	/* .zip files */
			".gzip",		// EM_BLOCK_GZIPDATA		= 2,	/* unused */
			".emscene",		// EM_BLOCK_SCENE			= 3,	/* .emscene files */
			".emscript",	// EM_BLOCK_INTERACTIONS	= 4,	/* .emscript files */
			".snd",			// EM_BLOCK_SNDDATA			= 5,	/* unused */
			".cfg",			// EM_BLOCK_CONFIG			= 6,	/* unused */
			".emsnd",		// EM_BLOCK_ZIPSNDDATA		= 7,	/* .emsnd files */
			".emcfg",		// EM_BLOCK_EMCONFIG		= 8,	/* unused */
		};

		struct EM_Block
		{
			uint8_t m_blocktype;
			uint8_t m_namesize;
			std::string m_name;
			uint32_t m_blocsize;
			std::string m_completeUrl;
			std::string m_hash;
		};

		struct EM_ZipBlock
		{
			uint32_t BlockSize;
			uint16_t NameSize;
			std::string Name;
			std::string CompleteURL;
		};

		typedef std::vector< EM_Block > BlockList;
		typedef std::vector< EM_ZipBlock > ZipBlockList;
	}
}

using namespace Elypse::Download;

#endif
