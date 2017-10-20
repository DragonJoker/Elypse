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

namespace Troll
{
	namespace GUI
	{
		class TreeScene
		{
		public:
			~TreeScene() = default;
			TreeScene( TreeScene && ) = default;
			TreeScene & operator=( TreeScene && ) = default;
			TreeScene( TreeScene const & ) = delete;
			TreeScene & operator=( TreeScene const & ) = delete;

			TreeScene( ProjectComponents::ScenePtr p_scene )
				: m_scene{ p_scene }
			{
			}

			ProjectComponents::ScenePtr m_scene;
			std::array< wxTreeItemId, ProjectComponents::FileTypeCount > m_folderIds;
			wxTreeItemId m_sceneId;
		};
		using TreeSceneMap = std::map< wxString, TreeScene >;

		class FilesTree
			: public wxTreeCtrl
		{
		public:
			FilesTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style );
			~FilesTree();

			void Cleanup();

			void CreateImageList( int size = 16 );

			void LoadProject( ProjectComponents::Project & p_project );
			void LoadScene( ProjectComponents::ScenePtr p_scene );
			TreeScene & AddSceneToProject( ProjectComponents::ScenePtr p_scene );
			wxTreeItemId AddFolderToScene( wxString const & p_sceneName, wxString const & p_folderName );
			wxTreeItemId AddFileToFolder( wxTreeItemId const & p_item, wxString const & idFile, TreeItemType p_type, bool p_exists );
			void ShowContextMenuFichier( wxPoint const & pos, TreeItemData * p_item );
			wxTreeItemId AddExistingFileToProjet( wxTreeItemId const & item, TreeItemType p_type, const wxString idFile );

			wxTreeItemId GetFolderId( wxString const & p_name );
			wxTreeItemId GetItemByName( wxString const & p_name );

			inline int GetImageSize()const
			{
				return m_imageSize;
			}
			inline wxTreeItemId const &	GetSelected()const
			{
				return m_selectedItem;
			}
			inline ProjectComponents::ScenePtr GetSelectedScene()const
			{
				return m_selectedScene;
			}

			inline void SetSelected( wxTreeItemId const & p_item )
			{
				m_selectedItem = p_item;
			}

		private:
			void DoListFolder();
			void DoLogEvent( const wxChar * name, const wxTreeEvent & p_event );
			ProjectComponents::ScenePtr DoGetSceneForItem( wxTreeItemId const & p_item );
			ProjectComponents::FileType DoGetFolderType( wxTreeItemId const & p_item );

			DECLARE_EVENT_TABLE()
			void OnItemClick( wxTreeEvent & p_event );
			void OnFichierRClick( wxTreeEvent & p_event );
			void OnFileActivated( wxTreeEvent & p_event );
			void OnBeginLabelEdit( wxTreeEvent & p_event );
			void OnEndLabelEdit( wxTreeEvent & p_event );
			void OnNewScene( wxCommandEvent & p_event );
			void OnSceneDependencies( wxCommandEvent & p_event );
			void OnDeleteScene( wxCommandEvent & p_event );
			void OnAddNewFile( wxCommandEvent & p_event );
			void OnAddExistingFile( wxCommandEvent & p_event );
			void OnOpenFile( wxCommandEvent & p_event );
			void OnSaveSelectedFile( wxCommandEvent & p_event );
			void OnSaveSelectedFileAs( wxCommandEvent & p_event );
			void OnListFolder( wxCommandEvent & p_event );
			void OnRenameObject( wxCommandEvent & p_event );
			void OnRemoveObject( wxCommandEvent & p_event );
			void OnDeleteObject( wxCommandEvent & p_event );
			void OnCloseObject( wxCommandEvent & p_event );
			void OnSaveProject( wxCommandEvent & p_event );

		private:
			int	m_imageSize{ 0 };
			TreeSceneMap m_scenes;
			wxTreeItemId m_idParent;
			wxTreeItemId m_rootId;
			wxTreeItemId m_rootProjetId;
			wxTreeItemId m_selectedItem;
			wxString m_itemName;
			wxString m_itemPath;
			ProjectComponents::ScenePtr m_selectedScene{ nullptr };
			ProjectFrame * m_projectFrame{ nullptr };

			TreeItemIdMap m_folders;
			MapTypeFile m_mapFile;
			size_t m_numSceneFile{ 1 };
			size_t m_numScriptFile{ 1 };
			size_t m_numDataFile{ 1 };
		};
	}
}

#endif
