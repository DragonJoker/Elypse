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
#ifndef ___LOCKLESS_QUEUE_H___
#define ___LOCKLESS_QUEUE_H___

namespace General
{
	namespace Templates
	{
		template<typename T, unsigned int c_maxElements = 100>
		class lockless_queue
		{
		private:
			T m_elements[c_maxElements];
			unsigned int m_head;
			unsigned int m_tail;

		public:
			lockless_queue()
				: m_head( 0 ),
					m_tail( 0 )
			{
			}

			~lockless_queue()
			{
			}

		public:
			inline unsigned int size()const
			{
				return c_maxElements;
			}
			inline bool empty()const
			{
				return m_head == m_tail;
			}

			inline bool push_back( const T & p_element )
			{
				unsigned int l_head = m_head;

				if ( ( l_head + 1 ) % size() == m_tail )
				{
					return false;
				}

				m_elements [l_head] = p_element;
				l_head ++;

				if ( l_head == size() )
				{
					l_head = 0;
				}

				m_head = l_head;
				return true;
			}

			inline T pop_front()
			{
				unsigned int l_tail = m_tail;

				if ( l_tail == m_head )
				{
					return T();
				}

				unsigned int l_index = l_tail;
				l_tail ++;

				if ( l_tail == size() )
				{
					l_tail = 0;
				}

				m_tail = l_tail;
				return m_elements[l_index];
			}
		};
	}
}

#endif
