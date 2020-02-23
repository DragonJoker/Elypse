/*
See LICENSE file in root folder
*/
#ifndef ___General_FileManager_H___
#define ___General_FileManager_H___

#include "Config.h"

#include "Url.h"

#include <string>
#include <functional>

namespace General::Files
{
	enum class FileError
	{
		SUCCESS,
		OPEN,
		READ,
		COPY,
	};

	class FileManager
	{
	public:
		FileManager();
		~FileManager();

		FileError OpenFile( Utils::Path const & p_path
			, size_t ( * p_function )( void *, size_t, size_t, void * )
			, void * p_data
			, std::string const & p_postParams = std::string() );
		FileError GetStringFromFile( Utils::Path const & p_path
			, std::string & p_contents
			, std::string const & p_postParams = std::string() );
		FileError DownloadFile( Utils::Path const & p_path
			, std::string const & p_filename
			, std::string const & p_postParams = std::string() );

		static std::string GetName( FileError p_errorCode );

	private:
		static size_t doGetString( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
		static size_t doDownload( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
		static size_t doNull( void * p_buffer, size_t p_size, size_t p_numItems, void * p_data );
	};
}

#endif
