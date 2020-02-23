/*
See LICENSE file in root folder
*/
#ifndef ___Troll_NewOverlayFrame___
#define ___Troll_NewOverlayFrame___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/2D/Module_2D.h"
#include "GUI/Properties/Module_Properties.h"

#include <EMOverlay.h>

#include <wx/frame.h>

namespace Troll
{
	namespace GUI
	{
		class NewOverlayFrame
			: public wxFrame
		{
		public:
			NewOverlayFrame( ProjectFrame * p_parent, wxPoint const & p_position, EMOverlay * p_parentOverlay, wxString const & p_name );

			inline bool MustBeClosed()const
			{
				return m_mustBeClosed;
			}

		private:
			void _getOverlayTemplates( wxArrayString & p_array );

			DECLARE_EVENT_TABLE()
			void OnSelectTemplate( wxCommandEvent & p_event );
			void OnOK( wxCommandEvent & p_event );
			void OnCancel( wxCommandEvent & p_event );

		private:
			ProjectComponents::Objects2D::TrollOverlay * m_overlay;
			EMOverlay * m_parentOverlay;
			EMOverlayGroup * m_parentGroup;
			EMGui * m_gui;
			Properties::PropertiesHolder * m_propertiesPanel;
			bool m_mustBeClosed;
			wxString m_overlayName;
			ProjectFrame * m_projectFrame;
		};
	}
}

#endif
