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
#ifndef ___CONFIG_FILE_H___
#define ___CONFIG_FILE_H___

#include "Module_Data.h"

#include "Module_Script.h"

#include <Mutex.h>
#include <Condition.h>
#include <Path.h>

namespace Elypse
{
	namespace Data
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
}

#endif
