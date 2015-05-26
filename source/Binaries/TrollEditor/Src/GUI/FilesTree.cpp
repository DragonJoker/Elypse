//*****************************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/FilesTree.h"
#include "GUI/main.h"
#include "GUI/MainFrame.h"
#include "GUI/TextPanel.h"
#include "GUI/TextCtrl.h"
#include "Project/Scene.h"
#include "Project/Project.h"

#include "xpm/icon1.xpm"
#include "xpm/icon2.xpm"
#include "xpm/icon3.xpm"
#include "xpm/icon4.xpm"
#include "xpm/icon5.xpm"
#include "xpm/icon6.xpm"
#include "xpm/icon7.xpm"
/*
#include <map>
#include <vector>

#include <Data/ConfigFile.h>

#include <General/Memory.h>
#include <General/STLMacros.h>
#include <wx/dnd.h>
*/
//*****************************************************************************************************************
using namespace Troll;
using namespace Troll::GUI;
//*****************************************************************************************************************
BEGIN_EVENT_TABLE( FilesTree, wxTreeCtrl )
	EVT_TREE_ITEM_MENU(	TreeFichier_Ctrl,	FilesTree::_onFichierRClick )
	EVT_TREE_END_LABEL_EDIT(	TreeFichier_Ctrl,	FilesTree::_onEndLabelEdit )
	EVT_TREE_BEGIN_LABEL_EDIT(	TreeFichier_Ctrl,	FilesTree::_onBeginLabelEdit )
	EVT_TREE_ITEM_ACTIVATED(	TreeFichier_Ctrl,	FilesTree::_onFileActivated )
	EVT_TREE_SEL_CHANGED(	TreeFichier_Ctrl,	FilesTree::_onItemClic )
END_EVENT_TABLE()
//*****************************************************************************************************************
extern MainFrame	* g_mainFrame;

//DECLARE_APP( TEApplication);
//*****************************************************************************************************************

FilesTree :: FilesTree( wxWindow * parent, const wxPoint & pos,
						const wxSize & size, long style )
	:	wxTreeCtrl( parent, TreeFichier_Ctrl, pos, size, style )
{
	CreateImageList();
	m_selectedScene = NULL;
}

//*****************************************************************************************************************

FilesTree :: ~FilesTree()
{
	Cleanup();
}

//*****************************************************************************************************************

void FilesTree :: Cleanup()
{
	m_scenes.clear();
}

//*****************************************************************************************************************

