#ifndef ___PASCAL_H___
#define ___PASCAL_H___

#include "Factorial.h"

namespace General
{
	namespace Math
	{
		template <unsigned int tp_class, typename T_floatingType = float>
		class Pascal
		{
		private:
			T_floatingType * m_values;
			RuntimeFactorial <tp_class, T_floatingType> m_factorials;

		public:
			Pascal()
			{
				m_values = new T_floatingType[tp_class + 1];
				T_floatingType n = m_factorials.Get( tp_class );

				for ( unsigned int i = 0 ; i < tp_class + 1; i ++ )
				{
					m_values[i] = n / ( m_factorials.Get( i ) * m_factorials.Get( tp_class - i ) );
				}
			}
			~Pascal()
			{
				delete [] m_values;
			}
		public:
			inline T_floatingType Get( unsigned int p_num )
			{
				return m_values[p_num];
			}
		};
	}
}

#endif
