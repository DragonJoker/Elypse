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
#ifndef ___LOG_LIST_BOX_H___
#define ___LOG_LIST_BOX_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include <wx/log.h>
#include <wx/listbox.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	// A log target which just redirects the messages to a listbox
	class wxLogListBox
		: public wxLog
	{
	public:
		wxLogListBox( wxListBox * p_list );

		virtual ~wxLogListBox();

	private:
		// implement sink functions
		virtual void DoLog( wxLogLevel p_level, const wxChar * p_line, time_t p_time );

		virtual void DoLogString( const wxChar * p_line, time_t p_time );

	private:
		// the control we use
		wxListBox * m_list;
	};
}
END_TROLL_GUI_NAMESPACE

#endif
