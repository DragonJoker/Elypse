/*
See LICENSE file in root folder
*/
#ifndef ___Troll_ObjectsTree___
#define ___Troll_ObjectsTree___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "TreeItemData.h"
#include "Project/Object.h"
#include "Project/3D/Module_3D.h"
#include "Project/2D/Module_2D.h"
#include "Project/Temporal/Module_Temporal.h"
#include "Project/Media/Module_Media.h"

#include <wx/treectrl.h>

namespace Troll
{
	namespace GUI
	{
		class ObjectsTree
			: public wxTreeCtrl
		{
		public:
			ObjectsTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style );
			~ObjectsTree();

			void Cleanup();

			void InitObjectList();
			void AddSceneNode( wxString const & p_name, wxString const & p_parentName );
			ProjectComponents::Objects2D::TrollOverlay * AddOverlay( ProjectComponents::Objects2D::TrollOverlay * p_overlay, wxString const & p_parentName );
			void CreateImageList( int size = 16 );
			void ShowContextMenuObject( wxPoint const & pos, wxTreeItemId p_item );
			void AddSceneObject( ProjectComponents::Object * p_object );
			wxTreeItemId GetFolderId( wxString const & p_name );

			inline int GetImageSize()const
			{
				return m_imageSize;
			}

		protected:
			inline bool _isTestItem( wxTreeItemId const & item )const
			{
				return GetItemParent( item ) == GetRootItem() && !GetPrevSibling( item );
			}

		private:
			void DoLogEvent( const wxChar * name, const wxTreeEvent & p_event );
			void DoAdd3DObject( ProjectComponents::Objects3D::Troll3DObject * p_object );
			void DoAddTemporalObject( ProjectComponents::Temporal::TemporalObject * p_object );
			void DoAddMediaObject( ProjectComponents::Media::MediaObject * p_object );

			DECLARE_EVENT_TABLE()
			void OnObjectLClic( wxTreeEvent & p_event );
			void OnObjectRClic( wxTreeEvent & p_event );
			void OnSaveModifications( wxCommandEvent & p_event );

		private:
			int m_imageSize;

			wxTreeItemId m_root;
			TreeItemIdMap m_folders;
			RecursiveTreeItemObjectPtrMap m_sceneNodes;
			TreeItemObjectMap m_objects;
			TreeItemObjectMap m_lights;
			TreeItemObjectMap m_cameras;
			RecursiveTreeItemObjectPtrMap m_overlays;
			TreeItemObjectMap m_animationGroups;
			TreeItemObjectMap m_animatedObjects;
			TreeItemObjectMap m_sounds;
			TreeItemObjectMap m_videos;

			wxTreeItemId m_idParent;
			ProjectFrame * m_projectFrame;
		};
	}
}

#endif

