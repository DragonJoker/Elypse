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

BEGIN_TROLL_GUI_NAMESPACE
{
	class ObjectsTree
		: public wxTreeCtrl
	{
	public:
		ObjectsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
		~ObjectsTree();

		void Cleanup();

		void InitObjectList();
		void AddSceneNode( const wxString & p_name, const wxString & p_parentName );
		TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * AddOverlay( TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * p_overlay, const wxString & p_parentName );
		void CreateImageList( int size = 16 );
		void ShowContextMenuObject( const wxPoint & pos, wxTreeItemId p_item );
		void AddSceneObject( TROLL_PROJECT_NAMESPACE::Object * p_object );
		wxTreeItemId GetFolderId( const wxString & p_name );

		inline int GetImageSize()const
		{
			return m_imageSize;
		}

	protected:
		inline bool _isTestItem( const wxTreeItemId & item )const
		{
			return GetItemParent( item ) == GetRootItem() && !GetPrevSibling( item );
		}

	private:
		void _logEvent( const wxChar * name, const wxTreeEvent & p_event );
		void _add3DObject( TROLL_PROJECT_3D_NAMESPACE::Troll3DObject * p_object );
		void _addTemporalObject( TROLL_PROJECT_TEMPORAL_NAMESPACE::TemporalObject * p_object );
		void _addMediaObject( TROLL_PROJECT_MEDIA_NAMESPACE::MediaObject * p_object );

		DECLARE_EVENT_TABLE()
		void OnObjectLClic( wxTreeEvent & p_event );
		void OnObjectRClic( wxTreeEvent & p_event );

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
	};
}
END_TROLL_GUI_NAMESPACE

#endif

