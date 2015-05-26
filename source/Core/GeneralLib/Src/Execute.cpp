#include <stdlib.h>
#include <iostream>

#include "Execute.h"

#if GENLIB_WINDOWS
#pragma warning( push )
#pragma warning( disable:4267 )
#pragma warning( disable:4312 )
#pragma warning( disable:4311 )
#	include <windows.h>
#pragma warning( pop )
#endif

#include "StringConverter.h"

bool General :: Utils :: Execute(	const std::string & p_name,
									const std::string & p_path,
									const std::string & p_params,
									ExecuteStyle p_style )
{
	if ( p_style == ES_SYSTEM )
	{
		return ( system( ( "\"" + p_path + p_name + "\" " + p_params ).c_str() ) == 0 );
	}

	if ( p_style == ES_SHELL )
	{
#if GENLIB_WINDOWS
		return ( 32 < reinterpret_cast< DWORD_PTR >( ShellExecuteA( NULL, NULL, ( p_path + p_name ).c_str(), ( p_params.empty() ? NULL : p_params.c_str() ), NULL, SW_SHOW ) ) );
#else
		return ( system( ( "\"" + p_path + p_name + "\" " + p_params + " &" ).c_str() ) == 0 );
#endif
	}

	if ( p_style == ES_PROCESS )
	{
#if  GENLIB_WINDOWS
		STARTUPINFOA l_startupInfo;
		PROCESS_INFORMATION l_processInfo;
		memset( & l_startupInfo, 0, sizeof( l_startupInfo ) );
		memset( & l_processInfo, 0, sizeof( l_processInfo ) );
		l_startupInfo.cb = sizeof( l_startupInfo );

		if ( CreateProcessA( NULL, const_cast <char *>( ( "\"" + p_path + p_name + "\" " + p_params ).c_str() ), 0, 0, false, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, & l_startupInfo, & l_processInfo ) == TRUE )
		{
			return ( WaitForSingleObject( l_processInfo.hProcess, INFINITE ) == WAIT_OBJECT_0 );
		}

#else
		return ( system( ( "\"" + p_path + p_name + "\" " + p_params + " &" ).c_str() ) == 0 );
#endif
	}

	return false;
}

#ifndef __GNUG__

bool General :: Utils :: Execute(	const std::wstring & p_name,
									const std::wstring & p_path,
									const std::wstring & p_params,
									ExecuteStyle p_style )
{
	std::wcout << L"GeneralLib//Execute : " << p_name << " // " << p_path << " // " << p_params << std::endl;

	if ( p_style == ES_SYSTEM )
	{
		if ( ! p_path.empty() )
		{
			wchar_t l_oldPath[256];
			GetCurrentDirectoryW( 255, l_oldPath );
			SetCurrentDirectoryW( p_path.c_str() );
			std::wstring l_string = p_name + L" " + p_params;
			int l_result = _wsystem( l_string.c_str() );
			SetCurrentDirectoryW( l_oldPath );
			return ( l_result == 0 );
		}

		return _wsystem( ( p_name + L" " + p_params ).c_str() ) == 0;
	}

	if ( p_style == ES_SHELL )
	{
		return ( 32 < reinterpret_cast< DWORD_PTR >( ShellExecuteW( NULL, NULL, ( p_path + p_name ).c_str(), ( p_params.empty() ? NULL : p_params.c_str() ), NULL, SW_SHOW ) ) );
	}

	if ( p_style == ES_PROCESS )
	{
		STARTUPINFOW l_startupInfo;
		PROCESS_INFORMATION l_processInfo;
		memset( & l_startupInfo, 0, sizeof( l_startupInfo ) );
		memset( & l_processInfo, 0, sizeof( l_processInfo ) );
		l_startupInfo.cb = sizeof( l_startupInfo );

		if ( CreateProcessW( NULL, const_cast <wchar_t *>( ( L"\"" + p_path + p_name + L"\" " + p_params ).c_str() ), 0, 0, false, CREATE_DEFAULT_ERROR_MODE, NULL, NULL, & l_startupInfo, & l_processInfo ) == TRUE )
		{
			return ( WaitForSingleObject( l_processInfo.hProcess, INFINITE ) == WAIT_OBJECT_0 );
		}
	}

	return false;
}

#endif
