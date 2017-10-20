/*
See LICENSE file in root folder
*/
#include "Config.h"

#if GENLIB_WINDOWS
#	pragma warning( push )
#	pragma warning( disable:4267 )
#	pragma warning( disable:4312 )
#	pragma warning( disable:4311 )
#		include <windows.h>
#	pragma warning( pop )
#endif

#include "FileVersion.h"
#include "StringConverter.h"

using namespace General::Utils;

bool General::Computer::GetFileVersion( std::string const & p_fileName, std::string & p_version )
{
#if GENLIB_WINDOWS

	VS_FIXEDFILEINFO * l_fileInfo{ 0 };
	DWORD l_handle{ 0 };
	UINT l_length = ::GetFileVersionInfoSizeA( p_fileName.c_str(), &l_handle );
	p_version = "0.0.0.0";

	if ( l_length == 0 )
	{
		return false;
	}

	std::vector< char > l_buffer;
	l_buffer.resize( size_t( l_length ) );

	if ( !::GetFileVersionInfoA( p_fileName.c_str(), l_handle, l_length, l_buffer.data() ) )
	{
		return false;
	}

	if ( ::VerQueryValueA( l_buffer.data(), "\\", reinterpret_cast< void ** >( &l_fileInfo ), &l_length ) )
	{
		p_version = ToString( HIWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToString( LOWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToString( HIWORD( l_fileInfo->dwFileVersionLS ) );
		p_version.push_back( '.' );
		p_version += ToString( LOWORD( l_fileInfo->dwFileVersionLS ) );
		return true;
	}

#endif

	return false;
}

bool General::Computer::GetFileVersion( std::wstring const & p_fileName, std::wstring & p_version )
{
#if GENLIB_WINDOWS

	VS_FIXEDFILEINFO * l_fileInfo{ 0 };
	DWORD l_handle{ 0 };
	UINT l_length = ::GetFileVersionInfoSizeW( p_fileName.c_str(), &l_handle );
	p_version = L"0.0.0.0";

	if ( l_length == 0 )
	{
		return false;
	}

	std::vector< wchar_t > l_buffer;
	l_buffer.resize( size_t( l_length ) );

	if ( !::GetFileVersionInfoW( p_fileName.c_str(), l_handle, l_length, l_buffer.data() ) )
	{
		return false;
	}

	if ( ::VerQueryValueW( l_buffer.data(), L"\\", reinterpret_cast< void ** >( &l_fileInfo ), &l_length ) )
	{
		p_version = ToWString( HIWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToWString( LOWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToWString( HIWORD( l_fileInfo->dwFileVersionLS ) );
		p_version.push_back( '.' );
		p_version += ToWString( LOWORD( l_fileInfo->dwFileVersionLS ) );
		return true;
	}

#endif

	return false;
}
