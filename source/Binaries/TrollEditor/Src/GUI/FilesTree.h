//***********************************************************************************************************
#ifndef ___Troll_FilesTree___
#define ___Troll_FilesTree___
//***********************************************************************************************************
#include "Module_GUI.h"
#include "TreeItemData.h"

#include <wx/treectrl.h>
//***********************************************************************************************************
namespace Troll
{
	class TrollScene;

	namespace GUI
	{
		enum FilesTreeIDs
		{
			FilesTreeIcon_File				= 0,
			FilesTreeIcon_FileSelected		= 1,
			FilesTreeIcon_Folder			= 2,
			FilesTreeIcon_FolderSelected	= 3,
			FilesTreeIcon_FolderOpened		= 4,
			FilesTreeIcon_ClassFunction		= 5,

			TreeFichier_Ctrl = 1001,
		};

		struct SceneItem
		{
			TrollScene * Trollscene;
			wxTreeItemId TreeItem;
		};

		class FilesTree : public wxTreeCtrl
		{
		private:
			int	m_imageSize;
			TreeItemMap m_scenes;
			wxTreeItemId m_idParent;
			wxTreeItemId m_rootId;
			wxTreeItemId m_rootProjetId;
			wxTreeItemId m_selectedItem;
			wxString m_itemName;
			wxString m_itemPath;
			TrollScene * m_selectedScene;

			TreeItemIdMap m_folders;
			MapTypeFile m_mapFile;

		public:
			FilesTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
			~FilesTree();

			void Cleanup();

			void CreateImageList( int size = 16 );

			void InitProjet( const wxString & p_projectName );
			wxTreeItemId AddSceneToProject( TrollScene * p_scene );
			wxTreeItemId AddFolderToScene( const wxString & p_sceneName, const wxString & p_folderName );
			wxTreeItemId AddFileToFolder( const wxTreeItemId & p_item, const wxString & idFile, TETreeItemType p_type, bool p_exists );
			void ShowContextMenuFichier( const wxPoint & pos, TETreeItemData * p_item );
			wxTreeItemId AddExistingFileToProjet( const wxTreeItemId & item, TETreeItemType p_type, const wxString idFile );

			wxTreeItemId GetFolderId( const wxString & p_name );
			wxTreeItemId GetItemByName( const wxString & p_name );

		private:
			void _logEvent( const wxChar * name, const wxTreeEvent & p_event );

		private:
			void _onItemClic( wxTreeEvent & p_event );
			void _onFichierRClick( wxTreeEvent & p_event );
			void _onFileActivated( wxTreeEvent & p_event );
			void _onBeginLabelEdit( wxTreeEvent & p_event );
			void _onEndLabelEdit( wxTreeEvent & p_event );


		protected:
			inline bool _isTestItem( const wxTreeItemId & item )
			{
				return GetItemParent( item ) == GetRootItem() &&  ! GetPrevSibling( item );
			}

		public:
			inline int					GetImageSize()const
			{
				return m_imageSize;
			}
			inline const wxTreeItemId &	GetSelected()const
			{
				return m_selectedItem;
			}
			inline TrollScene 	*		GetSelectedScene()const
			{
				return m_selectedScene;
			}

			inline void SetSelected( const wxTreeItemId & p_item )
			{
				m_selectedItem = p_item;
			}

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
