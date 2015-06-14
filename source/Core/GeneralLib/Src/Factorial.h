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

namespace General
{
	namespace Math
	{
		template <unsigned int N>
		class StaticFactorial
		{
		public:
			static const int value = N * StaticFactorial < N - 1 > ::value;
		};

		template <>
		class StaticFactorial <1>
		{
		public:
			static const int value = 1;
		};

		template <>
		class StaticFactorial <0>
		{
		public:
			static const int value = 1;
		};

		template <unsigned int p_class, typename T_floatingType = float>
		class RuntimeFactorial
		{
		private:
			T_floatingType * m_values;

		public:
			RuntimeFactorial()
			{
				m_values = new T_floatingType[p_class + 1];
				m_values[0] = 1.0;

				for ( unsigned int i = 1 ; i < ( p_class + 1 ) ; i++ )
				{
					m_values[i] = i * m_values[i - 1];
				}
			}

			~RuntimeFactorial()
			{
				delete [] m_values;
			}

		public:
			inline T_floatingType Get( unsigned int p_value )
			{
				return m_values[p_value];
			}
		};
	}
}

#endif
