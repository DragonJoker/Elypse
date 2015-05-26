#ifndef ____DOWNLOAD_MANAGER_H___
#define ____DOWNLOAD_MANAGER_H___

#include "Module_Download.h"
#include "Module_Data.h"
#include "Module_Main.h"

namespace EMuse
{
	namespace Download
	{
		class d_dll_export DownloadManager : d_noncopyable
		{
		private:
			EMuseLoadingBar * m_loadingBar;

			MuseFileMap m_museFiles;

			General::Utils::Path m_installPath;

			bool m_downloadFiles;

		public:
			DownloadManager(	const General::Utils::Path & p_installPath,
								EMuseLoadingBar * p_loadingBar,
								bool p_downloadFiles );
			~DownloadManager();

		public:
			MuseFile * StartDownloadFile( const General::Utils::Url & p_baseUrl, bool p_download = true );
			inline MuseFile * GetMuseFile( const String & p_filename )const
			{
				return General::Utils::map::findOrNull( m_museFiles, p_filename );
			}
		};
	}
}

#endif
