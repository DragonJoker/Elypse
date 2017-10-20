/*
See LICENSE file in root folder
*/
#include "DownloadableFile.h"

#include "File.h"
#include "MD5.h"
#include "StringConverter.h"

#include <iostream>

using namespace General::Utils;

DownloadableFile::DownloadableFile( std::wstring const & p_name, std::wstring const & p_path )
	: m_name{ p_name }
	, m_path{ p_path }
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

	if ( doExists() )
	{
		doGetHash();
	}
}

DownloadableFile::DownloadableFile( std::wstring const & p_name, WStringArray const & p_multiPath )
	: m_name{ p_name }
	, m_multiPath{ p_multiPath }
	, m_usesMultiPath{ true }
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

	if ( m_multiPath.size() < 2 )
	{
		m_usesMultiPath = false;

		if ( !m_multiPath.empty() )
		{
			m_path = m_multiPath[0];
			m_multiPath.clear();
		}
	}

	if ( doExists() )
	{
		doGetHash();
	}
}

DownloadableFile::~DownloadableFile()
{
}

bool DownloadableFile::doExists()
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

void DownloadableFile::doGetHash()
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

bool DownloadableFile::ExistsAndIsIdentical( std::string const & p_hash )
{
	if ( !m_exists )
	{
		return false;
	}

	if ( m_hash.empty() || p_hash.empty() )
	{
		return false;
	}

	return p_hash == m_hash;
}
