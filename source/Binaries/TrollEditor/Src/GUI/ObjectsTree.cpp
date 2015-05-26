//*****************************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/ObjectsTree.h"
#include "GUI/main.h"
#include "GUI/MainFrame.h"

#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/3D/3DObject.h"
#include "Project/Temporal/TemporalObject.h"
#include "Project/Temporal/AnimationGroup.h"
#include "Project/Temporal/AnimatedObject.h"
#include "Project/Media/MediaObject.h"
#include "Project/Media/Sound.h"
#include "Project/Media/SoundObject.h"
#include "Project/Media/Video.h"
#include "Project/Media/VideoObject.h"

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
using namespace Troll::Temporal;
using namespace Troll::Objects3D;
using namespace Troll::Objects2D;
using namespace Troll::Media;
//*****************************************************************************************************************
BEGIN_EVENT_TABLE( ObjectsTree, wxTreeCtrl )
	EVT_TREE_SEL_CHANGED(	TreeObjects_Ctrl,	ObjectsTree::_onObjectLClic )
	EVT_TREE_ITEM_MENU(	TreeObjects_Ctrl,	ObjectsTree::_onObjectRClic )
END_EVENT_TABLE()
//*****************************************************************************************************************
extern MainFrame	* g_mainFrame;
//*****************************************************************************************************************

ObjectsTree :: ObjectsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
	:	wxTreeCtrl( parent, TreeObjects_Ctrl, pos, size, style )
{
	CreateImageList();
}

//*****************************************************************************************************************

ObjectsTree :: ~ObjectsTree()
{
	Cleanup();
}

//*****************************************************************************************************************

void ObjectsTree :: Cleanup()
{
	General::Utils::map::deleteAll( m_overlays );
	General::Utils::map::deleteAll( m_sceneNodes );
}

//*****************************************************************************************************************

void ObjectsTree :: CreateImageList( int p_size )
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

void ObjectsTree :: _logEvent( const wxChar * name, const wxTreeEvent & p_event )
{
	wxTreeItemId item = p_event.GetItem();
	wxString text;

	if ( item.IsOk() )
	{
		text << _T( '"' ) << GetItemText( item ).c_str() << _T( '"' );
	}
	else
	{
		text = _T( "invalid item" );
	}

	g_mainFrame->LogDebugMessage( wxString( name ) + wxT( "(" ) + wxString( text.c_str() ) + wxT( ")" ) );
}

//*****************************************************************************************************************

void ObjectsTree :: ShowContextMenuObject( const wxPoint & pos, wxTreeItemId p_item )
{
	wxMenu menu;

	if ( p_item == m_root )
	{
		menu.Append( saveModifs, _T( "Sauvegarder Modifications" ) );
	}

	PopupMenu( & menu, pos.x, pos.y );
}

//*****************************************************************************************************************

