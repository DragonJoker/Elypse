
#ifndef ___EntityProperties___
#define ___EntityProperties___

#include "../../Project/3D/Module_3D.h"
#include "ObjectProperties.h"

namespace Troll
{
	namespace GUI
	{
		class EntityProperties : public ObjectProperties
		{
		public:
			Objects3D::TrollEntity * m_entity;
			wxTextCtrl * m_entityName;
			wxComboBox * m_parentNode;
			wxComboBox * m_meshName;
			wxListCtrl * m_submatValues;

			EntityProperties( Objects3D::TrollEntity * p_entity, wxWindow * parent,
							  const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize,
							  long style = wxBORDER_NONE );
			~EntityProperties();

			virtual void UpdateProperties();

		private:
			void _onEditName( wxCommandEvent & p_event );
			void _onSelectParent( wxCommandEvent & p_event );
			void _onSelectMesh( wxCommandEvent & p_event );
			void _onEndEditLabel( wxListEvent & p_event );

		public:
			DECLARE_EVENT_TABLE()
		};
	}
}

#endif

