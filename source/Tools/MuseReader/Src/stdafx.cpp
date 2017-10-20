/*
See LICENSE file in root folder
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

