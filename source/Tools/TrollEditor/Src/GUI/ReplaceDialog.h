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

#ifndef ___Troll_ReplaceDialog___
#define ___Troll_ReplaceDialog___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/frame.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	class ReplaceDialog
		: public wxFrame
	{
	public:
		ReplaceDialog( wxWindow * p_parent, wxStcTextEditor * p_textPanel );

	private:
		DECLARE_EVENT_TABLE()
		void OnFind( wxCommandEvent & p_event );
		void OnReplace( wxCommandEvent & p_event );
		void OnReplaceAll( wxCommandEvent & p_event );

	private:
		wxStcTextEditor * m_textPanel;
		wxTextCtrl * m_textToReplace;
		wxTextCtrl * m_replacementText;
		wxButton * m_find;
		wxButton * m_replace;
		wxButton * m_replaceAll;
		bool m_first;
		wxString m_text;
	};
}
END_TROLL_GUI_NAMESPACE

#endif