void FilesTree :: CreateImageList( int p_size )
{
	if ( p_size == -1 )
	{
		SetImageList( NULL );
		return;
	}

	if ( p_size == 0 )
	{
		p_size = m_imageSize;
	}
	else
	{
		m_imageSize = p_size;
	}

	wxImageList * l_images = new wxImageList( p_size, p_size, true );
	wxBusyCursor l_wait;
	wxIcon l_icons[7];
	l_icons[0] = wxIcon( icon1_xpm );
	l_icons[1] = wxIcon( icon2_xpm );
	l_icons[2] = wxIcon( icon3_xpm );
	l_icons[3] = wxIcon( icon4_xpm );
	l_icons[4] = wxIcon( icon5_xpm );
	l_icons[5] = wxIcon( icon6_xpm );
	l_icons[6] = wxIcon( icon7_xpm );
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

//*****************************************************************************************************************

void FilesTree :: InitProjet( const wxString & p_projectName )
{
//	g_mainFrame->LogDebugMessage( "TETreeCtrl :: InitProjet" + p_projectName);
//    int l_image = wxGetApp().ShowImages() ? FilesTreeIcon_Folder : -1;
	int l_image = FilesTreeIcon_Folder;
	m_rootProjetId = AddRoot( p_projectName, l_image, l_image, new TETreeItemData( wxT( "root" ), TITProjectRoot ) );

	if ( l_image != -1 )
	{
		SetItemImage( m_rootProjetId, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: AddSceneToProject( TrollScene * p_scene )
{
	TreeItemMap::iterator l_it = m_scenes.find( p_scene->GetName() );

	if ( l_it != m_scenes.end() )
	{
		wxMessageBox( wxT( "Impossible d'ajouter cette scène car elle existe déjà" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		throw false;
	}

	int l_image =  FilesTreeIcon_Folder;
	int l_imageSel = l_image + 1;
	wxTreeItemId l_id = AppendItem( m_rootProjetId, p_scene->GetName(), l_image, l_imageSel,
									new TETreeItemData( wxT( "scene" ), TITSceneRoot ) );
	m_idParent = l_id;
	SceneItem l_sceneItem;
	l_sceneItem.Trollscene = p_scene;
	l_sceneItem.TreeItem = l_id;
	m_scenes.insert( TreeItemMap::value_type( p_scene->GetName(), l_sceneItem ) );
	Expand( m_rootProjetId );

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	Collapse( l_id );
	return l_id;
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: AddFolderToScene( const wxString & p_sceneName, const wxString & p_folderName )
{
	TreeItemMap::iterator l_it = m_scenes.find( p_sceneName );

	if ( l_it == m_scenes.end() )
	{
		wxMessageBox( wxT( "Impossible d'ajouter de dossier à cette scène car elle n'existe pas" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_ERROR, this );
		throw false;
	}

	int l_image =  FilesTreeIcon_Folder;
	int l_imageSel = l_image + 1;
	TETreeItemType l_titype = TITSceneFile;

	if ( p_folderName == wxT( "Fichiers de script de load" ) )
	{
		l_titype = TITLoadScriptFile;
	}
	else if ( p_folderName == wxT( "Fichiers de script d'unload" ) )
	{
		l_titype = TITUnloadScriptFile;
	}
	else if ( p_folderName == wxT( "Datas Graphiques (3D et 2D)" ) )
	{
		l_titype = TITDataFile;
	}
	else if ( p_folderName == wxT( "Datas non Graphiques (autres)" ) )
	{
		l_titype = TITDataFolder;
	}

	wxTreeItemId l_id = AppendItem( l_it->second.TreeItem, p_folderName, l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), l_titype ) );
	m_folders.insert( std::make_pair( p_folderName, l_id ) );
	m_idParent = l_id;
	Collapse( l_it->second.TreeItem );

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, FilesTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	return l_id;
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: AddFileToFolder( const wxTreeItemId & p_item, const wxString & p_idFile,
		TETreeItemType p_type, bool p_exists )
{
	int l_image = FilesTreeIcon_File;
	int l_imageSel = l_image + 1;
	size_t l_index = min( p_idFile.find_last_of( wxT( '\\' ) ), p_idFile.find_last_of( wxT( '/' ) ) );
	wxString l_filename = p_idFile.substr( l_index + 1 );
	wxTreeItemId l_id = AppendItem( p_item, l_filename, l_image, l_imageSel, new TETreeItemData( wxT( "file" ), p_type ) );
	m_idParent = l_id;
//	RefreshItem( m_rootProjetId);
	Expand( p_item );

	if ( ! p_exists )
	{
		wxString l_path;
		Project * l_project = g_mainFrame->GetCurrentProject();

		if ( l_project != NULL )
		{
			l_path = l_project->GetPath();

			if ( m_selectedScene != NULL && ! m_selectedScene->IsMainScene() )
			{
				l_path += m_selectedScene->GetName() + wxT( "/" );
			}
		}

		fclose( fopen( ( l_path + p_idFile ).char_str().data(), "w" ) );
	}

	m_mapFile.insert( MapTypeFile::value_type( p_idFile , l_id ) );
	return l_id;
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: AddExistingFileToProjet( const wxTreeItemId & p_itemParent, TETreeItemType p_type,
		const wxString p_idFile )
{
	int l_image = FilesTreeIcon_File;
	int l_imageSel = l_image + 1;
	wxTreeItemId l_id = AppendItem( p_itemParent, p_idFile, l_image, l_imageSel, new TETreeItemData( wxT( "file" ), p_type ) );
	m_idParent = l_id;
//	RefreshItem( m_rootProjetId);
	Expand( p_itemParent );
	return l_id;
}

//*****************************************************************************************************************

void FilesTree :: _logEvent( const wxChar * p_name, const wxTreeEvent & p_event )
{
	wxTreeItemId l_item = p_event.GetItem();
	wxString l_text;

	if ( l_item.IsOk() )
	{
		l_text << wxT( '"' ) << GetItemText( l_item ).c_str() << wxT( '"' );
	}
	else
	{
		l_text = wxT( "invalid item" );
	}

	g_mainFrame->LogDebugMessage( wxString( p_name ) + wxT( "(" ) + l_text + wxT( ")" ) );
}

//*****************************************************************************************************************

void FilesTree :: ShowContextMenuFichier( const wxPoint & p_pos, TETreeItemData * p_item )
{
	wxMenu l_menu;

	if ( p_item == NULL )
	{
		l_menu.Append( TreeNewScene, wxT( "Ajouter une Scène" ) );
	}
	else
	{
		wxString l_description = p_item->GetDesc();

		if ( l_description == wxT( "scene" ) )
		{
			l_menu.Append( EditSceneDependencies, wxT( "Editer Dépendances" ) );
			l_menu.Append( RenameScene, wxT( "Renommer" ) );
			l_menu.Append( DeleteScene, wxT( "Supprimer" ) );
		}

		if ( l_description == wxT( "folder" ) )
		{
			FileType l_type = m_selectedScene->GetType( m_selectedItem );

			if ( l_type != dataFolder && l_type != dataFile )
			{
				l_menu.Append( Add, wxT( "Ajouter Nouveau Fichier" ) );
				l_menu.Append( IdAddExistingFile,  wxT( "Ajouter Fichier Existant" ) );
			}
			else
			{
				l_menu.Append( IdAddExistingFile,  wxT( "Ajouter Dossier" ) );
			}
		}

		if ( l_description == wxT( "file" ) )
		{
			if ( p_item->GetFileType() != TITDataFolder && p_item->GetFileType() != TITDataFile )
			{
				l_menu.Append( IdOuvrirFichier,  wxT( "Ouvrir" ) );
				l_menu.Append( IdOnSaveSelectedFile,  wxT( "Sauver Fichier" ) );
			}
			else
			{
				l_menu.Append( IdListFolder, wxT( "Lister" ) );
			}

			l_menu.Append( Rename,  wxT( "Renommer" ) );
			l_menu.AppendSeparator();			//*****************************//
			l_menu.Append( IdRemove,  wxT( "Enlever de la liste" ) );
			l_menu.Append( IdDelete, wxT( "Supprimer définitivement" ) );

			if ( p_item->GetFileType() != TITDataFolder )
			{
				l_menu.AppendSeparator();			//*****************************//
				l_menu.Append( IdFermer, wxT( "Fermer" ) );
			}
		}

		if ( l_description == wxT( "root" ) )
		{
			l_menu.Append( OpenProject, wxT( "Ouvrir Projet" ) );
			l_menu.Append( saveProject, wxT( "Sauver Projet..." ) );
			l_menu.Append( NewScene, wxT( "Nouvelle Scène" ) );
		}
	}

	PopupMenu( & l_menu, p_pos.x, p_pos.y );
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: GetFolderId( const wxString & p_name )
{
	if ( m_folders.find( p_name ) != m_folders.end() )
	{
		return m_folders.find( p_name )->second;
	}

	return 0;
}

//*****************************************************************************************************************

wxTreeItemId FilesTree :: GetItemByName( const wxString & p_name )
{
	MapTypeFile::iterator l_it = m_mapFile.find( p_name );

	if ( l_it == m_mapFile.end() )
	{
		wxTreeItemId gg = NULL;
		return gg;
	}

	return l_it->second;
}

//*****************************************************************************************************************

void FilesTree :: _onFichierRClick( wxTreeEvent & p_event )
{
	wxPoint l_point = p_event.GetPoint();
	m_selectedItem = p_event.GetItem();
	SelectItem( m_selectedItem );
	TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
	ShowContextMenuFichier( l_point, l_item );
}

//*****************************************************************************************************************

void FilesTree :: _onBeginLabelEdit( wxTreeEvent & p_event )
{
	m_selectedItem = p_event.GetItem();
	SelectItem( m_selectedItem );
	TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
	wxString l_typeOfItem = l_item->GetDesc();

	if ( l_typeOfItem == wxT( "file" ) )
	{
		wxString l_fullFileName = m_selectedScene->GetFile( m_selectedItem )->FileName;

		if ( l_fullFileName.Find( wxFileName::GetPathSeparator() ) == wxNOT_FOUND )
		{
			//itemName = l_fullFileName;
			//itemPath.clear();
			//std::cout << "renommage de : " << l_fullFileName << std::endl;
			size_t l_index = min( l_fullFileName.find_last_of( '\\' ), l_fullFileName.find_last_of( '/' ) );
			m_itemName = l_fullFileName;//.substr( l_index + 1, l_fullFileName.size());
			//itemPath = l_fullFileName.substr( 0, l_index + 1);
		}
		else
		{
			//std::cout << "renomage de : " << l_fullFileName << std::endl;
			size_t l_index = min( l_fullFileName.find_last_of( '\\' ), l_fullFileName.find_last_of( '/' ) );
			m_itemName = l_fullFileName;//.substr( l_index + 1, l_fullFileName.size());
			//itemPath = l_fullFileName.substr( 0, l_index + 1);
		}
	}
	else if ( l_typeOfItem == wxT( "scene" ) )
	{
		m_itemPath.clear();
		m_itemName = m_selectedScene->GetName();
	}
	else if ( l_typeOfItem == wxT( "root" ) )
	{
		m_itemPath.clear();

		if ( g_mainFrame->GetCurrentProject() )
		{
			m_itemName = g_mainFrame->GetCurrentProject()->GetName();
		}
	}
}

//*****************************************************************************************************************

void FilesTree :: _onEndLabelEdit( wxTreeEvent & p_event )
{
	Project * l_project = g_mainFrame->GetCurrentProject();
	TETreeItemData * l_item = reinterpret_cast <TETreeItemData *>( GetItemData( m_selectedItem ) );
	wxString l_typeOfItem = l_item->GetDesc();
	g_mainFrame->LogDebugMessage( p_event.GetLabel().c_str() );

	if ( l_typeOfItem == wxT( "file" ) && ! p_event.GetLabel().empty() )
	{
		TrollFile * l_file = m_selectedScene->GetFile( m_selectedItem );

//		std::cout << g_mainFrame->m_mainTabsContainer->GetPageCount() << std::endl;
		if ( l_file->Open )
		{
			for ( size_t i = 0 ; i <= g_mainFrame->m_mainTabsContainer->GetPageCount() ; i++ )
			{
				if ( g_mainFrame->m_mainTabsContainer->GetPageText( i ) == l_file->FileName == 0 )
				{
					g_mainFrame->m_mainTabsContainer->SetPageText( i, p_event.GetLabel().c_str() );
				}
			}
		}

		wxString l_path = l_project->GetPath() + ( m_selectedScene->IsMainScene() ? wxString( wxEmptyString ) : m_selectedScene->GetName() + wxT( "/" ) );

		if ( l_file->Saved )
		{
			rename( ( l_path + l_file->FileName ).char_str().data(), ( l_path + p_event.GetLabel() ).char_str().data() );
		}
		else
		{
			g_mainFrame->SaveFile( l_file, p_event.GetLabel().c_str() );
		}

		l_file->FileName = p_event.GetLabel().c_str();
		l_file->Folder = l_file->FileName;
		l_file->CfgFile = new ConfigFile( ( l_file->FileName.char_str().data() ), m_selectedScene->GetMuseFile() );
	}
	else if ( l_typeOfItem == wxT( "scene" ) )
	{
		m_selectedScene->SetName( p_event.GetLabel().c_str() );
	}
	else if ( l_typeOfItem == wxT( "root" ) )
	{
		if ( g_mainFrame->GetCurrentProject() )
		{
			g_mainFrame->GetCurrentProject()->SetName( p_event.GetLabel().c_str() );
		}
	}

	m_selectedItem = p_event.GetItem();
	SelectItem( m_selectedItem );
}

//*****************************************************************************************************************

void FilesTree :: _onFileActivated( wxTreeEvent & p_event )
{
	Project * l_project = g_mainFrame->GetCurrentProject();

	if ( l_project )
	{
		/*
				TrollScene * l_scene = m_selectedScene;
				if( !l_scene )
				{
					return;
				}
		*/
		m_selectedItem = p_event.GetItem();
		SelectItem( m_selectedItem );
		m_selectedScene = g_mainFrame->GetCurrentProject()->GetScene( m_selectedItem );
		TETreeItemData * l_item = reinterpret_cast< TETreeItemData * >( GetItemData( m_selectedItem ) );
		wxString l_typeOfItem = l_item->GetDesc();

		if ( l_typeOfItem == wxT( "file" ) )
		{
			if ( l_item->GetFileType() != TITDataFolder && l_item->GetFileType() != TITDataFile )
			{
				g_mainFrame->OpenSelectedFile();
			}
			else
			{
				g_mainFrame->ListFolder();
			}
		}
	}
}

//*****************************************************************************************************************

void FilesTree :: _onItemClic( wxTreeEvent & p_event )
{
	m_selectedItem = p_event.GetItem();

	if ( g_mainFrame->GetCurrentProject() != NULL )
	{
		m_selectedScene = g_mainFrame->GetCurrentProject()->GetScene( m_selectedItem );

		if ( m_selectedScene != NULL )
		{
			std::cout << "TETreeCtrl :: _onItemClic - TrollScene : " << m_selectedScene->GetName() << "\n";
		}
	}
}

//*****************************************************************************************************************
