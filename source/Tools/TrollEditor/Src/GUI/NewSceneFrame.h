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

#ifndef _NewSceneFrame_H
#define _NewSceneFrame_H

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Project/Module_Project.h"

#include <wx/dialog.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	enum NewSceneFrameIDs
	{
		ID_clBtn,
		ID_okBtn,
		ID_snETxt,
	};

	class NewSceneFrame : public wxDialog
	{
	private:
		// GUI controls
		wxTextCtrl * m_sceneName;
		wxButton * m_okBtn;
		wxButton * m_cancelBtn;

	public:
		NewSceneFrame( wxWindow * p_parent, const wxString & title = wxT( "Nouvelle Scène" ), const wxPoint & pos = wxDefaultPosition );
		~NewSceneFrame();

		wxString GetSceneName();

	private:
		void _onSceneChange( wxCommandEvent & p_event );
		void _onOK( wxCommandEvent & p_event );
		void _onCancel( wxCommandEvent & p_event );

		DECLARE_EVENT_TABLE()
	};
}
END_TROLL_GUI_NAMESPACE

#endif

