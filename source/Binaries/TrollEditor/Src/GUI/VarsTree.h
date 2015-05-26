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

#include <module_script.h>
//***********************************************************************************************************
namespace Troll
{
	class TEScene;

	namespace GUI
	{
		typedef std::vector <wxString> StrArray;
		typedef std::pair <wxString, wxString> StrPair;
		typedef std::vector <StrPair> StrPairArray;
		typedef std::map <wxString, StrPairArray> StrPairArrayStrMap;
		typedef std::multimap <wxString, wxString> StrStrMultiMap;
		typedef std::map <wxString, wxTreeItemId> ItemStrMap;

		class ConstantsTree : public wxTreeCtrl
		{
		private:
			int	m_imageSize;

			wxTreeItemId m_idParent;
			wxTreeItemId m_rootId;
			wxTreeItemId m_selectedItem;

			StrPairArrayStrMap m_constants;

		public:
			ConstantsTree( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style );
			~ConstantsTree();

			void Cleanup();

			void CreateTitreConstant( const wxString & idfunc );
			void CreaConst( const wxString & id, VariableType p_returnValue, std::vector <VariableType> p_params );

			void CreateFunctionsTree();
			void CreateConstantsTree();

			void CreateImageList( int size = 16 );
			void AddConstantsToTree();

		private:
			void _buildConstantsList( const StrPairArrayStrMap & p_list, int p_image, int p_imageSel );
			wxString _buildConstantHelper( const wxString & p_function, VariableType p_returnValue, std::vector <VariableType> p_params );
			void _logEvent( const wxChar * name, const wxTreeEvent & p_event );
			wxString _valueTypeToString( int p_valueType );

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
