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
#ifndef ___SLOTTED_ARRAY_POOL_H___
#define ___SLOTTED_ARRAY_POOL_H___

namespace General
{
	namespace Templates
	{
		template <typename T, unsigned short P_ArraySize>
		class SlottedArrayPool
		{
		private:
			T ** m_pool;
			T ** m_availableSlots;
			unsigned short m_numObjects;
			unsigned short m_numAvailableSlots;

		public:
			SlottedArrayPool()
				: m_pool( NULL ),
					m_numObjects( 0 ),
					m_availableSlots( 0 ),
					m_numAvailableSlots( NULL )
			{}
			~SlottedArrayPool()
			{
				for ( unsigned int i = 0 ; i < m_numObjects ; i ++ )
				{
					delete [] m_pool[i];
				}

				free( m_availableSlots );
				free( m_pool );
			}

		public:
			inline unsigned short NumAvailable()const
			{
				return m_numAvailableSlots;
			}
			inline unsigned short TotalNum()const
			{
				return m_numObjects;
			}
			inline void DoubleSize()
			{
				Allocate( m_numObjects );
			}

			inline T * Get()
			{
				if ( m_numAvailableSlots != 0 )
				{
					return m_availableSlots [-- m_numAvailableSlots];
				}

				return 0;
			}

			inline void Release( T * p_ptr )
			{
				if ( p_ptr != 0 )
				{
					m_availableSlots[m_numAvailableSlots ++] = p_ptr;
				}
			}

		public:
			void Allocate( unsigned short p_numObjects )
			{
				unsigned short l_num;
				l_num = m_numObjects;
				m_numObjects += p_numObjects;
				m_pool = static_cast <T **>( realloc( m_pool, m_numObjects * sizeof( T * ) ) );
				m_availableSlots = static_cast <T **>( realloc( m_availableSlots, m_numObjects * sizeof( T * ) ) );

				for ( unsigned short i = 0 ; i < p_numObjects ; i ++ )
				{
					m_pool[i + l_num] = new T[P_ArraySize];
					m_availableSlots[m_numAvailableSlots ++] = m_pool[i + l_num];
				}
			}
		};
	}
}

#endif
