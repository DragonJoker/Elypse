#include "PrecompiledHeader.h"

#include "MuseFile.h"
#include "DataFile.h"
#include "ConfigFile.h"

#include "MuseDownloader.h"
#include "EMuseController.h"
#include "EMuseLoadingBar.h"

#include "EMuseLogs.h"

#include <Utils.h>

MuseFile::MuseFile( const String & p_name )
	:	named( p_name )
{
	m_downloader = new MuseDownloader( this, p_name );
}

MuseFile::~MuseFile()
{
	General::Utils::map::deleteAll( m_dataFiles );
	General::Utils::map::deleteAll( m_configFiles );
	delete m_downloader;
}

bool MuseFile::WaitForFile( const String & p_filename, bool p_useLoadingBar )
{
	genlib_assert( ! p_filename.empty() );

	if ( p_useLoadingBar )
	{
		EMuseController::GetSingletonPtr()->GetLoadingBar()->StartDownloadOfFile();
	}

	while ( ! m_downloader->IsInitialised() )
	{
		GENLIB_SLEEP( 5 );
	}

	if ( m_downloader->CantDownload() )
	{
		return false;
	}

	size_t l_index = m_downloader->GetFileIndex( p_filename );

	if ( l_index == String::npos )
	{
		if ( p_useLoadingBar )
		{
			EMuseController::GetSingletonPtr()->GetLoadingBar()->EndDownloadOfFile();
		}

		EMUSE_LOG_MESSAGE_RELEASE( "MuseFile::WaitForFile - File " + p_filename + " not found within package " + m_name );
		return false;
	}

	while ( ! m_downloader->IsDownloadFinished( uint32_t( l_index ) ) )
	{
		GENLIB_SLEEP( 50 );

		if ( p_useLoadingBar )
		{
			EMuseController::GetSingletonPtr()->GetLoadingBar()->setPercent( m_downloader->GetPercent(), m_downloader->GetDownloadSpeed() );
		}
	}

	EMuseController::GetSingletonPtr()->GetLoadingBar()->EndDownloadOfFile();
	return true;
}

Path MuseFile::GetCompletePath()const
{
	return EMuseController::GetSingletonPtr()->GetInstallPath() / "rsc" / m_downloader->GetPath();
}

DataFile * MuseFile::GetDataFile( const String & p_filename )
{
	GENLIB_AUTO_SCOPED_LOCK();
	return General::Utils::map::insert( m_dataFiles, p_filename, p_filename, this );
}

ConfigFile * MuseFile::GetConfigFile( const String & p_filename )
{
	GENLIB_AUTO_SCOPED_LOCK();
	ConfigFile * l_configFile = General::Utils::map::insert( m_configFiles, p_filename, p_filename, this );
	return l_configFile;
}
