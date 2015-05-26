#ifndef ___GENLIB_STRING_UTILS_H___
#define ___GENLIB_STRING_UTILS_H___

#include <string>
#include <algorithm>
#include <vector>

namespace General
{
	namespace Utils
	{
		class string
		{
		public:
			static inline void trim( std::string & str, bool left = true, bool right = true )
			{
				static const std::string delims = " \t\r";

				if ( right )
				{
					str.erase( str.find_last_not_of( delims ) + 1 );
				}

				if ( left )
				{
					str.erase( 0, str.find_first_not_of( delims ) );
				}
			}

			static inline std::vector<std::string> split( const std::string & str, const std::string & delims = "\t\n ", unsigned int maxSplits = 10 )
			{
				std::vector<std::string> ret;
				ret.reserve( maxSplits ? maxSplits + 1 : 10 );
				unsigned int numSplits = 0;
				size_t start, pos;
				start = 0;

				do
				{
					pos = str.find_first_of( delims, start );

					if ( pos == start )
					{
						start = pos + 1;
					}
					else if ( pos == std::string::npos || ( maxSplits && numSplits == maxSplits ) )
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
				while ( pos != std::string::npos );

				return ret;
			}

			static inline void toLowerCase( std::string & p_str )
			{
				std::transform(
					p_str.begin(),
					p_str.end(),
					p_str.begin(),
					tolower );
			}

			static inline void toUpperCase( std::string & p_str )
			{
				std::transform(
					p_str.begin(),
					p_str.end(),
					p_str.begin(),
					toupper );
			}


			static inline std::string replace( const std::string & p_target, const std::string & p_find, const std::string & p_replaced )
			{
				std::string l_return;
				std::string l_temp;
				size_t l_currentPos = 0;
				size_t l_pos;

				while ( ( l_pos = p_target.find( p_find, l_currentPos ) ) != std::string::npos )
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

			static inline bool startsWith( const std::string & str, const std::string & pattern, bool lowerCase = true )
			{
				size_t thisLen = str.length();
				size_t patternLen = pattern.length();

				if ( thisLen < patternLen || patternLen == 0 )
				{
					return false;
				}

				std::string startOfThis = str.substr( 0, patternLen );

				if ( lowerCase )
				{
					toLowerCase( startOfThis );
				}

				return ( startOfThis == pattern );
			}

			static inline bool endsWith( const std::string & str, const std::string & pattern, bool lowerCase = true )
			{
				size_t thisLen = str.length();
				size_t patternLen = pattern.length();

				if ( thisLen < patternLen || patternLen == 0 )
				{
					return false;
				}

				std::string endOfThis = str.substr( thisLen - patternLen, patternLen );

				if ( lowerCase )
				{
					toLowerCase( endOfThis );
				}

				return ( endOfThis == pattern );
			}

		};
	}
}

#endif
