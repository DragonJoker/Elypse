/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/ObjectsTree.h"

#include "GUI/ProjectFrame.h"

#include "TrollEditorApplication.h"

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

namespace Troll
{
	namespace GUI
	{
		enum ObjectsTreeIDs
		{
			ObjectsTreeIcon_File = 0,
			ObjectsTreeIcon_FileSelected = 1,
			ObjectsTreeIcon_Folder = 2,
			ObjectsTreeIcon_FolderSelected = 3,
			ObjectsTreeIcon_FolderOpened = 4,
			ObjectsTreeIcon_ClassFunction = 5,

			TreeObjects_Ctrl = 2002,
			Menu_SaveModifs
		};

		ObjectsTree::ObjectsTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style )
			: wxTreeCtrl{ parent, TreeObjects_Ctrl, pos, size, style }
			, m_projectFrame{ projectFrame }
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );
			CreateImageList();
		}

		ObjectsTree::~ObjectsTree()
		{
			Cleanup();
		}

		void ObjectsTree::Cleanup()
		{
			General::Utils::map::deleteAll( m_overlays );
			General::Utils::map::deleteAll( m_sceneNodes );
		}

		void ObjectsTree::CreateImageList( int p_size )
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

		void ObjectsTree::ShowContextMenuObject( wxPoint const & pos, wxTreeItemId p_item )
		{
			wxMenu menu;

			if ( p_item == m_root )
			{
				menu.Append( Menu_SaveModifs, _( "Save Modifications" ) );
			}

			PopupMenu( &menu, pos.x, pos.y );
		}

		void ObjectsTree::InitObjectList()
		{
			wxTreeItemId l_id;
			int l_image, l_imageSel;
			//l_image = (wxGetApp().ShowImages() ? ObjectsTreeIcon_Folder : -1);
			l_image = ObjectsTreeIcon_Folder;
			m_root = AddRoot( _( "Objects" ), l_image, l_image, new TreeItemData( wxT( " Root item" ), TreeItemType::ObjectsRoot ) );

			if ( l_image != -1 )
			{
				SetItemImage( m_root, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_image = ObjectsTreeIcon_Folder;
			l_imageSel = l_image + 1;
			l_id = AppendItem( m_root, _( "Scene Nodes" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Nodes ) );
			m_idParent = l_id;
			m_folders[_( "Scene Nodes" )] = l_id;
			Expand( m_root );

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Objects" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Objects ) );
			m_idParent = l_id;
			m_folders[_( "Objects" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Lights" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Lights ) );
			m_idParent = l_id;
			m_folders[_( "Lights" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Cameras" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Cameras ) );
			m_idParent = l_id;
			m_folders[_( "Cameras" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Overlays" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Overlays ) );
			m_idParent = l_id;
			m_folders[_( "Overlays" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Animation Groups" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::AnimationGroups ) );
			m_idParent = l_id;
			m_folders[_( "Animation Groups" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Sounds" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Sounds ) );
			m_idParent = l_id;
			m_folders[_( "Sounds" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}

			l_id = AppendItem( m_root, _( "Videos" ), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FOLDER, TreeItemType::Videos ) );
			m_idParent = l_id;
			m_folders[_( "Videos" )] = l_id;

			if ( l_image != -1 )
			{
				SetItemImage( m_idParent, ObjectsTreeIcon_FolderOpened, wxTreeItemIcon_Expanded );
			}
		}

		void ObjectsTree::AddSceneNode( wxString const & p_name, wxString const & p_parentName )
		{
			wxTreeItemId l_id;
			ProjectComponents::Objects3D::Troll3DObject * l_node = reinterpret_cast< ProjectComponents::Objects3D::Troll3DObject * >( m_projectFrame->GetProject()->GetMainScene()->GetSceneNode( p_name ) );
			int l_image, l_imageSel;
			bool l_added = false;
			l_image = ObjectsTreeIcon_File;
			l_imageSel = l_image + 1;
			RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
			l_item->ItemObject.Object = l_node;

			if ( p_parentName.empty() )
			{
				l_id = AppendItem( m_folders.find( _( "Scene Nodes" ) )->second, l_node->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Node ) );
				l_added = true;
				l_item->ItemObject.ItemID = l_id;
				m_sceneNodes[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
				//std::cout << "TreeCtrl::AddSceneNode - TrollSceneNode " << p_name << " added in list\n";
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
					std::clog << "TreeCtrl::AddSceneNode - Parent " << p_parentName << " for " << p_name << " found in list\n";
					l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_node->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Node ) );
					l_added = true;
					l_item->ItemObject.ItemID = l_id;
					l_parentItem->Childs[wxTreeItemIdValue( l_id.m_pItem )] = l_item;
				}
				else
				{
					std::clog << "TreeCtrl::AddSceneNode - Parent " << p_parentName << " for " << p_name << " not found in list\n";
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
			}
			else
			{
				delete l_item;
			}
		}

		ProjectComponents::Objects2D::TrollOverlay * ObjectsTree::AddOverlay( ProjectComponents::Objects2D::TrollOverlay * p_overlay, wxString const & p_parentName )
		{
			wxTreeItemId l_id;
			ProjectComponents::Object * l_overlay = reinterpret_cast< ProjectComponents::Object * >( p_overlay );
			int l_image, l_imageSel;
			bool l_added = false;
			l_image = ObjectsTreeIcon_File;
			l_imageSel = l_image + 1;
			RecursiveTreeItemObject * l_item = new RecursiveTreeItemObject;
			l_item->ItemObject.Object = l_overlay;

			if ( p_parentName.empty() )
			{
				l_id = AppendItem( m_folders.find( _( "Overlays" ) )->second, l_overlay->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Overlay ) );
				l_added = true;
				l_item->ItemObject.ItemID = l_id;
				m_overlays.insert( std::make_pair( wxTreeItemIdValue( l_id.m_pItem ), l_item ) );
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
					l_id = AppendItem( l_parentItem->ItemObject.ItemID, l_overlay->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Overlay ) );
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

		void ObjectsTree::AddSceneObject( ProjectComponents::Object * p_object )
		{
			std::clog << "TETreeCtrl::AddSceneObject - Type : " << p_object->GetObjectType() << "\n";

			if ( p_object->GetObjectType() == ProjectComponents::tt3DObject )
			{
				ProjectComponents::Objects3D::Troll3DObject * l_3DObject = reinterpret_cast< ProjectComponents::Objects3D::Troll3DObject * >( p_object );
				DoAdd3DObject( l_3DObject );
			}
			else if ( p_object->GetObjectType() == ProjectComponents::ttTemporalObject )
			{
				ProjectComponents::Temporal::TemporalObject * l_temporalObject = reinterpret_cast< ProjectComponents::Temporal::TemporalObject * >( p_object );
				DoAddTemporalObject( l_temporalObject );
			}
			else if ( p_object->GetObjectType() == ProjectComponents::ttMediaObject )
			{
				ProjectComponents::Media::MediaObject * l_mediaObject = reinterpret_cast< ProjectComponents::Media::MediaObject * >( p_object );
				DoAddMediaObject( l_mediaObject );
			}

			wxRect rcRect = GetRect();
			Refresh( true, &rcRect );
			Update();
		}

		wxTreeItemId ObjectsTree::GetFolderId( wxString const & p_name )
		{
			if ( m_folders.find( p_name ) != m_folders.end() )
			{
				return m_folders.find( p_name )->second;
			}

			return 0;
		}


		void ObjectsTree::DoLogEvent( const wxChar * name, const wxTreeEvent & p_event )
		{
			wxTreeItemId item = p_event.GetItem();
			wxString text;

			if ( item.IsOk() )
			{
				text << wxT( '"' ) << GetItemText( item ).c_str() << wxT( '"' );
			}
			else
			{
				text = _( "Invalid item" );
			}

			LogDebug( wxString( name ) + wxT( "(" ) + wxString( text.c_str() ) + wxT( ")" ) );
		}
		void ObjectsTree::DoAdd3DObject( ProjectComponents::Objects3D::Troll3DObject * p_object )
		{
			wxTreeItemId l_id;
			bool l_added = false;
			TreeItemObject l_object;
			int l_image, l_imageSel;
			l_image = ObjectsTreeIcon_File;
			l_imageSel = l_image + 1;

			if ( p_object->Get3DObjectType() == ProjectComponents::Objects3D::ttEntity )
			{
				l_id = AppendItem( m_folders.find( _( "Objects" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Object ) );
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_objects[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				std::clog << "TETreeCtrl::AddSceneObject - Added Object - ID : " << l_id << " - NbElems : " << m_objects.size() << "\n";
				l_added = true;
			}
			else if ( p_object->Get3DObjectType() == ProjectComponents::Objects3D::ttLight )
			{
				l_id = AppendItem( m_folders.find( _( "Lights" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Light ) );
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_lights[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
			else if ( p_object->Get3DObjectType() == ProjectComponents::Objects3D::ttCamera )
			{
				l_id = AppendItem( m_folders.find( _( "Cameras" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Camera ) );
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

		void ObjectsTree::DoAddTemporalObject( ProjectComponents::Temporal::TemporalObject * p_object )
		{
			wxTreeItemId l_id;
			bool l_added = false;
			TreeItemObject l_object;
			int l_image, l_imageSel;
			l_image = ObjectsTreeIcon_File;
			l_imageSel = l_image + 1;

			if ( p_object->GetTemporalObjectType() == ProjectComponents::Temporal::ttAnimationGroup )
			{
				l_id = AppendItem( m_folders.find( _( "Animation Groups" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::AnimationGroup ) );
				TreeItemObject l_object;
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_animationGroups[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
			else if ( p_object->GetTemporalObjectType() == ProjectComponents::Temporal::ttAnimatedObject )
			{
				ProjectComponents::Temporal::TrollAnimatedObject * l_animatedObject = reinterpret_cast< ProjectComponents::Temporal::TrollAnimatedObject * >( p_object );
				wxString l_groupName = l_animatedObject->GetGroup()->GetObjectName();
				wxTreeItemId l_parentId;
				TreeItemObjectMap::iterator l_it = m_animationGroups.begin();
				bool l_found = false;

				std::clog << "TETreeCtrl::AddSceneObject - Looking for group " << l_groupName << "\n";

				while ( l_it != m_animationGroups.end() && !l_found )
				{
					std::clog << "TETreeCtrl::AddSceneObject - Current group : " << l_it->second.Object->GetObjectName() << "\n";

					if ( l_it->second.Object->GetObjectName() == l_groupName )
					{
						l_parentId = l_it->second.ItemID;
						l_found = true;
					}

					++l_it;
				}

				if ( l_found )
				{
					std::clog << "TETreeCtrl::AddSceneObject - Group found\n";
					l_id = AppendItem( l_parentId, l_animatedObject->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::AnimatedObject ) );
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

		void ObjectsTree::DoAddMediaObject( ProjectComponents::Media::MediaObject * p_object )
		{
			if ( !p_object )
			{
				return;
			}

			wxTreeItemId l_id;
			bool l_added = false;
			TreeItemObject l_object;
			int l_image, l_imageSel;
			l_image = ObjectsTreeIcon_File;
			l_imageSel = l_image + 1;

			if ( p_object->GetMediaObjectType() == ProjectComponents::Media::ttSound )
			{
				l_id = AppendItem( m_folders.find( _( "Sounds" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Sound ) );
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
			else if ( p_object->GetMediaObjectType() == ProjectComponents::Media::ttSoundObject )
			{
				ProjectComponents::Media::TrollSoundObject * l_soundObject = reinterpret_cast< ProjectComponents::Media::TrollSoundObject * >( p_object );

				if ( !l_soundObject->GetSound() )
				{
					return;
				}

				wxString l_soundName = l_soundObject->GetSound()->GetObjectName();
				wxTreeItemId l_parentId;
				TreeItemObjectMap::iterator l_it = m_sounds.begin();
				bool l_found = false;

				std::clog << "TETreeCtrl::AddSceneObject - Looking for parent node " << l_soundName << "\n";

				while ( l_it != m_sounds.end() && !l_found )
				{
					std::clog << "TETreeCtrl::AddSceneObject - Current node : " << l_it->second.Object->GetObjectName() << "\n";

					if ( l_it->second.Object->GetObjectName() == l_soundName )
					{
						l_parentId = l_it->second.ItemID;
						l_found = true;
					}

					++l_it;
				}

				if ( l_found )
				{
					std::clog << "TETreeCtrl::AddSceneObject - Parent node found\n";
					l_id = AppendItem( l_parentId, l_soundObject->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::SoundObject ) );
					TreeItemObject l_object;
					l_object.ItemID = l_id;
					l_object.Object = p_object;
					m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
					l_added = true;
				}
			}
			else if ( p_object->GetMediaObjectType() == ProjectComponents::Media::ttSoundPlaylist )
			{
				l_id = AppendItem( m_folders.find( _( "Sounds" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::SoundPlaylist ) );
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_sounds[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
			else if ( p_object->GetMediaObjectType() == ProjectComponents::Media::ttVideo )
			{
				l_id = AppendItem( m_folders.find( _( "Videos" ) )->second, p_object->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::Video ) );
				l_object.ItemID = l_id;
				l_object.Object = p_object;
				m_videos[wxTreeItemIdValue( l_id.m_pItem )] = l_object;
				l_added = true;
			}
			else if ( p_object->GetMediaObjectType() == ProjectComponents::Media::ttVideoObject )
			{
				ProjectComponents::Media::TrollVideoObject * l_videoObject = reinterpret_cast< ProjectComponents::Media::TrollVideoObject * >( p_object );

				if ( !l_videoObject->GetVideo() )
				{
					return;
				}

				wxString l_videoName = l_videoObject->GetVideo()->GetObjectName();
				wxTreeItemId l_parentId;
				TreeItemObjectMap::iterator l_it = m_videos.begin();
				bool l_found = false;
				std::clog << "TETreeCtrl::AddVideoObject - Looking for parent node " << l_videoName << "\n";

				while ( l_it != m_videos.end() && !l_found )
				{
					std::clog << "TETreeCtrl::AddVideoObject - Current node : " << l_it->second.Object->GetObjectName() << "\n";

					if ( l_it->second.Object->GetObjectName() == l_videoName )
					{
						l_parentId = l_it->second.ItemID;
						l_found = true;
					}

					++l_it;
				}

				if ( l_found )
				{
					std::clog << "TETreeCtrl::AddSceneObject - Parent node found\n";
					l_id = AppendItem( l_parentId, l_videoObject->GetObjectName(), l_image, l_imageSel, new TreeItemData( ITEM_DESC_FILE, TreeItemType::VideoObject ) );
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

		BEGIN_EVENT_TABLE( ObjectsTree, wxTreeCtrl )
			EVT_TREE_SEL_CHANGED( TreeObjects_Ctrl, ObjectsTree::OnObjectLClic )
			EVT_TREE_ITEM_MENU( TreeObjects_Ctrl, ObjectsTree::OnObjectRClic )
			EVT_MENU( Menu_SaveModifs, ObjectsTree::OnSaveModifications )
		END_EVENT_TABLE()

		void ObjectsTree::OnObjectLClic( wxTreeEvent & p_event )
		{
			wxTreeItemId l_item = p_event.GetItem();
			TreeItemData * l_itemData = reinterpret_cast < TreeItemData * >( GetItemData( l_item ) );
			wxString l_itemType = l_itemData->GetDesc();

			if ( l_itemType != ITEM_DESC_FOLDER )
			{
				TreeItemObjectMap::iterator l_it;
				wxTreeItemIdValue l_itemIndex = wxTreeItemIdValue( l_item.m_pItem );
				RecursiveTreeItemObject * l_elem;
				l_it = m_objects.find( l_itemIndex );

				if ( l_it != m_objects.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Entities\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_lights.find( l_itemIndex );

				if ( l_it != m_lights.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Lights\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_cameras.find( l_itemIndex );

				if ( l_it != m_cameras.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Cameras\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_animationGroups.find( l_itemIndex );

				if ( l_it != m_animationGroups.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Animation Groups\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_animatedObjects.find( l_itemIndex );

				if ( l_it != m_animatedObjects.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Animated Objects\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_sounds.find( l_itemIndex );

				if ( l_it != m_sounds.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Sounds\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				l_it = m_videos.find( l_itemIndex );

				if ( l_it != m_videos.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Videos\n";
					m_projectFrame->ShowObjectInfos( l_it->second.Object );
					return;
				}

				RecursiveTreeItemObjectPtrMap::iterator l_it2 = m_overlays.find( l_itemIndex );

				if ( l_it2 != m_overlays.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Overlays\n";
					m_projectFrame->ShowObjectInfos( l_it2->second->ItemObject.Object );
					return;
				}

				l_it2 = m_overlays.begin();

				while ( l_it2 != m_overlays.end() )
				{
					if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
					{
						std::clog << "TETreeCtrl::OnObjectClic - Found in Overlay Elements\n";
						m_projectFrame->ShowObjectInfos( l_elem->ItemObject.Object );
						return;
					}

					++l_it2;
				}

				l_it2 = m_sceneNodes.find( l_itemIndex );

				if ( l_it2 != m_sceneNodes.end() )
				{
					std::clog << "TETreeCtrl::OnObjectClic - Found in Scene Nodes\n";
					m_projectFrame->ShowObjectInfos( l_it2->second->ItemObject.Object );
					return;
				}

				l_it2 = m_sceneNodes.begin();

				while ( l_it2 != m_sceneNodes.end() )
				{
					if ( ( l_elem = l_it2->second->RecursiveIsIn( l_item.m_pItem ) ) != NULL )
					{
						std::clog << "TETreeCtrl::OnObjectClic - Found in Scene Nodes Childs\n";
						m_projectFrame->ShowObjectInfos( l_elem->ItemObject.Object );
						return;
					}

					++l_it2;
				}
			}
		}

		void ObjectsTree::OnObjectRClic( wxTreeEvent & p_event )
		{
			wxTreeItemId l_item = p_event.GetItem();
			wxPoint l_pos = p_event.GetPoint();

			if ( l_item == m_root )
			{
				std::clog << "Root right click\n";
				ShowContextMenuObject( l_pos, l_item );
			}
		}

		void ObjectsTree::OnSaveModifications( wxCommandEvent & p_event )
		{
		}
	}
}
