
#ifndef ___Troll_NewOverlayFrame___
#define ___Troll_NewOverlayFrame___

#include "Module_GUI.h"
#include "../Project/2D/Module_2D.h"
#include <wx/frame.h>


namespace Troll
{
	namespace GUI
	{
		class NewOverlayFrame : public wxFrame
		{
		private:
			Objects2D::TrollOverlay * m_overlay;
			EMOverlay * m_parentOverlay;
			EMOverlayGroup * m_parentGroup;
			EMGui * m_gui;
			wxPanel * m_propertiesPanel;
			bool m_mustBeClosed;
			wxString m_overlayName;

		public:
			NewOverlayFrame( const wxPoint & p_position, EMOverlay * p_parentOverlay, const wxString & p_name );

			inline bool MustBeClosed()const
			{
				return m_mustBeClosed;
			}

		private:
			void _getOverlayTemplates( wxArrayString & p_array );

			void _onSelectTemplate( wxCommandEvent & p_event );
			void _onOK( wxCommandEvent & p_event );
			void _onCancel( wxCommandEvent & p_event );

			DECLARE_EVENT_TABLE()
		};
	}
}


#endif

