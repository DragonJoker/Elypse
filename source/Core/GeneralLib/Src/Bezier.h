#ifndef ___BEZIER_H___
#define ___BEZIER_H___

#include "AutoSingleton.h"
#include "Pascal.h"
#include "MinMax.h"

namespace General
{
	namespace Math
	{
		template <unsigned int tp_class, unsigned int tp_numSamples = 10000, typename T_floatingType = float>
		class Bezier : public AutoSingleton <Bezier <tp_class, tp_numSamples, T_floatingType> >
		{
		private:
			T_floatingType * m_samples;
			Pascal < tp_class - 1 > m_pascal;

		public:
			Bezier()
			{
				m_samples = new T_floatingType[ tp_class * ( tp_numSamples + 1 ) ];
				size_t k = 0;

				for ( size_t i = 0 ; i < tp_numSamples ; i ++ )
				{
					T_floatingType l_f = i / static_cast <T_floatingType>( tp_numSamples );
					T_floatingType l_omf = static_cast <T_floatingType>( 1.0 ) - l_f;
					T_floatingType l_temp = static_cast <T_floatingType>( 1.0 );

					for ( unsigned int j = 0 ; j < tp_class - 1 ; j ++ )
					{
						l_temp *= l_omf;
					}

					T_floatingType l_temp2 = l_f / l_omf;

					for ( unsigned int j = 0 ; j < tp_class ; j ++ )
					{
						m_samples[k ++] = m_pascal.Get( j ) * l_temp;
						l_temp *= l_temp2;
					}
				}

				for ( unsigned int j = 0 ; j < tp_class ; j ++ )
				{
					m_samples[k ++] = static_cast <T_floatingType>( 0.0 );
				}

				m_samples[-- k] = static_cast <T_floatingType>( 1.0 );
			}

			~Bezier()
			{
				delete [] m_samples;
			}

		public:
			inline T_floatingType * Get( T_floatingType p_ratio )const
			{
				p_ratio = minmax <T_floatingType> ( 0.0, p_ratio, 1.0 );
				return & m_samples[ static_cast <unsigned int> ( p_ratio * tp_numSamples ) * tp_class ];
			}
		};
	}
}

#endif
