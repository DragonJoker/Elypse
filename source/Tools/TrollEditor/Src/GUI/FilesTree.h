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
#ifndef ___Troll_FilesTree___
#define ___Troll_FilesTree___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/treectrl.h>

#include "Project/Module_Project.h"
#include "GUI/TreeItemData.h"

BEGIN_TROLL_GUI_NAMESPACE
{
	enum FilesTreeIDs
	{
		FilesTreeIcon_File = 0,
		FilesTreeIcon_FileSelected = 1,
		FilesTreeIcon_Folder = 2,
		FilesTreeIcon_FolderSelected = 3,
		FilesTreeIcon_FolderOpened = 4,
		FilesTreeIcon_ClassFunction = 5,

		TreeFichier_Ctrl = 1001,
	};

	struct SceneItem
	{
		TROLL_PROJECT_NAMESPACE::Scene * Trollscene;
		wxTreeItemId TreeItem;
	};

	class FilesTree : public wxTreeCtrl
	{
	public:
		FilesTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
		~FilesTree();

		void Cleanup();

		void CreateImageList( int size = 16 );

		void InitProject( const wxString & p_projectName );
		wxTreeItemId AddSceneToProject( TROLL_PROJECT_NAMESPACE::Scene * p_scene );
		wxTreeItemId AddFolderToScene( const wxString & p_sceneName, const wxString & p_folderName );
		wxTreeItemId AddFileToFolder( const wxTreeItemId & p_item, const wxString & idFile, TETreeItemType p_type, bool p_exists );
		void ShowContextMenuFichier( const wxPoint & pos, TETreeItemData * p_item );
		wxTreeItemId AddExistingFileToProjet( const wxTreeItemId & item, TETreeItemType p_type, const wxString idFile );

		wxTreeItemId GetFolderId( const wxString & p_name );
		wxTreeItemId GetItemByName( const wxString & p_name );

		inline int GetImageSize()const
		{
			return m_imageSize;
		}
		inline const wxTreeItemId &	GetSelected()const
		{
			return m_selectedItem;
		}
		inline TROLL_PROJECT_NAMESPACE::Scene * GetSelectedScene()const
		{
			return m_selectedScene;
		}

		inline void SetSelected( const wxTreeItemId & p_item )
		{
			m_selectedItem = p_item;
		}

	protected:
		inline bool _isTestItem( const wxTreeItemId & item )
		{
			return GetItemParent( item ) == GetRootItem() &&  ! GetPrevSibling( item );
		}

	private:
		void _logEvent( const wxChar * name, const wxTreeEvent & p_event );
		DECLARE_EVENT_TABLE()
		void OnItemClic( wxTreeEvent & p_event );
		void OnFichierRClick( wxTreeEvent & p_event );
		void OnFileActivated( wxTreeEvent & p_event );
		void OnBeginLabelEdit( wxTreeEvent & p_event );
		void OnEndLabelEdit( wxTreeEvent & p_event );

	private:
		int	m_imageSize;
		TreeItemMap m_scenes;
		wxTreeItemId m_idParent;
		wxTreeItemId m_rootId;
		wxTreeItemId m_rootProjetId;
		wxTreeItemId m_selectedItem;
		wxString m_itemName;
		wxString m_itemPath;
		TROLL_PROJECT_NAMESPACE::Scene * m_selectedScene;

		TreeItemIdMap m_folders;
		MapTypeFile m_mapFile;
	};
}
END_TROLL_GUI_NAMESPACE

#endif

