/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
