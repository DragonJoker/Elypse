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
#ifndef ____DOWNLOAD_MANAGER_H___
#define ____DOWNLOAD_MANAGER_H___

#include "Module_Download.h"
#include "Module_Data.h"
#include "Module_Main.h"

namespace Elypse
{
	namespace Download
	{
		class d_dll_export DownloadManager : d_noncopyable
		{
		private:
			ElypseLoadingBar * m_loadingBar;

			MuseFileMap m_museFiles;

			General::Utils::Path m_installPath;

			bool m_downloadFiles;

		public:
			DownloadManager( const General::Utils::Path & p_installPath,
								ElypseLoadingBar * p_loadingBar,
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
