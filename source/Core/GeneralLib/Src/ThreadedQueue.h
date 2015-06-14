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
		private:
			std::vector< T > m_items;
			unsigned short m_maxSize;
			unsigned short m_next;
			unsigned short m_first;
			unsigned short m_count;
			Mutex m_mutex;
			Condition m_condition;

		public:
			ThreadedQueue( unsigned int p_maxSize )
				: m_maxSize( p_maxSize )
				, m_next( 0 )
				, m_first( 0 )
				, m_count( 0 )
				, m_items( p_maxSize )
			{
			}
			~ThreadedQueue()
			{
			}

		public:
			inline bool IsEmpty()const
			{
				return ( m_count == 0 );
			}
			inline bool IsFull()const
			{
				return ( m_count == m_maxSize );
			}

		public:
			const T & Pop()
			{
				GENLIB_AUTO_SCOPED_LOCK();

				if ( IsEmpty() )
				{
					GENLIB_CONDITION_WAIT( m_condition, m_mutex );
				}

				unsigned int l_returnIndex = m_first;
				m_first = ( m_first + 1 ) % m_maxSize;
				m_count --;

				if ( m_count == ( m_maxSize - 1 ) )
				{
					GENLIB_CONDITION_NOTIFY_ONE( m_condition );
				}

				return m_items[l_returnIndex];
			}

			void Push( const T & p_item )
			{
				GENLIB_AUTO_SCOPED_LOCK();

				if ( IsFull() )
				{
					GENLIB_CONDITION_WAIT( m_condition, m_mutex );
				}

				m_count ++;
				m_items[m_next] = p_item;
				m_next = ( m_next + 1 ) % m_maxSize;

				if ( m_count == 1 )
				{
					GENLIB_CONDITION_NOTIFY_ONE( m_condition );
				}
			}

			inline unsigned short size()const
			{
				return m_count;
			}
		};
	}
}

#endif
