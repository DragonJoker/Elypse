#ifndef ___ARRAY_H___
#define ___ARRAY_H___

#ifndef NULL
#	define NULL 0
#endif

#include <cstdlib>
#include <stdexcept>

/**
 * Class useful for memory-critical sections, it replaces the standard arrays, using the vector semantics.
**/

namespace General
{
	namespace Templates
	{
		template <typename T>
		class carray
		{
		private:
			T * m_datas;
			size_t m_size;

		public:
			inline carray()
				:	m_size( 0 ),
					m_datas( NULL )
			{}

			inline carray( size_t p_size )
				:	m_size( p_size )
			{
				m_datas = new T[p_size];
			}

			inline ~carray()
			{
				delete [] m_datas;
			}

		public:
			void resize( size_t p_newSize )
			{
				m_size = p_newSize;

				if ( m_datas )
				{
					m_datas = static_cast <T *>( realloc( m_datas, p_newSize ) );
				}
				else
				{
					m_datas = new T[p_newSize];
				}
			}

			inline T & operator[]( size_t p_index )
			{
				return m_datas[p_index];
			}

			inline T & at( size_t p_index )
			{
				if ( p_index >= m_size )
				{
					throw std::out_of_range( "invalid Array <T> subscript" );
				}

				return m_datas[p_index];
			}

			inline size_t size() const
			{
				return m_size;
			}

			inline void zero()
			{
				memset( m_datas, 0, m_size * sizeof( T ) );
			}
		};
	}
}

#endif
