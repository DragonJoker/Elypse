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
#include "stdafx.h"

wxString make_wxString( std::string const & p_value )
{
	return make_wxString( p_value.c_str() );
}

wxString make_wxString( char const * p_value )
{
	return wxString( p_value, wxConvLibc );
}

std::string make_string( wxString const & p_value )
{
	return std::string( p_value.char_str().data() );
}

std::string make_string( wxChar const * p_value )
{
	return make_string( wxString( p_value ) );
}

