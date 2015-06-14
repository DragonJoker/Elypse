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

BEGIN_TROLL_PROJECT_NAMESPACE
{
	struct File
	{
		File()
			: m_scene( NULL )
			, Filetype( sceneFile )
			, CfgFile()
			, FolderId( 0 )
			, Saved( false )
			, Open( false )
			, ItemId( 0 )
			, EditPanel( NULL )
			, IdOnglet( 0 )
			, m_compiled( false )
			, FileName()
		{
		}

		File( Scene * p_pScene, FileType p_eFileType, wxString p_strFile, Elypse::Data::MuseFile * p_pMuseFile, wxTreeItemId p_iFolderId )
			: m_scene( p_pScene )
			, Filetype( p_eFileType )
			, CfgFile( std::make_unique< ConfigFile >( GuiCommon::make_string( p_strFile ), p_pMuseFile ) )
			, FolderId( p_iFolderId )
			, Saved( true )
			, Open( false )
			, ItemId( 0 )
			, EditPanel( NULL )
			, IdOnglet( 0 )
			, m_compiled( false )
		{
			//TODOMIAOU ?
			CfgFile->DownloadFinished();
			p_strFile.Replace( wxT( "/" ), wxString() << wxFileName::GetPathSeparator() );
			p_strFile.Replace( wxT( "\\" ), wxString() << wxFileName::GetPathSeparator() );
			std::size_t l_index = p_strFile.find_last_of( wxFileName::GetPathSeparator() );
			FileName = p_strFile.substr( l_index + 1 );
		}

		File( const File & p_file )
			: FileName( p_file.FileName )
			, Open( p_file.Open )
			, Saved( p_file.Saved )
			, ItemId( p_file.ItemId )
			, FolderId( p_file.FolderId )
			, Filetype( p_file.Filetype )
			, CfgFile( std::make_unique< ConfigFile >( p_file.CfgFile->GetName(), p_file.CfgFile->GetOwner() ) )
			, EditPanel( p_file.EditPanel )
			, IdOnglet( p_file.IdOnglet )
			, m_scene( p_file.m_scene )
			, m_compiled( p_file.m_compiled )
		{
		}

		~File()
		{
		}

		wxString FileName;
		bool Open;
		bool Saved;
		wxTreeItemId ItemId;
		wxTreeItemId FolderId;
		FileType Filetype;
		std::unique_ptr< ConfigFile > CfgFile;
		Troll::GUI::wxStcTextEditor * EditPanel;
		int IdOnglet;
		Scene * m_scene;
		bool m_compiled;
	};
}
END_TROLL_PROJECT_NAMESPACE

#endif
