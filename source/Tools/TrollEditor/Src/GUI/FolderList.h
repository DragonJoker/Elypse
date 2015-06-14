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
#ifndef ___Troll_FolderList___
#define ___Troll_FolderList___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/dialog.h>

class wxListBox;

BEGIN_TROLL_GUI_NAMESPACE
{
	class FolderList
		: public wxDialog
	{
	private:
		wxListBox * m_filesList;
		wxString m_folder;

	public:
		FolderList( const wxString & p_folder, wxWindow * p_parent, const wxPoint & p_position = wxPoint( 100, 100 ) );
		~FolderList();

		void RemoveFile( const wxString & p_file );
		void AddFile( const wxString & p_file );

	private:
		void _listFolder();
		void OnQuit( wxCommandEvent & p_event );
		void OnAddFile( wxCommandEvent & p_event );
		void OnDelFile( wxCommandEvent & p_event );
		void OnKeyUp( wxKeyEvent & p_event );

	private:
		DECLARE_EVENT_TABLE()
	};
}
END_TROLL_GUI_NAMESPACE

#endif
