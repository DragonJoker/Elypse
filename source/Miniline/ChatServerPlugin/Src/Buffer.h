#ifndef ___GENLIB_BUFFER_H___
#define ___GENLIB_BUFFER_H___

#include <string>

/**
 * Generic buffer packers / unpackers.
 *
 * usage:
 *
 * WriteBuffer l_buffer;
 * l_buffer.CreateBuffer(50);
 * l_buffer << 5;
 * l_buffer << 5.0f;
 * l_buffer << "meow !";
 * l_buffer.c_str(); // you can only do it once, it gives the buffer, then discards it, so you are responsible for its destruction.
**/

namespace General
{
	namespace Templates
	{
		class WriteBuffer
		{
		private:
			char * m_buffer;
			char * m_currentIndex;

			unsigned int m_capacity;
			unsigned int m_size;

		public:
			WriteBuffer()
				:	m_buffer( NULL ),
					m_currentIndex( NULL ),
					m_capacity( 0 ),
					m_size( 0 )
			{
			}
			WriteBuffer( char * p_buffer, unsigned int p_length )
				:	m_buffer( p_buffer ),
					m_currentIndex( p_buffer ),
					m_capacity( p_length ),
					m_size( 0 )
			{
				memset( m_buffer, 0, p_length );
			}

		private:
			inline bool _canAdd( unsigned int p_size )
			{
				return ( ( m_currentIndex - m_buffer ) + p_size ) <= m_capacity;
			}

		public:
			inline void SetBuffer( char * p_buffer, unsigned int p_length )
			{
				m_buffer = p_buffer;
				m_currentIndex = p_buffer;
				m_capacity = p_length;
				m_size = 0;
				memset( m_buffer, 0, p_length );
			}

			inline void CreateBuffer( unsigned int p_length )
			{
				m_buffer = new char [p_length];
				m_currentIndex = m_buffer;
				m_capacity = p_length;
				m_size = 0;
				memset( m_buffer, 0, p_length );
			}

			template <typename T>
			inline void write( const T & p_value )
			{
				if ( _canAdd( sizeof( T ) ) )
				{
					memcpy( m_currentIndex, & p_value, sizeof( T ) );
					m_currentIndex += sizeof( T );
					m_size += sizeof( T );
				}
			}

			template <typename T>
			inline void writeArray( const T * p_value, unsigned int p_numItems )
			{
				if ( _canAdd( sizeof( T ) * p_numItems ) )
				{
					memcpy( m_currentIndex, p_value, sizeof( T ) * p_numItems );
					m_currentIndex += sizeof( T ) * p_numItems;
					m_size += sizeof( T ) * p_numItems;
				}
			}

			template <typename T>
			inline WriteBuffer & operator << ( const T & p_value )
			{
				write( p_value );
				return *this;
			}

		public:
			inline char * c_str()
			{
				return m_buffer;
			}
			inline unsigned int length()const
			{
				return m_size;
			}
			inline unsigned int size()const
			{
				return m_size;
			}
			inline unsigned int capacity()const
			{
				return m_capacity;
			}
		};

		template <>
		inline void WriteBuffer::write <std::string> ( const std::string & p_value )
		{
			writeArray( p_value.c_str(), static_cast <unsigned int>( p_value.length() ) );
		}

		class ReadBuffer
		{
		private:
			char * m_buffer;
			char * m_currentIndex;

			unsigned int m_size;

		public:
			ReadBuffer()
				:	m_buffer( NULL ),
					m_currentIndex( NULL ),
					m_size( 0 )
			{
			}
			ReadBuffer( char * p_buffer, unsigned int p_length )
				:	m_buffer( p_buffer ),
					m_currentIndex( p_buffer ),
					m_size( p_length )
			{
			}
		private:
			inline bool _canRead( unsigned int p_size )
			{
				return ( ( m_currentIndex - m_buffer ) + p_size ) <= m_size;
			}

		public:
			inline void SetBuffer( char * p_buffer, unsigned int p_length )
			{
				m_buffer = p_buffer;
				m_currentIndex = p_buffer;
				m_size = p_length;
			}

			template <typename T>
			inline T read()
			{
				T l_t;

				if ( _canRead( sizeof( T ) ) )
				{
					memcpy( & l_t, m_currentIndex, sizeof( T ) );
					m_currentIndex += sizeof( T );
				}

				return l_t;
			}

			template <typename T>
			inline T * readArray( unsigned int p_num )
			{
				if ( _canRead( sizeof( T ) * p_num ) )
				{
					m_currentIndex += sizeof( T ) * p_num;
					return static_cast <T *>( m_currentIndex - sizeof( T ) * p_num );
				}

				return NULL;
			}

			template <typename T>
			inline ReadBuffer & operator >> ( T & p_value )
			{
				p_value = read <T> ();
				return * this;
			}

		public:
			inline unsigned int length()const
			{
				return m_size;
			}
			inline unsigned int size()const
			{
				return m_size;
			}
			inline unsigned int left()const
			{
				return m_size + m_buffer - m_currentIndex;
			}
			inline char * c_str()
			{
				return m_buffer;
			}
		};
	}
}

#endif
