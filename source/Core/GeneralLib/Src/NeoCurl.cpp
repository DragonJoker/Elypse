#include <fstream>

#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#include "NeoCurl.h"

#include "StringConverter.h"
#include "StringUtils.h"
#pragma warning( pop )

#if GENLIB_WINDOWS
#	pragma comment( lib, "ws2_32.lib")
#	pragma comment( lib, "winmm.lib")
#endif

using namespace General::Files;
using namespace General::Utils;

CURLManager::CURLManager( bool p_autoInitialise )
	: m_initialised( false )
	, m_handle( NULL )
{
	if ( p_autoInitialise )
	{
		Initialise();
	}
}

CURLManager::~CURLManager()
{
	Close();
}

void CURLManager::Close()
{
	if ( m_initialised )
	{
		if ( m_handle )
		{
			curl_easy_cleanup( m_handle );
		}

		curl_global_cleanup();
		m_initialised = false;
		m_handle = NULL;
	}
}

bool CURLManager::Initialise()
{
	bool l_return = false;
#if GENLIB_WINDOWS

	if ( !curl_global_init( CURL_GLOBAL_WIN32 ) )
#else
	if ( !curl_global_init( CURL_GLOBAL_NOTHING ) )
#endif
	{
		m_handle = curl_easy_init();
		m_initialised = true;
		l_return = ( m_handle != NULL );
	}

	return l_return;
}

CURLcode CURLManager::SetCookieFile( const char * p_filename )
{
	return curl_easy_setopt( m_handle, CURLOPT_COOKIEFILE, p_filename );
}

CURLcode CURLManager::SetCookieString( const char * p_filename )
{
	return curl_easy_setopt( m_handle, CURLOPT_COOKIE, p_filename );
}

CURLcode CURLManager::OpenUrl( const std::string & p_url, size_t ( * p_function )( void *, size_t, size_t, void * ), void * p_data, const std::string & p_postParams )
{
	if ( !m_initialised || !m_handle )
	{
		return CURLE_FAILED_INIT;
	}

	std::string l_url( MimeEncode( p_url ) );
	char * l_buffer = CStrCopy( l_url );
	char * l_bufferPost = NULL;
	CURLcode l_returnValue = CURLE_OK;

	do
	{
		if ( !p_postParams.empty() )
		{
			l_bufferPost = CStrCopy( p_postParams );
			l_returnValue = curl_easy_setopt( m_handle, CURLOPT_POST, 1 );

			if ( l_returnValue != CURLE_OK )
			{
				break;
			}

			l_returnValue = curl_easy_setopt( m_handle, CURLOPT_POSTFIELDS, l_bufferPost );

			if ( l_returnValue != CURLE_OK )
			{
				break;
			}
		}

		l_returnValue = curl_easy_setopt( m_handle, CURLOPT_URL, l_buffer );

		if ( l_returnValue != CURLE_OK )
		{
			break;
		}

		l_returnValue = curl_easy_setopt( m_handle, CURLOPT_WRITEFUNCTION, p_function );

		if ( l_returnValue != CURLE_OK )
		{
			break;
		}

		if ( p_data )
		{
			l_returnValue = curl_easy_setopt( m_handle, CURLOPT_WRITEDATA, p_data );

			if ( l_returnValue != CURLE_OK )
			{
				break;
			}
		}
	}
	while ( false );

	l_returnValue = curl_easy_perform( m_handle );
	delete [] l_buffer;

	if ( l_bufferPost )
	{
		delete [] l_bufferPost;
	}

	return l_returnValue;
}

CURLcode CURLManager::GetStringFromUrl( const std::string & p_url, std::string & p_contents, const std::string & p_postParams )
{
	return OpenUrl( p_url, &CURLManager::_getString, & p_contents, p_postParams );
}

CURLcode CURLManager::DownloadFile( const std::string & p_url, const std::string & p_filename, const std::string & p_postParams )
{
	std::ofstream l_file;
	l_file.open( p_filename.c_str(), std::ios::binary | std::ios::out );

	if ( l_file.is_open() )
	{
		return OpenUrl( p_url, & CURLManager::_download, & l_file, p_postParams );
	}

	return CURLE_WRITE_ERROR;
}

size_t CURLManager::_getString( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data )
{
	std::string & l_string = * static_cast <std::string *>( p_data );
	l_string.append( static_cast <char *>( p_buffer ), p_size = p_numItems );
	return p_size = p_numItems;
}

size_t CURLManager::_download( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data )
{
	std::ofstream & l_out = * static_cast <std::ofstream *>( p_data );
	l_out.write( static_cast <const char *>( p_buffer ), static_cast <std::streamsize>( p_size * p_numItems ) );
	return p_size = p_numItems;
}

size_t CURLManager::_null( void *, size_t, size_t, void * )
{
	return 0;
}

std::string CURLManager::CurlError( CURLcode p_errorCode )
{
	const char * l_strError = curl_easy_strerror( p_errorCode );
	std::string l_returnValue = std::string( l_strError );
	curl_free( const_cast <char *>( l_strError ) );
	return l_returnValue;
}

std::string CURLManager::GetLastContentType()
{
	char * l_contentType = NULL;
	CURLcode l_errorCode = curl_easy_getinfo( m_handle, CURLINFO_CONTENT_TYPE, & l_contentType );

	if ( l_contentType == NULL || l_errorCode != CURLE_OK )
	{
		return std::string();
	}

	return std::string( l_contentType );
}

std::string CURLManager::GetContentType( const std::string & p_url, const std::string & p_postParams )
{
	OpenUrl( p_url, & CURLManager::_null, NULL, p_postParams );
	return GetLastContentType();
}

std::string CURLManager::MimeEncode( const std::string & p_string )
{
	return string::replace( p_string, " ", "%20" );
}
