/*
See LICENSE file in root folder
*/
#ifndef ___GENLIB_URL_H___
#define ___GENLIB_URL_H___

/*
	A standard class for urls.
*/

#include "Assertions.h"
#include "Path.h"
#include "StringUtils.h"

namespace General
{
	namespace Utils
	{
		enum UrlProtocol
		{
			HTTP,
			HTTPS,
			FTP,
			LOCAL,
			UNKNOWN
		};
		inline std::string getName( UrlProtocol protocol )
		{
			switch ( protocol )
			{
			case General::Utils::HTTP:
				return "http";
			case General::Utils::HTTPS:
				return "https";
			case General::Utils::FTP:
				return "ftp";
			case General::Utils::LOCAL:
				return "file";
			case General::Utils::UNKNOWN:
				return "unknown";
			}
			return "unknown";
		}

		template< typename CharT >
		class UrlT
			: public std::basic_string< CharT >
		{
			using my_string = std::basic_string< CharT >;

		public:
			inline UrlT()
				: std::string{}
			{
			}
			
			inline UrlT( UrlProtocol protocol )
				: std::string{ getName( protocol ) + "://" }
			{
			}

			inline UrlT( CharT const * const p_data )
				: my_string{ Normalise( p_data ) }
			{
				doRetrieveProtocol();
			}

			inline UrlT( my_string const & p_data )
				: my_string{ Normalise( p_data ) }
			{
				doRetrieveProtocol();
			}

			inline UrlT( UrlT< CharT > const & p_data )
				: my_string( p_data )
				, m_offset( p_data.m_offset )
			{
				doRetrieveProtocol();
			}

			inline UrlT< CharT > & operator/=( UrlT< CharT > const & p_url )
			{
				if ( !empty() )
				{
					push_back( m_separator );
				}

				append( p_url );
				doCheckProtocol();
				return *this;
			}

			inline UrlT< CharT > & operator/=( my_string const & p_url )
			{
				if ( !empty() )
				{
					push_back( m_separator );
				}

				append( Normalise( p_url ) );
				doCheckProtocol();
				return *this;
			}

			inline const my_string GetBaseSite()const
			{
				my_string l_return;
				auto l_index = find( m_separator, m_offset );

				if ( l_index != my_string::npos )
				{
					l_return = substr( m_offset, l_index - m_offset );
				}

				return l_return;
			}

			inline const my_string GetDirectory()const
			{
				auto l_start = find( m_separator, m_offset );
				auto l_end = find_last_of( m_separator );
				my_string l_return;

				if ( l_start != l_end && l_start != my_string::npos && l_end != my_string::npos )
				{
					l_return = substr( l_start + 1, l_end - l_start - 1 );
				}

				return l_return;
			}

			inline const my_string GetFilename()const
			{
				auto l_index = find_last_of( m_separator );
				my_string l_return;

				if ( l_index != my_string::npos )
				{
					l_return = substr( l_index + 1 );
				}
				else
				{
					l_return = *this;
				}

				return l_return;
			}

			inline const my_string GetAllButFilename()const
			{
				auto l_index = find_last_of( m_separator );
				my_string l_return;

				if ( l_index != my_string::npos )
				{
					l_return = substr( 0, l_index + 1 );
				}
				else
				{
					l_return = *this;
				}

				return l_return;
			}

			inline UrlProtocol GetProtocol()const
			{
				genlib_assert( m_protocol != UNKNOWN
					&& m_offset != my_string::npos );
				return m_protocol;
			}

			inline UrlProtocol GetProtocol()
			{
				doCheckProtocol();
				return m_protocol;
			}

		private:
			inline void doRetrieveProtocol()
			{
				m_protocol = UNKNOWN;
				m_offset = my_string::find( "://" );

				if ( m_offset != my_string::npos )
				{
					my_string l_temp( substr( 0, m_offset ) );

					if ( l_temp == getName( HTTP ) )
					{
						m_protocol = HTTP;
						m_offset = find_first_not_of( ":/", m_offset );
					}
					else if ( l_temp == getName( HTTPS ) )
					{
						m_protocol = HTTPS;
						m_offset = find_first_not_of( ":/", m_offset );
					}
					else if ( l_temp == getName( LOCAL ) )
					{
						m_protocol = LOCAL;
						m_offset = find_first_not_of( ":/", m_offset );
					}
					else if ( l_temp == getName( FTP ) )
					{
						m_protocol = FTP;
						m_offset = find_first_not_of( ":/", m_offset );
					}
					else
					{
						m_offset = my_string::npos;
					}
				}
			}

			inline void doCheckProtocol()
			{
				if ( m_protocol == UNKNOWN
					|| m_offset == my_string::npos )
				{
					doRetrieveProtocol();
				}
			}

			static inline my_string Normalise( my_string const & p_path )
			{
				return string::replace( p_path, CharT{ '\\' }, m_separator );
			}

		private:
			UrlProtocol m_protocol{ UNKNOWN };
			size_t m_offset{ 0 };
			static CharT const m_separator = '/';
		};

		template< typename CharT >
		inline UrlT< CharT > operator/( UrlT< CharT > const & p_lhs, UrlT< CharT > const & p_rhs )
		{
			return UrlT< CharT >{ p_lhs } /= p_rhs;
		}

		template< typename CharT >
		inline UrlT< CharT > operator/( UrlT< CharT > const & p_lhs, std::basic_string< CharT > const & p_rhs )
		{
			return UrlT< CharT >{ p_lhs } /= p_rhs;
		}

		template< typename CharT >
		inline UrlT< CharT > operator/( const UrlT< CharT > & lhs, char const * const rhs )
		{
			return UrlT( lhs ) /= rhs;
		}

		using Url = UrlT< char >;
		using WUrl = UrlT< wchar_t >;

		inline Url operator/( const Url & lhs, Path const & rhs )
		{
			return Url( lhs ) /= rhs;
		}
	}
}

#endif
