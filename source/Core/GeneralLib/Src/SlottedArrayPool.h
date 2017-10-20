/*
See LICENSE file in root folder
*/
#ifndef ___SLOTTED_ARRAY_POOL_H___
#define ___SLOTTED_ARRAY_POOL_H___

namespace General
{
	namespace Templates
	{
		template< typename T, uint16_t P_ArraySize >
		class SlottedArrayPool
		{
		public:
			SlottedArrayPool()
			{
			}

			~SlottedArrayPool()
			{
				for ( uint32_t i = 0 ; i < m_numObjects ; i ++ )
				{
					delete [] m_pool[i];
				}

				free( m_availableSlots );
				free( m_pool );
			}

			inline uint16_t NumAvailable()const
			{
				return m_numAvailableSlots;
			}

			inline uint16_t TotalNum()const
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
					return m_availableSlots[--m_numAvailableSlots];
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

			void Allocate( uint16_t p_numObjects )
			{
				uint16_t l_num = m_numObjects;
				m_numObjects += p_numObjects;
				m_pool = static_cast< T ** >( realloc( m_pool, m_numObjects * sizeof( T * ) ) );
				m_availableSlots = static_cast< T ** >( realloc( m_availableSlots, m_numObjects * sizeof( T * ) ) );

				for ( uint16_t i = 0 ; i < p_numObjects ; i ++ )
				{
					m_pool[i + l_num] = new T[P_ArraySize];
					m_availableSlots[m_numAvailableSlots ++] = m_pool[i + l_num];
				}
			}

		private:
			T ** m_pool{ nullptr };
			T ** m_availableSlots{ nullptr };
			uint16_t m_numObjects{ 0 };
			uint16_t m_numAvailableSlots{ 0 };
		};
	}
}

#endif
