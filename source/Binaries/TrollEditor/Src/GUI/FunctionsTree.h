//***********************************************************************************************************
#ifndef ___FunctionsTree___
#define ___FunctionsTree___
//***********************************************************************************************************
#include "TreeItemData.h"
#include "TextCtrl.h"

#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <wx/log.h>
#include <wx/colordlg.h>
#include <wx/numdlg.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/math.h>
#include <wx/event.h>
#include "../Project/Object.h"
#include "../Project/3D/Module_3D.h"
#include "../Project/2D/Module_2D.h"
#include "../Project/Temporal/Module_Temporal.h"
#include "../Project/Media/Module_Media.h"

#include <Module_Script.h>
//***********************************************************************************************************
namespace Troll
{
	class TrollScene;

	namespace GUI
	{
		enum FunctionsTreeIDs
		{
			FunctionsTreeIcon_File				= 0,
			FunctionsTreeIcon_FileSelected		= 1,
			FunctionsTreeIcon_Folder			= 2,
			FunctionsTreeIcon_FolderSelected	= 3,
			FunctionsTreeIcon_FolderOpened		= 4,
			FunctionsTreeIcon_ClassFunction		= 5,

			TreeFunctions_Ctrl	= 1000,
		};

		typedef std::vector <wxString> StrArray;
		typedef std::pair <wxString, wxString> StrPair;
		typedef std::vector <StrPair> StrPairArray;
		typedef std::map <wxString, StrPairArray> StrPairArrayStrMap;
		typedef std::multimap <wxString, wxString> StrStrMultiMap;
		typedef std::map <wxString, wxString> StrStrMap;
		typedef std::map <wxString, wxTreeItemId> ItemStrMap;

		class FunctionsTree : public wxTreeCtrl
		{
		private:
			int				m_imageSize;

			wxTreeItemId m_idParent;
			wxTreeItemId m_rootId;
			wxTreeItemId m_functionsId;
			wxTreeItemId m_constantsId;
			wxTreeItemId m_selectedItem;

			ItemStrMap m_functionGroupsItems;
			ItemStrMap m_constantGroupsItems;

			StrArray m_functionsGroups;
			StrArray m_constantsGroups;
			StrPairArrayStrMap m_functions;
			StrPairArrayStrMap m_classFunctions;
			StrPairArrayStrMap m_constants;
			wxString m_currentGroup;

		public:
			FunctionsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
			~FunctionsTree();

			void Cleanup();

			void CreateTitreFunction( const wxString & p_name );
			void CreaFunc( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void ClassFunc( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void CreateTitreConstant( const wxString & p_name );
			void CreateConstant( const wxString & p_constName, VariableBaseType p_type );

			void CreateFunctionsTree();
			void CreateConstantsTree();

			void CreateImageList( int size = 16 );
			void AddFunctionsToTree();

		private:
			void _buildFunctionsList( const StrPairArrayStrMap & p_list, int p_image, int p_imageSel );
			wxString _buildFunctionHelper( const wxString & p_function, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void _logEvent( const wxChar * name, const wxTreeEvent & p_event );
//		wxString _valueTypeToString( int p_valueType );

		private:
			void _onDragBegin( wxTreeEvent & p_event );
			void _onDragEnd( wxTreeEvent & p_event );
			void _onItemActivated( wxTreeEvent & p_event );
			void _onItemRClick( wxTreeEvent & p_event );
			void _onShowToolTip( wxTreeEvent & p_event );


		protected:
			inline bool _isTestItem( const wxTreeItemId & item )
			{
				return GetItemParent( item ) == GetRootItem() && !GetPrevSibling( item );
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
