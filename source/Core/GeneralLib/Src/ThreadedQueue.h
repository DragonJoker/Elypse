/*
See LICENSE file in root folder
*/
#ifndef ___THREAD_SAFE_QUEUE_H___
#define ___THREAD_SAFE_QUEUE_H___

#include "Mutex.h"
#include "Condition.h"
#include "NonCopyable.h"

namespace General
{
	namespace MultiThreading
	{
		template< typename T >
		class ThreadedQueue
			: d_noncopyable
		{
		public:
			ThreadedQueue( uint16_t p_maxSize )
				: m_maxSize{ p_maxSize }
				, m_items{ p_maxSize }
			{
			}

			~ThreadedQueue()
			{
			}

			inline bool IsEmpty()const
			{
				return ( m_count == 0 );
			}

			inline bool IsFull()const
			{
				return ( m_count == m_maxSize );
			}

			inline uint16_t size()const
			{
				return m_count;
			}

			T const & Pop()
			{
				auto l_lock = make_unique_lock( m_mutex );

				if ( IsEmpty() )
				{
					m_condition.wait( l_lock );
				}

				uint32_t l_returnIndex{ m_first };
				m_first = ( m_first + 1 ) % m_maxSize;
				m_count --;

				if ( m_count == ( m_maxSize - 1 ) )
				{
					m_condition.notify_one();
				}

				return m_items[l_returnIndex];
			}

			void Push( T const & p_item )
			{
				auto l_lock = make_unique_lock( m_mutex );

				if ( IsFull() )
				{
					m_condition.wait( l_lock );
				}

				m_count++;
				m_items[m_next] = p_item;
				m_next = ( m_next + 1 ) % m_maxSize;

				if ( m_count == 1 )
				{
					m_condition.notify_one();
				}
			}

		private:
			std::vector< T > m_items;
			uint16_t m_maxSize{ 0 };
			uint16_t m_next{ 0 };
			uint16_t m_first{ 0 };
			uint16_t m_count{ 0 };
			std::mutex m_mutex;
			std::condition_variable m_condition;
		};
	}
}

#endif
