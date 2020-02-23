/*
See LICENSE file in root folder
*/
#ifndef ___GL_CircularQueue_H___
#define ___GL_CircularQueue_H___

namespace General
{
	namespace Templates
	{
		template< typename T, uint32_t MaxElements = 100 >
		class CircularQueue
		{
		public:
			CircularQueue()
				: m_head{ 0 }
				, m_tail{ 0 }
			{
			}

		public:
			inline uint32_t size()const
			{
				return MaxElements;
			}
			inline bool empty()const
			{
				return m_head == m_tail;
			}

			inline bool push_back( T const & p_element )
			{
				uint32_t l_head = m_head;

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
				uint32_t l_tail = m_tail;

				if ( l_tail == m_head )
				{
					return T{};
				}

				uint32_t l_index = l_tail;
				l_tail ++;

				if ( l_tail == size() )
				{
					l_tail = 0;
				}

				m_tail = l_tail;
				return m_elements[l_index];
			}

		private:
			std::array< T, MaxElements > m_elements;
			uint32_t m_head;
			uint32_t m_tail;
		};
	}
}

#endif
