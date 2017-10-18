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
