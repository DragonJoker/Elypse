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
#include "TrollEditorPch.h"

#include "ObjectProperties.h"

BEGIN_TROLL_GUI_PROPERTIES_NAMESPACE
{
	ObjectProperties::ObjectProperties( wxWindow * parent, const wxPoint & pos, const wxSize & size, long style )
		: wxPanel( parent, wxID_ANY, pos, size, style )
		, m_updatingProperties( false )
	{
	}

	ObjectProperties::~ObjectProperties()
	{
	}
}
END_TROLL_GUI_PROPERTIES_NAMESPACE
