#include "DownloadableFile.h"

#include "File.h"
#include "MD5.h"
#include "StringConverter.h"

#include <iostream>

using namespace General::Utils;

DownloadableFile :: DownloadableFile(	const std::wstring & p_name,
										const std::wstring & p_path )
	:	m_name( p_name ),
		m_path( p_path ),
		m_executable( false ),
		m_zipped( false ),
		m_usesMultiPath( false ),
		m_toExec( false )
{
	if ( p_name.length() > 4 )
	{
		m_executable = ( p_name.substr( p_name.length() - 4, 4 ) == L".exe" );
		m_zipped = ( p_name.substr( p_name.length() - 4, 4 ) == L".zip" );

//		std::cout<<"m_name : "<<m_name<<" m_executable : "<<(m_executable?"oui":"non")<<std::endl;
//		std::cout<<"m_name : "<<m_name<<" m_zipped : "<<(m_zipped?"oui":"non")<<std::endl;

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

DownloadableFile :: DownloadableFile(	const std::wstring & p_name,
										const WStringArray & p_multiPath )
	:	m_name( p_name ),
		m_multiPath( p_multiPath ),
		m_executable( false ),
		m_zipped( false ),
		m_usesMultiPath( true ),
		m_toExec( false )
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

DownloadableFile :: ~DownloadableFile()
{
}

bool DownloadableFile :: _exists()
{
	if ( ! m_usesMultiPath )
	{
		m_exists = FileExists( m_path + m_name );
	}
	else
	{
		m_exists = true;

		for ( size_t i = 0 ; i < m_multiPath.size() ; i ++ )
		{
			m_exists = FileExists( m_multiPath[i] + m_name );
		}
	}

	return m_exists;
}

void DownloadableFile :: _getHash()
{
	if ( m_exists )
	{
		if ( ! m_usesMultiPath )
		{
			d_coucou;
			m_hash = MD5::HashFile( ToString( m_path + m_name ) );
			d_coucou;
			return;
		}

		m_hash = MD5::HashFile( ToString( m_multiPath[0] + m_name ) );

		for ( size_t i = 1 ; i < m_multiPath.size() ; i ++ )
		{
			std::string l_hash;
			d_coucou;
			l_hash = MD5::HashFile( ToString( m_multiPath[i] + m_name ) );
			d_coucou;

			if ( l_hash != m_hash )
			{
				m_hash.clear();
				return;
			}
		}
	}
}

bool DownloadableFile :: ExistsAndIsIdentical( const std::string & p_hash )
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