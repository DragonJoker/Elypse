/*
See LICENSE file in root folder
*/
#ifndef ___CONFIG_FILE_H___
#define ___CONFIG_FILE_H___

#include "Module_Data.h"

#include "Module_Script.h"

#include <Mutex.h>
#include <Condition.h>
#include <Path.h>

namespace Elypse::Data
{
	class d_dll_export ConfigFile
		: public General::Theory::named
		, public General::Theory::owned_by< MuseFile >
		, d_noncopyable
	{
	public:
		ConfigFile( MuseFile & p_owner );
		ConfigFile( String const & p_name, MuseFile & p_owner );
		~ConfigFile();

		void Use();
		void Release();
		void DownloadFinished();
		String GetDescriptiveName()const;

		inline bool IsDownloaded()const
		{
			return m_downloaded;
		}

	protected:
		void DoLoad();

	protected:
		uint32_t m_useCount;
		bool m_downloaded;
		std::mutex m_mutex;
	};
}

#endif
