#ifndef ___CONFIG_FILE_H___
#define ___CONFIG_FILE_H___

#include "Module_Data.h"

#include "Module_Script.h"

#include <Mutex.h>
#include <Condition.h>
#include <Path.h>

namespace EMuse
{
	namespace Data
	{
		class d_dll_export ConfigFile : public General::Theory::named, public General::Theory::owned_by<MuseFile>, d_noncopyable
		{
		protected:
			unsigned int m_useCount;

			bool m_downloaded;

			General::MultiThreading::Mutex m_mutex;
		public:
			ConfigFile( const String & p_name, MuseFile * p_owner );
			~ConfigFile();

		protected:
			void _load();

		public:
			void Use();
			void Release();
			void DownloadFinished();
//		bool IsParsed();
//		void SetParsed( bool p_parsed);
//		void SetCompiled( ScriptNode * p_compiledScriptNode);
//		void ReleaseScript() d_no_throw;
			String GetDescriptiveName()const ;

		public:
			inline bool							IsDownloaded()const
			{
				return m_downloaded;
			}
		};
	}
}

#endif
