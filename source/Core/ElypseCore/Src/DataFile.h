#ifndef ___DATA_FILE_H___
#define ___DATA_FILE_H___

#include "Module_Data.h"

#include <Mutex.h>
#include <Path.h>

namespace EMuse
{
	namespace Data
	{
		class d_dll_export DataFile : public General::Theory::named, public General::Theory::owned_by<MuseFile>, d_noncopyable
		{
		protected:
			bool m_initialised;
			bool m_downloaded;
			bool m_preInit;
			General::MultiThreading::Mutex m_mutex;
			UIntMap m_loadedBy;

		public:
			DataFile( const String & p_filename, MuseFile * p_owner );
			~DataFile();

		protected:
			void _load();
			void _unload();

		public:
			void Use();
			void Release();
			void DownloadFinished();
			String GetDescriptiveName()const;

		public:
			inline bool IsDownloaded()const
			{
				return m_downloaded;
			}
		};
	}
}

#endif
