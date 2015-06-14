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
#include "Config.h"

#if GENLIB_WINDOWS
#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#	include <windows.h>
#pragma warning( pop )
#endif

#include "FileVersion.h"
#include "StringConverter.h"

using namespace General::Utils;

bool General::Computer::GetFileVersion( const std::string & p_fileName, std::string & p_version )
{
#if GENLIB_WINDOWS
	VS_FIXEDFILEINFO * l_fileInfo;
	DWORD l_handle;
	UINT l_length = GetFileVersionInfoSizeA( p_fileName.c_str(), &l_handle );
	p_version = "0.0.0.0";

	if ( l_length == 0 )
	{
		return false;
	}

	char * l_buffer = new char[l_length];

	if ( l_buffer == NULL )
	{
		return false;
	}

	if ( !GetFileVersionInfoA( p_fileName.c_str(), l_handle, l_length, l_buffer ) )
	{
		delete [] l_buffer;
		return false;
	}

	if ( VerQueryValueA( l_buffer, "\\", reinterpret_cast< void ** >( &l_fileInfo ), &l_length ) )
	{
		p_version = ToString( HIWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToString( LOWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToString( HIWORD( l_fileInfo->dwFileVersionLS ) );
		p_version.push_back( '.' );
		p_version += ToString( LOWORD( l_fileInfo->dwFileVersionLS ) );
		delete [] l_buffer;
		return true;
	}

	delete [] l_buffer;
#endif
	return false;
}

bool General::Computer::GetFileVersion( const std::wstring & p_fileName, std::wstring & p_version )
{
#if GENLIB_WINDOWS
	VS_FIXEDFILEINFO * l_fileInfo;
	DWORD l_handle;
	UINT l_length = GetFileVersionInfoSizeW( p_fileName.c_str(), &l_handle );
	p_version = L"0.0.0.0";

	if ( l_length == 0 )
	{
		return false;
	}

	wchar_t * l_buffer = new wchar_t[l_length];

	if ( l_buffer == NULL )
	{
		return false;
	}

	if ( !GetFileVersionInfoW( p_fileName.c_str(), l_handle, l_length, l_buffer ) )
	{
		delete [] l_buffer;
		return false;
	}

	if ( VerQueryValueW( l_buffer, L"\\", reinterpret_cast <void **>( &l_fileInfo ), &l_length ) )
	{
		p_version = ToWString( HIWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToWString( LOWORD( l_fileInfo->dwFileVersionMS ) );
		p_version.push_back( '.' );
		p_version += ToWString( HIWORD( l_fileInfo->dwFileVersionLS ) );
		p_version.push_back( '.' );
		p_version += ToWString( LOWORD( l_fileInfo->dwFileVersionLS ) );
		delete [] l_buffer;
		return true;
	}

	delete [] l_buffer;
#endif
	return false;
}
