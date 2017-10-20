/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/FilesTree.h"
#include "GUI/FolderList.h"
#include "GUI/MainFrame.h"
#include "GUI/ProjectFrame.h"
#include "GUI/SceneDependenciesFrame.h"

#include "TrollEditorApplication.h"
#include "ProjectSaver.h"

#include "Project/Scene.h"
#include "Project/Project.h"

#include "xpm/icon1.xpm"
#include "xpm/icon2.xpm"
#include "xpm/icon3.xpm"
#include "xpm/icon4.xpm"
#include "xpm/icon5.xpm"
#include "xpm/icon6.xpm"
#include "xpm/icon7.xpm"

namespace Troll
{
	using namespace ProjectComponents;

	namespace GUI
	{
		enum FilesTreeIcons
		{
			FilesTreeIcon_File = 0,
			FilesTreeIcon_FileSelected = 1,
			FilesTreeIcon_Folder = 2,
			FilesTreeIcon_FolderSelected = 3,
			FilesTreeIcon_FolderOpened = 4,
			FilesTreeIcon_ClassFunction = 5,
		};

		enum FilesTreeIDs
		{
			FilesTreeId = 1001,
			TreeMenu_NewScene,
			TreeMenu_SceneDependencies,
			TreeMenu_RenameScene,
			TreeMenu_DeleteScene,
			TreeMenu_AddNewFile,
			TreeMenu_AddExistingFile,
			TreeMenu_OpenFile,
			TreeMenu_SaveSelectedFile,
			TreeMenu_ListFolder,
			TreeMenu_Rename,
			TreeMenu_Remove,
			TreeMenu_Delete,
			TreeMenu_Close,
			TreeMenu_SaveProject,
		};

