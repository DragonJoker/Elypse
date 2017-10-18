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
#include "TrollEditorPch.h"

#include "GUI/FolderList.h"

#include <File.h>

namespace Troll
{
	namespace GUI
	{
		enum FolderListIDs
		{
			Liste,
			Supprimer,
			Ajouter,
		};

		BEGIN_EVENT_TABLE( FolderList, wxDialog )
			EVT_BUTTON( Ajouter, FolderList::OnAddFile )
			EVT_BUTTON( Supprimer, FolderList::OnDelFile )
		END_EVENT_TABLE()

		FolderList::FolderList( wxString const & p_folder, wxWindow * p_parent, wxPoint const & p_position )
			: wxDialog( p_parent, wxID_ANY, _( "Files List" ), p_position, wxSize( 200, 350 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT )
			, m_folder( p_folder )
			, m_filesList( NULL )
		{
			SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			SetForegroundColour( PANEL_FOREGROUND_COLOUR );

			//std::cout << "FolderList::FolderList - " << m_folder << "\n";
			wxSize l_size = GetClientSize();
			int l_iBtnCx = 80;
			int l_iBtnCy = 25;
			int l_iOffset = 5;
			int l_iBtnX = l_iOffset;
			int l_iBtnY = l_size.y - ( l_iOffset + l_iBtnCy ) * 2;
			wxButton * l_buttonDel = new wxButton( this, Supprimer, TE_REMOVE, wxPoint( l_iBtnX, l_iBtnY ), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
			l_iBtnX = l_size.x - l_iBtnCx - l_iOffset;
			wxButton * l_buttonAdd = new wxButton( this, Ajouter, _( "Add" ), wxPoint( l_iBtnX, l_iBtnY ), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
			l_iBtnX = ( l_size.x - l_iBtnCx ) / 2;
			l_iBtnY += l_iOffset + l_iBtnCy;
			wxButton * l_buttonQuit = new wxButton( this, wxID_CANCEL, _( "Back" ), wxPoint( l_iBtnX, l_iBtnY ), wxSize( l_iBtnCx, l_iBtnCy ), wxBORDER_SIMPLE );
			_listFolder();
		}

		FolderList::~FolderList()
		{
		}

		void FolderList::RemoveFile( wxString const & p_file )
		{
			FileDelete( make_string( m_folder + wxT( "/" ) + p_file ) );
		}

		void FolderList::AddFile( wxString const & p_file )
		{
			auto l_index = min( p_file.find_last_of( wxT( "/" ) ), p_file.find_last_of( wxT( "\\" ) ) );
			wxString l_fileName = p_file.substr( l_index + 1 );
			std::clog << "FolderList::AddFile - " << m_folder << "/" << l_fileName << "\n";

			if ( FileCopy( make_string( p_file ), make_string( m_folder + wxT( "/" ) + l_fileName ) ) )
			{
				m_filesList->InsertItems( 1, &l_fileName, m_filesList->GetCount() );
			}
			else
			{
				std::cout << "FolderList::AddFile - " << _( "Can't copy from : " ) << p_file << _( " to : " ) << m_folder << "/" << l_fileName << std::endl;
			}
		}

		void FolderList::_listFolder()
		{
			if ( m_filesList != NULL )
			{
				m_filesList->Clear();
				RemoveChild( m_filesList );
				delete m_filesList;
				m_filesList = NULL;
			}

			if ( !General::Files::DirectoryExists( make_string( m_folder ) ) )
			{
				std::cout << "FolderList::_listFolder - Folder " << m_folder << " doesn't exist\n";
			}
			else
			{
				wxArrayString l_files;

				General::Files::ListDirectoryFiles( make_string( m_folder ), [this, &l_files]( General::Utils::Path const & p_path )
				{
					l_files.push_back( make_wxString( p_path.GetLeaf() ) );
				}, false );

				wxPoint l_position( 0, 0 );
				wxSize l_size = GetClientSize();
				m_filesList = new wxListBox( this, Liste, l_position, wxSize( l_size.x, l_size.y - 70 ), l_files, wxLB_EXTENDED | wxLB_NEEDED_SB );
				m_filesList->SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
				m_filesList->SetForegroundColour( PANEL_FOREGROUND_COLOUR );
			}
		}

		void FolderList::OnAddFile( wxCommandEvent & p_event )
		{
			wxFileDialog l_dialog( this, _( "Choose one or more files to open" ), wxT( "" ), wxT( "" ), wxString() << _( "All" ) << wxT( "|*.*" ),
								   wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE );
			l_dialog.SetBackgroundColour( PANEL_BACKGROUND_COLOUR );
			l_dialog.SetForegroundColour( PANEL_FOREGROUND_COLOUR );

			if ( l_dialog.ShowModal() == wxID_OK )
			{
				wxArrayString l_fileNames;
				l_dialog.GetPaths( l_fileNames );

				for ( uint32_t i = 0; i < l_fileNames.size(); i++ )
				{
					AddFile( l_fileNames[i] );
				}
			}
		}

		void FolderList::OnDelFile( wxCommandEvent & p_event )
		{
			if ( m_filesList == NULL )
			{
				return;
			}

			wxArrayInt l_selection;
			m_filesList->GetSelections( l_selection );

			for ( uint32_t i = 0; i < l_selection.size(); i++ )
			{
				RemoveFile( m_filesList->GetString( l_selection[i] ) );
			}

			for ( auto i = l_selection.size() - 1; i >= 0; i-- )
			{
				m_filesList->Delete( l_selection[i] );
			}
		}

		void FolderList::OnKeyUp( wxKeyEvent & p_event )
		{
			if ( p_event.GetKeyCode() == WXK_ESCAPE )
			{
				EndDialog( wxID_CANCEL );
			}
		}
	}
}
