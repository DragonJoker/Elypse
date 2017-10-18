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
			{
				static_assert( sizeof( StorageType ) == 1u, "TWriteBuffer template storage type must be int8_t or uint8_t" );
			}

			TWriteBuffer( StorageType * p_buffer, uint32_t p_length )
				: TWriteBuffer{}
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
				DoAdd( reinterpret_cast< StorageType const * >( &p_value ), uint32_t( sizeof( T ) ) );
			}

			template< typename T >
			inline void writeArray( T const * p_value, size_t p_count )
			{
				DoAdd( reinterpret_cast< StorageType const * >( p_value ), uint32_t( sizeof( T ) * p_count ) );
			}

			template< typename T, size_t N >
			inline void writeArray( T const( & p_value )[N] )
			{
				writeArray( p_value, N );
			}

			template< typename T, size_t N >
			inline void writeArray( std::array< T, N > const & p_value )
			{
				writeArray( p_value.data(), p_value.size() );
			}

			template< size_t N >
			inline void writeArray( char const( &p_value )[N] )
			{
				writeArray( p_value, N - 1 );
			}

			inline void writeString( std::string const & p_value )
			{
				write( uint32_t( p_value.size() ) );
				writeArray( p_value.c_str(), uint32_t( p_value.size() ) );
			}

			inline void write( std::string const & p_value )
			{
				DoAdd( reinterpret_cast< StorageType const * >( p_value.c_str() ), static_cast< uint32_t >( p_value.length() ) );
			}

			template< typename T >
			inline TWriteBuffer & operator<<( T const & p_value )
			{
				write( p_value );
				return *this;
			}

			inline TWriteBuffer & operator<<( std::string const & p_value )
			{
				writeString( p_value );
				return *this;
			}

			inline StorageType const * data()const
			{
				return m_buffer;
			}

			inline uint32_t length()const
			{
				return m_size;
			}

			inline uint32_t size()const
			{
				return m_size;
			}

			inline uint32_t capacity()const
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
					std::memcpy( m_currentIndex, p_buffer, p_size );
					m_currentIndex += p_size;
					m_size += p_size;
				}
			}

		private:
			StorageType * m_buffer{ nullptr };
			StorageType * m_currentIndex{ nullptr };
			uint32_t m_capacity{ 0u };
			uint32_t m_size{ 0u };
		};

		template< typename StorageType >
		class TReadBuffer
		{
		public:
			TReadBuffer()
			{
				static_assert( sizeof( StorageType ) == 1u, "TReadBuffer template storage type must be int8_t or uint8_t" );
			}

			TReadBuffer( StorageType const * p_buffer, uint32_t p_length )
				: TReadBuffer{}
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
					std::memcpy( &l_t, m_currentIndex, sizeof( T ) );
					m_currentIndex += sizeof( T );
				}

				return l_t;
			}

			template< typename T >
			inline T const * readArray( uint32_t p_num )
			{
				T const * l_return = nullptr;

				if ( DoCanRead( sizeof( T ) * p_num ) )
				{
					l_return = static_cast< T const * >( m_currentIndex );
					m_currentIndex += sizeof( T ) * p_num;
				}

				return l_return;
			}

			inline std::string readString()
			{
				uint32_t l_size = read< uint32_t >();
				return std::string( readArray< char >( l_size ), l_size );
			}

			template< typename T >
			inline TReadBuffer & operator>>( T & p_value )
			{
				p_value = read< T >();
				return * this;
			}

			inline TReadBuffer & operator>>( std::string & p_value )
			{
				p_value = readString();
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
			StorageType const * m_buffer{ nullptr };
			StorageType const * m_currentIndex{ nullptr };
			uint32_t m_size{ 0 };
		};

		typedef TWriteBuffer< char > WriteBuffer;
		typedef TReadBuffer< char > ReadBuffer;

		template< typename StorageType >
		TWriteBuffer< StorageType > make_wbuffer( StorageType * p_buffer, uint32_t p_size )
		{
			return TWriteBuffer< StorageType > { p_buffer, p_size };
		}

		template< typename StorageType, size_t Size >
		TWriteBuffer< StorageType > make_wbuffer( StorageType( & p_buffer )[Size] )
		{
			return TWriteBuffer< StorageType > { p_buffer, Size };
		}

		template< typename StorageType >
		TReadBuffer< StorageType > make_rbuffer( StorageType * p_buffer, uint32_t p_size )
		{
			return TReadBuffer< StorageType > { p_buffer, p_size };
		}

		template< typename StorageType, size_t Size >
		TReadBuffer< StorageType > make_rbuffer( StorageType const( & p_buffer )[Size] )
		{
			return TReadBuffer< StorageType > { p_buffer, p_size };
		}
	}
}

#endif
