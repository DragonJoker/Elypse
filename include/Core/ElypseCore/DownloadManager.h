/*
See LICENSE file in root folder
*/
#ifndef ____DOWNLOAD_MANAGER_H___
#define ____DOWNLOAD_MANAGER_H___

#include "Module_Download.h"
#include "Module_Data.h"
#include "Module_Main.h"

namespace Elypse::Download
{
	class d_dll_export DownloadManager : d_noncopyable
	{
	public:
		DownloadManager( const General::Utils::Path & p_installPath
			, ElypseLoadingBar * p_loadingBar
			, bool p_downloadFiles );

		MuseFile * StartDownloadFile( const General::Utils::Url & p_baseUrl, bool p_download = true );
		MuseFile * GetMuseFile( String const & p_filename )const;

	private:
		ElypseLoadingBar * m_loadingBar;
		MuseFileMap m_museFiles;
		General::Utils::Path m_installPath;
		bool m_downloadFiles;
	};
}

#endif
