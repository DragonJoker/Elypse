/*
See LICENSE file in root folder
*/
#ifndef ___SLOTTED_OBJECT_POOL_H___
#define ___SLOTTED_OBJECT_POOL_H___

namespace General
{
	namespace Templates
	{
		template< typename T >
		class SlottedObjectPool
		{
		public:
			SlottedObjectPool()
			{
			}

			~SlottedObjectPool()
			{
				for ( uint32_t i = 0 ; i < m_numPools ; i ++ )
				{
					for ( uint32_t j = 0 ; j < m_poolSizes[i] ; j ++ )
					{
						delete m_pools[i][j];
					}

					delete [] m_pools[i];
				}

				delete [] m_poolSizes;
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


			T * Get()
			{
				if ( m_numAvailableSlots )
				{
					return m_availableSlots[--m_numAvailableSlots];
				}

				DoubleSize();
				return Get();
			}

			void Release( T * p_ptr )
			{
				if ( p_ptr )
				{
					m_availableSlots[m_numAvailableSlots++] = p_ptr;
				}
			}

			void Allocate( uint32_t p_numObjects )
			{
				m_numObjects += p_numObjects;
				m_numPools++;
				m_pools = static_cast< T ** * >( realloc( m_pools, m_numPools * sizeof( T ** ) ) );
				m_poolSizes = static_cast< uint32_t * >( realloc( m_poolSizes, m_numPools * sizeof( uint32_t ) ) );
				T ** l_newPool = new T * [p_numObjects];
				m_pools[m_numPools - 1] = l_newPool;
				m_poolSizes[m_numPools - 1] = p_numObjects;
				m_availableSlots = static_cast< T ** >( realloc( m_availableSlots, m_numObjects * sizeof( T * ) ) );

				for ( uint32_t i = 0 ; i < p_numObjects ; i ++ )
				{
					l_newPool[i] = new T;
					m_availableSlots[m_numAvailableSlots ++] = l_newPool[i];
				}
			}

		private:
			T ** * m_pools{ nullptr };
			T ** m_availableSlots{ nullptr };
			uint32_t * m_poolSizes{ nullptr };
			uint32_t m_numPools{ 0 };
			uint32_t m_numObjects{ 0 };
			uint32_t m_numAvailableSlots{ 0 };
		};
	}
}

#endif
