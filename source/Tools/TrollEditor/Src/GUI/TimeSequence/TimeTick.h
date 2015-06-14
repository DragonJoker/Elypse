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
#ifndef ___Troll_TimeTick___
#define ___Troll_TimeTick___

#include "Module_Time.h"

#include <wx/panel.h>

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	class TimeTick
		: public wxPanel
	{
	public:
		TimeTick( wxWindow * p_parent, wxWindowID p_id = wxID_ANY, const wxPoint & p_position = wxDefaultPosition, const wxSize & p_size = wxDefaultSize );

	private:
		void OnLeftMouseDown( wxMouseEvent & p_event );
		void OnLeftMouseUp( wxMouseEvent & p_event );

		DECLARE_EVENT_TABLE()

	protected:
		TimePanel * m_panel;
	};
}
END_TROLL_GUI_TIME_NAMESPACE

#endif
