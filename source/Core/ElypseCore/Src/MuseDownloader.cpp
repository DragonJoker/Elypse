/*********************************************************************************************************************

	Author :	Sylvain DOREMUS

	Company :	ForDev Studio - Copyright 2006

	Files :		DownloadManager.h - DownloadManager.cpp

	Desc :		Class used to stream the datas used for a 3D scene.

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "MuseDownloader.h"
#include "DownloadManager.h"

#include "DataFile.h"
#include "ConfigFile.h"

#include "MusePack.h"

#include "EMuseLogs.h"

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

MuseDownloader::MuseDownloader( MuseFile * p_owner, const Path & p_basePath )
	:	named( p_basePath ),
		owned_by( p_owner ),
		m_cantDownload( false ),
		m_initialised( false ),
		m_stopThread( false ),
		m_verifyHeader( true ),
		m_musePack( NULL ),
		m_curlManager( new CURLManager ),
		m_thread( NULL )
{
	EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::base path : " + p_basePath );
	m_basePath = p_basePath.substr( 0, p_basePath.find_last_of( "." ) );
}

MuseDownloader::~MuseDownloader()
{
	if ( m_musePack != NULL )
	{
		delete m_musePack;
		m_musePack = NULL;
	}

	if ( m_thread )
	{
		m_thread->join();
		delete m_thread;
	}

	delete m_curlManager;
}

void MuseDownloader::SetURL( const String & p_url )
{
	m_url = p_url;
	m_stopThread = false;
	m_cantDownload = false;
	m_initialised = false;

	if ( m_musePack != NULL )
	{
		delete m_musePack;
		m_musePack = NULL;
	}
}

void MuseDownloader::StartDownload()
{
	m_musePack = new MusePack( this, m_installPath, m_basePath, m_verifyHeader );
	m_thread = GENLIB_THREAD_CREATE_MEMBER_FUNC_THREAD( MuseDownloader, this, Execute );
}

void MuseDownloader::Execute()
{
	PreciseTimer l_timer;
	EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::Execute -> curl : " + m_url );
	CURLcode l_error = m_curlManager->OpenUrl( m_url, MusePack::CurlCallback, m_musePack );

	if ( l_error != CURLE_OK && ! m_musePack->IsOK() )
	{
		EMUSE_CONSOLE_MESSAGE_DEBUG( "MuseDownloader::Execute -> curl error happened : " + CURLManager::CurlError( l_error ) );
		m_cantDownload = true;
		return;
	}

	EMUSE_LOG_MESSAGE_RELEASE( "MuseDownloader - " + m_url + " finished, downloaded in " + StringConverter::toString( static_cast <Real>( l_timer.Time() ) ) + " seconds" );
}

String MuseDownloader::GetExtension( const String & p_fileurl )
{
	size_t l_index = p_fileurl.find_last_of( "." );

	if ( l_index == String::npos )
	{
		return EMPTY_STRING;
	}

	return p_fileurl.substr( l_index, String::npos );
}

DataBlockType MuseDownloader::GetFileType( const String & p_filename )
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
		return EM_BLOCK_CONFIG;
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

unsigned int MuseDownloader::GetNbBlocks()const
{
	return m_musePack->GetNbBlocks();
}

const String & MuseDownloader::GetFileName( unsigned int p_index )const
{
	return m_musePack->GetFileName( p_index );
}

size_t MuseDownloader::GetFileIndex( const String & p_name )const
{
	return m_musePack->GetFileIndex( p_name );
}

bool MuseDownloader::IsDownloadFinished( unsigned int p_index )const
{
	return m_musePack->IsDownloadFinished( p_index );
}
