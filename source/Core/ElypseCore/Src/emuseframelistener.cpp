#include "PrecompiledHeader.h"

#include "EMuseFrameListener.h"
#include "EMuseController.h"
#include "EMuseInstance.h"
#include "Context.h"
#include "ElypseResourceGroupManager.h"

#include <OgreLog.h>
#include <OgreOverlayManager.h>
#include <OgreCamera.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreRenderWindow.h>
#include <OgreStringConverter.h>
#include <OgreSceneManager.h>

#include "AnimatedObjectGroup.h"
#include "AnimationManager.h"

#include "ScriptNode.h"
#include "ScriptTimerManager.h"
#include "ScriptEngine.h"
#include "ScriptFunctions.h"

#include "PhysicsEngine.h"
#include "PhysicsSimulation.h"

#include "SceneFileParser.h"
#include "SceneFileSaver.h"

#include "CamTex.h"

#include "Universe.h"
#include "EMGui.h"
#include "EMOverlay.h"

#include "SoundManager.h"
#include "VideoManager.h"
#include "PubManager.h"

#include "Mirror.h"
#include "MirrorManager.h"
#include "PostEffect.h"
#include "ObjectMaterial.h"

#include "DownloadManager.h"

#include "SequenceManager.h"

#include "NetworkManager.h"

#include "EMusePlugin.h"
#include "EMuseLoadingBar.h"

#include "MuseFile.h"

#include <Manager.h>
#include "EMuseLogs.h"

EMuseFrameListener::EMuseFrameListener(	EMuseInstance * p_ogre, RenderWindow * p_window,
										const Url & p_baseURL, const Path & p_installDir,
										const String & p_appStringIndex )
	:	owned_by <EMuseInstance>	( p_ogre ),			m_window( p_window ),
				 m_sceneManager( NULL ),				m_objectMaterialManager( NULL ),
				 m_gui( NULL ),				m_physics( NULL ),
				 m_scriptEngine( NULL ),				m_parser( NULL ),
				 m_mirrorManager( NULL ),				m_universeManager( NULL ),
				 m_camTexManager( NULL ),				m_museFile( NULL ),
				 m_postEffectsManager( NULL ),				m_animationManager( NULL ),
				 m_sequenceManager( NULL ),				m_context( NULL ),
				 m_scriptQueue( 50 ),				m_keysDown( NULL ),
				 m_main( true ),				m_showFPS( false ),
				 m_ready( false ),			m_mouseMove( false ),
				 m_mainUrl( p_baseURL ),		m_filename( p_baseURL.GetFilename() ),
				 m_appIndexStr( p_appStringIndex ),	m_installDir( p_installDir ),
				 m_deletableScriptQueue( 50 )
{
	genlib_assert( m_owner != NULL );
	genlib_assert( m_window != NULL );
	genlib_assert( ! m_installDir.empty() );
	genlib_assert( ! m_appIndexStr.empty() );
	m_network = new NetworkManager;
	m_museFile = EMuseController::GetSingletonPtr()->GetDownloadManager()->StartDownloadFile( m_mainUrl );
	m_debugOverlay = OverlayManager::getSingleton().getByName( "Core/DebugOverlay" );

	if ( m_filename.find( ".muse" ) != String::npos )
	{
		m_filename = m_filename.substr( 0 , m_filename.length() - 5 );
	}

	ShowDebugOverlay( false );

	for ( unsigned int i = 0 ; i < static_cast<unsigned int>( NUM_SCRIPT_VAR ); i ++ )
	{
		m_scriptVars[i] = NULL;
	}

	for ( unsigned int i = 0 ; i < static_cast<unsigned int>( NUM_SCRIPT_BINDS ); i ++ )
	{
		m_scriptBinds[i] = NULL;
	}

	m_startUpScriptNode = NULL;
	EMUSE_LOG_MESSAGE_DEBUG( "Framelistener : creating" );
}

