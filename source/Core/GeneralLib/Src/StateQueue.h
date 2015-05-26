#ifndef ___STATE_QUEUE_H___
#define ___STATE_QUEUE_H___

#include <vector>

#ifndef d_defaultStateQueueLimit
#	define d_defaultStateQueueLimit 1000
#endif

namespace General
{
	namespace Templates
	{
		template <typename T>
		class StateQueue
		{
		private:
			typedef std::vector <T> t_type;

		private:
			size_t m_limit;
			size_t m_currentIndex;
			t_type m_queue;

		public:
			StateQueue()
				:	m_limit( d_defaultStateQueueLimit ),
					m_currentIndex( 0 )
			{
			}
			~StateQueue()
			{
			}

		public:
			inline bool Empty()const
			{
				return m_queue.empty();
			}
			inline bool AtBeginning()const
			{
				return ( m_currentIndex == 0 );
			}
			inline bool AtEnd()const
			{
				return ( m_queue.size() == m_currentIndex );
			}

		public:
			void Clear()
			{
				m_queue.clear();
				m_currentIndex = 0;
			}

			void SetLimit( size_t p_limit )
			{
				m_limit = p_limit;

				if ( m_queue.size() >= p_limit )
				{
					m_queue.erase( m_queue.begin() + p_limit, m_queue.begin() + p_limit + m_queue.size() );
				}
			}

			void Add( const T & p_state )
			{
				if ( ! AtEnd() )
				{
					m_queue.erase( m_queue.begin() + m_currentIndex, m_queue.begin() + m_currentIndex + m_queue.size() );
				}

				if ( m_queue.size() == m_limit )
				{
					m_queue.erase( m_queue.begin(), m_queue.begin() + 1 );
					m_currentIndex --;
				}

				m_queue.push_back( p_state );
				m_currentIndex ++;
			}

			T * Back()
			{
				if ( ! AtBeginning() )
				{
					return & m_queue[-- m_currentIndex];
				}

				return NULL;
			}

			T * AntiBack()
			{
				if ( ! AtEnd() )
				{
					return & m_queue[++ m_currentIndex];
				}

				return NULL;
			}
		};
	}
}

#endif
