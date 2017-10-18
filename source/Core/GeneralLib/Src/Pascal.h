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
#ifndef ___PASCAL_H___
#define ___PASCAL_H___

#include "Factorial.h"

namespace General
{
	namespace Math
	{
		template< uint32_t tp_class, typename T_floatingType = float >
		class Pascal
		{
		public:
			Pascal()
			{
				T_floatingType n = m_factorials.Get( tp_class );

				for ( uint32_t i = 0 ; i < tp_class + 1; i ++ )
				{
					m_values[i] = n / ( m_factorials.Get( i ) * m_factorials.Get( tp_class - i ) );
				}
			}
			~Pascal()
			{
			}

			inline T_floatingType Get( uint32_t p_num )
			{
				return m_values[p_num];
			}

		private:
			std::array < T_floatingType, tp_class + 1 > m_values;
			Factorial< tp_class, T_floatingType > m_factorials;
		};
	}
}

#endif