EMuseFrameListener::EMuseFrameListener( EMuseInstance * p_instance, EMuseFrameListener * p_ofl,
										RenderWindow * p_window, const String & p_appStringIndex )
	:	owned_by <EMuseInstance>	( p_instance ),
		   m_window( p_window ),
		   m_scriptQueue( 50 ),
		   m_deletableScriptQueue( 50 ),
		   m_main( false ),
		   m_mouseMove( false ),
		   m_appIndexStr( p_appStringIndex )
{
	genlib_assert( m_owner != NULL );
	genlib_assert( p_ofl != NULL );
	genlib_assert( m_window != NULL );
	genlib_assert( ! m_appIndexStr.empty() );
	EMUSE_LOG_MESSAGE_DEBUG( "Framelistener : creating" );
	_initialiseFromExisting( p_ofl );
	EMUSE_LOG_MESSAGE_DEBUG( "Framelistener : finished creation" );
}

EMuseFrameListener::~EMuseFrameListener()
{
	EMUSE_LOG_MESSAGE_DEBUG( "EMuseFrameListener : ~EMuseFrameListener" );
	_destroyBinds();

	if ( m_main )
	{
		delete m_postEffectsManager;
		delete m_animationManager;
		delete [] m_keysDown;
		delete m_mirrorManager;
		delete m_universeManager;
		delete m_gui;
		delete m_physics;
		delete m_parser;
		delete m_camTexManager;
		delete m_context;
		delete m_sequenceManager;
		delete m_objectMaterialManager;
		delete m_network;
		delete m_scriptEngine;
	}
	else
	{
		delete [] m_keysDown;
	}

	EMUSE_LOG_MESSAGE_DEBUG( "EMuseFrameListener : Destroyed" );
}

void EMuseFrameListener::_setupEffectsManager()
{
	m_camTexManager = new CamTexManager;
	m_mirrorManager = new MirrorManager( m_appIndexStr );
	m_postEffectsManager = new PostEffectManager;
	m_animationManager = new AnimationManager;
	m_sequenceManager = new SequenceManager;
	m_objectMaterialManager = new ObjectMaterialManager;
}

void EMuseFrameListener::_setupUniverse()
{
	m_universeManager = new UniverseManager( m_appIndexStr );
	Universe * l_uni = m_universeManager->CreateUniverse( "Main" );
//	Universe * l_uni = new Universe( "Main"/*, m_owner->GetSceneManager()*/);
	l_uni->SetRenderTarget( m_window );
//	m_universeManager->AddElement( l_uni);
	m_sceneManager = l_uni->GetManager();
	m_camera = l_uni->GetCamera();
	m_camera->setNearClipDistance( 5.0 );
}

void EMuseFrameListener::_setupInterfaces()
{
	m_gui = new EMGui( m_appIndexStr, m_camera->getViewport()->getActualWidth() / static_cast<Real>( m_camera->getViewport()->getActualHeight() ) );
	m_parser = new SceneFileParser( m_appIndexStr, m_installDir / "rsc" / m_filename );
	m_physics = new PhysicsEngine;
}

void EMuseFrameListener::_setupScript()
{
	m_scriptEngine = new ScriptEngine( m_installDir / "rsc" / m_filename, m_owner->GetLoadingBar() );
	m_scriptEngine->Initialise();
	m_parser->m_scriptEngine = m_scriptEngine;
	m_context = new Context;
	m_context->gui = m_gui;
	m_context->physicsEngine = m_physics;
	m_context->scriptEngine = m_scriptEngine;
	m_context->mirrorManager = m_mirrorManager;
	m_context->universeManager = m_universeManager;
	m_context->animationManager = m_animationManager;
	m_context->downloadManager = m_owner->GetDownloadManager();
	m_context->camTexManager = m_camTexManager;
	m_context->universe = m_universeManager->GetElementByName( "Main" );
	m_context->sceneManager = m_sceneManager;
	m_context->postEffectManager = m_postEffectsManager;
	m_context->sceneFileParser = m_parser;
	m_context->instanceNum = m_appIndexStr;
	m_context->mainCamera = m_camera;
	m_context->mainViewport = m_camera->getViewport();
	m_context->plugin = m_owner->GetPlugin();
	m_context->imageDir = m_installDir / "rsc" / m_filename / "Images";
	m_context->emuseInstance = m_owner;
	m_context->sequenceManager = m_sequenceManager;
	m_context->network = m_network;
	m_context->baseURL = m_mainUrl;
	m_context->objectMaterialManager = m_objectMaterialManager;
	m_context->currentMuseFile = EMuseController::GetSingletonPtr()->GetDownloadManager()->GetMuseFile( m_filename + ".muse" );
	m_context->physicsSimulation = m_physics->GetSimulation( "Main" );
	m_context->physicsSimulation->SetAutoUpdated( true );
	m_context->physicsSpace = m_context->physicsSimulation->GetDynamicSpace();
	m_parser->m_context = m_context;
	m_scriptEngine->SetContext( m_context );
	m_scriptEngine->GetVariable( "GENERAL_SCREEN_WIDTH" )->set( static_cast <Real>( m_window->getWidth() ) );
	m_scriptEngine->GetVariable( "GENERAL_SCREEN_HEIGHT" )->set( static_cast <Real>( m_window->getHeight() ) );
}

