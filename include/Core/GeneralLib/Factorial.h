/*
See LICENSE file in root folder
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
