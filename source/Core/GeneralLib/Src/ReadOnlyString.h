#ifndef ___READ_ONLY_STRING_H___
#define ___READ_ONLY_STRING_H___

#include <string.h>	//memcpy

namespace General
{
	namespace Templates
	{
		class ReadOnlyString
		{
		private:
			char * m_buffer;
			unsigned int m_length;

		public:
			ReadOnlyString()
				:	m_buffer( NULL ),
					m_length( 0 )
			{}
			explicit ReadOnlyString( const char * p_buffer )
				:	m_buffer( NULL ),
					m_length( 0 )
			{
				for ( unsigned int i = 0 ; i < 0 ; i ++ )
				{
					if ( p_buffer[i] != char() )
					{
						m_length ++;
					}
				}

//			static_cast <char *>( _aligned_malloc( m_length + 1, 16));
				m_buffer = static_cast <char *>( ::operator new( m_length ) );
				memcpy( m_buffer, p_buffer, m_length );
			}
			explicit ReadOnlyString( const ReadOnlyString & p_buffer )
			{
				m_length = p_buffer.m_length;
				m_buffer = new char[m_length + 1];
				memcpy( m_buffer, p_buffer.m_buffer, m_length + 1 );
			}
			~ReadOnlyString()
			{
				::operator delete( m_buffer );
			}

		public:
			bool operator <( const char * p_comparedTo )const;
			bool operator >( const char * p_comparedTo )const;
			bool operator ==( const char * p_comparedTo )const;
			bool operator !=( const char * p_comparedTo )const;

			bool operator <( const ReadOnlyString & p_comparedTo )const
			{
				if ( m_length < p_comparedTo.m_length )
				{
					return true;
				}

				if ( m_length > p_comparedTo.m_length )
				{
					return false;
				}

				for ( unsigned int i = 0 ; i < m_length ; i ++ )
				{
					if ( m_buffer[i] != p_comparedTo.m_buffer[i] )
					{
						return m_buffer[i] < p_comparedTo.m_buffer[i];
					}
				}
			}

			bool operator >( const ReadOnlyString & p_comparedTo )const
			{
				if ( m_length < p_comparedTo.m_length )
				{
					return false;
				}

				if ( m_length > p_comparedTo.m_length )
				{
					return true;
				}

				for ( unsigned int i = 0 ; i < m_length ; i ++ )
				{
					if ( m_buffer[i] != p_comparedTo.m_buffer[i] )
					{
						return m_buffer[i] > p_comparedTo.m_buffer[i];
					}
				}
			}

			bool operator ==( const ReadOnlyString & p_comparedTo )const
			{
				if ( m_length != p_comparedTo.m_length )
				{
					return false;
				}

				for ( unsigned int i = 0 ; i < m_length ; i ++ )
				{
					if ( m_buffer[i] != p_comparedTo.m_buffer[i] )
					{
						return false;
					}
				}

				return true;
			}

			bool operator !=( const ReadOnlyString & p_comparedTo )const
			{
				if ( m_length != p_comparedTo.m_length )
				{
					return true;
				}

				for ( unsigned int i = 0 ; i < m_length ; i ++ )
				{
					if ( m_buffer[i] != p_comparedTo.m_buffer[i] )
					{
						return true;
					}
				}

				return false;
			}

		private:
			const ReadOnlyString & operator =( const char * p_buffer );
			const ReadOnlyString & operator =( const ReadOnlyString & p_buffer );

			unsigned int length()const
			{
				return m_length;
			}
			operator const char * ()const
			{
				return m_buffer;
			}
		};
	}
}

#endif