		FilesTree::FilesTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style )
			: wxTreeCtrl{ parent, FilesTreeId, pos, size, style }
			, m_projectFrame{ projectFrame }
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );
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
				wxIcon l_icons[7] =
				{
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

		void FilesTree::LoadProject( Project & p_project )
		{
			wxString const & l_name = p_project.GetName();
			std::clog << "FilesTree::LoadProject - " << l_name << std::endl;
			int l_image = FilesTreeIcon_Folder;
			m_rootProjetId = AddRoot( l_name, l_image, l_image, new TreeItemData( ITEM_DESC_ROOT, TreeItemType::ProjectRoot ) );

			if ( l_image != -1 )
			{
				SetItemImage( m_rootProjetId, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			auto l_mainScene = p_project.GetMainScene();

			for ( auto & l_scene : p_project )
			{
				if ( l_mainScene != l_scene )
				{
					LoadScene( l_scene );
				}
			}

			if ( l_mainScene )
			{
				LoadScene( l_mainScene );
			}
		}

		void FilesTree::LoadScene( ScenePtr p_scene )
		{
			auto & l_scene = AddSceneToProject( p_scene );
			l_scene.m_folderIds[sceneFile] = AddFolderToScene( p_scene->GetName(), SCENE_FILES );
			l_scene.m_folderIds[loadScriptFile] = AddFolderToScene( p_scene->GetName(), LOAD_SCRIPT_FILES );
			l_scene.m_folderIds[unloadScriptFile] = AddFolderToScene( p_scene->GetName(), UNLOAD_SCRIPT_FILES );
			l_scene.m_folderIds[dataFile] = AddFolderToScene( p_scene->GetName(), GRAPHIC_DATA_FOLDERS );
			l_scene.m_folderIds[dataFolder] = AddFolderToScene( p_scene->GetName(), OTHER_DATA_FOLDERS );

			for ( auto & l_file : p_scene->GetSceneFiles() )
			{
				l_file.FolderId = l_scene.m_folderIds[sceneFile];
				l_file.ItemId = AddFileToFolder( l_file.FolderId, l_file.FileName, TreeItemType::SceneFile, true );
			}

			for ( auto & l_file : p_scene->GetLoadScriptFiles() )
			{
				l_file.FolderId = l_scene.m_folderIds[loadScriptFile];
				l_file.ItemId = AddFileToFolder( l_file.FolderId, l_file.FileName, TreeItemType::LoadScriptFile, true );
			}

			for ( auto & l_file : p_scene->GetUnloadScriptFiles() )
			{
				l_file.FolderId = l_scene.m_folderIds[unloadScriptFile];
				l_file.ItemId = AddFileToFolder( l_file.FolderId, l_file.FileName, TreeItemType::UnloadScriptFile, true );
			}

			for ( auto & l_file : p_scene->GetDataFiles() )
			{
				l_file.FolderId = l_scene.m_folderIds[dataFile];
				l_file.ItemId = AddFileToFolder( l_file.FolderId, l_file.FileName, TreeItemType::DataFile, true );
			}

			for ( auto & l_file : p_scene->GetDataFolders() )
			{
				l_file.FolderId = l_scene.m_folderIds[dataFolder];
				l_file.ItemId = AddFileToFolder( l_file.FolderId, l_file.FileName, TreeItemType::DataFolder, true );
			}
		}

		TreeScene & FilesTree::AddSceneToProject( ProjectComponents::ScenePtr p_scene )
		{
			auto l_it = m_scenes.find( p_scene->GetName() );

			if ( l_it != m_scenes.end() )
			{
				GENLIB_EXCEPTION( make_string( _( "Can't add an already existing scene" ) ) );
			}

			int l_image = FilesTreeIcon_Folder;
			int l_imageSel = l_image + 1;
			wxTreeItemId l_id = AppendItem( m_rootProjetId, p_scene->GetName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_SCENE, TreeItemType::SceneRoot ) );
			m_idParent = l_id;
			l_it = m_scenes.insert( { p_scene->GetName(), TreeScene{ p_scene } } ).first;
			l_it->second.m_sceneId = l_id;
			Expand( m_rootProjetId );

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			Collapse( l_id );
			return l_it->second;
		}

		wxTreeItemId FilesTree::AddFolderToScene( wxString const & p_sceneName, wxString const & p_folderName )
		{
			auto l_it = m_scenes.find( p_sceneName );

			if ( l_it == m_scenes.end() )
			{
				throw std::runtime_error( make_string( _( "Can't add a folder to a non existing scene" ) ) );
			}

			int l_image = FilesTreeIcon_Folder;
			int l_imageSel = l_image + 1;
			TreeItemType l_titype = TreeItemType::SceneFile;

			if ( p_folderName == LOAD_SCRIPT_FILES )
			{
				l_titype = TreeItemType::LoadScriptFile;
			}
			else if ( p_folderName == UNLOAD_SCRIPT_FILES )
			{
				l_titype = TreeItemType::UnloadScriptFile;
			}
			else if ( p_folderName == GRAPHIC_DATA_FOLDERS )
			{
				l_titype = TreeItemType::DataFile;
			}
			else if ( p_folderName == OTHER_DATA_FOLDERS )
			{
				l_titype = TreeItemType::DataFolder;
			}

			wxTreeItemId l_id = AppendItem( l_it->second.m_sceneId, p_folderName, l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, l_titype ) );
			m_folders.insert( { p_folderName, l_id } );
			m_idParent = l_id;
			Collapse( l_it->second.m_sceneId );

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			return l_id;
		}

		wxTreeItemId FilesTree::AddFileToFolder( wxTreeItemId const & p_item, wxString const & p_idFile, TreeItemType p_type, bool p_exists )
		{
			int l_image = FilesTreeIcon_File;
			int l_imageSel = l_image + 1;
			size_t l_index = min( p_idFile.find_last_of( wxT( '\\' ) ), p_idFile.find_last_of( wxT( '/' ) ) );
			wxString l_filename = p_idFile.substr( l_index + 1 );
			wxTreeItemId l_id = AppendItem( p_item, l_filename, l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, p_type ) );
			m_idParent = l_id;
			Expand( p_item );

			if ( !p_exists )
			{
				wxString l_path;
				auto && l_project = m_projectFrame->GetProject();

				if ( l_project )
				{
					l_path = l_project->GetPath() + m_selectedScene->GetName() + wxFileName::GetPathSeparator();
				}

				fclose( fopen( make_string( l_path + p_idFile ).c_str(), "w" ) );
			}

			m_mapFile.insert( std::make_pair( p_idFile, l_id ) );
			return l_id;
		}

		wxTreeItemId FilesTree::AddExistingFileToProjet( wxTreeItemId const & p_itemParent, TreeItemType p_type, const wxString p_idFile )
		{
			int l_image = FilesTreeIcon_File;
			int l_imageSel = l_image + 1;
			wxTreeItemId l_id = AppendItem( p_itemParent, p_idFile, l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, p_type ) );
			m_idParent = l_id;
			Expand( p_itemParent );
			return l_id;
		}

		void FilesTree::ShowContextMenuFichier( wxPoint const & p_pos, TreeItemData * p_item )
		{
			wxMenu l_menu;

			if ( !p_item )
			{
				l_menu.Append( TreeMenu_NewScene, _( "Add a Scene" ) );
			}
			else
			{
				wxString l_description = p_item->GetDesc();
				auto l_type = p_item->GetFileType();

				if ( l_description == ITEM_DESC_SCENE )
				{
					l_menu.Append( TreeMenu_SceneDependencies, _( "Edit scene dependencies" ) );
					l_menu.Append( TreeMenu_RenameScene, _( "Rename" ) );
					l_menu.Append( TreeMenu_DeleteScene, TE_REMOVE );
				}

				if ( l_description == ITEM_DESC_FOLDER )
				{
					if ( l_type != TreeItemType::DataFolder && l_type != TreeItemType::DataFile )
					{
						l_menu.Append( TreeMenu_AddNewFile, _( "Add a new file" ) );
						l_menu.Append( TreeMenu_AddExistingFile, _( "Add an existing file" ) );
					}
					else
					{
						l_menu.Append( TreeMenu_AddExistingFile, _( "Add a folder" ) );
					}
				}

				if ( l_description == ITEM_DESC_FILE )
				{
					if ( l_type != TreeItemType::DataFolder && l_type != TreeItemType::DataFile )
					{
						l_menu.Append( TreeMenu_OpenFile, _( "Open" ) );
						l_menu.Append( TreeMenu_SaveSelectedFile, _( "Save" ) );
					}
					else
					{
						l_menu.Append( TreeMenu_ListFolder, _( "Display content" ) );
					}

					l_menu.Append( TreeMenu_Rename, _( "Rename" ) );
					l_menu.AppendSeparator();			//*****************************//
					l_menu.Append( TreeMenu_Remove, _( "Remove from the list" ) );
					l_menu.Append( TreeMenu_Delete, _( "Delete from file system" ) );

					if ( l_type != TreeItemType::DataFolder )
					{
						l_menu.AppendSeparator();			//*****************************//
						l_menu.Append( TreeMenu_Close, _( "Close" ) );
					}
				}

				if ( l_description == ITEM_DESC_ROOT )
				{
					l_menu.Append( TreeMenu_SaveProject, _( "Save project" ) );
					l_menu.Append( TreeMenu_NewScene, _( "New Scene" ) );
				}
			}

			PopupMenu( &l_menu, p_pos.x, p_pos.y );
		}

		wxTreeItemId FilesTree::GetFolderId( wxString const & p_name )
		{
			if ( m_folders.find( p_name ) != m_folders.end() )
			{
				return m_folders.find( p_name )->second;
			}

			return 0;
		}

		wxTreeItemId FilesTree::GetItemByName( wxString const & p_name )
		{
			MapTypeFile::iterator l_it = m_mapFile.find( p_name );

			if ( l_it == m_mapFile.end() )
			{
				wxTreeItemId gg = NULL;
				return gg;
			}

			return l_it->second;
		}

		void FilesTree::DoLogEvent( const wxChar * p_name, const wxTreeEvent & p_event )
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

		void FilesTree::DoListFolder()
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					auto l_itemId = GetSelected();
					File & l_file = l_scene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );
					FolderList l_list( m_projectFrame->GetProject()->GetPath() + l_file.m_scene->GetName() + wxFileName::GetPathSeparator() + l_file.FileName, this );
					l_list.ShowModal();
				}
				catch ( std::exception const & p_exc )
				{
					wxMessageBox( wxString( _( "Folder listing problem:\n" ) ) << p_exc.what(), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
				}
			}
		}

		ScenePtr FilesTree::DoGetSceneForItem( wxTreeItemId const & p_item )
		{
			auto l_it = std::find_if( m_scenes.begin()
				, m_scenes.end()
				, [&p_item]( std::pair< wxString const, TreeScene > const & p_scene )
				{
					return p_scene.second.m_sceneId == p_item
						|| p_scene.second.m_scene->HasFile( [p_item]( File const & p_file )
						{
							return p_file.ItemId == p_item;
						} );
				} );

			if ( l_it == m_scenes.end() )
			{
				GENLIB_EXCEPTION( "No Scene with this item ID." );
			}

			LogDebug( wxT( "Project::GetScene - Scene : " ) + l_it->first );
			return l_it->second.m_scene;
		}

		FileType FilesTree::DoGetFolderType( wxTreeItemId const & p_item )
		{
			FileType l_return = FileTypeCount;

			for ( auto const & l_item : m_scenes )
			{
				if ( l_return == FileTypeCount )
				{
					for ( uint32_t i = 0; i < FileTypeCount; ++i )
					{
						if ( p_item == l_item.second.m_folderIds[i] )
						{
							l_return = FileType( i );
						}
					}
				}
			}

			return l_return;
		}

		BEGIN_EVENT_TABLE( FilesTree, wxTreeCtrl )
			EVT_TREE_ITEM_MENU( FilesTreeId, FilesTree::OnFichierRClick )
			EVT_TREE_END_LABEL_EDIT( FilesTreeId, FilesTree::OnEndLabelEdit )
			EVT_TREE_BEGIN_LABEL_EDIT( FilesTreeId, FilesTree::OnBeginLabelEdit )
			EVT_TREE_ITEM_ACTIVATED( FilesTreeId, FilesTree::OnFileActivated )
			EVT_TREE_SEL_CHANGED( FilesTreeId, FilesTree::OnItemClick )
			EVT_MENU( TreeMenu_NewScene, FilesTree::OnNewScene )
			EVT_MENU( TreeMenu_SceneDependencies, FilesTree::OnSceneDependencies )
			EVT_MENU( TreeMenu_RenameScene, FilesTree::OnRenameObject )
			EVT_MENU( TreeMenu_DeleteScene, FilesTree::OnDeleteScene )
			EVT_MENU( TreeMenu_AddNewFile, FilesTree::OnAddNewFile )
			EVT_MENU( TreeMenu_AddExistingFile, FilesTree::OnAddExistingFile )
			EVT_MENU( TreeMenu_OpenFile, FilesTree::OnOpenFile )
			EVT_MENU( TreeMenu_SaveSelectedFile, FilesTree::OnSaveSelectedFile )
			EVT_MENU( TreeMenu_ListFolder, FilesTree::OnListFolder )
			EVT_MENU( TreeMenu_Rename, FilesTree::OnRenameObject )
			EVT_MENU( TreeMenu_Remove, FilesTree::OnRemoveObject )
			EVT_MENU( TreeMenu_Delete, FilesTree::OnDeleteObject )
			EVT_MENU( TreeMenu_Close, FilesTree::OnCloseObject )
			EVT_MENU( TreeMenu_SaveProject, FilesTree::OnSaveProject )
		END_EVENT_TABLE()

		void FilesTree::OnItemClick( wxTreeEvent & p_event )
		{
			m_selectedItem = p_event.GetItem();

			if ( m_projectFrame->GetProject() )
			{
				try
				{
					m_selectedScene = DoGetSceneForItem( m_selectedItem );
					std::clog << "TETreeCtrl::OnItemClic - Scene : " << m_selectedScene->GetName() << "\n";
				}
				catch ( std::exception & )
				{
					// Scene not found but not a real problem?
				}
			}
		}

		void FilesTree::OnFichierRClick( wxTreeEvent & p_event )
		{
			wxPoint l_point = p_event.GetPoint();
			m_selectedItem = p_event.GetItem();
			SelectItem( m_selectedItem );
			TreeItemData * l_item = reinterpret_cast < TreeItemData * >( GetItemData( m_selectedItem ) );
			ShowContextMenuFichier( l_point, l_item );
		}

		void FilesTree::OnBeginLabelEdit( wxTreeEvent & p_event )
		{
			m_selectedItem = p_event.GetItem();
			SelectItem( m_selectedItem );
			TreeItemData * l_item = reinterpret_cast < TreeItemData * >( GetItemData( m_selectedItem ) );
			wxString l_typeOfItem = l_item->GetDesc();

			if ( l_typeOfItem == ITEM_DESC_FILE )
			{
				File & l_file = m_selectedScene->FindFile( [this]( File & p_file )
				{
					return p_file.ItemId == m_selectedItem;
				} );
				wxString l_fullFileName = l_file.FileName;

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

				if ( m_projectFrame->GetProject() )
				{
					m_itemName = m_projectFrame->GetProject()->GetName();
				}
			}
		}

		void FilesTree::OnEndLabelEdit( wxTreeEvent & p_event )
		{
			auto && l_project = m_projectFrame->GetProject();
			TreeItemData * l_item = reinterpret_cast < TreeItemData * >( GetItemData( m_selectedItem ) );
			wxString l_typeOfItem = l_item->GetDesc();
			std::clog << p_event.GetLabel() << std::endl;

			if ( l_typeOfItem == ITEM_DESC_FILE && !p_event.GetLabel().empty() )
			{
				auto l_itemId = GetSelected();
				File & l_file = m_selectedScene->FindFile( [this]( File & p_file )
				{
					return p_file.ItemId == m_selectedItem;
				} );

				if ( l_file.Open )
				{
					m_projectFrame->UpdatePageLabel( l_file.FileName, p_event.GetLabel() );
				}

				wxString l_path = l_project->GetPath() + m_selectedScene->GetName() + wxFileName::GetPathSeparator();

				if ( l_file.Saved )
				{
					wxRenameFile( l_path + l_file.FileName, l_path + p_event.GetLabel() );
				}
				else
				{
					ProjectSaver::SaveFile( *l_project, l_file, p_event.GetLabel().c_str() );
				}

				l_file.FileName = p_event.GetLabel().c_str();
				l_file.CfgFile = std::make_unique< ConfigFile >( make_string( l_file.FileName ), m_selectedScene->GetMuseFile() );
			}
			else if ( l_typeOfItem == ITEM_DESC_SCENE )
			{
				m_selectedScene->SetName( p_event.GetLabel().c_str() );
			}
			else if ( l_typeOfItem == ITEM_DESC_ROOT )
			{
				if ( m_projectFrame->GetProject() )
				{
					m_projectFrame->GetProject()->SetName( p_event.GetLabel().c_str() );
				}
			}

			m_selectedItem = p_event.GetItem();
			SelectItem( m_selectedItem );
		}

		void FilesTree::OnFileActivated( wxTreeEvent & p_event )
		{
			auto && l_project = m_projectFrame->GetProject();

			if ( l_project )
			{
				m_selectedItem = p_event.GetItem();
				SelectItem( m_selectedItem );

				try
				{
					m_selectedScene = DoGetSceneForItem( m_selectedItem );
					TreeItemData * l_item = reinterpret_cast< TreeItemData * >( GetItemData( m_selectedItem ) );
					wxString l_typeOfItem = l_item->GetDesc();

					if ( l_typeOfItem == ITEM_DESC_FILE )
					{
						if ( l_item->GetFileType() != TreeItemType::DataFolder && l_item->GetFileType() != TreeItemType::DataFile )
						{
							auto l_itemId = l_item->GetId();
							File & l_file = m_selectedScene->FindFile( [l_itemId]( File & p_file )
							{
								return p_file.ItemId == l_itemId;
							} );
							m_projectFrame->OpenFile( *m_selectedScene, l_file );
						}
						else
						{
							DoListFolder();
						}
					}
				}
				catch ( std::exception & )
				{
					// Scene not found but not a real problem?
				}
			}
		}

		void FilesTree::OnNewScene( wxCommandEvent & p_event )
		{
			try
			{
				wxString l_sceneName = _( "New Scene" );
				auto l_project = m_projectFrame->GetProject();

				if ( l_project->AddScene( std::make_shared< Scene >( *l_project, l_sceneName ) ) )
				{
					AddSceneToProject( l_project->GetScene( l_sceneName ) );
				}
			}
			catch ( std::exception const & p_exc )
			{
				wxMessageBox( wxString( _( "New scene - Add problem:\n" ) ) << p_exc.what(), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
			}
		}

		void FilesTree::OnSceneDependencies( wxCommandEvent & p_event )
		{
			SceneDependenciesFrame l_frame{ this };
			l_frame.Show();
		}

		void FilesTree::OnDeleteScene( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				wxTreeItemId l_item = GetSelected();
				DeleteChildren( l_item );
				Delete( l_item );
				m_projectFrame->GetProject()->RemoveScene( l_scene );
			}
		}

		void FilesTree::OnAddNewFile( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				wxTreeItemId l_folderId = GetSelected();
				wxString l_str;
				FileType l_fileType = DoGetFolderType( l_folderId );
				TreeItemType l_titype;

				if ( l_fileType == sceneFile )
				{
					l_titype = TreeItemType::SceneFile;
					l_str.Printf( wxT( "NewFile%d.emscene" ), m_numSceneFile++ );
				}
				else if ( l_fileType == loadScriptFile || l_fileType == unloadScriptFile )
				{
					if ( l_fileType == loadScriptFile )
					{
						l_titype = TreeItemType::LoadScriptFile;
					}
					else
					{
						l_titype = TreeItemType::UnloadScriptFile;
					}

					l_str.Printf( wxT( "NewFile%d.emscript" ), m_numScriptFile++ );
				}
				else if ( l_fileType == dataFile || l_fileType == dataFolder )
				{
					return;
				}

				File & l_file = l_scene->AddFile( l_fileType, l_str, true, true );
				l_file.FolderId = l_folderId;
				l_file.ItemId = AddFileToFolder( l_folderId, l_str, l_titype, false );
			}
		}

		void FilesTree::OnAddExistingFile( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				auto l_projectPath = m_projectFrame->GetProject()->GetPath();
				wxTreeItemId l_folderId = GetSelected();
				wxString l_str;
				FileType l_fileType = DoGetFolderType( l_folderId );
				TreeItemType l_titype;
				wxString filename;

				if ( l_fileType == dataFolder )
				{
					l_titype = TreeItemType::DataFolder;
					wxDirDialog l_dialog( nullptr, _( "Select a directory" ), l_projectPath );

					if ( l_dialog.ShowModal() == wxID_OK )
					{
						filename = l_dialog.GetPath();
					}
				}

				if ( l_fileType == sceneFile )
				{
					l_titype = TreeItemType::SceneFile;
					filename = wxFileSelector( _( "Choose a file to open" )
						, l_projectPath
						, wxEmptyString
						, wxEmptyString
						, wxString() << _( "Elypse Scene files" ) << wxT( " (*.emscene)|*.emscene" ) );
				}
				else if ( l_fileType == loadScriptFile || l_fileType == unloadScriptFile )
				{
					if ( l_fileType == loadScriptFile )
					{
						l_titype = TreeItemType::LoadScriptFile;
					}
					else
					{
						l_titype = TreeItemType::UnloadScriptFile;
					}

					filename = wxFileSelector( _( "Choose a file to open" )
						, l_projectPath
						, wxEmptyString
						, wxEmptyString
						, wxString() << _( "Elypse Script Files" ) << wxT( " (*.emscript)|*.emscript" ) );
				}
				else if ( l_fileType == dataFile )
				{
					l_titype = TreeItemType::DataFile;
					wxDirDialog l_dialog( nullptr, _( "Choose a folder" ), l_projectPath );

					if ( l_dialog.ShowModal() == wxID_OK )
					{
						filename = l_dialog.GetPath();
					}
				}

				size_t posPoint = 0;

				if ( !filename.empty() )
				{
					posPoint = filename.find_last_of( wxFileName::GetPathSeparator() ) + 1;
					wxString text = filename.substr( posPoint, filename.size() - posPoint );
					File & l_file = l_scene->AddFile( l_fileType, l_str, true, true );
					l_file.FolderId = GetSelected();
					l_file.ItemId = AddExistingFileToProjet( l_folderId, l_titype, text );
				}
			}
		}

		void FilesTree::OnOpenFile( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					wxTreeItemId l_itemId = GetSelected();
					File & l_file = m_selectedScene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );
					m_projectFrame->OpenFile( *l_scene, l_file );
				}
				catch ( std::exception const & p_exc )
				{
					wxMessageBox( wxString( _( "File open problem:\n" ) ) << p_exc.what(), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
				}
			}
		}

		void FilesTree::OnSaveSelectedFile( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					wxTreeItemId l_itemId = GetSelected();
					File const & l_file = m_selectedScene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );

					if ( l_file.Saved )
					{
						//std::cout << "ProjectFrame::OnSaveSelectedFile" << l_file->FileName << "\n";
						l_file.EditPanel->SaveFile( m_projectFrame->GetProject()->GetPath() );
					}
					else
					{
						OnSaveSelectedFileAs( p_event );
					}
				}
				catch ( General::Utils::GenException & p_exc )
				{
					wxMessageBox( wxString( _( "File save problem:" ) ) << "\n" << p_exc.what() );
				}
			}
		}

		void FilesTree::OnSaveSelectedFileAs( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					wxTreeItemId l_itemId = GetSelected();
					File const & l_file = m_selectedScene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );
					auto l_name = ProjectSaver::SaveFileAs( *m_projectFrame->GetProject(), l_file );

					if ( !l_name.empty() )
					{
						SetItemText( l_file.ItemId, l_name );
					}
				}
				catch ( General::Utils::GenException & p_exc )
				{
					wxMessageBox( wxString( _( "File save problem:" ) ) << "\n" << p_exc.what() );
				}
			}
		}

		void FilesTree::OnListFolder( wxCommandEvent & p_event )
		{
			DoListFolder();
		}

		void FilesTree::OnRenameObject( wxCommandEvent & p_event )
		{
			EditLabel( GetSelected() );
		}

		void FilesTree::OnRemoveObject( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					auto l_sceneIt = m_scenes.find( l_scene->GetName() );
					auto l_itemId = GetSelected();
					File & l_file = l_scene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );
					m_projectFrame->CloseFile( l_file );
					l_scene->RemoveFile( l_file.Filetype, l_file.FileName );
					Delete( l_itemId );
				}
				catch ( General::Utils::GenException & p_exc )
				{
					wxMessageBox( wxString( _( "File removal problem:" ) ) << "\n" << p_exc.what() );
				}
			}
		}

		void FilesTree::OnDeleteObject( wxCommandEvent & p_event )
		{
			if ( wxMessageBox( _( "Do you really want to remove this file?" ), _( "Confirm" ), wxYES_NO | wxICON_EXCLAMATION, this ) == wxYES )
			{
				auto l_scene = GetSelectedScene();

				if ( l_scene )
				{
					try
					{
						wxTreeItemId l_itemId = GetSelected();
						File & l_file = l_scene->FindFile( [l_itemId]( File & p_file )
						{
							return p_file.ItemId == l_itemId;
						} );
						l_file.Open = false;
						l_file.EditPanel = nullptr;
						m_projectFrame->CloseFile( l_file );
						l_file.Open = false;
						l_file.EditPanel = nullptr;

						if ( !wxRemoveFile( m_projectFrame->GetProject()->GetPath() + l_file.FileName ) )
						{
							GENLIB_EXCEPTION( make_string( _( "The file could not be deleted" ) ) );
						}

						std::clog << "ProjectFrame::OnDeleteFile - OK\n";
						l_scene->RemoveFile( l_file.Filetype, l_file.FileName );
						Delete( l_itemId );
					}
					catch ( General::Utils::GenException & p_exc )
					{
						wxMessageBox( wxString( _( "File deletion problem:" ) ) << "\n" << p_exc.what() );
					}
				}
			}
		}

		void FilesTree::OnCloseObject( wxCommandEvent & p_event )
		{
			auto l_scene = GetSelectedScene();

			if ( l_scene )
			{
				try
				{
					wxTreeItemId l_itemId = GetSelected();
					File & l_file = l_scene->FindFile( [l_itemId]( File & p_file )
					{
						return p_file.ItemId == l_itemId;
					} );

					if ( l_file.Open )
					{
						m_projectFrame->CloseFile( l_file );
						l_file.Open = false;
						l_file.EditPanel = nullptr;
					}
				}
				catch ( std::exception const & p_exc )
				{
					wxMessageBox( wxString( _( "File close problem:\n" ) ) << p_exc.what(), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
				}
			}
		}

		void FilesTree::OnSaveProject( wxCommandEvent & p_event )
		{
			m_projectFrame->SaveProject();
		}
	}
}
