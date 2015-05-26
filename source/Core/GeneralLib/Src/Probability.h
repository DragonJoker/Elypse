#ifndef ___PROBABILITY_H___
#define ___PROBABILITY_H___

namespace General
{
	namespace Math
	{
		class Probability
		{
		private:
			float m_proba;

		public:
			inline Probability()
				: m_proba( 0.0f )
			{}
			inline Probability( float p_proba )
				: m_proba( p_proba )
			{}
			inline Probability( const Probability & p_proba )
				: m_proba( p_proba.m_proba )
			{}
			inline Probability( bool p_bool )
			{
				m_proba = ( p_bool ? 1.0f : 0.0f );
			}
			inline ~Probability() {}

		public:

			inline float GetValue()const
			{
				return m_proba;
			}
			inline bool operator <( const Probability & p_proba )const
			{
				return ( m_proba < p_proba.m_proba );
			}
			inline bool operator >( const Probability & p_proba )const
			{
				return ( m_proba > p_proba.m_proba );
			}
			inline bool operator <=( const Probability & p_proba )const
			{
				return ( m_proba <= p_proba.m_proba );
			}
			inline bool operator >=( const Probability & p_proba )const
			{
				return ( m_proba >= p_proba.m_proba );
			}
			inline bool operator ==( const Probability & p_proba )const
			{
				return ( m_proba == p_proba.m_proba );
			}
			inline bool operator !=( const Probability & p_proba )const
			{
				return ( m_proba != p_proba.m_proba );
			}

			inline const Probability & operator =( const Probability & p_proba )
			{
				m_proba = p_proba.m_proba;
				return ( * this );
			}

			inline const Probability & operator =( float p_proba )
			{
				m_proba = p_proba;
				return ( * this );
			}

			inline const Probability & operator =( bool p_bool )
			{
				m_proba = ( p_bool ? 1.0f : 0.0f );
				return ( * this );
			}

			inline Probability operator ||( const Probability & p_proba )const
			{
				return Probability( p_proba.m_proba + m_proba - p_proba.m_proba * m_proba );
			}

			inline Probability operator &&( const Probability & p_proba )const
			{
				return Probability( p_proba.m_proba * m_proba );
			}

			inline Probability operator !()const
			{
				return Probability( 1.0f - m_proba );
			}
		};
	}
}

#endif
