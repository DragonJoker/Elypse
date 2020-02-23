/*
See LICENSE file in root folder
*/
#ifndef _Popup_H
#define _Popup_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/popupwin.h>
#include <wx/spinctrl.h>

namespace Troll
{
	namespace GUI
	{
		class SimpleTransientPopup
			: public wxPopupTransientWindow
		{
		public:
			SimpleTransientPopup( wxWindow * p_parent );
			virtual ~SimpleTransientPopup();

			virtual void Popup( wxWindow * focus = nullptr );
			virtual void OnDismiss();
			virtual bool ProcessLeftDown( wxMouseEvent & p_event );
			virtual bool Show( bool show = true );

		public:
			inline wxScrolledWindow * GetChild()
			{
				return m_panel;
			}

		private:
			DECLARE_EVENT_TABLE()
			void OnMouse( wxMouseEvent & p_event );
			void OnSize( wxSizeEvent & p_event );
			void OnSetFocus( wxFocusEvent & p_event );
			void OnKillFocus( wxFocusEvent & p_event );
			void OnButton( wxCommandEvent & p_event );
			void OnSpinCtrl( wxSpinEvent & p_event );

		private:
			wxScrolledWindow * m_panel;
			wxButton * m_button;
			wxSpinCtrl * m_spinCtrl;
			wxStaticText * m_mouseText;
		};
	}
}

#endif
