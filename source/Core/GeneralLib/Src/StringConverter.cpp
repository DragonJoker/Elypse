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
#include "StringConverter.h"
#include "StringUtils.h"

std::string General::Utils::ConvertToString( const std::wstring & p_wideString, const char * p_locale, const char p_unknownCharReplacedBy )
{
	std::string l_return;
	l_return.resize( p_wideString.length() );
	std::locale loc( p_locale );
	std::use_facet <std::ctype <wchar_t> > ( loc ).narrow( & p_wideString[0], & p_wideString[p_wideString.length()], p_unknownCharReplacedBy, & l_return[0] );
	return l_return;
}

std::wstring General::Utils::ConvertToWString( const std::string & p_string, const char * p_locale )
{
	std::wstring l_return;
	l_return.resize( p_string.length() );
	std::locale loc( p_locale );
	std::use_facet <std::ctype <wchar_t> > ( loc ).widen( & p_string[0], & p_string[p_string.length()], & l_return[0] );
	return l_return;
}

float General::Utils::parseFloat( const std::string & val )
{
	std::istringstream str( val );
	float ret = 0;
	str >> ret;
	return ret;
}

int General::Utils::parseInt( const std::string & val )
{
	std::istringstream str( val );
	int ret = 0;
	str >> ret;
	return ret;
}

unsigned int General::Utils::parseUnsignedInt( const std::string & val )
{
	std::istringstream str( val );
	unsigned int ret = 0;
	str >> ret;
	return ret;
}

bool General::Utils::parseBool( const std::string & val )
{
	return ( string::startsWith( val, "true" ) || string::startsWith( val, "yes" )
			 || string::startsWith( val, "1" ) );
}