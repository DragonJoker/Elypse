/*
See LICENSE file in root folder
*/
#ifndef ___CURL_PLUS_PLUS_H___
#define ___CURL_PLUS_PLUS_H___

#define CURL_STATICLIB
#include "Config.h"

#include <string>
#include <functional>

#include <curl/curl.h>
/*
#if GENLIB_WINDOWS
#	if defined(_MT) || defined(__MT__)
#		pragma comment( lib, "curllib_mt.lib")
#	else
#		pragma comment( lib, "curllib_md.lib")
#	endif
#	pragma comment( lib, "ws2_32.lib")
#	pragma comment( lib, "winmm.lib")
#endif
*/
namespace General
{
	namespace Files
	{
		class CURLManager
		{
		public:
			CURLManager( bool p_autoInitialise = true );
			~CURLManager();

		public:
			bool Initialise();
			void Close();
			CURLcode OpenUrl( std::string const & p_url, size_t ( * p_function )( void *, size_t, size_t, void * ), void * p_data, std::string const & p_postParams = std::string() );

			CURLcode GetStringFromUrl( std::string const & p_url, std::string & p_contents, std::string const & p_postParams = std::string() );
			CURLcode DownloadFile( std::string const & p_url, std::string const & p_filename, std::string const & p_postParams = std::string() );

			std::string GetLastContentType();
			std::string GetContentType( std::string const & p_url, std::string const & p_postParams = std::string() );

			CURLcode SetCookieFile( char const * const p_filename );
			CURLcode SetCookieString( char const * const p_cookieString );

		public:
			static std::string CurlError( CURLcode p_errorCode );
			static std::string MimeEncode( std::string const & p_string );

		private:
			static size_t doGetString( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
			static size_t doDownload( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
			static size_t doNull( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );

		private:
			CURL * m_handle{ nullptr };
			bool m_initialised{ false };
		};
	}
}

#endif
