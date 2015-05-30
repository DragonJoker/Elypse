#include "StringConverter.h"

std::vector <std::string> General::Utils::Split( const std::string & p_toSplit, const std::string & p_delims, unsigned int p_maxSplits )
{
	std::vector <std::string> l_toReturn;

	if ( p_toSplit.empty() || p_delims.empty() )
	{
		return l_toReturn;
	}

	l_toReturn.reserve( p_maxSplits ? p_maxSplits + 1 : 10 );

	size_t l_numSplits = 0;
	size_t l_start = 0;
	size_t l_pos = 0;

	do
	{
		l_pos = p_toSplit.find_first_of( p_delims, l_start );

		if ( l_pos == l_start )
		{
			l_start = l_pos + 1;
		}
		else if ( l_pos == std::string::npos || ( p_maxSplits && l_numSplits == p_maxSplits ) )
		{
			l_toReturn.push_back( p_toSplit.substr( l_start ) );
			return l_toReturn;
		}
		else
		{
			l_toReturn.push_back( p_toSplit.substr( l_start, l_pos - l_start ) );
			l_start = l_pos + 1;
		}

		l_start = p_toSplit.find_first_not_of( p_delims, l_start );
		++ l_numSplits;
	}
	while ( l_pos != std::string::npos );

	return l_toReturn;
}

std::string General::Utils::ConvertToString( const std::wstring & p_wideString, const char * p_locale, const char p_unknownCharReplacedBy )
{
	std::string l_return;

	l_return.resize( p_wideString.length() );

	std::locale loc( p_locale );

	std::use_facet <std::ctype <wchar_t> > ( loc ).narrow( & p_wideString[0], & p_wideString[p_wideString.length()], p_unknownCharReplacedBy, & l_return[0] );

	return l_return;
}

std::wstring General::Utils::ConvertToWString( const std::string & p_string, const char * p_locale )
{
	std::wstring l_return;

	l_return.resize( p_string.length() );

	std::locale loc( p_locale );

	std::use_facet <std::ctype <wchar_t> > ( loc ).widen( & p_string[0], & p_string[p_string.length()], & l_return[0] );

	return l_return;
}
