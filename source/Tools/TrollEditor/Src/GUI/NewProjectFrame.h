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

#ifndef _NewProjectFrame_H
#define _NewProjectFrame_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"

#include <wx/dialog.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	class NewProjectFrame
		: public wxDialog
	{
	public:
		NewProjectFrame( wxWindow * p_parent, const wxString & title = _( "New Project" ), const wxPoint & pos = wxDefaultPosition );
		~NewProjectFrame();

	private:
		DECLARE_EVENT_TABLE()
		void OnSelectBackgroundType( wxCommandEvent & p_event );
		void OnSelectBackgroundButton( wxCommandEvent & p_event );
		void OnMainSceneChange( wxCommandEvent & p_event );
		void OnShadowsChange( wxCommandEvent & p_event );
		void OnAntiAliasingChange( wxCommandEvent & p_event );
		void OnOK( wxCommandEvent & p_event );
		void OnCancel( wxCommandEvent & p_event );
		void OnBrowse( wxCommandEvent & p_event );

	private:
		bool m_shadows;
		TROLL_PROJECT_NAMESPACE::AntiAliasing m_antiAliasing;
		TROLL_PROJECT_NAMESPACE::BackgroundType m_backgroundType;
		wxString m_bgColour;
		wxString m_bgImage;
		wxString m_path;

		// GUI controls
		wxTextCtrl * m_projectName;
		wxButton * m_folderButton;
		wxTextCtrl * m_projectFolder;
		wxComboBox * m_bgRBox;
		wxButton * m_bgSelect;
		wxTextCtrl * m_mainSceneName;
		wxCheckBox * m_shCkb;
		wxComboBox * m_antiAlias;
		wxTextCtrl * m_width;
		wxTextCtrl * m_height;
		wxButton * m_okBtn;
		wxButton * m_cancelBtn;
	};
}
END_TROLL_GUI_NAMESPACE

#endif

