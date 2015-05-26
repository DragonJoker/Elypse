#ifndef ___MODULE_DOWNLOAD_H___
#define ___MODULE_DOWNLOAD_H___

#include "EMuse.h"

#include <Path.h>
#include <Url.h>

namespace EMuse
{
	namespace Download
	{
		class CMD5Checksum;
		class MuseDownloader;
		class MusePack;
		class MuseFilePack;
		class DownloadManager;

		struct EM_SndHeader;

		typedef std::map	<String, MuseDownloader *> DownloaderMap;
		typedef std::vector	<MuseFilePack *>				MuseFileArray;
		typedef std::vector	<EM_SndHeader *>			SndHeaderArray;
		typedef std::vector <char>						DataArray;

		enum DataBlockType
		{
			EM_BLOCK_ZIPDATA		= 1,	/* .zip files */
			EM_BLOCK_GZIPDATA		= 2,	/* unused */
			EM_BLOCK_SCENE			= 3,	/* .emscene files */
			EM_BLOCK_INTERACTIONS	= 4,	/* .emscript files */
			EM_BLOCK_SNDDATA		= 5,	/* unused */
			EM_BLOCK_CONFIG			= 6,	/* unused */
			EM_BLOCK_ZIPSNDDATA		= 7,	/* .emsnd files */
			EM_BLOCK_EMCONFIG		= 8,	/* unused */
			EM_NB_BLOCK_TYPES		= 8,
			EM_BLOCK_ERROR			= 9,
		};

		enum SessionType
		{
			EM_TYPE_LOCAL,
			EM_TYPE_HTTP,
			EM_TYPE_FTP,
			EM_TYPE_ERROR
		};

		enum HeaderStatus
		{
			HEADEREMPTY			= 0,
			TYPEISOK			= 1,
			FILENAMESIZEISOK	= 2,
			FILENAMEISOK		= 3,
			FILESIZEISOK		= 4,
			HEADERCOMPLETE		= 5,
			HEADERERROR			= 6
		};
	}
}

using namespace EMuse::Download;

#endif
