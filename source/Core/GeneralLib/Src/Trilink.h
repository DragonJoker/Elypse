#ifndef ___TRI_LINK_H___
#define ___TRI_LINK_H___

#include "Link.h"

namespace General
{
	namespace Theory
	{
		template <typename TypeLeft, typename TypeCenter, typename TypeRight>
		class TriLink
		{
		private:
			TypeLeft * m_left;
			TypeCenter * m_center;
			TypeRight * m_right;
			Link <TypeLeft, TypeCenter>		m_antiRight;
			Link <TypeCenter, TypeRight>	m_antiLeft;
			Link <TypeRight, TypeLeft>		m_antiCenter;

		private:
			TriLink( const TriLink & ) {}
			const TriLink & operator =( const TriLink & ) {}

		public:
			inline TriLink(	TypeLeft * p_left,
							TypeCenter * p_center, TypeRight * p_right )
				:	m_left( p_left ),
					m_right( p_right ),
					m_center( p_center ),
					m_antiRight( p_left, p_center ),
					m_antiCenter( p_left, p_right ),
					m_antiLeft( p_center, p_right )
			{}
			inline ~TriLink() {}

		public:
			inline const Link <TypeCenter, TypeRight> * Get( TypeLeft * p_left )const
			{
				return & m_antiLeft;
			}
			inline const Link <TypeLeft, TypeCenter> * Get( TypeRight * p_right )const
			{
				return & m_antiRight;
			}
			inline const Link <TypeLeft, TypeRight> * Get( TypeCenter * p_center )const
			{
				return & m_antiCenter;
			}
		};

		template <typename T>
		class TriLink <T, T, T>
		{
		private:
			T * m_left;
			T * m_right;
			T * m_center;
			Link <T, T> m_antiLeft;
			Link <T, T> m_antiRight;
			Link <T, T> m_antiCenter;

		public:
			inline TriLink(	T * p_left,
							T * p_center, T * p_right )
				:	m_left( p_left ),
					m_right( p_right ),
					m_center( p_center ),
					m_antiRight( p_left, p_center ),
					m_antiCenter( p_left, p_right ),
					m_antiLeft( p_center, p_right )
			{}
			inline ~TriLink() {}

		public:
			inline const Link <T, T> & GetAntiLeft()const
			{
				return m_antiLeft;
			}
			inline const Link <T, T> & GetAntiRight()const
			{
				return m_antiRight;
			}
			inline const Link <T, T> & GetAntiCenter()const
			{
				return m_antiCenter;
			}
			inline T * GetLeft()const
			{
				return m_left;
			}
			inline T * GetRight()const
			{
				return m_right;
			}
			inline T * GetCenter()const
			{
				return m_center;
			}
			inline const Link <T, T> * Get( T * p_anti )const
			{
				if ( p_anti == m_left )
				{
					return & m_antiLeft;
				}

				if ( p_anti == m_center )
				{
					return & m_antiCenter;
				}

				return & m_antiRight;
			}
		};
	}
}

#endif
