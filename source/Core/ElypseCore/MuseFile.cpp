/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "MuseFile.h"
#include "DataFile.h"
#include "ConfigFile.h"

#include "ElypseController.h"
#include "ElypseLoadingBar.h"

#include "ElypseLogs.h"

#include <Utils.h>

namespace Elypse::Data
{
	void MuseFileDeleter::operator()( MuseFile * ptr )const noexcept
	{
		delete ptr;
	}

	MuseFile::MuseFile( String const & p_name )
		: named( p_name )
		, m_downloader( *this, p_name )
	{
	}

	bool MuseFile::WaitForFile( String const & p_filename, bool p_useLoadingBar )
	{
		genlib_assert( !p_filename.empty() );

		if ( p_useLoadingBar )
		{
			ElypseController::GetSingletonPtr()->GetLoadingBar()->StartDownloadOfFile();
		}

		while ( !m_downloader.IsInitialised() )
		{
			std::this_thread::sleep_for( 5ms );
		}

		if ( m_downloader.CantDownload() )
		{
			return false;
		}

		size_t l_index = m_downloader.GetFileIndex( p_filename );

		if ( l_index == String::npos )
		{
			if ( p_useLoadingBar )
			{
				ElypseController::GetSingletonPtr()->GetLoadingBar()->EndDownloadOfFile();
			}

			EMUSE_LOG_MESSAGE_RELEASE( "MuseFile::WaitForFile - File " + p_filename + " not found within package " + m_name );
			return false;
		}

		while ( !m_downloader.IsDownloadFinished( uint32_t( l_index ) ) )
		{
			std::this_thread::sleep_for( 50ms );

			if ( p_useLoadingBar )
			{
				ElypseController::GetSingletonPtr()->GetLoadingBar()->setPercent( m_downloader.GetPercent(), m_downloader.GetDownloadSpeed() );
			}
		}

		ElypseController::GetSingletonPtr()->GetLoadingBar()->EndDownloadOfFile();
		return true;
	}

	Path MuseFile::GetCompletePath()const
	{
		return ElypseController::GetSingletonPtr()->GetInstallPath() / "rsc" / m_downloader.GetPath();
	}

	DataFile * MuseFile::GetDataFile( String const & p_filename )
	{
		auto l_lock = make_unique_lock( m_mutex );
		return General::Utils::map::insert( m_dataFiles, p_filename, p_filename, *this );
	}

	ConfigFile * MuseFile::GetConfigFile( String const & p_filename )
	{
		auto l_lock = make_unique_lock( m_mutex );
		ConfigFile * l_configFile = General::Utils::map::insert( m_configFiles, p_filename, p_filename, *this );
		return l_configFile;
	}
}
