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
