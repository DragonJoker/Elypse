//***********************************************************************************************************
#ifndef ___Troll_ObjectsTree___
#define ___Troll_ObjectsTree___
//***********************************************************************************************************
#include "TreeItemData.h"
#include "Module_GUI.h"
#include "../Project/Object.h"
#include "../Project/3D/Module_3D.h"
#include "../Project/2D/Module_2D.h"
#include "../Project/Temporal/Module_Temporal.h"
#include "../Project/Media/Module_Media.h"

#include <wx/treectrl.h>
//***********************************************************************************************************
namespace Troll
{
	namespace GUI
	{
		enum ObjectsTreeIDs
		{
			ObjectsTreeIcon_File			= 0,
			ObjectsTreeIcon_FileSelected	= 1,
			ObjectsTreeIcon_Folder			= 2,
			ObjectsTreeIcon_FolderSelected	= 3,
			ObjectsTreeIcon_FolderOpened	= 4,
			ObjectsTreeIcon_ClassFunction	= 5,

			TreeObjects_Ctrl = 1002,
		};

		class ObjectsTree : public wxTreeCtrl
		{
		private:
			int				m_imageSize;

			wxTreeItemId					m_root;
			TreeItemIdMap					m_folders;
			RecursiveTreeItemObjectPtrMap	m_sceneNodes;
			TreeItemObjectMap				m_objects;
			TreeItemObjectMap				m_lights;
			TreeItemObjectMap				m_cameras;
			RecursiveTreeItemObjectPtrMap	m_overlays;
			TreeItemObjectMap				m_animationGroups;
			TreeItemObjectMap				m_animatedObjects;
			TreeItemObjectMap				m_sounds;
			TreeItemObjectMap				m_videos;

			wxTreeItemId m_idParent;

		public:
			ObjectsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
			~ObjectsTree();

			void Cleanup();

			void InitObjectList();

			void AddSceneNode( const wxString & p_name, const wxString & p_parentName );
			Objects2D::TrollOverlay * AddOverlay( Objects2D::TrollOverlay * p_overlay, const wxString & p_parentName );

			void CreateImageList( int size = 16 );

			void ShowContextMenuObject( const wxPoint & pos, wxTreeItemId p_item );

			void AddSceneObject( TrollObject * p_object );
			wxTreeItemId GetFolderId( const wxString & p_name );

		private:
			void _logEvent( const wxChar * name, const wxTreeEvent & p_event );
			void _add3DObject( Objects3D::Troll3DObject * p_object );
			void _addTemporalObject( Temporal::TemporalObject * p_object );
			void _addMediaObject( Media::MediaObject * p_object );

		private:
			void _onObjectLClic( wxTreeEvent & p_event );
			void _onObjectRClic( wxTreeEvent & p_event );


		protected:
			inline bool _isTestItem( const wxTreeItemId & item )const
			{
				return GetItemParent( item ) == GetRootItem() && !GetPrevSibling( item );
			}

		public:
			inline int					GetImageSize()const
			{
				return m_imageSize;
			}

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
