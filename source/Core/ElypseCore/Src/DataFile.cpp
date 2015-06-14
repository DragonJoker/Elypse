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

DataFile::DataFile( const String & p_name, MuseFile * p_owner )
	: named( p_name ),
		owned_by( p_owner ),
		m_initialised( false ),
		m_downloaded( false ),
		m_preInit( false )
{
}

DataFile::~DataFile()
{
	UIntMap::const_iterator i = m_loadedBy.begin();
	const UIntMap::const_iterator & iend = m_loadedBy.end();
	String l_curPrefix = ElypseResourceGroupManager::getPrefix();

	for ( ; i != iend; ++ i )
	{
		ElypseResourceGroupManager::setPrefix( i->first );
		_unload();
	}

	m_loadedBy.clear();
	ElypseResourceGroupManager::setPrefix( l_curPrefix );
}

String DataFile::GetDescriptiveName()const
{
	return m_owner->GetName() + " > " + m_name;
}

void DataFile::_load()
{
	GENLIB_LOCK_MUTEX( m_mutex );

	if ( ! m_downloaded )
	{
		GENLIB_UNLOCK_MUTEX( m_mutex );
		m_owner->WaitForFile( m_name, true );
		GENLIB_LOCK_MUTEX( m_mutex );
	}

	const Path & l_fullPath = m_owner->GetCompletePath();
	const Path & l_extractPath = l_fullPath / ( m_name + "_" );

	if ( ! m_preInit )
	{
		m_preInit = true;

		if ( ! DirectoryExists( l_extractPath ) )
		{
			ZipArchive * l_archive = new ZipArchive( m_name, l_fullPath );

			if ( ! l_archive->Extract( l_extractPath ) )
			{
				EMUSE_LOG_MESSAGE_RELEASE( "Cannot extract zip [" + m_name + "] for some reason" );
			}

			delete l_archive;
		}
	}

//	if ( ! m_initialised)
//	{
	EMUSE_LOG_MESSAGE_RELEASE( "Load group : " + ElypseResourceGroupManager::getPrefix() + m_name );
	ElypseResourceGroupManager::addResourceLocation( l_extractPath, "FileSystem", m_name );
	ElypseResourceGroupManager::initialiseResourceGroup( m_name );
//		m_initialised = true;
//	}

	try
	{
		ElypseResourceGroupManager::loadResourceGroup( m_name, true, false );
	}
	catch ( Exception & p_exception )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Exception during data load in pack [" + m_name + "] : " + p_exception.getFullDescription() );
	}

	GENLIB_UNLOCK_MUTEX( m_mutex );
}

void DataFile::_unload()
{
	ElypseResourceGroupManager::unloadResourceGroup( m_name, true );
}

void DataFile::Use()
{
	if ( ! map::has( m_loadedBy, ElypseResourceGroupManager::getPrefix() ) )
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
	GENLIB_AUTO_SCOPED_LOCK();
	m_downloaded = true;
}
