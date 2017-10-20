/*
See LICENSE file in root folder
*/
#ifndef ___TEDISPLAYABLE_H___
#define ___TEDISPLAYABLE_H___

#include "Module_Project.h"

#include <wx/treebase.h>
#include <wx/propgrid/propgrid.h>

#include "GUI/Properties/Module_Properties.h"

namespace Troll
{
	namespace ProjectComponents
	{
		class Displayable
		{
		public:
			Displayable();
			virtual ~Displayable();

			GUI::Properties::ObjectProperty * CreateProperties( wxPropertyGrid * p_grid );
			void HideProperties();

			inline wxTreeItemId	GetTreeItemId()const
			{
				return m_treeItemID;
			}

			inline void SetTreeItemId( wxTreeItemId p_id )
			{
				m_treeItemID = p_id;
			}

		private:
			virtual GUI::Properties::ObjectProperty * DoCreateProperties() = 0;

		protected:
			GUI::Properties::ObjectProperty * m_properties{ nullptr };
			wxTreeItemId m_treeItemID{ 0 };
		};
	}
}

#endif
