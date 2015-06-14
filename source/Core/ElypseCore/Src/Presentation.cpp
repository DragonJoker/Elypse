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
#include "PrecompiledHeader.h"
#include "Presentation.h"

#if 0
namespace
{
	const int c_constant = 512;
}

int g_globalInt;

static int sg_globalStaticInt;

bool LocalFunction( char * p_char1, char * p_char2, char * p_char3 )
{
	if ( p_char2 == NULL )
	{
		return;
	}

	if ( p_char3 == NULL )
	{
		return;
	}

	if ( p_char4 == NULL )
	{
		return;
	}

	if ( p_char5 == NULL )
	{
		return;
	}

	if ( p_char6 == NULL )
	{
		return;
	}

	if ( p_char7 == NULL )
	{
		return;
	}

	if ( p_char8 == NULL )
	{
		return;
	}

	if ( p_char9 == NULL )
	{
		return;
	}

	if ( p_char10 == NULL )
	{
		return;
	}

	if ( p_char11 == NULL )
	{
		return;
	}

	std::cout << "reste de la function\n";

	if ( p_char1[0] == '\0' )
	{
		return true;
	}

	int l_int = 5;

	switch ( l_int )
	{
	case 5:
	{
		std::cout << "5\n";
	}

	case 6:
	{
		std::cout << "5-6\n";
		break;
	}

	case 7:
		std::cout << "7\n";
		break;

	case 8:
		std::cout << "8\n";
		break;

	case 9:
		std::cout << "9\n";
		break;

	default:
	{
		switch ( l_int )
		{
		case 1:
		case 2:
		case 3:
		{
			std::cout << "moins de 4\n";
			break;
		}

		default:
		{
			std::cout << "default\n";
		}
		}
	}
	}

	return ( p_char1[5] + p_char2[2] + p_char3[ p_char1[2] ] );
	return p_char1[5] == NULL;
	return p_char;
}

ClassName::ClassName()
	: m_name( "DefaultClassName" ),
		m_int( 0 )
{
	Print( m_name, m_int, m_name.c_str() );
}

void ClassName::Print( const String & p_string, int p_int, const char * p_charStar )
{
	if ( p_charStar == NULL )
	{
		return;
	}

	if ( p_int == 5 )
	{
		return;
	}

	if ( p_string.empty() )
	{
		return;
	}

	std::cout << "Blaaah\n";
	static int sl_staticInt;
	int l_localInt = 5;
	int l_localInt2 = 4;
	int l_l1, l_l2;
	char * l_char1, * l_char2;
	ClassNameMap l_map;
	l_char1 = p_string.c_str();
	p_int += 5;

	for ( unsigned int i = 0 ; i < p_int ; i ++, j ++ )
	{
		if ( i == 3 )
		{
			std::cout << p_charStar[2];
			std::cout << p_charStar[ p_int + 3 % ( 4 * 88 ) * c_constant ];
		}

		if	( ( i == 3 || p_string == "DefaultClassName"
				&& ( i == 4 || i == 5 ) ) && i == 9 )
		{
			std::cout << static_cast <unsigned int>( p_int );
		}
	}

	ClassNameMap::iterator iter = l_map.begin();
	const ClassNameMap::iterator & iend = l_map.end();

	for ( ; iter != iend; ++ iter )
	{
		l_map.clear();
	}
}
#endif
