//********************************************************************************************************************
#include "PrecompiledHeader.h"

#include "Main/TrollFrameListener.h"
#include "SceneFile/TrollSceneFileParser.h"
#include "Script/TrollScriptEngine.h"
#include "GUI/MainFrame.h"
/*
#include <OgreLog.h>

#include <GUI/EMGui.h>
#include <Main/Context.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/PhysicsSimulation.h>
#include <SpecialEffects/MirrorManager.h>
#include <SpecialEffects/PostEffect.h>
#include <Simulation/SurfaceManager.h>
#include <Animation/AnimationManager.h>
#include <Network/NetworkManager.h>
#include <EMuseLogs.h>
#include <ScriptTimerManager.h>
*/
//********************************************************************************************************************
using namespace Troll::Main;
using namespace Troll::Script;
//********************************************************************************************************************

TrollFrameListener :: TrollFrameListener( TrollInstance * p_ogre, RenderWindow * p_window,
		const String & p_baseURL, const String & p_installDir,
		const String & p_appStringIndex )
	:	EMuseFrameListener( p_ogre, p_window, p_baseURL, p_installDir, p_appStringIndex ),
		m_ogre( p_ogre )
{
	EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : created" );
}

//********************************************************************************************************************

TrollFrameListener :: TrollFrameListener( TrollInstance * p_instance, TrollFrameListener * p_ofl,
		RenderWindow * p_window, const String & p_appStringIndex )
	:	EMuseFrameListener( p_instance, p_ofl, p_window, p_appStringIndex )
{
	EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : finished creation" );
}

//********************************************************************************************************************

TrollFrameListener :: ~TrollFrameListener()
{
	EMUSE_LOG_MESSAGE_RELEASE( "GenderzFramelistener : Destroyed" );
}

//********************************************************************************************************************

void TrollFrameListener :: _setupInterfaces()
{
	m_gui = new EMGui( m_appIndexStr, m_camera->getViewport()->getActualWidth() / static_cast<Real>( m_camera->getViewport()->getActualHeight() ) );
	m_parser = new TrollSceneFileParser( m_appIndexStr, m_installDir + "rsc/" + m_filename + "/" );
	m_physics = new PhysicsEngine;
}

//********************************************************************************************************************

void TrollFrameListener :: _setupScript()
{
	m_scriptEngine = new TrollScriptEngine( m_installDir / "rsc" / m_filename, m_ogre->GetLoadingBar() );
	static_cast <TrollScriptEngine *>( m_scriptEngine )->Initialise();
	m_parser->m_scriptEngine = m_scriptEngine;
	m_context = new Context;
	m_context->gui = m_gui;
	m_context->physicsEngine = m_physics;
	m_context->scriptEngine = m_scriptEngine;
	m_context->mirrorManager = m_mirrorManager;
	m_context->universeManager = m_universeManager;
	m_context->animationManager = m_animationManager;
	m_context->downloadManager = m_ogre->GetDownloadManager();
	m_context->camTexManager = m_camTexManager;
	m_context->universe = m_universeManager->GetElementByName( "Main" );
	m_context->sceneManager = m_sceneManager;
	m_context->postEffectManager = m_postEffectsManager;
//	m_context->currentUniverse = l_uni;
	m_context->sceneFileParser = m_parser;
	m_context->instanceNum = m_appIndexStr;
	m_context->mainCamera = m_camera;
	m_context->mainViewport = m_camera->getViewport();
	m_context->plugin = m_ogre->GetPlugin();
	m_context->imageDir = m_installDir / "rsc" / m_filename / "Images";
	m_context->emuseInstance = m_ogre;
	m_context->sequenceManager = m_sequenceManager;
	m_context->network = m_network;
	m_context->baseURL = m_mainUrl;
	m_context->currentMuseFile = EMuseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( m_filename + ".muse" );
	m_context->physicsSimulation = m_physics->GetSimulation( "Main" );
	m_context->physicsSimulation->SetAutoUpdated( true );
	m_context->physicsSpace = m_context->physicsSimulation->GetDynamicSpace();
	m_parser->m_context = m_context;
	m_scriptEngine->SetContext( m_context );
	m_scriptEngine->GetVariable( "GENERAL_SCREEN_WIDTH" )->set( static_cast <Real>( m_window->getWidth() ) );
	m_scriptEngine->GetVariable( "GENERAL_SCREEN_HEIGHT" )->set( static_cast <Real>( m_window->getHeight() ) );
}