void ObjectsTree :: InitObjectList()
{
	wxTreeItemId l_id;
	int l_image, l_imageSel;
//	l_image = (wxGetApp().ShowImages() ? ObjectsTreeIcon_Folder : -1);
	l_image = ObjectsTreeIcon_Folder;
	m_root = AddRoot( _T( "Objets" ), l_image, l_image, new TETreeItemData( _T( " Root item" ), TITObjectsRoot ) );

	if ( l_image != -1 )
	{
		SetItemImage( m_root, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_image =  FunctionsTreeIcon_Folder;
	l_imageSel = l_image + 1;
	l_id = AppendItem( m_root, wxT( "TrollScene Nodes" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITNodes ) );
	m_idParent = l_id;
	m_folders[wxT( "TrollScene Nodes" )] = l_id;
	Expand( m_root );

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Objects" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITObjects ) );
	m_idParent = l_id;
	m_folders[wxT( "Objects" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Lights" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITLights ) );
	m_idParent = l_id;
	m_folders[wxT( "Lights" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Cameras" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITCameras ) );
	m_idParent = l_id;
	m_folders[wxT( "Cameras" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Overlays" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITOverlays ) );
	m_idParent = l_id;
	m_folders[wxT( "Overlays" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Animation Groups" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITAnimationGroups ) );
	m_idParent = l_id;
	m_folders[wxT( "Animation Groups" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Sounds" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITSounds ) );
	m_idParent = l_id;
	m_folders[wxT( "Sounds" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}

	l_id = AppendItem( m_root, wxT( "Videos" ), l_image, l_imageSel, new TETreeItemData( wxT( "folder" ), TITVideos ) );
	m_idParent = l_id;
	m_folders[wxT( "Videos" )] = l_id;

	if ( l_image != -1 )
	{
		SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
	}
}

//*****************************************************************************************************************

void ObjectsTree :: AddSceneNode( const wxString & p_name, const wxString & p_parentName )
{
	wxTreeItemId l_id;
	Troll3DObject * l_node = reinterpret_cast <Troll3DObject *>( g_mainFrame->GetCurrentProject()->GetMainScene()->GetSceneNode( p_name ) );
	int l_image, l_imageSel;
	bool l_added = false;
	l_image = ObjectsTreeIcon_File;
	l_imageSel = l_image + 1;
	RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
	l_item->ItemObject.Object = l_node;

	if ( p_parentName.empty() )
	{
		l_id = AppendItem( m_folders.find( wxT( "TrollScene Nodes" ) )->second, l_node->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITNode ) );
		l_added = true;
		l_item->ItemObject.ItemID = l_id;
		m_sceneNodes[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
//		std::cout << "TreeCtrl :: AddSceneNode - TrollSceneNode " << p_name << " added in list\n";
	}
	else
	{
		RecursiveTreeItemObjectPtrMap::iterator l_it = m_sceneNodes.begin();
		RecursiveTreeItemObject * l_parentItem = NULL;

		while ( l_it != m_sceneNodes.end() && ( l_parentItem = l_it->second->RecursiveIsIn( p_parentName ) ) == NULL )
		{
			++l_it;
		}

		if ( l_parentItem )
		{
//			std::cout << "TreeCtrl :: AddSceneNode - Parent " << p_parentName << " for " << p_name << " found in list\n";
			l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_node->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITNode ) );
			l_added = true;
			l_item->ItemObject.ItemID = l_id;
			l_parentItem->Childs[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
		}

		/*
				else
				{
					std::cout << "TreeCtrl :: AddSceneNode - Parent " << p_parentName << " for " << p_name << " not found in list\n";
				}
		*/
	}

	if ( l_added )
	{
		m_idParent = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( l_id, l_imageSel, wxTreeItemIcon_Expanded );
		}

		wxRect rcRect = GetRect();
		Refresh( true, &rcRect );
		Update();
	}
	else
	{
		delete l_item;
	}
}

//*****************************************************************************************************************

TrollOverlay * ObjectsTree :: AddOverlay( TrollOverlay * p_overlay, const wxString & p_parentName )
{
	wxTreeItemId l_id;
	TrollObject * l_overlay = reinterpret_cast <TrollObject *>( p_overlay );
	int l_image, l_imageSel;
	bool l_added = false;
	l_image = ObjectsTreeIcon_File;
	l_imageSel = l_image + 1;
	RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
	l_item->ItemObject.Object = l_overlay;

	if ( p_parentName.empty() )
	{
		l_id = AppendItem( m_folders.find( wxT( "Overlays" ) )->second, l_overlay->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITOverlay ) );
		l_added = true;
		l_item->ItemObject.ItemID = l_id;
		m_overlays.insert( std::make_pair( wxTreeItemIdValue( l_id.m_pItem ), l_item ) );
//		std::cout << "TreeCtrl :: AddOverlay - Overlay " << p_name << " added in list\n";
	}
	else
	{
		RecursiveTreeItemObjectPtrMap::iterator l_it = m_overlays.begin();
		RecursiveTreeItemObject * l_parentItem = NULL;

		while ( l_it != m_overlays.end() && ( l_parentItem = l_it->second->RecursiveIsIn( p_parentName ) ) == NULL )
		{
			++l_it;
		}

		if ( l_parentItem )
		{
//			std::cout << "TreeCtrl :: AddOverlay - Parent " << p_parentName << " for " << p_name << " found in list\n";
			l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_overlay->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITOverlay ) );
			l_added = true;
			l_item->ItemObject.ItemID = l_id;
			l_parentItem->Childs.insert( std::make_pair( wxTreeItemIdValue( l_id.m_pItem ), l_item ) );
		}
	}

	if ( l_added )
	{
		m_idParent = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( l_id, l_imageSel, wxTreeItemIcon_Expanded );
		}

		wxRect rcRect = GetRect();
		Refresh( true, &rcRect );
		Update();
		return p_overlay;
	}

	return NULL;
}

//*****************************************************************************************************************

void ObjectsTree :: AddSceneObject( TrollObject * p_object )
{
//	std::cout << "TETreeCtrl :: AddSceneObject - Type : " << p_object->GetObjectType() << "\n";
	if ( p_object->GetObjectType() == tt3DObject )
	{
		Troll3DObject * l_3DObject = reinterpret_cast <Troll3DObject *>( p_object );
		_add3DObject( l_3DObject );
	}
	else if ( p_object->GetObjectType() == ttTemporalObject )
	{
		TemporalObject * l_temporalObject = reinterpret_cast <TemporalObject *>( p_object );
		_addTemporalObject( l_temporalObject );
	}
	else if ( p_object->GetObjectType() == ttMediaObject )
	{
		MediaObject * l_mediaObject = reinterpret_cast <MediaObject *>( p_object );
		_addMediaObject( l_mediaObject );
	}

	wxRect rcRect = GetRect();
	Refresh( true, &rcRect );
	Update();
}

//*****************************************************************************************************************

wxTreeItemId ObjectsTree :: GetFolderId( const wxString & p_name )
{
	if ( m_folders.find( p_name ) != m_folders.end() )
	{
		return m_folders.find( p_name )->second;
	}

	return 0;
}

//*****************************************************************************************************************

void ObjectsTree :: _onObjectLClic( wxTreeEvent & p_event )
{
	wxTreeItemId l_item = p_event.GetItem();
	TETreeItemData * l_itemData = reinterpret_cast <TETreeItemData *>( GetItemData( l_item ) );
	wxString l_itemType = l_itemData->GetDesc();

	if ( l_itemType != wxT( "folder" ) )
	{
		TreeItemObjectMap::iterator l_it;
		wxTreeItemIdValue l_itemIndex = wxTreeItemIdValue( l_item.m_pItem );
		RecursiveTreeItemObject * l_elem;
		l_it = m_objects.find( l_itemIndex );

		if ( l_it != m_objects.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Entities\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_lights.find( l_itemIndex );

		if ( l_it != m_lights.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Lights\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_cameras.find( l_itemIndex );

		if ( l_it != m_cameras.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Cameras\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_animationGroups.find( l_itemIndex );

		if ( l_it != m_animationGroups.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Animation Groups\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_animatedObjects.find( l_itemIndex );

		if ( l_it != m_animatedObjects.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Animated Objects\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_sounds.find( l_itemIndex );

		if ( l_it != m_sounds.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Sounds\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		l_it = m_videos.find( l_itemIndex );

		if ( l_it != m_videos.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Videos\n";
			g_mainFrame->ShowObjectInfos( l_it->second.Object );
			return;
		}

		RecursiveTreeItemObjectPtrMap::iterator l_it2 = m_overlays.find( l_itemIndex );

		if ( l_it2 != m_overlays.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in Overlays\n";
			g_mainFrame->ShowObjectInfos( l_it2->second->ItemObject.Object );
			return;
		}

		l_it2 = m_overlays.begin();

		while ( l_it2 != m_overlays.end() )
		{
			if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
			{
//				std::cout << "TETreeCtrl :: _onObjectClic - Found in Overlay Elements\n";
				g_mainFrame->ShowObjectInfos( l_elem->ItemObject.Object );
				return;
			}

			++l_it2;
		}

		l_it2 = m_sceneNodes.find( l_itemIndex );

		if ( l_it2 != m_sceneNodes.end() )
		{
//			std::cout << "TETreeCtrl :: _onObjectClic - Found in TrollScene Nodes\n";
			g_mainFrame->ShowObjectInfos( l_it2->second->ItemObject.Object );
			return;
		}

		l_it2 = m_sceneNodes.begin();

		while ( l_it2 != m_sceneNodes.end() )
		{
			if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
			{
//				std::cout << "TETreeCtrl :: _onObjectClic - Found in TrollScene Nodes Childs\n";
				g_mainFrame->ShowObjectInfos( l_elem->ItemObject.Object );
				return;
			}

			++l_it2;
		}
	}
}

//*****************************************************************************************************************

void ObjectsTree :: _onObjectRClic( wxTreeEvent & p_event )
{
	wxTreeItemId l_item = p_event.GetItem();
	wxPoint l_pos = p_event.GetPoint();

	if ( l_item == m_root )
	{
//		std::cout << "Root right click\n";
		ShowContextMenuObject( l_pos, l_item );
	}
}

//*****************************************************************************************************************

void ObjectsTree :: _add3DObject( Troll3DObject * p_object )
{
	wxTreeItemId l_id;
	bool l_added = false;
	TreeItemObject l_object;
	int l_image, l_imageSel;
	l_image = ObjectsTreeIcon_File;
	l_imageSel = l_image + 1;

	if ( p_object->Get3DObjectType() == ttEntity )
	{
		l_id = AppendItem( m_folders.find( wxT( "Objects" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITObject ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_objects[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
//			std::cout << "TETreeCtrl :: AddSceneObject - Added Object - ID : " << l_id << " - NbElems : " << m_objects.size() << "\n";
		l_added = true;
	}
	else if ( p_object->Get3DObjectType() == ttLight )
	{
		l_id = AppendItem( m_folders.find( wxT( "Lights" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITLight ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_lights[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
		l_added = true;
	}
	else if ( p_object->Get3DObjectType() == ttCamera )
	{
		l_id = AppendItem( m_folders.find( wxT( "Cameras" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITCamera ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_cameras[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
		l_added = true;
	}

	if ( l_added )
	{
		m_idParent = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}
	}
}

//*****************************************************************************************************************

void ObjectsTree :: _addTemporalObject( TemporalObject * p_object )
{
	wxTreeItemId l_id;
	bool l_added = false;
	TreeItemObject l_object;
	int l_image, l_imageSel;
	l_image = ObjectsTreeIcon_File;
	l_imageSel = l_image + 1;

	if ( p_object->GetTemporalObjectType() == ttAnimationGroup )
	{
		l_id = AppendItem( m_folders.find( wxT( "Animation Groups" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITAnimationGroup ) );
		TreeItemObject l_object;
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_animationGroups[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
//			std::cout << "TETreeCtrl :: AddSceneObject - Added animation group - ID : " << l_id << " - " << l_temporalObject->GetName() << " - NbElems : " << m_animationGroups.size() << "\n";
		l_added = true;
	}
	else if ( p_object->GetTemporalObjectType() == ttAnimatedObject )
	{
//			std::cout << "TETreeCtrl :: AddSceneObject - Adding " << l_temporalObject->GetName() << "\n";
		TrollAnimatedObject * l_animatedObject = reinterpret_cast <TrollAnimatedObject *>( p_object );
		wxString l_groupName = l_animatedObject->GetGroup()->GetName();
		wxTreeItemId l_parentId;
		TreeItemObjectMap::iterator l_it = m_animationGroups.begin();
		bool l_found = false;

//		std::cout << "TETreeCtrl :: AddSceneObject - Looking for group " << l_groupName << "\n";
		while ( l_it != m_animationGroups.end() && ! l_found )
		{
//			std::cout << "TETreeCtrl :: AddSceneObject - Current group : " << l_it->second.Object->GetName() << "\n";
			if ( l_it->second.Object->GetName() == l_groupName )
			{
				l_parentId = l_it->second.ItemID;
				l_found = true;
			}

			++l_it;
		}

		if ( l_found )
		{
//			std::cout << "TETreeCtrl :: AddSceneObject - Group found\n";
			l_id = AppendItem( l_parentId, l_animatedObject->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITAnimatedObject ) );
			TreeItemObject l_object;
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_animatedObjects[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
	}

	if ( l_added )
	{
		m_idParent = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}
	}
}

//*****************************************************************************************************************

void ObjectsTree :: _addMediaObject( MediaObject * p_object )
{
	if ( ! p_object )
	{
		return;
	}

	wxTreeItemId l_id;
	bool l_added = false;
	TreeItemObject l_object;
	int l_image, l_imageSel;
	l_image = ObjectsTreeIcon_File;
	l_imageSel = l_image + 1;

	if ( p_object->GetMediaObjectType() == ttSound )
	{
		l_id = AppendItem( m_folders.find( wxT( "Sounds" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITSound ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
		l_added = true;
	}
	else if ( p_object->GetMediaObjectType() == ttSoundObject )
	{
		TrollSoundObject * l_soundObject = reinterpret_cast <TrollSoundObject *>( p_object );

		if ( ! l_soundObject->GetSound() )
		{
			return;
		}

		wxString l_soundName = l_soundObject->GetSound()->GetName();
		wxTreeItemId l_parentId;
		TreeItemObjectMap::iterator l_it = m_sounds.begin();
		bool l_found = false;

//		std::cout << "TETreeCtrl :: AddSceneObject - Looking for parent node " << l_soundName << "\n";
		while ( l_it != m_sounds.end() && ! l_found )
		{
//			std::cout << "TETreeCtrl :: AddSceneObject - Current node : " << l_it->second.Object->GetName() << "\n";
			if ( l_it->second.Object->GetName() == l_soundName )
			{
				l_parentId = l_it->second.ItemID;
				l_found = true;
			}

			++l_it;
		}

		if ( l_found )
		{
//			std::cout << "TETreeCtrl :: AddSceneObject - Parent node found\n";
			l_id = AppendItem( l_parentId, l_soundObject->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITSoundObject ) );
			TreeItemObject l_object;
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
	}
	else if ( p_object->GetMediaObjectType() == ttSoundPlaylist )
	{
		l_id = AppendItem( m_folders.find( wxT( "Sounds" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITSoundPlaylist ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
		l_added = true;
	}
	else if ( p_object->GetMediaObjectType() == ttVideo )
	{
		l_id = AppendItem( m_folders.find( wxT( "Videos" ) )->second, p_object->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITVideo ) );
		l_object.ItemID = l_id;
		l_object.Object = p_object;
		m_videos[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
		l_added = true;
	}
	else if ( p_object->GetMediaObjectType() == ttVideoObject )
	{
		TrollVideoObject * l_videoObject = reinterpret_cast <TrollVideoObject *>( p_object );

		if ( ! l_videoObject->GetVideo() )
		{
			return;
		}

		wxString l_videoName = l_videoObject->GetVideo()->GetName();
		wxTreeItemId l_parentId;
		TreeItemObjectMap::iterator l_it = m_videos.begin();
		bool l_found = false;
		std::cout << "TETreeCtrl :: AddVideoObject - Looking for parent node " << l_videoName << "\n";

		while ( l_it != m_videos.end() && ! l_found )
		{
			std::cout << "TETreeCtrl :: AddVideoObject - Current node : " << l_it->second.Object->GetName() << "\n";

			if ( l_it->second.Object->GetName() == l_videoName )
			{
				l_parentId = l_it->second.ItemID;
				l_found = true;
			}

			++l_it;
		}

		if ( l_found )
		{
//			std::cout << "TETreeCtrl :: AddSceneObject - Parent node found\n";
			l_id = AppendItem( l_parentId, l_videoObject->GetName(), l_image, l_imageSel, new TETreeItemData( wxT( "file" ), TITVideoObject ) );
			TreeItemObject l_object;
			l_object.ItemID = l_id;
			l_object.Object = p_object;
			m_videos[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
			l_added = true;
		}
	}

	if ( l_added )
	{
		m_idParent = l_id;

		if ( l_image != -1 )
		{
			SetItemImage( l_id, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
		}
	}
}

//*****************************************************************************************************************