void EMuseFrameListener::_setupMainScript()
{
	if ( ! m_startupScript.empty() )
	{
		m_startUpScriptNode = m_scriptEngine->CompileScript( m_startupScript );

		if ( m_startUpScriptNode != NULL )
		{
			m_startUpScriptNode->Use();

			if ( m_startUpScriptNode->HasFunction() )
			{
				m_startUpScriptNode->Execute();
			}
		}
	}

	if ( ! m_museFile->WaitForFile( "main.emcfg", true ) )
	{
		std::cout << "Set graphical status, not found main.emcfg" << std::endl;
		OverlayManager::getSingletonPtr()->getByName( "Core/NoScene" )->show();
		m_owner->GetLoadingBar()->finish();
//		m_owner->GetPlugin()->SetGraphicalStatus( StatusErrorUnknown);
		//TODO : could not download the main file
	}
	else
	{
//		std::cout << "ParseScript main : " << std::endl;
		m_parser->ParseScript( m_museFile->GetConfigFile( "main.emcfg" ) );
	}
}

void EMuseFrameListener::PreInitialise()
{
	_setupUniverse();
}

void EMuseFrameListener::Initialise()
{
	EMUSE_LOG_MESSAGE_RELEASE( "Framelistener : _initialise" );
	_setupEffectsManager();
	_setupInterfaces();
	_setupScript();
	_setupMainScript();
	_setupBinds();
}

void EMuseFrameListener::_initialiseFromExisting( EMuseFrameListener * p_other )
{
	genlib_assert( p_other != NULL );
	m_debugOverlay = p_other->m_debugOverlay;
	m_sceneManager = p_other->m_sceneManager;
	m_gui = p_other->m_gui;
	m_physics = p_other->m_physics;
	m_scriptEngine = p_other->m_scriptEngine;
	m_mirrorManager = p_other->m_mirrorManager;
	m_camTexManager = NULL;
	m_universeManager = NULL;

	if ( m_sceneManager->hasCamera( m_owner->GetName() ) )
	{
		m_camera = m_sceneManager->getCamera( m_owner->GetName() );
	}
	else
	{
		m_camera = m_sceneManager->createCamera( m_owner->GetName() );
	}

	Viewport * l_viewport = m_window->addViewport( m_camera );
	m_camera->setAspectRatio( Real( l_viewport->getActualWidth() ) / Real( l_viewport->getActualHeight() ) );
	_setupBinds();
}

void EMuseFrameListener::_setupBinds()
{
	EMUSE_CONSOLE_MESSAGE_NORMAL( "_setupBinds" );
#define MT_RETRIEVE_VAR( X) m_scriptVars[X] = m_scriptEngine->GetVariable( #X)
	MT_RETRIEVE_VAR( MOUSE_POSITION_X );
	MT_RETRIEVE_VAR( MOUSE_POSITION_Y );
	MT_RETRIEVE_VAR( MOUSE_DIFFERENCE_X );
	MT_RETRIEVE_VAR( MOUSE_DIFFERENCE_Y );
	MT_RETRIEVE_VAR( GENERAL_ELAPSED_TIME );
	MT_RETRIEVE_VAR( KEY_CURRENT );
	MT_RETRIEVE_VAR( NETWORK_CURRENT_MESSAGE );
#undef MT_RETRIEVE
	m_mouseLDown = false;
	m_mouseRDown = false;
	m_mouseMDown = false;
	m_keysDown = new bool[NUM_KEYS];

	for ( size_t i = 0 ; i < NUM_KEYS ; i ++ )
	{
		m_keysDown[i] = false;
	}

	m_ready = true;
}

