/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_MUSE_FILE_H___
#define ___TROLL_MUSE_FILE_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <MuseFile.h>

namespace Troll
{
	namespace ElypseRW
	{
		class TrollMuseFile
			: public Elypse::Data::MuseFile
		{
		public:
			TrollMuseFile( String const & p_name )
				: MuseFile( p_name )
			{
			}

			virtual ~TrollMuseFile()
			{
			}

			virtual bool WaitForFile( String const & p_filename, bool p_useLoadingBar )
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

		private:
			General::Utils::Path m_completePath;
		};
	}
}

#endif
