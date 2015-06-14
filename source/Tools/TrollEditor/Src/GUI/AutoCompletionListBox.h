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
#ifndef ___MYLISTBOX_H___
#define ___MYLISTBOX_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/listbox.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	class AutoCompletionListBox
		: public wxListBox
	{
	public:
		AutoCompletionListBox( wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size, const wxArrayString & choices );

	private:
		DECLARE_EVENT_TABLE()
		void OnMouseLeftDown( wxMouseEvent & p_event );
		void OnKeyDown( wxKeyEvent & p_event );
	};
}
END_TROLL_GUI_NAMESPACE

#endif

