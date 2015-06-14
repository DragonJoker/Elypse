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
#ifndef ___ProjectProperties___
#define ___ProjectProperties___

#include "Module_Properties.h"

#include <wx/dialog.h>

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	class ProjectProperties
		: public wxDialog
	{
	public:
		ProjectProperties( TROLL_PROJECT_NAMESPACE::Project & p_project, wxWindow * parent, const wxPoint & pos = wxDefaultPosition );
		~ProjectProperties();

		void UpdateProperties();

	private:
		DECLARE_EVENT_TABLE()
		void OnSelectBackgroundType( wxCommandEvent & p_event );
		void OnBackground( wxCommandEvent & p_event );
		void OnShadowsChange( wxCommandEvent & p_event );
		void OnSelectFSAA( wxCommandEvent & p_event );
		void OnShowConsoleChange( wxCommandEvent & p_event );
		void OnShowFPSChange( wxCommandEvent & p_event );
		void OnStartupScriptChange( wxCommandEvent & p_event );
		void OnBack( wxCommandEvent & p_event );

	protected:
		TROLL_PROJECT_NAMESPACE::Project & m_project;

		wxStaticText * m_projectName;
		wxComboBox * m_backgroundType;
		wxButton * m_background;
		wxCheckBox * m_shadows;
		wxComboBox * m_fsaa;
		wxCheckBox * m_showDebugConsole;
		wxCheckBox * m_showFPS;
		wxTextCtrl * m_startupScript;
		wxButton * m_back;
	};
}
END_TROLL_GUI_PROPERTIES_NAMESPACE

#endif