void EMuseFrameListener::_destroyBinds()
{
	for ( unsigned int i = 0 ; i < static_cast<unsigned int>( NUM_SCRIPT_BINDS ); i ++ )
	{
		if ( m_scriptBinds[i] != NULL )
		{
			m_scriptBinds[i]->Delete();
			m_scriptBinds[i] = NULL;
		}
	}

	if ( m_startUpScriptNode != NULL )
	{
		m_startUpScriptNode->Delete();
		m_startUpScriptNode = NULL;
	}
}

void EMuseFrameListener::ShowDebugOverlay( bool show )
{
	m_showFPS = show;

	if ( m_debugOverlay )
	{
		if ( show )
		{
			m_debugOverlay->show();
		}
		else
		{
			m_debugOverlay->hide();
		}
	}
}

void EMuseFrameListener::_updateStats( void )
{
	OverlayElement * guiAvg = OverlayManager::getSingleton().getOverlayElement( "Core/AverageFps" );
	OverlayElement * guiCurr = OverlayManager::getSingleton().getOverlayElement( "Core/CurrFps" );
	OverlayElement * guiBest = OverlayManager::getSingleton().getOverlayElement( "Core/BestFps" );
	OverlayElement * guiWorst = OverlayManager::getSingleton().getOverlayElement( "Core/WorstFps" );
	const RenderTarget::FrameStats & stats = m_window->getStatistics();
	guiAvg->setCaption( "Average FPS: " + StringConverter::toString( stats.avgFPS ) );
	guiCurr->setCaption( "Current FPS: " + StringConverter::toString( stats.lastFPS ) );
	guiBest->setCaption( "Best FPS: " + StringConverter::toString( stats.bestFPS )
						 + " " + StringConverter::toString( stats.bestFrameTime ) + " ms" );
	guiWorst->setCaption( "Worst FPS: " + StringConverter::toString( stats.worstFPS )
						  + " " + StringConverter::toString( stats.worstFrameTime ) + " ms" );
	OverlayElement * guiTris = OverlayManager::getSingleton().getOverlayElement( "Core/NumTris" );
	guiTris->setCaption( "Triangle Count: " + StringConverter::toString( stats.triangleCount ) + ", Batches : " + StringConverter::toString( stats.batchCount ) );
}

