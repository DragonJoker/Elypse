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
#ifndef ___TE_TROLL_FILE_H___
#define ___TE_TROLL_FILE_H___

#include "Module_Project.h"

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/string.h>
#include <ConfigFile.h>

namespace Troll
{
	namespace ProjectComponents
	{
		struct File
		{
			File() = default;

			File( Scene * p_pScene, FileType p_eFileType, wxString p_strFile, Elypse::Data::MuseFile & p_pMuseFile, wxTreeItemId p_iFolderId = 0 )
				: m_scene{ p_pScene }
				, Filetype{ p_eFileType }
				, CfgFile{ std::make_shared< ConfigFile >( GuiCommon::make_string( p_strFile ), p_pMuseFile ) }
				, FolderId{ p_iFolderId }
				, Saved{ true }
			{
				//TODOMIAOU ?
				CfgFile->DownloadFinished();
				p_strFile.Replace( wxT( "/" ), wxString() << wxFileName::GetPathSeparator() );
				p_strFile.Replace( wxT( "\\" ), wxString() << wxFileName::GetPathSeparator() );
				std::size_t l_index = p_strFile.find_last_of( wxFileName::GetPathSeparator() );
				FileName = p_strFile.substr( l_index + 1 );
			}

			wxString FileName;
			bool Open{ false };
			bool Saved{ false };
			wxTreeItemId ItemId{ 0 };
			wxTreeItemId FolderId{ 0 };
			FileType Filetype{ sceneFile };
			std::shared_ptr< ConfigFile > CfgFile{ nullptr };
			Troll::GUI::wxStcTextEditor * EditPanel{ nullptr };
			int IdOnglet{ 0 };
			Scene * m_scene{ nullptr };
			bool m_compiled{ false };
		};
	}
}

#endif
