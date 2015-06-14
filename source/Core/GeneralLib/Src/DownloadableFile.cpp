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
#include "DownloadableFile.h"

#include "File.h"
#include "MD5.h"
#include "StringConverter.h"

#include <iostream>

using namespace General::Utils;

DownloadableFile::DownloadableFile( const std::wstring & p_name, const std::wstring & p_path )
	: m_name( p_name )
	, m_path( p_path )
	, m_executable( false )
	, m_zipped( false )
	, m_usesMultiPath( false )
	, m_toExec( false )
{
	if ( p_name.length() > 4 )
	{
		m_executable = ( p_name.substr( p_name.length() - 4, 4 ) == L".exe" );
		m_zipped = ( p_name.substr( p_name.length() - 4, 4 ) == L".zip" );

		if ( m_zipped )
		{
			m_name.erase( p_name.length() - 4, 4 );
		}
	}

	if ( _exists() )
	{
		_getHash();
	}
}

DownloadableFile::DownloadableFile( const std::wstring & p_name, const WStringArray & p_multiPath )
	: m_name( p_name )
	, m_multiPath( p_multiPath )
	, m_executable( false )
	, m_zipped( false )
	, m_usesMultiPath( true )
	, m_toExec( false )
{
	if ( p_name.length() > 4 )
	{
		m_executable = ( p_name.substr( p_name.length() - 4, 4 ) == L".exe" );
		m_zipped = ( p_name.substr( p_name.length() - 4, 4 ) == L".zip" );

		if ( m_zipped )
		{
			m_name.erase( p_name.length() - 4, 4 );
		}
	}

	if ( m_multiPath.empty() )
	{
		m_usesMultiPath = false;
	}
	else if ( m_multiPath.size() == 1 )
	{
		m_usesMultiPath = false;
		m_path = m_multiPath[0];
		m_multiPath.clear();
	}

	if ( _exists() )
	{
		_getHash();
	}
}

DownloadableFile::~DownloadableFile()
{
}

bool DownloadableFile::_exists()
{
	if ( !m_usesMultiPath )
	{
		m_exists = FileExists( m_path + m_name );
	}
	else
	{
		m_exists = true;

		for ( auto && l_it : m_multiPath )
		{
			m_exists = FileExists( l_it + m_name );
		}
	}

	return m_exists;
}

void DownloadableFile::_getHash()
{
	if ( m_exists )
	{
		if ( !m_usesMultiPath )
		{
			d_coucou;
			m_hash = MD5::HashFile( ToString( m_path + m_name ) );
			d_coucou;
			return;
		}

		m_hash = MD5::HashFile( ToString( m_multiPath[0] + m_name ) );

		for ( auto && l_it : m_multiPath )
		{
			std::string l_hash;
			d_coucou;
			l_hash = MD5::HashFile( ToString( l_it + m_name ) );
			d_coucou;

			if ( l_hash != m_hash )
			{
				m_hash.clear();
				return;
			}
		}
	}
}

bool DownloadableFile::ExistsAndIsIdentical( const std::string & p_hash )
{
	if ( ! m_exists )
	{
		return false;
	}

	if ( m_hash.empty() || p_hash.empty() )
	{
		return false;
	}

	return p_hash == m_hash;
}
