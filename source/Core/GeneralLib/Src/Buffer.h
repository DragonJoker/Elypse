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
		template< typename StorageType >
		class TWriteBuffer
		{
		public:
			TWriteBuffer()
				: m_buffer( NULL )
				, m_currentIndex( NULL )
				, m_capacity( 0 )
				, m_size( 0 )
			{
				static_assert( sizeof( StorageType ) == 1, "TWriteBuffer template storage type must be int8_t or uint8_t" );
			}

			TWriteBuffer( StorageType * p_buffer, uint32_t p_length )
				: TWriteBuffer()
			{
				setBuffer( p_buffer, p_length );
			}

			inline void createBuffer( uint32_t p_length )
			{
				setBuffer( new StorageType[p_length], p_length );
			}

			inline void setBuffer( StorageType * p_buffer, uint32_t p_length )
			{
				m_buffer = p_buffer;
				m_currentIndex = p_buffer;
				m_capacity = p_length;
				m_size = 0;
				memset( m_buffer, 0, p_length );
			}

			template< typename T >
			inline void write( T const & p_value )
			{
				DoAdd( reinterpret_cast< StorageType const * >( &p_value ), sizeof( T ) );
			}

			template< typename T >
			inline void writeArray( T const * p_value, uint32_t p_count )
			{
				DoAdd( reinterpret_cast< StorageType const * >( p_value ), sizeof( T ) * p_count );
			}

			inline void write( std::string const & p_value )
			{
				DoAdd( reinterpret_cast< StorageType const * >( p_value.c_str() ), static_cast< uint32_t >( p_value.length() ) );
			}

			template< typename T >
			inline TWriteBuffer & operator << ( T const & p_value )
			{
				write( p_value );
				return *this;
			}

			inline StorageType const * data()const
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

		private:
			inline bool DoCanAdd( uint32_t p_size )
			{
				return ( ( m_currentIndex - m_buffer ) + p_size ) <= m_capacity;
			}

			inline void DoAdd( StorageType const * p_buffer, uint32_t p_size )
			{
				if ( DoCanAdd( p_size ) )
				{
					memcpy( m_currentIndex, p_buffer, p_size );
					m_currentIndex += p_size;
					m_size += p_size;
				}
			}

		private:
			StorageType * m_buffer;
			StorageType * m_currentIndex;
			uint32_t m_capacity;
			uint32_t m_size;
		};

		template< typename StorageType >
		class TReadBuffer
		{
		public:
			TReadBuffer()
				: m_buffer( NULL )
				, m_currentIndex( NULL )
				, m_size( 0 )
			{
				static_assert( sizeof( StorageType ) == 1, "TReadBuffer template storage type must be int8_t or uint8_t" );
			}

			TReadBuffer( StorageType const * p_buffer, uint32_t p_length )
				: TReadBuffer()
			{
				setBuffer( p_buffer, p_length );
			}

			inline void setBuffer( StorageType const * p_buffer, uint32_t p_length )
			{
				m_buffer = p_buffer;
				m_currentIndex = p_buffer;
				m_size = p_length;
			}

			template< typename T >
			inline T read()
			{
				T l_t;

				if ( DoCanRead( sizeof( T ) ) )
				{
					memcpy( &l_t, m_currentIndex, sizeof( T ) );
					m_currentIndex += sizeof( T );
				}

				return l_t;
			}

			template< typename T >
			inline T const * readArray( uint32_t p_num )
			{
				T const * l_return = NULL;

				if ( DoCanRead( sizeof( T ) * p_num ) )
				{
					l_return = static_cast< T const * >( m_currentIndex - sizeof( T ) * p_num );
					m_currentIndex += sizeof( T ) * p_num;
				}

				return l_return;
			}

			template <typename T>
			inline TReadBuffer & operator >> ( T & p_value )
			{
				p_value = read< T > ();
				return * this;
			}

			inline uint32_t length()const
			{
				return m_size;
			}

			inline uint32_t size()const
			{
				return m_size;
			}

			inline uint32_t left()const
			{
				return m_size + uint32_t( m_buffer - m_currentIndex );
			}

			inline StorageType const * data()const
			{
				return m_buffer;
			}

		private:
			inline bool DoCanRead( uint32_t p_size )
			{
				return ( ( m_currentIndex - m_buffer ) + p_size ) <= m_size;
			}

		private:
			StorageType const * m_buffer;
			StorageType const * m_currentIndex;
			uint32_t m_size;
		};

		typedef TWriteBuffer< char > WriteBuffer;
		typedef TReadBuffer< char > ReadBuffer;

		template< typename StorageType >
		TWriteBuffer< StorageType > make_wbuffer( StorageType * p_buffer, uint32_t p_size )
		{
			return TWriteBuffer< StorageType >( p_buffer, p_size );
		}

		template< typename StorageType, size_t Size >
		TWriteBuffer< StorageType > make_wbuffer( StorageType( & p_buffer )[Size] )
		{
			return TWriteBuffer< StorageType >( p_buffer, Size );
		}

		template< typename StorageType >
		TReadBuffer< StorageType > make_rbuffer( StorageType * p_buffer, uint32_t p_size )
		{
			return TReadBuffer< StorageType >( p_buffer, p_size );
		}

		template< typename StorageType, size_t Size >
		TReadBuffer< StorageType > make_rbuffer( StorageType const( & p_buffer )[Size] )
		{
			return TReadBuffer< StorageType >( p_buffer, Size );
		}
	}
}

#endif
