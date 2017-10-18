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
#ifndef ___FACTORIAL_H___
#define ___FACTORIAL_H___

#include <array>

namespace General
{
	namespace Math
	{
		template< uint32_t p_class, typename T_floatingType = float >
		class Factorial
		{
		public:
			Factorial()
			{
				m_values[0] = 1.0;

				for ( uint32_t i = 1 ; i < ( p_class + 1 ) ; i++ )
				{
					m_values[i] = i * m_values[i - 1];
				}
			}

			inline T_floatingType Get( uint32_t p_value )const
			{
				return m_values[p_value];
			}

		private:
			std::array < T_floatingType, p_class + 1 > m_values;
		};
	}
}

#endif
