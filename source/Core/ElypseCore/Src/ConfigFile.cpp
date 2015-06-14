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

#include "ConfigFile.h"
#include "MuseFile.h"

#include "ScriptNode.h"

//#include "ElypseController.h"
#include "DownloadManager.h"

#include "ElypseLogs.h"

ConfigFile::ConfigFile()
	: ConfigFile( String(), NULL )
{
}

ConfigFile::ConfigFile( const String & p_name, MuseFile * p_owner )
	: named( p_name )
	, owned_by( p_owner )
	, m_useCount( 0 )
	, m_downloaded( false )
{
}

ConfigFile::~ConfigFile()
{
}

void ConfigFile::Use()
{
	if ( !m_useCount )
	{
		DoLoad();
	}

	m_useCount++;
}

void ConfigFile::Release()
{
	m_useCount--;
}

void ConfigFile::DownloadFinished()
{
	GENLIB_AUTO_SCOPED_LOCK();
	m_downloaded = true;
}

String ConfigFile::GetDescriptiveName()const
{
	return m_owner->GetName() + " > " + m_name;
}

void ConfigFile::DoLoad()
{
	GENLIB_AUTO_SCOPED_LOCK();
	EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Load - " + m_name + "\n" );

	if ( !m_downloaded )
	{
		GENLIB_UNLOCK_MUTEX( m_mutex );
		m_owner->WaitForFile( m_name, true );
		GENLIB_LOCK_MUTEX( m_mutex );
	}

	EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Loaded - " + m_name + "\n" );
}
