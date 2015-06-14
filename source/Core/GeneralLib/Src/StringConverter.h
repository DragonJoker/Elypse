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
#ifndef ___STRING_CONVERTER_H___
#define ___STRING_CONVERTER_H___

#include <sstream>
#include <vector>
#include <cstring>

#include "Macros.h"

namespace General
{
	namespace Utils
	{
		std::string d_dll ConvertToString( const std::wstring & p_wideString, const char * p_locale = "french", const char p_unknownCharReplacedBy = '?' );
		std::wstring d_dll ConvertToWString( const std::string & p_string, const char * p_locale = "french" );

		const size_t MAX_INT_LEN = 33;

		inline char * itoa( int p_value, size_t & p_len, int p_base = 10 )
		{
			static char l_buffer[MAX_INT_LEN] = { 0 };
			p_len = 0;

			if ( p_base < 2 || p_base > 36 )
			{
				return l_buffer;
			}

			if ( p_value == 0 )
			{
				l_buffer[0] = '0';
				p_len = 1;
				return l_buffer;
			}

			size_t l_indexBuffer = MAX_INT_LEN - 2;
			char l_signe = 0;

			if ( p_value < 0 )
			{
				unsigned int l_value = static_cast <unsigned int>( -p_value );
				l_signe = '-';

				for ( ; l_value && l_indexBuffer ; -- l_indexBuffer, l_value /= p_base )
				{
					l_buffer[l_indexBuffer] = "0123456789abcdefghijklmnopqrstuvwxyz"[ l_value % p_base ];
					p_len ++;
				}
			}
			else
			{
				for ( ; p_value && l_indexBuffer ; -- l_indexBuffer, p_value /= p_base )
				{
					l_buffer[l_indexBuffer] = "0123456789abcdefghijklmnopqrstuvwxyz"[ p_value % p_base ];
					p_len ++;
				}
			}

			if ( l_signe != 0 )
			{
				l_buffer[l_indexBuffer] = l_signe;
				p_len ++;
			}
			else
			{
				l_indexBuffer ++;
			}

			return & l_buffer[l_indexBuffer];
		}

		inline char * uitoa( unsigned int p_value, size_t & p_len, int p_base = 10 )
		{
			static char l_buffer[MAX_INT_LEN] = { 0 };
			p_len = 0;

			if ( p_base < 2 || p_base > 36 )
			{
				return l_buffer;
			}

			if ( p_value == 0 )
			{
				l_buffer[0] = '0';
				p_len = 1;
				return l_buffer;
			}

			size_t l_indexBuffer = MAX_INT_LEN - 2;

			for ( ; p_value && l_indexBuffer ; -- l_indexBuffer, p_value /= p_base )
			{
				l_buffer[l_indexBuffer] = "0123456789abcdefghijklmnopqrstuvwxyz"[ p_value % p_base ];
				p_len ++;
			}

			return & l_buffer[++ l_indexBuffer];
		}

		template <typename T>
		inline std::string ToString( const T & p_val )
		{
			std::stringstream l_out;
			l_out << p_val;
			return l_out.str();
		}

		template <typename T>
		inline std::wstring ToWString( const T & p_val )
		{
			std::wstringstream l_out;
			l_out << p_val;
			return l_out.str();
		}


		template <>
		inline std::wstring ToWString <std::string> ( const std::string & p_val )
		{
			return ConvertToWString( p_val );
		}

		template <>
		inline std::string ToString <bool> ( const bool & p_val )
		{
			if ( p_val )
			{
				return std::string( "true" );
			}

			return std::string( "false" );
		}

		template <>
		inline std::string ToString <std::wstring> ( const std::wstring & p_val )
		{
			return ConvertToString( p_val );
		}
		inline std::string ToString( wchar_t * p_val )
		{
			return ConvertToString( std::wstring( p_val ) );
		}
		inline std::string ToString( const wchar_t * p_val )
		{
			return ConvertToString( std::wstring( p_val ) );
		}

		template <>
		inline std::string ToString <int> ( const int & p_val )
		{
#	if GENLIB_WINDOWS
			char l_buffer[16];
			_itoa_s( p_val, l_buffer, 16, 10 );
			return std::string( l_buffer );
#	else
			size_t l_len;
			return std::string( itoa( p_val, l_len ) );
#	endif
		}

		template <>
		inline std::string ToString <unsigned int> ( const unsigned int & p_val )
		{
#	if GENLIB_WINDOWS
			char l_buffer[16];
			_itoa_s( p_val, l_buffer, 16, 10 );
			return std::string( l_buffer );
#	else
			size_t l_len;
			return std::string( uitoa( p_val, l_len ) );
#	endif
		}

		inline char * CStrCopy( const std::string & p_string )
		{
			if ( p_string.empty() )
			{
				return NULL;
			}

			char * l_buffer = new char[p_string.size() + 1];
			std::memcpy( l_buffer, p_string.c_str(), p_string.size() );
			l_buffer[p_string.size()] = '\0';
			return l_buffer;
		}


		template< typename OutCharType, typename InCharType > struct StringCastHelper;

		template< typename InCharType >
		struct StringCastHelper< char, InCharType >
		{
			static inline std::string Cast( const std::basic_string< InCharType > & p_val )
			{
				return ToString( p_val );
			}
		};

		template< typename InCharType >
		struct StringCastHelper< wchar_t, InCharType >
		{
			static inline std::wstring Cast( const std::basic_string< InCharType > & p_val )
			{
				return ToWString( p_val );
			}
		};

		template< typename OutCharType, typename InCharType >
		inline std::basic_string< OutCharType > string_cast( const std::basic_string< InCharType > & p_val )
		{
			return StringCastHelper< OutCharType, InCharType >::Cast( p_val );
		}

		float parseFloat( const std::string & val );
		int parseInt( const std::string & val );
		unsigned int parseUnsignedInt( const std::string & val );
		bool parseBool( const std::string & val );

	}
}

#endif
