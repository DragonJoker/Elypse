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
#ifndef ___GENLIB_URL_H___
#define ___GENLIB_URL_H___

/*
	A standard class for urls.
*/

#include "Path.h"

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

			inline UrlT( CharT const * const p_data )
				: std::string{ p_data }
			{
				doRetrieveProtocol();
			}

			inline UrlT( my_string const & p_data )
				: std::string{ p_data }
			{
				doRetrieveProtocol();
			}

			inline UrlT( UrlT< CharT > const & p_data )
				: std::string( p_data )
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

			inline UrlProtocol GetProtocol()
			{
				doCheckProtocol();
				return m_protocol;
			}

		private:
			inline void doRetrieveProtocol()
			{
				m_protocol = UNKNOWN;

				if ( size() > 4 )
				{
					my_string l_temp( substr( 0, 4 ) );

					if ( l_temp == my_string{ "http" } )
					{
						m_protocol = HTTP;
						m_separator = CharT{ '/' };
						m_offset = find_first_not_of( ":/", 4 );
					}
					else if ( l_temp == my_string{ "file" } )
					{
						m_protocol = LOCAL;
						m_separator = CharT{ d_path_slash };
						m_offset = find_first_not_of( ":/", 4 );
					}
					else if ( l_temp == my_string{ "ftp:" } )
					{
						m_protocol = FTP;
						m_separator = CharT{ '/' };
						m_offset = find_first_not_of( ":/", 4 );
					}
				}
			}

			inline void doCheckProtocol()
			{
				if ( m_protocol == UNKNOWN )
				{
					doRetrieveProtocol();
				}
			}

		private:
			UrlProtocol m_protocol{ UNKNOWN };
			size_t m_offset{ 0 };
			CharT m_separator = '/';
		};

		template< typename CharT >
		inline UrlT< CharT > operator/( UrlT< CharT > const & p_lhs, UrlT< CharT > const & p_rhs )
		{
			UrlT< CharT > l_url{ p_lhs };
			l_url /= p_rhs;
			return l_url;
		}

		using Url = UrlT< char >;
		using WUrl = UrlT< wchar_t >;
	}
}

#endif
