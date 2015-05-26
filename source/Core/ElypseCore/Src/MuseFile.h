#ifndef ___MUSE_FILE_H___
#define ___MUSE_FILE_H___

#include "Module_Data.h"
#include "Module_Download.h"

#include <Mutex.h>

namespace EMuse
{
	namespace Data
	{
		class d_dll_export MuseFile : public General::Theory::named, d_noncopyable
		{
		protected:
			MuseDownloader * m_downloader;
			MuseFileArray m_childs;
			DataFileMap m_dataFiles;
			ConfigFileMap m_configFiles;
			General::MultiThreading::Mutex m_mutex;

		public:
			MuseFile( const String & p_name );
			virtual ~MuseFile();

		public:
			virtual DataFile * GetDataFile( const String & p_filename );
			virtual ConfigFile * GetConfigFile( const String & p_filename );
			virtual bool WaitForFile( const String & p_filename, bool p_useLoadingBar );
			virtual Path GetCompletePath()const;

		public:
			inline MuseDownloader * GetDownloader()const
			{
				return m_downloader;
			}
		};
	}
}

#endif
