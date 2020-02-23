/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "DataFile.h"
#include "MuseFile.h"

#include "ElypseController.h"
#include "DownloadManager.h"
#include "ElypseResourceGroupManager.h"

#include <OgreResourceGroupManager.h>
#include <OgreException.h>

#include <Zip.h>
#include <File.h>
#include <ZipArchive.h>

#include "ElypseLogs.h"

using namespace General::Files;
using namespace General::Theory;
using namespace General::Utils;

namespace Elypse::Data
{
	void DataFileDeleter::operator()( DataFile * ptr )const noexcept
	{
		delete ptr;
	}

	DataFile::DataFile( String const & p_name, MuseFile & p_owner )
		: named( p_name )
		, owned_by( p_owner )
		, m_initialised( false )
		, m_downloaded( false )
		, m_preInit( false )
	{
	}

	DataFile::~DataFile()
	{
		String l_curPrefix = ElypseResourceGroupManager::getPrefix();

		for ( auto & i : m_loadedBy )
		{
			ElypseResourceGroupManager::setPrefix( i.first );
			_unload();
		}

		m_loadedBy.clear();
		ElypseResourceGroupManager::setPrefix( l_curPrefix );
	}

	String DataFile::GetDescriptiveName()const
	{
		return GetOwner()->GetName() + " > " + m_name;
	}

	void DataFile::_load()
	{
		m_mutex.lock();

		if ( !m_downloaded )
		{
			m_mutex.unlock();
			GetOwner()->WaitForFile( m_name, true );
			m_mutex.lock();
		}

		const Path & l_fullPath = GetOwner()->GetCompletePath();
		const Path & l_extractPath = l_fullPath / ( m_name + "_" );

		if ( !m_preInit )
		{
			m_preInit = true;

			if ( !DirectoryExists( l_extractPath ) )
			{
				ZipArchive * l_archive = new ZipArchive( m_name, l_fullPath );

				if ( !l_archive->Extract( l_extractPath ) )
				{
					EMUSE_LOG_MESSAGE_RELEASE( "Cannot extract zip [" + m_name + "] for some reason" );
				}

				delete l_archive;
			}
		}

		EMUSE_LOG_MESSAGE_RELEASE( "Load group : " + ElypseResourceGroupManager::getPrefix() + m_name );
		ElypseResourceGroupManager::addResourceLocation( l_extractPath, "FileSystem", m_name );
		ElypseResourceGroupManager::initialiseResourceGroup( m_name );

		try
		{
			ElypseResourceGroupManager::loadResourceGroup( m_name, true, false );
		}
		catch ( Exception & p_exception )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "Exception during data load in pack [" + m_name + "] : " + p_exception.getFullDescription() );
		}

		m_mutex.unlock();
	}

	void DataFile::_unload()
	{
		ElypseResourceGroupManager::unloadResourceGroup( m_name, true );
	}

	void DataFile::Use()
	{
		if ( !map::has( m_loadedBy, ElypseResourceGroupManager::getPrefix() ) )
		{
			m_loadedBy.insert( std::make_pair( ElypseResourceGroupManager::getPrefix(), 1 ) );
			_load();
		}
		else
		{
			m_loadedBy[ElypseResourceGroupManager::getPrefix()] ++;
		}
	}

	void DataFile::Release()
	{
		UIntMap::iterator l_find = m_loadedBy.find( ElypseResourceGroupManager::getPrefix() );
		l_find->second--;

		if ( l_find->second == 0 )
		{
			_unload();
			m_loadedBy.erase( l_find );
		}
	}

	void DataFile::DownloadFinished()
	{
		auto l_lock = make_unique_lock( m_mutex );
		m_downloaded = true;
	}
}
