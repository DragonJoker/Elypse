/*
See LICENSE file in root folder
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
