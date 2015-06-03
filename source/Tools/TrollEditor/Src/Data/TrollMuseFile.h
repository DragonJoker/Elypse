#ifndef ___TROLL_MUSE_FILE_H___
#define ___TROLL_MUSE_FILE_H___

#include <MuseFile.h>
namespace EMuse
{
	namespace Data
	{
		class TrollMuseFile : public MuseFile
		{
		public:
			TrollMuseFile( const String & p_name ) : MuseFile( p_name ) {}
			virtual ~TrollMuseFile() {}

		public:
			virtual bool WaitForFile( const String & p_filename, bool p_useLoadingBar )
			{
				return true;
			}
			virtual Path GetCompletePath()const
			{
				return m_completePath;
			}

			void SetCompletePath( const General::Utils::Path & p_path )
			{
				m_completePath = p_path;
			}

		public:
			inline MuseDownloader * GetDownloader()const
			{
				return m_downloader;
			}

		private:
			General::Utils::Path m_completePath;
		};
	}
}

#endif
