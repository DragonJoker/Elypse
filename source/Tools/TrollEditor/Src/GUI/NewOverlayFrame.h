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
#ifndef ___Troll_NewOverlayFrame___
#define ___Troll_NewOverlayFrame___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/frame.h>

#include "Project/2D/Module_2D.h"

BEGIN_TROLL_GUI_NAMESPACE
{
	class NewOverlayFrame
		: public wxFrame
	{
	public:
		NewOverlayFrame( wxWindow * p_parent, const wxPoint & p_position, EMOverlay * p_parentOverlay, const wxString & p_name );

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
		TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * m_overlay;
		EMOverlay * m_parentOverlay;
		EMOverlayGroup * m_parentGroup;
		EMGui * m_gui;
		wxPanel * m_propertiesPanel;
		bool m_mustBeClosed;
		wxString m_overlayName;
	};
}
END_TROLL_GUI_NAMESPACE


#endif

