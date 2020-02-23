/*
See LICENSE file in root folder
*/
#ifndef ___FunctionsTree___
#define ___FunctionsTree___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/colordlg.h>
#include <wx/event.h>
#include <wx/image.h>
#include <wx/imaglist.h>
#include <wx/log.h>
#include <wx/math.h>
#include <wx/numdlg.h>
#include <wx/treebase.h>
#include <wx/treectrl.h>

#include <Module_Script.h>

#include "GUI/TreeItemData.h"

#include "Project/Object.h"
#include "Project/3D/Module_3D.h"
#include "Project/2D/Module_2D.h"
#include "Project/Temporal/Module_Temporal.h"
#include "Project/Media/Module_Media.h"

namespace Troll
{
	namespace GUI
	{
		class FunctionsTree
			: public wxTreeCtrl
		{
		public:
			FunctionsTree( wxWindow * parent, ProjectFrame * projectFrame, wxPoint const & pos, wxSize const & size, long style );
			~FunctionsTree();

			void Cleanup();
			void CreateTitreFunction( wxString const & p_name );
			void CreaFunc( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void ClassFunc( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void CreateTitreConstant( wxString const & p_name );
			void CreateConstant( wxString const & p_constName, VariableBaseType p_type );
			void CreateFunctionsTree();
			void CreateConstantsTree();
			void CreateImageList( int size = 16 );
			void AddFunctionsToTree();

			inline int GetImageSize()const
			{
				return m_imageSize;
			}
			inline wxTreeItemId const &	GetSelected()const
			{
				return m_selectedItem;
			}

			inline void SetSelected( wxTreeItemId const & p_item )
			{
				m_selectedItem = p_item;
			}

		protected:
			inline bool _isTestItem( wxTreeItemId const & item )
			{
				return GetItemParent( item ) == GetRootItem() && !GetPrevSibling( item );
			}

		private:
			void _buildFunctionsList( const StrPairArrayStrMap & p_list, int p_image, int p_imageSel );
			wxString _buildFunctionHelper( wxString const & p_function, VariableBaseType p_returnValue, std::vector <VariableBaseType> p_params );
			void _logEvent( const wxChar * name, const wxTreeEvent & p_event );

			DECLARE_EVENT_TABLE()
			void OnDragBegin( wxTreeEvent & p_event );
			void OnDragEnd( wxTreeEvent & p_event );
			void OnItemActivated( wxTreeEvent & p_event );
			void OnItemRClick( wxTreeEvent & p_event );
			void OnShowToolTip( wxTreeEvent & p_event );

		private:
			typedef std::map< wxString, wxString > wxStrStrMap;

			int m_imageSize;

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

			wxStrStrMap m_map;
			ProjectFrame * m_projectFrame;
		};
	}
}

#endif

