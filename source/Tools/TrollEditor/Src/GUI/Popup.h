#ifndef _Popup_H
#define _Popup_H

#include <wx/popupwin.h>
#include <wx/spinctrl.h>

namespace Troll
{
	namespace GUI
	{
		class SimpleTransientPopup
			: public wxPopupTransientWindow
		{
		private:
			wxScrolledWindow * m_panel;
			wxButton * m_button;
			wxSpinCtrl * m_spinCtrl;
			wxStaticText * m_mouseText;

		public:
			SimpleTransientPopup( wxWindow * p_parent );
			virtual ~SimpleTransientPopup();

			virtual void Popup( wxWindow * focus = NULL );
			virtual void OnDismiss();
			virtual bool ProcessLeftDown( wxMouseEvent & p_event );
			virtual bool Show( bool show = true );

		public:
			inline wxScrolledWindow * GetChild()
			{
				return m_panel;
			}

		private:
			void _onMouse( wxMouseEvent & p_event );
			void _onSize( wxSizeEvent & p_event );
			void _onSetFocus( wxFocusEvent & p_event );
			void _onKillFocus( wxFocusEvent & p_event );
			void _onButton( wxCommandEvent & p_event );
			void _onSpinCtrl( wxSpinEvent & p_event );

		private:
			DECLARE_EVENT_TABLE()
		};
	}
}
#endif
