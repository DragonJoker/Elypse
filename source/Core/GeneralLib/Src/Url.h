/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007 - 2008

*****************************************************************************/
#ifndef ___GENLIB_URL_H___
#define ___GENLIB_URL_H___

/*
	A standard class for urls.
*/

#include <string>

namespace General
{
	namespace Utils
	{
		enum UrlProtocol
		{
			HTTP,
			FTP,
			LOCAL,
			UNKNOWN
		};

		class Url : public std::string
		{
		private:
			UrlProtocol m_protocol;
			size_t m_offset;
#if GENLIB_WINDOWS
			char m_separator;
#else
#		define m_separator '/'
#endif
		public:
			inline Url()
				:	m_protocol( UNKNOWN ),
					m_offset( 0 )
#if GENLIB_WINDOWS
					, m_separator( '/' )
#endif
			{}

			inline Url( const char * p_data )
				:	std::string( p_data )
			{
				RetrieveProtocol();
			}

			inline Url( const std::string & p_data )
				:	std::string( p_data )
			{
				RetrieveProtocol();
			}

			inline Url( const Url & p_data )
				:	std::string( p_data ),
					m_offset( p_data.m_offset )
#if GENLIB_WINDOWS
					, m_separator( p_data.m_separator )
#endif
			{
				RetrieveProtocol();
			}

			inline const Url operator /( const Url & p_url )const
			{
				Url l_url( * this );

				if ( ! empty() )
				{
					l_url.push_back( m_separator );
				}

				l_url.append( p_url );
				return l_url;
			}

			inline Url & operator /=( const Url & p_url )
			{
				if ( ! empty() )
				{
					push_back( m_separator );
				}

				append( p_url );
				return *this;
			}

			inline const std::string GetBaseSite()const
			{
				size_t l_index = find( m_separator, m_offset );

				if ( l_index != std::string::npos )
				{
					return substr( m_offset, l_index - m_offset );
				}

				return std::string();
			}

			inline const std::string GetDirectory()const
			{
				size_t l_start = find( m_separator, m_offset );
				size_t l_end = find_last_of( m_separator );

				if ( l_start != l_end && l_start != std::string::npos && l_end != std::string::npos )
				{
					return substr( l_start + 1, l_end - l_start - 1 );
				}

				return std::string();
			}

			inline const std::string GetFilename()const
			{
				size_t l_index = find_last_of( m_separator );

				if ( l_index != std::string::npos )
				{
					return substr( l_index + 1 );
				}

				return std::string( *this );
			}

			inline const std::string GetAllButFilename()const
			{
				size_t l_index = find_last_of( m_separator );

				if ( l_index != std::string::npos )
				{
					return substr( 0, l_index + 1 );
				}

				return std::string( *this );
			}

			UrlProtocol GetProtocol()
			{
				_checkProtocol();
				return m_protocol;
			}

			inline void RetrieveProtocol()
			{
				m_protocol = UNKNOWN;

				if ( size() > 4 )
				{
					std::string l_temp( substr( 0, 4 ) );

					if ( l_temp == "http" )
					{
						m_protocol = HTTP;
#if GENLIB_WINDOWS
						m_separator = '/';
#endif
						m_offset = find_first_not_of( ":/", 4 );
					}
					else if ( l_temp == "file" )
					{
						m_protocol = LOCAL;
#if GENLIB_WINDOWS
						m_separator = '\\';
#endif
						m_offset = find_first_not_of( ":/", 4 );
					}
					else if ( l_temp == "ftp:" )
					{
						m_protocol = FTP;
#if GENLIB_WINDOWS
						m_separator = '/';
#endif
						m_offset = find_first_not_of( ":/", 4 );
					}
				}
			}
		private:
			inline void _checkProtocol()
			{
				if ( m_protocol == UNKNOWN )
				{
					RetrieveProtocol();
				}
			}
#if ! GENLIB_WINDOWS
#	undef m_separator
#endif
		};
	}
}

#endif
