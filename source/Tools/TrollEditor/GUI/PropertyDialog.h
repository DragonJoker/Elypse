/*
See LICENSE file in root folder
*/

#ifndef ___TROLL_GUI_PROPERTY_DIALOG_H___
#define ___TROLL_GUI_PROPERTY_DIALOG_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Properties/Module_Properties.h"

#include <wx/dialog.h>

namespace Troll
{
	namespace GUI
	{
		class PropertyDialog
			: public wxDialog
		{
		public:
			PropertyDialog( wxWindow * p_parent, wxString const & p_title, Properties::ObjectProperty * p_props, wxPoint const & p_position = wxDefaultPosition, wxSize const & p_size = wxDefaultSize );
			~PropertyDialog();

		private:
			DECLARE_EVENT_TABLE()
			void OnOK( wxCommandEvent & p_event );
			void OnCancel( wxCommandEvent & p_event );

		private:
			Properties::ObjectProperty * m_props{ nullptr };
			Properties::PropertiesHolder * m_holder{ nullptr };
		};
	}
}

#endif
