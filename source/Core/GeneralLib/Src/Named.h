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
#ifndef ___GENERAL_NAMED_ANY_H___
#define ___GENERAL_NAMED_ANY_H___

#include <string>

#include "Assertions.h"

namespace General
{
	namespace Theory
	{
		template< typename T = std::string >
		class namedBase
		{
		public:
			namedBase( const T & p_name )
				: m_name( p_name )
			{
				genlib_assert( ! m_name.empty() );
			}
			~namedBase() {}

		public:
			inline const T & GetName()const
			{
				return m_name;
			}

		protected:
			T m_name;
		};

		typedef namedBase< std::string > named;
		typedef namedBase< std::wstring > wnamed;
	}
}

#endif
