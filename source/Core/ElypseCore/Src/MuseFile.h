/*
See LICENSE file in root folder
*/
#ifndef ___MUSE_FILE_H___
#define ___MUSE_FILE_H___

#include "Module_Data.h"
#include "Module_Download.h"

#include "MuseDownloader.h"

#include <Mutex.h>

namespace Elypse
{
	namespace Data
	{
		class d_dll_export MuseFile
			: public General::Theory::named
			, d_noncopyable
		{
		public:
			MuseFile( String const & p_name );
			virtual ~MuseFile();

			virtual DataFile * GetDataFile( String const & p_filename );
			virtual ConfigFile * GetConfigFile( String const & p_filename );
			virtual bool WaitForFile( String const & p_filename, bool p_useLoadingBar );
			virtual Path GetCompletePath()const;

			inline MuseDownloader const & GetDownloader()const
			{
				return m_downloader;
			}

			inline MuseDownloader & GetDownloader()
			{
				return m_downloader;
			}

		protected:
			MuseDownloader m_downloader;
			MuseFileArray m_childs;
			DataFileMap m_dataFiles;
			ConfigFileMap m_configFiles;
			std::mutex m_mutex;
		};
	}
}

#endif
