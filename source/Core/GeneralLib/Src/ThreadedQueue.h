/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
