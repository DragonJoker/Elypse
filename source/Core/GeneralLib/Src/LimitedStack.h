#ifndef ___LIMITED_STACK_H___
#define ___LIMITED_STACK_H___

#include <deque>

/**
 * A limited stack is like a normal stack, except limited to a certain number of elements.
 * A push_back will erase the head if no space remains.
**/

namespace General
{
	namespace Templates
	{
		template <typename T>
		class LimitedStack
		{
		private:
			std::deque <T *> m_stack;
			unsigned int m_count;
			unsigned int m_max;

		public:
			LimitedStack( unsigned int p_max = 10 )
				:	m_count( 0 ),
					m_max( p_max )
			{}
			~LimitedStack() {}

		public:
			inline void Push( T * p_value )
			{
				m_stack.push_back( p_value );
				m_count ++;

				if ( m_count > m_max )
				{
					m_count = m_max;
					m_stack.pop_front();
				}
			}

			inline T * Pop()
			{
				T * l_ret = NULL;

				if ( m_count != 0 )
				{
					m_count --;
					l_ret = m_stack.back();
					m_stack.pop_back();
				}

				return l_ret;
			}

			inline unsigned int Size()const
			{
				return m_count;
			}
			inline void clear()
			{
				m_stack.clear();
			}
			inline bool empty()
			{
				return ( m_count == 0 );
			}
			inline void SetMax( unsigned int p_newMax )
			{
				m_max = p_newMax;

				while ( p_newMax < m_count )
				{
					m_stack.pop_front();
					m_count --;
				}
			}
		};
	}
}

#endif
