/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "MuseDownloader.h"
#include "DownloadManager.h"

#include "DataFile.h"
#include "ConfigFile.h"

#include "ScriptNode.h"

#include "MuseFile.h"
#include "MusePack.h"

#include "ElypseLogs.h"

#include <iostream>

#include <zlib.h>

#include <OgreLog.h>
#include <OgreStringConverter.h>

#include <File.h>
#include <PreciseTimer.h>
#include <NeoCurl.h>

extern Ogre::String g_programFilesDirectory;

using namespace General::Files;
using namespace General::Utils;

namespace Elypse::Download
{
	MuseDownloader::MuseDownloader( MuseFile & p_owner, const Path & p_basePath )
		: named( p_basePath )
		, owned_by( p_owner )
	{
		EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::base path : " + p_basePath );
		m_basePath = p_basePath.substr( 0, p_basePath.find_last_of( "." ) );
	}

	MuseDownloader::~MuseDownloader()
	{
		m_musePack.reset();

		if ( m_thread )
		{
			m_thread->join();
			delete m_thread;
		}
	}

	void MuseDownloader::SetURL( Url const & p_url )
	{
		m_url = p_url;
		m_stopThread = false;
		m_cantDownload = false;
		m_initialised = false;
		m_musePack.reset();
	}

	void MuseDownloader::StartDownload()
	{
		m_musePack = MusePackPtr{ new MusePack{ *this, m_installPath, m_basePath, m_verifyHeader } };
		m_thread = new std::thread{ [this]()
		{
			Execute();
		} };
	}

	void MuseDownloader::StartDownload( std::function< void( MuseFile * ) > onEnd )
	{
		m_musePack = MusePackPtr{ new MusePack{ *this, m_installPath, m_basePath, m_verifyHeader } };
		Execute();
		onEnd( GetOwner() );
	}

	void MuseDownloader::Execute()
	{
		PreciseTimer l_timer;
		EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::Execute -> curl : " + m_url );

		if ( m_url.GetProtocol() == UrlProtocol::LOCAL )
		{
			Path l_path{ m_url.GetBaseSite() };
			l_path /= m_url.GetDirectory();
			l_path /= m_url.GetFilename();
			auto l_error = m_fileManager.OpenFile( l_path, &MusePack::StCurlCallback, m_musePack.get() );

			if ( l_error != FileError::SUCCESS && !m_musePack->IsOK() )
			{
				EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::Execute -> file error happened : " + CURLManager::CurlError( l_error ) );
				m_cantDownload = true;
				return;
			}
		}
		else
		{
			CURLcode l_error = m_curlManager.OpenUrl( m_url, &MusePack::StCurlCallback, m_musePack.get() );

			if ( l_error != CURLE_OK && !m_musePack->IsOK() )
			{
				EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::Execute -> curl error happened : " + CURLManager::CurlError( l_error ) );
				m_cantDownload = true;
				return;
			}
		}

		EMUSE_LOG_MESSAGE_RELEASE( "MuseDownloader - " + m_url + " finished, downloaded in " + StringConverter::toString( Real( l_timer.Time() ) ) + " seconds" );
		m_downloaded = true;
	}

	String MuseDownloader::GetExtension( String const & p_fileurl )
	{
		size_t l_index = p_fileurl.find_last_of( "." );

		if ( l_index == String::npos )
		{
			return EMPTY_STRING;
		}

		return p_fileurl.substr( l_index, String::npos );
	}

	DataBlockType MuseDownloader::GetFileType( String const & p_filename )
	{
		String l_extension = GetExtension( p_filename );

		if ( l_extension.empty() )
		{
			return EM_BLOCK_ERROR;
		}

		if ( l_extension == String( "emscene" ) )
		{
			return EM_BLOCK_SCENE;
		}

		if ( l_extension == String( "emscript" ) )
		{
			return EM_BLOCK_INTERACTIONS;
		}

		if ( l_extension == String( "zip" ) )
		{
			return EM_BLOCK_ZIPDATA;
		}

		if ( l_extension == String( "gzip" ) )
		{
			return EM_BLOCK_GZIPDATA;
		}

		if ( l_extension == String( "cfg" ) )
		{
			return EM_BLOCK_CONFIG;
		}

		if ( l_extension == String( "emcfg" ) )
		{
			return EM_BLOCK_EMCONFIG;
		}

		if ( l_extension == String( "emsnd" ) )
		{
			return EM_BLOCK_ZIPSNDDATA;
		}

		return EM_BLOCK_ERROR;
	}

	Real MuseDownloader::GetPercent()const
	{
		return m_musePack->GetPercent();
	}

	Real MuseDownloader::GetDownloadSpeed()const
	{
		return m_musePack->GetDownloadSpeed();
	}

	uint32_t MuseDownloader::GetNbBlocks()const
	{
		return m_musePack->GetNbBlocks();
	}

	String const & MuseDownloader::GetFileName( uint32_t p_index )const
	{
		return m_musePack->GetFileName( p_index );
	}

	size_t MuseDownloader::GetFileIndex( String const & p_name )const
	{
		return m_musePack->GetFileIndex( p_name );
	}

	bool MuseDownloader::IsDownloadFinished( uint32_t p_index )const
	{
		return m_musePack->IsDownloadFinished( p_index );
	}
}
