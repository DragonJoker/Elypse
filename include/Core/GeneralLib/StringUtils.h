/*
See LICENSE file in root folder
*/
#ifndef ___GENLIB_STRING_UTILS_H___
#define ___GENLIB_STRING_UTILS_H___

#include <string>
#include <algorithm>
#include <vector>

#include "StringConverter.h"

namespace General
{
	namespace Utils
	{
		class string
		{
		public:
			template< typename CharType >
			static inline void trim( std::basic_string< CharType > & str, bool left = true, bool right = true )
			{
				static const std::basic_string< CharType > delims = string_cast< CharType >( std::string( " \t\r" ) );

				if ( right )
				{
					str.erase( str.find_last_not_of( delims ) + 1 );
				}

				if ( left )
				{
					str.erase( 0, str.find_first_not_of( delims ) );
				}
			}

			template< typename CharType >
			static inline std::vector< std::basic_string< CharType > > split( std::basic_string< CharType > const & str, const CharType * delims, uint32_t maxSplits = 10 )
			{
				std::vector< std::basic_string< CharType > > ret;
				ret.reserve( maxSplits ? maxSplits + 1 : 10 );
				uint32_t numSplits = 0;
				size_t start = 0;
				size_t pos = 0;

				do
				{
					pos = str.find_first_of( delims, start );

					if ( pos == start )
					{
						start = pos + 1;
					}
					else if ( pos == std::basic_string< CharType >::npos || ( maxSplits && numSplits == maxSplits ) )
					{
						ret.push_back( str.substr( start ) );
						break;
					}
					else
					{
						ret.push_back( str.substr( start, pos - start ) );
						start = pos + 1;
					}

					start = str.find_first_not_of( delims, start );
					++ numSplits;
				}
				while ( pos != std::basic_string< CharType >::npos );

				return ret;
			}

			template< typename CharType >
			static inline std::vector< std::basic_string< CharType > > split( std::basic_string< CharType > const & str, const std::basic_string< CharType > & delims = string_cast< CharType >( "\t\n " ), uint32_t maxSplits = 10 )
			{
				return split( str, delims.c_str(), maxSplits );
			}

			static inline void toLowerCase( std::string & p_str )
			{
				std::transform( p_str.begin(), p_str.end(), p_str.begin(), tolower );
			}

			static inline void toUpperCase( std::string & p_str )
			{
				std::transform( p_str.begin(), p_str.end(), p_str.begin(), toupper );
			}

			template< typename CharType >
			static inline std::basic_string< CharType > replace( const std::basic_string< CharType > & p_target, const std::basic_string< CharType > & p_find, const std::basic_string< CharType > & p_replaced )
			{
				std::basic_string< CharType > l_return;
				std::basic_string< CharType > l_temp;
				size_t l_currentPos = 0;
				size_t l_pos = 0;

				while ( ( l_pos = p_target.find( p_find, l_currentPos ) ) != std::basic_string< CharType >::npos )
				{
					l_return.append( p_target.substr( l_currentPos, l_pos - l_currentPos ) );
					l_return.append( p_replaced );
					l_currentPos = l_pos + p_find.size();
				}

				if ( l_currentPos != p_target.size() )
				{
					l_return.append( p_target.substr( l_currentPos, l_pos - l_currentPos ) );
				}

				return l_return;
			}

			template< typename CharType >
			static inline bool startsWith( const std::basic_string< CharType > & str, const std::basic_string< CharType > & pattern, bool lowerCase = true )
			{
				size_t thisLen = str.length();
				size_t patternLen = pattern.length();

				if ( thisLen < patternLen || patternLen == 0 )
				{
					return false;
				}

				std::basic_string< CharType > startOfThis = str.substr( 0, patternLen );

				if ( lowerCase )
				{
					toLowerCase( startOfThis );
				}

				return ( startOfThis == pattern );
			}

			template< typename CharType >
			static inline bool endsWith( const std::basic_string< CharType > & str, const std::basic_string< CharType > & pattern, bool lowerCase = true )
			{
				size_t thisLen = str.length();
				size_t patternLen = pattern.length();

				if ( thisLen < patternLen || patternLen == 0 )
				{
					return false;
				}

				std::basic_string< CharType > endOfThis = str.substr( thisLen - patternLen, patternLen );

				if ( lowerCase )
				{
					toLowerCase( endOfThis );
				}

				return ( endOfThis == pattern );
			}

			template< typename CharType >
			static inline bool startsWith( const std::basic_string< CharType > & str, const CharType * pattern, bool lowerCase = true )
			{
				return startsWith( str, std::basic_string< CharType >( pattern ), lowerCase );
			}

			template< typename CharType >
			static inline bool endsWith( const std::basic_string< CharType > & str, const CharType * pattern, bool lowerCase = true )
			{
				return endsWith( str, std::basic_string< CharType >( pattern ), lowerCase );
			}

			template< typename CharType >
			static inline std::basic_string< CharType > replace( const std::basic_string< CharType > & p_target, const CharType * p_find, const CharType * p_replaced )
			{
				typedef std::basic_string< CharType > StringType;
				return replace( p_target, StringType() + p_find, StringType() + p_replaced );
			}

			template< typename CharType >
			static inline std::basic_string< CharType > replace( const std::basic_string< CharType > & p_target, const CharType * p_find, const CharType p_replaced )
			{
				typedef std::basic_string< CharType > StringType;
				return replace( p_target, StringType() + p_find, StringType() + p_replaced );
			}

			template< typename CharType >
			static inline std::basic_string< CharType > replace( const std::basic_string< CharType > & p_target, const CharType p_find, const CharType * p_replaced )
			{
				typedef std::basic_string< CharType > StringType;
				return replace( p_target, StringType() + p_find, StringType() + p_replaced );
			}

			template< typename CharType >
			static inline std::basic_string< CharType > replace( const std::basic_string< CharType > & p_target, const CharType p_find, const CharType p_replaced )
			{
				typedef std::basic_string< CharType > StringType;
				return replace( p_target, StringType() + p_find, StringType() + p_replaced );
			}
		};
	}
}

#endif
