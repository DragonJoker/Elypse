#ifndef ___LINK_2_H___
#define ___LINK_2_H___

namespace General
{
	namespace Theory
	{
		template <typename TypeLeft, typename TypeRight>
		class Link
		{
		private:
			TypeLeft * m_left;
			TypeRight * m_right;

		public:
			explicit Link( TypeLeft * p_left, TypeRight * p_right )
				:	m_left( p_left ),
					m_right( p_right )
			{}
			explicit Link( TypeRight * p_right, TypeLeft * p_left )
				:	m_left( p_left ),
					m_right( p_right )
			{}
			Link()
				:	m_left( 0 ),
					m_right( 0 )
			{}
			~Link() {}

		private:
			Link( const Link & ) {}
			const Link & operator =( const Link & ) {}

		public:
			inline TypeLeft * Get( TypeRight * p_self )const
			{
				return m_left;
			}
			inline TypeRight * Get( TypeLeft * p_self )const
			{
				return m_right;
			}
			inline TypeLeft * Left()const
			{
				return m_left;
			}
			inline TypeRight * Right()const
			{
				return m_right;
			}
			inline void Set( TypeRight * p_self )
			{
				m_right = p_self;
			}
			inline void Set( TypeLeft * p_self )
			{
				m_left = p_self;
			}

			inline void SetBoth( TypeLeft * p_left, TypeRight * p_right )
			{
				m_left = p_left;
				m_right = p_right;
			}

			inline void SetBoth( TypeRight * p_right, TypeLeft * p_left )
			{
				m_left = p_left;
				m_right = p_right;
			}

			inline const Link & operator =( TypeRight * p_right )
			{
				m_right = p_right;
				return ( * this );
			}

			inline const Link & operator =( TypeLeft * p_left )
			{
				m_left = p_left;
				return ( * this );
			}
		};

		//Refinement if same types:

		template <typename T>
		class Link <T, T>
		{
		private:
			T * m_left;
			T * m_right;

		public:
			explicit Link( T * p_left, T * p_right )
				:	m_right( p_right ),
					m_left( p_left )
			{}
			Link()
				:	m_right( 0 ),
					m_left( 0 )
			{}
			~Link() {}

		private:
			Link( const Link & );
			const Link & operator =( const Link & );

		public:
			inline T * Get( T * p_this )const
			{
				return ( ( m_left == p_this ) ? m_right : p_this );
			}
			inline T * Left()const
			{
				return m_left;
			}
			inline T * Right()const
			{
				return m_right;
			}

			inline void Set( T * p_value )
			{
				if ( m_right == 0 )
				{
					m_right = p_value;
					return;
				}

				if ( m_left == 0 )
				{
					m_left = p_value;
					return;
				}

				m_right = p_value;
			}

			inline void SetBoth( T * p_right, T * p_left )
			{
				m_right = p_right;
				m_left = p_left;
			}

			inline const Link & operator =( T * p_value )
			{
				Set( p_value );
				return ( * this );
			}
		};
	}
}

#endif
