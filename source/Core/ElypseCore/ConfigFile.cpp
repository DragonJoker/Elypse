/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ConfigFile.h"
#include "MuseFile.h"

#include "ScriptNode.h"

//#include "ElypseController.h"
#include "DownloadManager.h"

#include "ElypseLogs.h"

namespace Elypse::Data
{
	void ConfigFileDeleter::operator()( ConfigFile * ptr )const noexcept
	{
		delete ptr;
	}

	ConfigFile::ConfigFile( MuseFile & p_owner )
		: ConfigFile( String(), p_owner )
	{
	}

	ConfigFile::ConfigFile( String const & p_name, MuseFile & p_owner )
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
		auto l_lock = make_unique_lock( m_mutex );
		m_downloaded = true;
	}

	String ConfigFile::GetDescriptiveName()const
	{
		return GetOwner()->GetName() + " > " + m_name;
	}

	void ConfigFile::DoLoad()
	{
		auto l_lock = make_unique_lock( m_mutex );
		EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Load - " + m_name + "\n" );

		if ( !m_downloaded )
		{
			m_mutex.unlock();
			GetOwner()->WaitForFile( m_name, true );
			m_mutex.lock();
		}

		EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Loaded - " + m_name + "\n" );
	}
}
