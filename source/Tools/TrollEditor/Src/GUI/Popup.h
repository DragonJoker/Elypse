/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef _Popup_H
#define _Popup_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/popupwin.h>
#include <wx/spinctrl.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	class SimpleTransientPopup
		: public wxPopupTransientWindow
	{
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
END_TROLL_GUI_NAMESPACE
#endif