//********************************************************************************************************************

void TrollFrameListener :: Initialise()
{
	EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : _initialise" );
	_setupEffectsManager();
	_setupInterfaces();
	_setupScript();
	_setupMainScript();
	_setupBinds();
}

//********************************************************************************************************************

bool TrollFrameListener :: frameStarted( Real p_timeSinceLastFrame )
{
	ScriptNode * l_script;

	if ( m_window->isClosed() )
	{
		return false;
	}

	GENLIB_AUTO_SCOPED_LOCK();

//	ResourceGroupManager::setPrefix( EMPTY_STRING);

	if ( m_main )
	{
		m_scriptEngine->UseContext();
		m_scriptEngine->m_context->mainCamera = m_camera;
		m_scriptEngine->m_context->mainViewport = m_camera->getViewport();
		m_scriptVars[GENERAL_ELAPSED_TIME]->set( p_timeSinceLastFrame );
		m_scriptEngine->m_context->timerManager->UpdateAll( p_timeSinceLastFrame );
		m_scriptEngine->m_context->frameTime = p_timeSinceLastFrame;
		m_gui->SetAllOverlaysVisibility( true );
		m_sequenceManager->Update( p_timeSinceLastFrame );

		if ( m_mouseMove )
		{
			m_gui->MouseOver( m_scriptVars[MOUSE_POSITION_X]->get<Real>() / m_camera->getViewport()->getActualWidth(), m_scriptVars[MOUSE_POSITION_Y]->get<Real>() / m_camera->getViewport()->getActualHeight() );
			m_mouseMove = false;
		}

		ElypseMessage * l_message = m_network->GetLastMessage();

		while ( l_message != NULL )
		{
			m_scriptVars[NETWORK_CURRENT_MESSAGE]->set<String>( l_message->m_message );

			if ( m_scriptBinds[NETWORK_MESSAGE] != NULL )
			{
				m_scriptEngine->Execute( m_scriptBinds[NETWORK_MESSAGE] );
			}

			delete l_message;
			l_message = m_network->GetLastMessage();
		}

		if ( ! m_scriptQueue.IsEmpty() )
		{
			l_script = m_scriptQueue.Pop();

			while ( l_script != NULL )
			{
				m_scriptEngine->Execute( l_script );
				l_script = NULL;

				if ( ! m_scriptQueue.IsEmpty() )
				{
					l_script = m_scriptQueue.Pop();
				}
			}
		}

		m_mirrorManager->UpdateAll();
		SetMouse( m_scriptVars[MOUSE_POSITION_X]->get<Real>(), m_scriptVars[MOUSE_POSITION_Y]->get<Real>() );
		m_physics->Update();
		m_animationManager->Update( p_timeSinceLastFrame );
	}
	else
	{
		m_scriptEngine->UseContext();
		m_scriptEngine->m_context->mainCamera = m_camera;
		m_scriptEngine->m_context->mainViewport = m_camera->getViewport();
		m_gui->SetAllOverlaysVisibility( true );
		m_gui->MouseOver( m_scriptVars[MOUSE_POSITION_X]->get<Real>(), m_scriptVars[MOUSE_POSITION_Y]->get<Real>() );
		l_script = m_scriptQueue.Pop();

		while ( l_script != NULL )
		{
			m_scriptEngine->Execute( l_script );
			l_script = m_scriptQueue.Pop();
		}

		m_mirrorManager->UpdateAll();
		SetMouse( m_scriptVars[MOUSE_POSITION_X]->get<Real>(), m_scriptVars[MOUSE_POSITION_Y]->get<Real>() );
	}

	return true;
}

//********************************************************************************************************************
