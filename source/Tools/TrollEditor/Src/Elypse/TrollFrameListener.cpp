/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Elypse/TrollFrameListener.h"

#include "Elypse/TrollSceneFileParser.h"
#include "Elypse/TrollScriptEngine.h"
#include "Elypse/TrollInstance.h"

#include "GUI/MainFrame.h"

namespace Troll
{
	namespace ElypseRW
	{
		TrollFrameListener::TrollFrameListener( TrollInstance & p_instance, RenderWindow * p_window,
												String const & p_baseURL, String const & p_installDir,
												String const & p_appStringIndex )
			: ElypseFrameListener( p_instance, p_window, p_baseURL, p_installDir, p_appStringIndex )
			, m_ogre( &p_instance )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : created" );
		}

		TrollFrameListener::TrollFrameListener( TrollInstance & p_instance, TrollFrameListener * p_ofl,
												RenderWindow * p_window, String const & p_appStringIndex )
			: ElypseFrameListener( p_instance, p_ofl, p_window, p_appStringIndex )
		{
			EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : finished creation" );
		}

		TrollFrameListener::~TrollFrameListener()
		{
			EMUSE_LOG_MESSAGE_RELEASE( "GenderzFramelistener : Destroyed" );
		}

		void TrollFrameListener::_setupInterfaces()
		{
			m_gui = new EMGui( m_appIndexStr, m_camera->getViewport()->getActualWidth() / static_cast< Real >( m_camera->getViewport()->getActualHeight() ) );
			m_parser = new TrollSceneFileParser( m_appIndexStr, m_installDir + "rsc/" + m_filename + "/" );
			m_physics = new PhysicsEngine;
		}

		void TrollFrameListener::_setupScript()
		{
			m_scriptEngine = new TrollScriptEngine( m_installDir / "rsc" / m_filename, m_ogre->GetLoadingBar() );
			static_cast< TrollScriptEngine * >( m_scriptEngine )->Initialise();
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
			//m_context->currentUniverse = l_uni;
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
			m_context->currentMuseFile = ElypseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( m_filename + ".muse" );
			m_context->physicsSimulation = m_physics->GetSimulation( "Main" );
			m_context->physicsSimulation->SetAutoUpdated( true );
			m_context->physicsSpace = m_context->physicsSimulation->GetDynamicSpace();
			m_parser->m_context = m_context;
			m_scriptEngine->SetContext( m_context );
			m_scriptEngine->GetVariable( "GENERAL_SCREEN_WIDTH" )->set( Real( m_window->getWidth() ) );
			m_scriptEngine->GetVariable( "GENERAL_SCREEN_HEIGHT" )->set( Real( m_window->getHeight() ) );
		}

		void TrollFrameListener::Initialise()
		{
			EMUSE_LOG_MESSAGE_RELEASE( "TrollFrameListener : _initialise" );
			_setupEffectsManager();
			_setupInterfaces();
			_setupScript();
			_setupMainScript();
			_setupBinds();
		}

		bool TrollFrameListener::frameStarted( Real p_timeSinceLastFrame )
		{

			if ( m_window->isClosed() )
			{
				return false;
			}

			auto l_lock = make_unique_lock( m_mutex );

			//ResourceGroupManager::setPrefix( EMPTY_STRING);

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

				ElypseMessage l_message = m_network->GetLastMessage();

				while ( l_message )
				{
					m_scriptVars[NETWORK_CURRENT_MESSAGE]->set( l_message.m_message );

					if ( m_scriptBinds[NETWORK_MESSAGE] )
					{
						m_scriptEngine->Execute( m_scriptBinds[NETWORK_MESSAGE] );
					}

					l_message = m_network->GetLastMessage();
				}

				while ( !m_scriptQueue.IsEmpty() )
				{
					m_scriptEngine->Execute( m_scriptQueue.Pop() );
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

				while ( !m_scriptQueue.IsEmpty() )
				{
					m_scriptEngine->Execute( m_scriptQueue.Pop() );
				}

				m_mirrorManager->UpdateAll();
				SetMouse( m_scriptVars[MOUSE_POSITION_X]->get<Real>(), m_scriptVars[MOUSE_POSITION_Y]->get<Real>() );
			}

			return true;
		}
	}
}