bool EMuseFrameListener::frameStarted( Real p_timeSinceLastFrame )
{
	ScriptNode * l_script;

	if ( m_window->isClosed() )
	{
		return false;
	}

	GENLIB_AUTO_SCOPED_LOCK();
	ElypseResourceGroupManager::setPrefix( m_owner->GetName() );

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

			//TODO : pas sur.
			if ( m_gui->GetTopmost() != NULL && m_gui->GetTopmost()->HasScript() )
			{
				m_owner->GetPlugin()->ChangeCursorTo( ArrowCursor );
			}
			else
			{
				m_owner->GetPlugin()->ChangeCursorTo( m_owner->GetPlugin()->GetBaseCursor() );
			}

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

		if ( ! m_deletableScriptQueue.IsEmpty() )
		{
			l_script = m_deletableScriptQueue.Pop();

			while ( l_script != NULL )
			{
				m_scriptEngine->Execute( l_script );
				l_script->Delete();
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

bool EMuseFrameListener::frameEnded( Real p_timeSinceLastFrame )
{
	if ( m_showFPS )
	{
		_updateStats();
	}

	_updateMedia( p_timeSinceLastFrame );
	m_gui->SetAllOverlaysVisibility( false );
	return true;
}

void EMuseFrameListener::MouseMove( Real p_x, Real p_y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	m_scriptVars[MOUSE_DIFFERENCE_X]->get <Real> () += p_x - m_scriptVars[MOUSE_POSITION_X]->get <Real> ();
	m_scriptVars[MOUSE_DIFFERENCE_Y]->get <Real> () += p_y - m_scriptVars[MOUSE_POSITION_Y]->get <Real> ();
	m_scriptVars[MOUSE_POSITION_X]->set( p_x );
	m_scriptVars[MOUSE_POSITION_Y]->set( p_y );
	m_mouseMove = true;
}

void EMuseFrameListener::SetMouse( Real p_x, Real p_y )
{
	m_scriptVars[MOUSE_DIFFERENCE_X]->set <Real> ( 0.0 );
	m_scriptVars[MOUSE_DIFFERENCE_Y]->set <Real> ( 0.0 );

	if ( p_x != m_scriptVars[MOUSE_POSITION_X]->get <Real> () || p_y != m_scriptVars[MOUSE_POSITION_Y]->get <Real> () )
	{
		m_mouseMove = true;
		m_scriptVars[MOUSE_POSITION_X]->set( p_x );
		m_scriptVars[MOUSE_POSITION_Y]->set( p_y );
	}
}

void EMuseFrameListener::MouseRDown( Real x, Real y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	_tryPush( m_scriptBinds[MOUSE_R_DOWN] );
	m_mouseRDown = true;
}

void EMuseFrameListener::MouseLDown( Real x, Real y )
{
	GENLIB_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	x /= m_camera->getViewport()->getActualWidth();
	y /= m_camera->getViewport()->getActualHeight();

	if ( m_mouseMove )
	{
		m_gui->MouseOver( x, y );
		m_mouseMove = false;
	}

	/*
	EMOverlay * l_over = m_gui->GetTopmost();

	if (l_over)
	{
		std::cout << "Nom de l'overlay topmost " << l_over->GetName() << std::endl;
	}
	*/
	ScriptNode * l_scriptNode = m_gui->Click( x, y );

	if ( l_scriptNode != NULL )
	{
		m_scriptQueue.Push( l_scriptNode );
	}
	else
	{
		PubManager::GetSingletonPtr()->OnClick( x, y, m_sceneManager, m_camera, m_physics->GetSimulation( "Main" ) );
		_tryPush( m_scriptBinds[MOUSE_L_DOWN] );
	}

	m_mouseLDown = true;
}

void EMuseFrameListener::MouseMDown( Real x, Real y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	_tryPush( m_scriptBinds[MOUSE_M_DOWN] );
	m_mouseMDown = true;
}

void EMuseFrameListener::MouseRUp( Real x, Real y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	_tryPush( m_scriptBinds[MOUSE_R_UP] );
	m_mouseRDown = false;
}

void EMuseFrameListener::MouseLUp( Real x, Real y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	_tryPush( m_scriptBinds[MOUSE_L_UP] );
	m_mouseLDown = false;
}

void EMuseFrameListener::MouseMUp( Real x, Real y )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	SetMouse( x, y );
	_tryPush( m_scriptBinds[MOUSE_M_UP] );
	m_mouseMDown = false;
}

void EMuseFrameListener::MouseWheel( short move )
{
//	EMUSE_AUTO_SCOPED_LOCK();
	if ( move > 0 )
	{
		_tryPush( m_scriptBinds[MOUSE_WHEEL_UP] );
	}
	else
	{
		_tryPush( m_scriptBinds[MOUSE_WHEEL_DOWN] );
	}
}

void EMuseFrameListener::ResetAll()
{
	if ( m_mouseLDown )
	{
		MouseLUp( m_scriptVars[MOUSE_POSITION_X]->get <Real> (), m_scriptVars[MOUSE_POSITION_Y]->get <Real> () );
	}

	if ( m_mouseRDown )
	{
		MouseRUp( m_scriptVars[MOUSE_POSITION_X]->get <Real> (), m_scriptVars[MOUSE_POSITION_Y]->get <Real> () );
	}

	if ( m_mouseMDown )
	{
		MouseMUp( m_scriptVars[MOUSE_POSITION_X]->get <Real> (), m_scriptVars[MOUSE_POSITION_Y]->get <Real> () );
	}

	for ( unsigned int i = 0 ; i < NUM_KEYS ; i ++ )
	{
		if ( m_keysDown[i] )
		{
			KeyUp( i );
		}
	}
}

void EMuseFrameListener::KeyDown( unsigned int p_key )
{
//	EMUSE_AUTO_SCOPED_LOCK();
//	std::cout << "keydown : " << p_key << std::endl;
	m_keysDown[p_key] = true;
	ScriptNode * l_node = m_scriptEngine->GetKeyboardBind( p_key, KBT_KEY_DOWN );

	if ( l_node != NULL )
	{
		m_scriptQueue.Push( l_node );
	}
}

void EMuseFrameListener::KeyUp( unsigned int p_key )
{
//	EMUSE_AUTO_SCOPED_LOCK();
//	std::cout << "keyup : " << p_key << std::endl;
	m_keysDown[p_key] = false;
	ScriptNode * l_node = m_scriptEngine->GetKeyboardBind( p_key, KBT_KEY_UP );

	if ( l_node != NULL )
	{
		m_scriptQueue.Push( l_node );
	}
}

void EMuseFrameListener::KeyRepeat( unsigned int p_key, unsigned int p_repeat )
{
//	EMUSE_AUTO_SCOPED_LOCK();
//	std::cout << "keyrepeat : " << p_key << std::endl;
	ScriptNode * l_node = m_scriptEngine->GetKeyboardBind( p_key, KBT_KEY_REPEAT );

	if ( l_node != NULL )
	{
		for ( unsigned int i = 0; i < p_repeat; i ++ )
		{
			m_scriptQueue.Push( l_node );
		}
	}
}

void EMuseFrameListener::OnChar( unsigned int p_char )
{
//	std::cout << "on char : " << _keycodeToChar( p_char) << " (" << p_char << ")" << std::endl;
	if ( m_scriptBinds[KEY_CHAR_DOWN] != NULL )
	{
		m_scriptVars[KEY_CURRENT]->set( _keycodeToChar( p_char ) );
		m_scriptQueue.Push( m_scriptBinds[KEY_CHAR_DOWN] );
	}
}

void EMuseFrameListener::OnCharRepeat( unsigned int p_char, unsigned int p_numRepeat )
{
//	std::cout << "on char repeat : " << _keycodeToChar( p_char) << std::endl;
	if ( m_scriptBinds[KEY_CHAR_REPEAT] != NULL )
	{
		m_scriptVars[KEY_CURRENT]->set( _keycodeToChar( p_char ) );

		for ( unsigned int i = 0; i < p_numRepeat; i ++ )
		{
			m_scriptQueue.Push( m_scriptBinds[KEY_CHAR_REPEAT] );
		}
	}
}

void EMuseFrameListener::_updateMedia( Real p_time )
{
	// Get camera position to compute volume and balance
	Vector3 l_position;

	if ( m_camera->isAttached() )
	{
		l_position = m_camera->getParentSceneNode()->_getDerivedPosition() + m_camera->getPosition();
	}
	else
	{
		l_position = m_camera->getPosition();
	}

	Radian m_cameraYaw = Degree( 0 );

	// Update Sounds volume, balance and time
	if ( SoundManager::GetSingletonPtr() != NULL && m_camera->isAttached() )
	{
		SoundManager::GetSingletonPtr()->Update( l_position, m_camera->getParentSceneNode()->getLocalAxes()[1], m_camera->getParentSceneNode()->getLocalAxes()[2], p_time );
	}
}

void EMuseFrameListener::AddToScriptQueue( ScriptNode * p_node )
{
	if ( p_node != NULL )
	{
		m_scriptQueue.Push( p_node );
	}
}

void EMuseFrameListener::AddToScriptQueue( const String & p_script )
{
	GENLIB_AUTO_SCOPED_LOCK();
//	std::cout << "AddToScriptQueue : " << p_script << std::endl;
	String l_string = p_script;
	ScriptNode * l_node = m_scriptEngine->CompileScript( l_string );

	if ( l_node != NULL )
	{
		l_node->Use();
		m_deletableScriptQueue.Push( l_node );
	}
}

void EMuseFrameListener::SetCallback( CallbackType p_type, ScriptNode * p_node )
{
	genlib_assert( p_node != NULL );

	if ( m_scriptBinds[p_type] != NULL )
	{
		m_scriptBinds[p_type]->Delete();
		m_scriptBinds[p_type] = NULL;
	}

	if ( p_node->HasFunction() )
	{
		m_scriptBinds[p_type] = p_node;
		p_node->Use();
	}
}

char EMuseFrameListener::_keycodeToChar( unsigned int p_keycode )
{
	//TODO : delegate to plugin, avec check du mappage clavier et check maj/min, etc, pour avoir la vraie touche.
	return static_cast <char>( p_keycode );
}
