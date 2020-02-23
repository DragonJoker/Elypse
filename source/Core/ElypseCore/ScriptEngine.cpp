/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ScriptEngine.h"
#include "ScriptFunctions.h"

#include "ScriptNode.h"
#include "ScriptCompiler.h"
#include "ScriptTimerManager.h"
#include "ScriptTimer.h"

#include "Zone.h"
#include "Universe.h"
#include "SceneFileParser.h"
#include "ElypseLoadingBar.h"
#include "Context.h"
#include "ElypseInstance.h"
#include "ConfigFile.h"

#include <OgreLog.h>
#include <OgreDataStream.h>

#include <Exception.h>
#include <Utils.h>

#include "ElypseLogs.h"

Context * ScriptEngine::sm_context = nullptr;

ScriptEngine::ScriptEngine( ScriptCompiler * p_compiler, const Path & p_path, ElypseLoadingBar * p_loadingBar )
	: m_path( p_path )
	, m_loadingBar( p_loadingBar )
	, m_compiler( p_compiler )
{
}

ScriptEngine::ScriptEngine( const Path & p_path, ElypseLoadingBar * p_loadingBar )
	: ScriptEngine( new ScriptCompiler( p_path ), p_path, p_loadingBar )
{
}

ScriptEngine::~ScriptEngine()
{
	delete m_timerManager;
	delete m_compiler;
}

void ScriptEngine::Initialise()
{
	m_compiler->Initialise();
	m_timerManager = new ScriptTimerManager( this );
}

void ScriptEngine::SetContext( Context * p_context )
{
	m_context = p_context;
	m_context->timerManager = m_timerManager;
	UseContext();
}

void ScriptEngine::AddScriptZone( String & p_scriptString )
{
	ScriptNode * l_node = nullptr;

	try
	{
		l_node = CompileScript( p_scriptString );
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ScriptEngine : Failed compilation of code zone from scene file -> " + p_exception.GetDescription() );
		l_node = nullptr;
	}

	if ( l_node  != nullptr )
	{
		l_node->Use();
		Execute( l_node );
		l_node->Delete();
	}
}

void ScriptEngine::Execute( ScriptNode * p_node )
{
	if ( p_node->HasFunction() )
	{
		p_node->Execute();
	}
}

ScriptNode * ScriptEngine::GetKeyboardBind( uint32_t p_keyCode, uint32_t p_bindType )const
{
	return m_compiler->m_keyboardBinds[p_bindType][p_keyCode];
}

ScriptNode * ScriptEngine::CompileScript( String & p_scriptString )
{
	return m_compiler->CompileScript( p_scriptString );
}

ScriptNode * ScriptEngine::GetVariable( String const & p_variableName )const
{
	return m_compiler->GetProgramConstant( p_variableName );
}

ScriptNode * ScriptEngine::CompileScriptFile( ConfigFile * p_scriptFile )
{
	return m_compiler->CompileScriptFile( p_scriptFile );
}

void ScriptEngine::LoadZone( Zone * p_zone )
{
//	std::cout << "LoadZone : " << p_zone->GetName() << " from isntance : " << ResourceGroupManager::getPrefix() << std::endl;
	if ( p_zone == nullptr )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Internal error : ScriptEngine tried to unload a null scene" );
		return;
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start : " + p_zone->GetName() );
	size_t i;
	ScriptNode * l_node;

	if ( ! p_zone->m_dataFiles.empty() )
	{
		m_loadingBar->SetWindow( m_context->emuseInstance->GetWindow() );
		m_loadingBar->finish();
		m_loadingBar->start( uint16_t( p_zone->m_dataFiles.size() ), uint16_t( p_zone->m_dataFiles.size() ), 0.1f );
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start2 : " + p_zone->GetName() );
	p_zone->Load();
	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start2 : " + p_zone->GetName() );

	if ( p_zone->m_firstInit )
	{
		p_zone->m_firstInit = false;

		for ( i = 0 ; i < p_zone->m_loadScriptFiles.size() ; i ++ )
		{
			l_node = CompileScriptFile( p_zone->m_loadScriptFiles[i] );

			if ( l_node != nullptr )
			{
				l_node->Use();
				p_zone->m_loadScriptNodes[i] = l_node;
//				p_zone->m_loadScriptFiles[i]->SetCompiled( l_node);
			}
		}

		for ( i = 0 ; i < p_zone->m_unloadScriptFiles.size() ; i ++ )
		{
			l_node = CompileScriptFile( p_zone->m_unloadScriptFiles[i] );

			if ( l_node != nullptr )
			{
				l_node->Use();
				p_zone->m_unloadScriptNodes[i] = l_node;
				//			p_zone->m_unloadScriptFiles[i]->SetCompiled( l_node);
			}
		}
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start3 : " + p_zone->GetName() );
	m_context->universe = p_zone->GetOwner()->shared_from_this();
	m_context->scene = p_zone;
	m_context->sceneManager = m_context->universe->GetManager();

	for ( i = 0 ; i < p_zone->m_sceneFiles.size() ; i ++ )
	{
		m_context->sceneFileParser->ParseScript( p_zone->m_sceneFiles[i] );
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start3 : " + p_zone->GetName() );

	for ( i = 0 ; i < p_zone->m_loadScriptFiles.size() ; i ++ )
	{
		ScriptNode * l_node = p_zone->m_loadScriptNodes[i];

		if ( l_node != nullptr )
		{
			if ( l_node->HasFunction() )
			{
				l_node->Execute();
			}
		}
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone start4 : " + p_zone->GetName() );

	if ( ! p_zone->m_dataFiles.empty() )
	{
		m_loadingBar->finish();
	}

	EMUSE_LOG_MESSAGE_DEBUG( "Load zone end : " + p_zone->GetName() );
}

void ScriptEngine::UnloadZone( Zone * p_zone )
{
	if ( p_zone == nullptr )
	{
		EMUSE_LOG_MESSAGE_RELEASE( "Internal error : ScriptEngine tried to unload a null scene" );
		return;
	}

	p_zone->Unload();
	p_zone->ClearPortals();
	size_t imax = p_zone->m_unloadScriptFiles.size();

	for ( size_t i = 0 ; i < imax ; i ++ )
	{
		ScriptNode * l_node = p_zone->m_unloadScriptNodes[i];

		if ( l_node != nullptr )
		{
			if ( l_node->HasFunction() )
			{
				l_node->Execute();
			}
		}
	}
}

void ScriptEngine::ScriptError( ScriptNode * p_node, String const & p_string )
{
	EMUSE_LOG_MESSAGE_RELEASE( String( "Runtime Error Line # " ) + StringConverter::toString( p_node->m_createdAtLine ) + " @ file " + ( p_node->m_file != nullptr ? p_node->m_file->GetName() : "none" ) + " : " + p_string );
}

void ScriptEngine::UseContext()const
{
	sm_context = m_context;
	ScriptCompiler::SetSingleton( m_compiler );
}

ScriptNode * ScriptEngine::GetFunction( String const & p_functionName )const
{
	return m_compiler->GetUsableFunctionNode( p_functionName );
}
