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

#include "GUI/FilesTree.h"
#include "GUI/MainFrame.h"

#include "TrollEditorApplication.h"

#include "Project/Scene.h"
#include "Project/Project.h"

#include "xpm/icon1.xpm"
#include "xpm/icon2.xpm"
#include "xpm/icon3.xpm"
#include "xpm/icon4.xpm"
#include "xpm/icon5.xpm"
#include "xpm/icon6.xpm"
#include "xpm/icon7.xpm"

BEGIN_TROLL_GUI_NAMESPACE
{
	BEGIN_EVENT_TABLE( FilesTree, wxTreeCtrl )
		EVT_TREE_ITEM_MENU( TreeFichier_Ctrl, FilesTree::OnFichierRClick )
		EVT_TREE_END_LABEL_EDIT( TreeFichier_Ctrl, FilesTree::OnEndLabelEdit )
		EVT_TREE_BEGIN_LABEL_EDIT( TreeFichier_Ctrl, FilesTree::OnBeginLabelEdit )
		EVT_TREE_ITEM_ACTIVATED( TreeFichier_Ctrl, FilesTree::OnFileActivated )
		EVT_TREE_SEL_CHANGED( TreeFichier_Ctrl, FilesTree::OnItemClic )
	END_EVENT_TABLE()

	FilesTree::FilesTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: wxTreeCtrl( parent, TreeFichier_Ctrl, pos, size, style )
		, m_selectedScene( NULL )
		, m_imageSize( 0 )
	{
		CreateImageList();
	}

	FilesTree::~FilesTree()
	{
		Cleanup();
	}

	void FilesTree::Cleanup()
	{
		m_scenes.clear();
	}

	void FilesTree::CreateImageList( int p_size )
	{
		if ( p_size == -1 )
		{
			SetImageList( NULL );
		}
		else
		{
			if ( !p_size )
			{
				p_size = m_imageSize;
			}
			else
			{
				m_imageSize = p_size;
			}

			wxImageList * l_images = new wxImageList( p_size, p_size, true );
			wxBusyCursor l_wait;
			wxIcon l_icons[7] = {
				wxIcon( icon1_xpm ),
				wxIcon( icon2_xpm ),
				wxIcon( icon3_xpm ),
				wxIcon( icon4_xpm ),
				wxIcon( icon5_xpm ),
				wxIcon( icon6_xpm ),
				wxIcon( icon7_xpm ),
			};
			int l_sizeOrig = l_icons[0].GetWidth();

			for ( size_t i = 0; i < WXSIZEOF( l_icons ); i++ )
			{
				if ( p_size == l_sizeOrig )
				{
					l_images->Add( l_icons[i] );
				}
				else
				{
					l_images->Add( wxBitmap( wxBitmap( l_icons[i] ).ConvertToImage().Rescale( p_size, p_size ) ) );
				}
			}

			AssignImageList( l_images );
		}
	}

	void FilesTree::InitProject( const wxString & p_projectName )
	{
		std::clog << "FilesTree::InitProject - " << p_projectName << std::endl;
		int l_image = FilesTreeIcon_Folder;
		m_rootProjetId = AddRoot( p_projectName, l_image, l_image, new TETreeItemData( ITEM_DESC_ROOT, TITProjectRoot ) );

		if ( l_image != -1 )
		{
			SetItemImage( m_rootProjetId, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}
	}

	wxTreeItemId FilesTree::AddSceneToProject( TROLL_PROJECT_NAMESPACE::Scene * p_scene )
	{
		TreeItemMap::iterator l_it = m_scenes.find( p_scene->GetName() );

		if ( l_it != m_scenes.end() )
		{
			throw std::runtime_error( make_string( _( "Can't add an already existing scene" ) ) );
		}

		int l_image = FilesTreeIcon_Folder;
		int l_imageSel = l_image + 1;
		wxTreeItemId l_id = AppendItem( m_rootProjetId, p_scene->GetName(), l_image, l_imageSel, new TETreeItemData( ITEM_DESC_SCENE, TITSceneRoot ) );
		m_idParent = l_id;
		SceneItem l_sceneItem;
		l_sceneItem.Trollscene = p_scene;
		l_sceneItem.TreeItem = l_id;
		m_scenes.insert( std::make_pair( p_scene->GetName(), l_sceneItem ) );
		Expand( m_rootProjetId );

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		Collapse( l_id );
		return l_id;
	}

	wxTreeItemId FilesTree::AddFolderToScene( const wxString & p_sceneName, const wxString & p_folderName )
	{
		TreeItemMap::iterator l_it = m_scenes.find( p_sceneName );

		if ( l_it == m_scenes.end() )
		{
			throw std::runtime_error( make_string( _( "Can't add a folder to a non existing scene" ) ) );
		}

		int l_image = FilesTreeIcon_Folder;
		int l_imageSel = l_image + 1;
		TETreeItemType l_titype = TITSceneFile;

		if ( p_folderName == LOAD_SCRIPT_FILES )
		{
			l_titype = TITLoadScriptFile;
		}
		else if ( p_folderName == UNLOAD_SCRIPT_FILES )
		{
			l_titype = TITUnloadScriptFile;
		}
		else if ( p_folderName == GRAPHIC_DATA_FOLDERS )
		{
			l_titype = TITDataFile;
		}
		else if ( p_folderName == OTHER_DATA_FOLDERS )
		{
			l_titype = TITDataFolder;
		}

		wxTreeItemId l_id = AppendItem( l_it->second.TreeItem, p_folderName, l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FOLDER, l_titype ) );
		m_folders.insert( std::make_pair( p_folderName, l_id ) );
		m_idParent = l_id;
		Collapse( l_it->second.TreeItem );

		if ( l_image != -1 )
		{
			SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}

		return l_id;
	}

	wxTreeItemId FilesTree::AddFileToFolder( const wxTreeItemId & p_item, const wxString & p_idFile,
			TETreeItemType p_type, bool p_exists )
	{
		int l_image = FilesTreeIcon_File;
		int l_imageSel = l_image + 1;
		size_t l_index = min( p_idFile.find_last_of( wxT( '\\' ) ), p_idFile.find_last_of( wxT( '/' ) ) );
		wxString l_filename = p_idFile.substr( l_index + 1 );
		wxTreeItemId l_id = AppendItem( p_item, l_filename, l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, p_type ) );
		m_idParent = l_id;
		Expand( p_item );

		if ( ! p_exists )
		{
			wxString l_path;
			auto && l_project = GUI::MainFrame::GetInstance()->GetCurrentProject();

			if ( l_project )
			{
				l_path = l_project->GetPath() + m_selectedScene->GetName() + wxFileName::GetPathSeparator();
			}

			fclose( fopen( make_string( l_path + p_idFile ).c_str(), "w" ) );
		}

		m_mapFile.insert( std::make_pair( p_idFile , l_id ) );
		return l_id;
	}

	wxTreeItemId FilesTree::AddExistingFileToProjet( const wxTreeItemId & p_itemParent, TETreeItemType p_type,
			const wxString p_idFile )
	{
		int l_image = FilesTreeIcon_File;
		int l_imageSel = l_image + 1;
		wxTreeItemId l_id = AppendItem( p_itemParent, p_idFile, l_image, l_imageSel, new TETreeItemData( ITEM_DESC_FILE, p_type ) );
		m_idParent = l_id;
		Expand( p_itemParent );
		return l_id;
	}

	void FilesTree::_logEvent( const wxChar * p_name, const wxTreeEvent & p_event )
	{
		wxTreeItemId l_item = p_event.GetItem();
		String l_text;

		if ( l_item.IsOk() )
		{
			l_text += '"' + make_string( GetItemText( l_item ) ) + '"';
		}
		else
		{
			l_text = make_string( _( "invalid item" ) );
		}

		std::cout << make_string( p_name ) << "(" << l_text << ")" << std::endl;
	}

	void FilesTree::ShowContextMenuFichier( const wxPoint & p_pos, TETreeItemData * p_item )
	{
		wxMenu l_menu;

		if ( p_item == NULL )
		{
			l_menu.Append( TreeNewScene, _( "Add a Scene" ) );
		}
		else
		{
			wxString l_description = p_item->GetDesc();

			if ( l_description == ITEM_DESC_SCENE )
			{
				l_menu.Append( EditSceneDependencies, _( "Edit scene dependencies" ) );
				l_menu.Append( RenameScene, _( "Rename" ) );
				l_menu.Append( DeleteScene, TE_REMOVE );
			}

			if ( l_description == ITEM_DESC_FOLDER )
			{
				TROLL_PROJECT_NAMESPACE::FileType l_type = m_selectedScene->GetType( m_selectedItem );

				if ( l_type != TROLL_PROJECT_NAMESPACE::dataFolder && l_type != TROLL_PROJECT_NAMESPACE::dataFile )
				{
					l_menu.Append( Add, _( "Add a new file" ) );
					l_menu.Append( IdAddExistingFile, _( "Add an existing file" ) );
				}
				else
				{
					l_menu.Append( IdAddExistingFile, _( "Add a folder" ) );
				}
			}

			if ( l_description == ITEM_DESC_FILE )
			{
				if ( p_item->GetFileType() != TITDataFolder && p_item->GetFileType() != TITDataFile )
				{
					l_menu.Append( IdOuvrirFichier, _( "Open" ) );
					l_menu.Append( IdOnSaveSelectedFile, _( "Save" ) );
				}
				else
				{
					l_menu.Append( IdListFolder, _( "Display content" ) );
				}

				l_menu.Append( Rename, _( "Rename" ) );
				l_menu.AppendSeparator();			//*****************************//
				l_menu.Append( IdRemove, _( "Remove from the list" ) );
				l_menu.Append( IdDelete, _( "Delete from file system" ) );

				if ( p_item->GetFileType() != TITDataFolder )
				{
					l_menu.AppendSeparator();			//*****************************//
					l_menu.Append( IdFermer, _( "Close" ) );
				}
			}

			if ( l_description == ITEM_DESC_ROOT )
			{
				l_menu.Append( OpenProject, _( "Open project" ) );
				l_menu.Append( saveProject, _( "Save project" ) );
				l_menu.Append( NewScene, _( "New Scene" ) );
			}
		}

		PopupMenu( & l_menu, p_pos.x, p_pos.y );
	}

	wxTreeItemId FilesTree::GetFolderId( const wxString & p_name )
	{
		if ( m_folders.find( p_name ) != m_folders.end() )
		{
			return m_folders.find( p_name )->second;
		}

		return 0;
	}

	wxTreeItemId FilesTree::GetItemByName( const wxString & p_name )
	{
		MapTypeFile::iterator l_it = m_mapFile.find( p_name );

		if ( l_it == m_mapFile.end() )
		{
			wxTreeItemId gg = NULL;
			return gg;
		}

		return l_it->second;
	}

	void FilesTree::OnFichierRClick( wxTreeEvent & p_event )
	{
		wxPoint l_point = p_event.GetPoint();
		m_selectedItem = p_event.GetItem();
		SelectItem( m_selectedItem );
		TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
		ShowContextMenuFichier( l_point, l_item );
	}

	void FilesTree::OnBeginLabelEdit( wxTreeEvent & p_event )
	{
		m_selectedItem = p_event.GetItem();
		SelectItem( m_selectedItem );
		TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
		wxString l_typeOfItem = l_item->GetDesc();

		if ( l_typeOfItem == ITEM_DESC_FILE )
		{
			wxString l_fullFileName = m_selectedScene->GetFile( m_selectedItem )->FileName;

			if ( l_fullFileName.Find( wxFileName::GetPathSeparator() ) == wxNOT_FOUND )
			{
				size_t l_index = min( l_fullFileName.find_last_of( '\\' ), l_fullFileName.find_last_of( '/' ) );
				m_itemName = l_fullFileName;
			}
			else
			{
				size_t l_index = min( l_fullFileName.find_last_of( '\\' ), l_fullFileName.find_last_of( '/' ) );
				m_itemName = l_fullFileName;
			}
		}
		else if ( l_typeOfItem == ITEM_DESC_SCENE )
		{
			m_itemPath.clear();
			m_itemName = m_selectedScene->GetName();
		}
		else if ( l_typeOfItem == ITEM_DESC_ROOT )
		{
			m_itemPath.clear();

			if ( GUI::MainFrame::GetInstance()->GetCurrentProject() )
			{
				m_itemName = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetName();
			}
		}
	}

	void FilesTree::OnEndLabelEdit( wxTreeEvent & p_event )
	{
		auto && l_project = GUI::MainFrame::GetInstance()->GetCurrentProject();
		TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
		wxString l_typeOfItem = l_item->GetDesc();
		std::clog << p_event.GetLabel() << std::endl;

		if ( l_typeOfItem == ITEM_DESC_FILE && ! p_event.GetLabel().empty() )
		{
			auto && l_file = m_selectedScene->GetFile( m_selectedItem );

			if ( l_file->Open )
			{
				GUI::MainFrame::GetInstance()->UpdatePageLabel( l_file->FileName, p_event.GetLabel() );
			}

			wxString l_path = l_project->GetPath() + m_selectedScene->GetName() + wxFileName::GetPathSeparator();

			if ( l_file->Saved )
			{
				wxRenameFile( l_path + l_file->FileName, l_path + p_event.GetLabel() );
			}
			else
			{
				GUI::MainFrame::GetInstance()->SaveFile( l_file, p_event.GetLabel().c_str() );
			}

			l_file->FileName = p_event.GetLabel().c_str();
			l_file->CfgFile = std::make_unique< ConfigFile >( make_string( l_file->FileName ), m_selectedScene->GetMuseFile() );
		}
		else if ( l_typeOfItem == ITEM_DESC_SCENE )
		{
			m_selectedScene->SetName( p_event.GetLabel().c_str() );
		}
		else if ( l_typeOfItem == ITEM_DESC_ROOT )
		{
			if ( GUI::MainFrame::GetInstance()->GetCurrentProject() )
			{
				GUI::MainFrame::GetInstance()->GetCurrentProject()->SetName( p_event.GetLabel().c_str() );
			}
		}

		m_selectedItem = p_event.GetItem();
		SelectItem( m_selectedItem );
	}

	void FilesTree::OnFileActivated( wxTreeEvent & p_event )
	{
		auto && l_project = GUI::MainFrame::GetInstance()->GetCurrentProject();

		if ( l_project )
		{
			m_selectedItem = p_event.GetItem();
			SelectItem( m_selectedItem );
			m_selectedScene = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetScene( m_selectedItem );
			TETreeItemData * l_item = reinterpret_cast< TETreeItemData * >( GetItemData( m_selectedItem ) );
			wxString l_typeOfItem = l_item->GetDesc();

			if ( l_typeOfItem == ITEM_DESC_FILE )
			{
				if ( l_item->GetFileType() != TITDataFolder && l_item->GetFileType() != TITDataFile )
				{
					GUI::MainFrame::GetInstance()->OpenSelectedFile();
				}
				else
				{
					GUI::MainFrame::GetInstance()->ListFolder();
				}
			}
		}
	}

	void FilesTree::OnItemClic( wxTreeEvent & p_event )
	{
		m_selectedItem = p_event.GetItem();

		if ( GUI::MainFrame::GetInstance()->GetCurrentProject() )
		{
			m_selectedScene = GUI::MainFrame::GetInstance()->GetCurrentProject()->GetScene( m_selectedItem );

			if ( m_selectedScene )
			{
				std::clog << "TETreeCtrl::OnItemClic - Scene : " << m_selectedScene->GetName() << "\n";
			}
		}
	}
}
END_TROLL_GUI_NAMESPACE
