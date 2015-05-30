#include "PrecompiledHeader.h"

#include "ConfigFile.h"
#include "MuseFile.h"

#include "ScriptNode.h"

//#include "EMuseController.h"
#include "DownloadManager.h"

#include "EMuseLogs.h"

ConfigFile::ConfigFile( const String & p_name, MuseFile * p_owner )
	:	named( p_name ),
		owned_by( p_owner ),
		m_useCount( 0 ),
		m_downloaded( false )
//		m_parsed		(false),
//		m_compiled		(NULL)
{
}

ConfigFile::~ConfigFile()
{
//	ReleaseScript();
}

String ConfigFile::GetDescriptiveName()const
{
	return m_owner->GetName() + " > " + m_name;
}

/*
void ConfigFile::ReleaseScript() d_no_throw
{
	if (m_compiled != NULL)
	{
		try
		{
			EMUSE_LOG_MESSAGE_RELEASE( "ConfigFile - deleting script in file " + m_name);
			m_compiled->Delete();
			EMUSE_LOG_MESSAGE_RELEASE( "ConfigFile - end deleting script in file " + m_name);
		}
		catch (...)
		{
			EMUSE_LOG_MESSAGE_RELEASE( "ConfigFile - Error while deleting the script " + m_name);
		}
		m_compiled = NULL;
	}
}
*/
void ConfigFile::_load()
{
	GENLIB_AUTO_SCOPED_LOCK();
	EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Load - " + m_name + "\n" );

	if ( ! m_downloaded )
	{
		GENLIB_UNLOCK_MUTEX( m_mutex );
		m_owner->WaitForFile( m_name, true );
		GENLIB_LOCK_MUTEX( m_mutex );
	}

	EMUSE_CONSOLE_MESSAGE_DEBUG( "ConfigFile::Loaded - " + m_name + "\n" );
}

void ConfigFile::Use()
{
	if ( m_useCount == 0 )
	{
		_load();
	}

	m_useCount ++;
}

void ConfigFile::Release()
{
	m_useCount --;
}

void ConfigFile::DownloadFinished()
{
	GENLIB_AUTO_SCOPED_LOCK();
	m_downloaded = true;
}
/*
bool ConfigFile::IsParsed ()
{
	GENLIB_AUTO_SCOPED_LOCK();

	return m_parsed;
}

void ConfigFile::SetParsed( bool p_parsed)
{
	GENLIB_AUTO_SCOPED_LOCK();

	m_parsed = p_parsed;
}

void ConfigFile::SetCompiled( ScriptNode * p_compiledScriptNode)
{
	genlib_assert( p_compiledScriptNode != NULL);

	p_compiledScriptNode->Use();
	m_compiled = p_compiledScriptNode;
}
*/
