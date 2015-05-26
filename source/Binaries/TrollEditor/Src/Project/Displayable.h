//***********************************************************************************************************
#ifndef ___TEDISPLAYABLE_H___
#define ___TEDISPLAYABLE_H___
//***********************************************************************************************************
#include <wx/treebase.h>
#include "../GUI/Module_GUI.h"
//***********************************************************************************************************
class wxListCtrl;
class wxWindow;
class wxSize;
class wxListEvent;
//***********************************************************************************************************
namespace Troll
{
	class TEDisplayable
	{
	protected:
		GUI::ObjectProperties * m_propertiesPanel;
		wxTreeItemId m_treeItemID;

	public:
		TEDisplayable();
		virtual ~TEDisplayable();

		virtual int BuildPanel( wxWindow * p_parent, int p_width ) = 0;
		virtual void ShowProperties();
		virtual void HideProperties();

	public:
		inline wxTreeItemId	GetTreeItemId()const
		{
			return m_treeItemID;
		}
		inline wxPanel *	GetPanel()const
		{
			return reinterpret_cast <wxPanel *>( m_propertiesPanel );
		}

		inline void SetTreeItemId( wxTreeItemId p_id )
		{
			m_treeItemID = p_id;
		}
	};
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************