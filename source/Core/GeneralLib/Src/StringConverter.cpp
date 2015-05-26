#include "StringConverter.h"
#include "StringUtils.h"

std::string General :: Utils :: ConvertToString( const std::wstring & p_wideString, const char * p_locale, const char p_unknownCharReplacedBy )
{
	std::string l_return;
	l_return.resize( p_wideString.length() );
	std::locale loc( p_locale );
	std::use_facet <std::ctype <wchar_t> > ( loc ).narrow( & p_wideString[0], & p_wideString[p_wideString.length()], p_unknownCharReplacedBy, & l_return[0] );
	return l_return;
}

std::wstring General :: Utils :: ConvertToWString( const std::string & p_string, const char * p_locale )
{
	std::wstring l_return;
	l_return.resize( p_string.length() );
	std::locale loc( p_locale );
	std::use_facet <std::ctype <wchar_t> > ( loc ).widen( & p_string[0], & p_string[p_string.length()], & l_return[0] );
	return l_return;
}

float General :: Utils :: parseFloat( const std::string & val )
{
	std::istringstream str( val );
	float ret = 0;
	str >> ret;
	return ret;
}

int General :: Utils :: parseInt( const std::string & val )
{
	std::istringstream str( val );
	int ret = 0;
	str >> ret;
	return ret;
}

unsigned int General :: Utils :: parseUnsignedInt( const std::string & val )
{
	std::istringstream str( val );
	unsigned int ret = 0;
	str >> ret;
	return ret;
}

bool General :: Utils :: parseBool( const std::string & val )
{
	return ( string::startsWith( val, "true" ) || string::startsWith( val, "yes" )
			 || string::startsWith( val, "1" ) );
}