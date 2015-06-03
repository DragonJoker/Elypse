/*********************************************************************************************************************

	Author :	Sylvain DOREMUS & Marc BILLON

	Company:	ForDev Studio - Copyright 2006

	Files :		EMuseController.h EMuseController.cpp

	Desc :

*********************************************************************************************************************/
#include "PrecompiledHeader.h"

#include "EMuseController.h"
#include "EMuseLoadingBar.h"
#include "EMuseInstance.h"
#include "EMusePlugin.h"
#include "ElypseResourceGroupManager.h"

#include <OgreRoot.h>
#include <OgreLog.h>
#include <OgreRenderSystem.h>
#include <OgreAnimation.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreRenderWindow.h>
#include <OgreLogManager.h>
#include <OgreParticleSystemManager.h>
#include <OgreCompositorManager.h>

#include "DownloadManager.h"

#include "SoundManager.h"
#include "VideoManager.h"
#include "PubManager.h"

#include "ScriptNode.h"

#include "WebImageManager.h"


#include "EMuseLogs.h"

#include <ode/error.h>
#include <ode/collision.h>

#include <PreciseTimer.h>
#include <ThreadId.h>
#include <Utils.h>
#include <File.h>
#include <NeoCurl.h>

#include <OgreResourceBackgroundQueue.h>

#include "OgrePlugins.h"

#pragma warning( disable:4996 )

using namespace Ogre;

namespace
{
	const char * c_installWebsiteUrl = "http://www.fordev-studio.com/Elypse/Install/";
}

void OdeErrorCallback( int p_errnum, const char * p_msg, va_list ap )
{
	EMUSE_MESSAGE_RELEASE( "ERROR( " + StringConverter::toString( p_errnum ) + " ) -> " + String( p_msg ) );
}

GENLIB_INIT_SINGLETON( EMuseController );

EMuseController::EMuseController()
	:	owned_by<EMuseInstance>	( NULL ),
		m_root( NULL ),
		m_log( NULL ),
		m_primaryRenderWindow( NULL ),
		m_downloadManager( NULL ),
		m_direct3D( true ),
		m_oneTimePostWindowInit( false ),
		m_useConsole( false ),
		m_downloadFiles( true ),
		m_initialised( false ),
		m_stopThread( false ),
		m_vsync( true ),
		m_numInstance( 0 ),
		m_status( CS_NULL )
{
	new EMuse::Debug::EMuseLogs();
	GENLIB_SET_SINGLETON();
	/*
		std::locale::global( std::locale(""));
		setlocale( LC_ALL, "");
		std::cout.imbue( std::locale("") );
		std::wcout.imbue( std::locale("") );
	/**/
	m_loadingBar = new EMuseLoadingBar;
	std::cout << std::boolalpha;
	//Optims de cout / cin ...
	//std::ios_base::sync_with_stdio(false);
	//std::cin.tie(0);
	EMUSE_MESSAGE_DEBUG( "EMuseController::EMuseController" );
	dInitODE();
	dSetErrorHandler( OdeErrorCallback );
	dSetDebugHandler( OdeErrorCallback );
	dSetMessageHandler( OdeErrorCallback );
//	ElypseResourceGroupManager::setPrefix( "Miaou");
	srand( uint32_t( time( NULL ) ) );
}

void EMuseController::PreInit( bool p_useConsole, bool p_direct3D, bool p_download, const Path & p_installationDir )
{
	genlib_assert( ! p_installationDir.empty() );
	EMUSE_MESSAGE_DEBUG( "EMuseController::PreInit" );
	m_useConsole = p_useConsole;
	m_direct3D = p_direct3D;
	m_downloadFiles = p_download;
	m_installDir = p_installationDir;
//	CheckForUpdates();
}

EMuseController::~EMuseController()
{
	EMUSE_MESSAGE_DEBUG( "EMuseController::~EMuseController" );
	GENLIB_SCOPED_LOCK( m_selfMutex );
	dCloseODE();
	EMUSE_MESSAGE_DEBUG( "EMuseController::~EMuseController" );
	delete EMuse::Debug::EMuseLogs::GetSingletonPtr();
}

void EMuseController::AddApplication( EMuseInstance * p_instance )
{
	genlib_assert( p_instance != NULL );
	EMUSE_MESSAGE_DEBUG( "EMuseController::AddApplication" );
	GENLIB_AUTO_SCOPED_LOCK();
	m_instances.insert( InstanceMap::value_type( p_instance->GetName(), p_instance ) );
}

bool EMuseController::_getNextAvailableRenderer( String & p_pluginName, String & p_rendererName )
{
	if ( m_availableRenderers.empty() )
	{
		p_rendererName.clear();
		return false;
	}

	p_pluginName = m_availableRenderers.begin()->first;
	p_rendererName = m_availableRenderers.begin()->second;
	m_availableRenderers.erase( m_availableRenderers.begin() );
	return true;
}

bool EMuseController::_loadRenderer( const String & p_pluginName, const String & p_rendererName )
{
	try
	{
		m_log->logMessage( "Loading RenderSystem Plugin : " + p_pluginName );
		m_root->loadPlugin( p_pluginName );
		m_root->setRenderSystem( m_root->getRenderSystemByName( p_rendererName ) );
		m_log->logMessage( "Initialisation RenderSystem : " + p_rendererName );
		m_root->getRenderSystem()->_initialise( false );
		return true;
	}
	catch ( const Exception & p_exception )
	{
		std::cout << "Exception caught ! " << std::endl;
		m_log->logMessage( "Error while Loading Renderer : " + p_exception.getDescription() );
		return false;
	}
}

RenderWindow * EMuseController::CreateRenderWindow( const String & p_name, unsigned int p_width, unsigned int p_height, const String & p_handle, unsigned int p_antialiasing,  bool & p_mainWindow )
{
	NameValuePairList list;
	list["externalWindowHandle"] = p_handle;
	list["parentWindowHandle"] = p_handle;
	list["vsync"] = StringConverter::toString( m_vsync );
	list["FSAA"] = StringConverter::toString( p_antialiasing );
	list["FSAAQUALITY"] = StringConverter::toString( p_antialiasing * p_antialiasing );
	RenderWindow * l_rend = NULL;
	bool l_windowCreate = false;
	m_owner->GetPlugin()->LockGui();
	l_rend = m_root->getRenderSystem()->_createRenderWindow( p_name, p_width, p_height, false, & list );
	l_windowCreate = true;
	m_owner->GetPlugin()->UnlockGui();

	if ( ! m_oneTimePostWindowInit )
	{
		m_log->logMessage( "CEMuseController::Main window created" );
		m_oneTimePostWindowInit = true;
		MaterialManager::getSingletonPtr()->initialise();
		ParticleSystemManager::getSingletonPtr()->_initialise();
		ResourceBackgroundQueue::getSingletonPtr()->initialise();
		m_primaryRenderWindow = l_rend;
		p_mainWindow = true;
	}
	else
	{
		p_mainWindow = false;
	}

	return l_rend;
}

SceneManager * EMuseController::CreateSceneManager( const String & p_name )
{
	genlib_assert( ! p_name.empty() );
	EMUSE_MESSAGE_DEBUG( "EMuseController::CreateSceneManager" );
	return m_root->createSceneManager( ST_GENERIC, p_name );
}

void EMuseController::AddThread( EMuseInstance * p_owner )
{
	EMUSE_MESSAGE_DEBUG( "EMuseController::AddThread" );

	if ( m_status == CS_RENDERING )
	{
		GENLIB_AUTO_SCOPED_LOCK();
		EMUSE_MESSAGE_DEBUG( "EMuseController::AddThread // OK, WAITING" );
		GENLIB_CONDITION_WAIT( m_threadCurrentlyRendering, m_mutex );
		EMUSE_MESSAGE_DEBUG( "EMuseController::AddThread // OK, END WAITING" );
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "EMuseController::AddThread // OK, NOT WAITING" );
	}

	EMUSE_MESSAGE_DEBUG( "EMuseController::AddThread // OK, STARTED" );

	if ( !GetSingletonPtr() )
	{
		EMUSE_MESSAGE_RELEASE( "EMuseController::AddThread // Problem : sm_singleton == NULL" );
		return;
	}

	EMUSE_MESSAGE_RELEASE( "EMuseController::AddThread // Pre check m_status" );

	if ( m_status != CS_DESTROYING && m_status != CS_DESTROYED )
	{
		m_owner = p_owner;
		//	m_currentOwner = p_owner;
		m_owner->SetMain( true );
		MainLoop();
	}
}

void EMuseController::Initialise()
{
	EMUSE_MESSAGE_DEBUG( "EMuseController::Initialise" );
	std::cout << "EMuseController::Initialise()" << std::endl;
	ElypseResourceGroupManager::setPrefix( EMPTY_STRING );
	GENLIB_AUTO_SCOPED_LOCK();

	if ( m_initialised )
	{
		return;
	}

	if ( m_useConsole )
	{
//		AllocConsole();
//		freopen ("CONOUT$", "w", stdout);
//		freopen ("CONOUT$", "w", stderr );
	}

	EMUSE_MESSAGE_DEBUG( "EMuseController::Root::getSingletonPtr" );

	if ( Root::getSingletonPtr() == NULL )
	{
		Path l_directory = m_installDir / "cfg";
		EMUSE_MESSAGE_DEBUG( "EMuseController::Initialisation 1" );

		try
		{
			m_root = new Root( EMPTY_STRING, EMPTY_STRING, l_directory / "Ogre.log" );
		}
		catch ( Ogre::Exception & e )
		{
			EMUSE_CONSOLE_MESSAGE_RELEASE( "Exception at Ogre::Root creation(Ogre) : " + e.getFullDescription() );
		}
		catch ( std::exception & e )
		{
			EMUSE_CONSOLE_MESSAGE_RELEASE( "Exception at Ogre::Root creation(Stl) : " + String( e.what() ) );
			return;
		}
		catch ( ... )
		{
			EMUSE_CONSOLE_MESSAGE_RELEASE( "Exception at Ogre::Root creation(unknown)" );
		}

		m_log = LogManager::getSingletonPtr()->createLog( l_directory / "Emuse.log", false, false, false );
		m_log->logMessage( "EmuseController Initialisation Started" );
		m_downloadManager = new  DownloadManager( m_installDir, m_loadingBar, m_downloadFiles );
		EMUSE_LOG_MESSAGE_RELEASE( "miaou 1" );

		if ( ! PluginLoader::GetSingletonPtr()->load( m_installDir, m_root ) )
		{
			m_owner->GetPlugin()->SetGraphicalStatus( StatusErrorOgre );
		}

#if ELYPSE_WINDOWS
		m_root->setRenderSystem( m_root->getRenderSystemByName( "Direct3D9 Rendering Subsystem" ) );
#else
		m_root->setRenderSystem( m_root->getRenderSystemByName( "OpenGL Rendering Subsystem" ) );
#endif

		if ( m_root->getRenderSystem() )
		{
			std::cout << "named : " << m_root->getRenderSystem()->getName() << std::endl;
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 7" );
			EMUSE_MESSAGE_DEBUG( "EMuseController::Initialisation 6" );
			m_root->initialise( false );
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 8" );
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 9" );
			m_log->logMessage( "CEMuseController::Root Initialisation Started" );
			EMUSE_MESSAGE_DEBUG( "EMuseController::Initialisation 7" );
			new SoundManager( m_installDir / "rsc" );
			new VideoManager();
			new PubManager();
			new WebImageManager();
			EMUSE_MESSAGE_DEBUG( "EMuseController::Initialisation	 8" );
			m_log->logMessage( "CEMuseController::Media Managers created" );
			Ogre::Animation::setDefaultInterpolationMode( Ogre::Animation::IM_LINEAR );
			Ogre::Animation::setDefaultRotationInterpolationMode( Ogre::Animation::RIM_LINEAR );
//	ElypseResourceGroupManager::setPrefix( EMPTY_STRING);
			EMUSE_MESSAGE_DEBUG( "EMuseController::InitialiseRessources , next is get win dir for fonts" );
			/* // A check, normalement les font sont maintenant incluses dans le core.zip
			#if ELYPSE_WINDOWS
			//	char temp[100];
			//	GetWindowsDirectoryA( temp, 100);
			//	ElypseResourceGroupManager::addResourceLocation( String(temp) + "\\Fonts","FileSystem", "Fonts");
			//	ElypseResourceGroupManager::initialiseResourceGroup( "Fonts");
			#elif ELYPSE_LINUX
				ElypseResourceGroupManager::createResourceGroup("fonts.zip");
				ElypseResourceGroupManager::addResourceLocation( m_installDir / "cfg" / "fonts.zip", "Zip", "fonts.zip", false);
				ElypseResourceGroupManager::initialiseResourceGroup( "fonts.zip");
				ElypseResourceGroupManager::loadResourceGroup( "fonts.zip");
			#endif
			*/
			MeshManager::getSingleton().setBoundsPaddingFactor( 0 );
			EMUSE_MESSAGE_DEBUG( "EMuseController::Initialise , the end" );
			m_initialised = true;
			m_status = CS_INITIALISED;
		}
		else
		{
			m_initialised = false;
			m_status = CS_NULL;
		}
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "EMuseController::Root::getSingletonPtr already exists" );
		m_initialised = true;
		m_status = CS_INITIALISED;
	}
}

void EMuseController::InitialiseRessources( const String & p_prefix )
{
	ElypseResourceGroupManager::setPrefix( p_prefix );
	EMUSE_MESSAGE_DEBUG( "EMuseController::Initialise , next is get core.zip" );

	if ( ! FileExists( m_installDir / "cfg" / "core.zip" ) )
	{
#if ELYPSE_WINDOWS
		MessageBoxA( NULL, "Le player ne trouve pas certains fichiers nécessaires\nVeuillez réinstaller le player\nhttp://www.fdsstudio.com", "Player mal installé", 0 );
#else
		//
#endif
		//
		//CURLManager l_curl;
		//l_curl.DownloadFile( String( c_installWebsiteUrl) + "cfg/core.zip", m_installDir / "cfg" / "core.zip");
	}

	ElypseResourceGroupManager::createResourceGroup( "core.zip" );
	ElypseResourceGroupManager::addResourceLocation( m_installDir / "cfg" / "core.zip", "Zip", "core.zip", false );

	if ( ! p_prefix.empty() )
	{
		ElypseResourceGroupManager::createResourceGroup( ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
		ElypseResourceGroupManager::createResourceGroup( ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		ElypseResourceGroupManager::createResourceGroup( ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
		CompositorManager::getSingletonPtr()->initialise();
		MaterialPtr l_defaultSettings = MaterialManager::getSingletonPtr()->create( "DefaultSettings", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
		// Add a single technique and pass, non-programmable
		l_defaultSettings->createTechnique()->createPass();
		MaterialManager::getSingletonPtr()->create( "BaseWhite", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
		MaterialPtr baseWhiteNoLighting = MaterialManager::getSingletonPtr()->create( "BaseWhiteNoLighting", ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
		baseWhiteNoLighting->setLightingEnabled( false );
	}

	ElypseResourceGroupManager::initialiseResourceGroup( "core.zip" );
	ElypseResourceGroupManager::loadResourceGroup( "core.zip" );
}

void EMuseController::UninitialiseRessources( const String & p_prefix )
{
	ElypseResourceGroupManager::setPrefix( p_prefix );

	if ( ! p_prefix.empty() )
	{
		ElypseResourceGroupManager::destroyResourceGroup( ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME );
		ElypseResourceGroupManager::destroyResourceGroup( ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		ElypseResourceGroupManager::destroyResourceGroup( ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME );
	}

	ElypseResourceGroupManager::unloadResourceGroup( "core.zip" );
	ElypseResourceGroupManager::destroyResourceGroup( "core.zip" );
}

void EMuseController::MainLoop()
{
	GENLIB_SCOPED_LOCK( m_selfMutex );
	EMUSE_MESSAGE_DEBUG( "EMuseController::MainLoop() : start" );
	EMuseInstance * l_ogreApp;

	if ( ! m_initialised )
	{
		Initialise();
	}

	m_stopThread = false;

	if ( ! m_initialised )
	{
		m_stopThread = true;
	}

	m_status = CS_RENDERING;
	InstanceMap::iterator l_iterator;
	bool l_hasFocus = false;

	while ( ! m_stopThread )
	{
		if ( ! m_direct3D || ! l_hasFocus )
		{
			GENLIB_SLEEP( 10 );
		}

//		m_currentOwner->GetPlugin()->LockGui();
		WebImageManager::GetSingletonPtr()->Unlock();
		WebImageManager::GetSingletonPtr()->Lock();

		try
		{
			Root::getSingletonPtr()->renderOneFrame();
			VideoManager::GetSingletonPtr()->Update();
		}
		catch ( Exception & e )
		{
			EMUSE_MESSAGE_RELEASE( "Exception during Root::renderOneFrame : " + e.getDescription() );
		}

//		m_currentOwner->GetPlugin()->UnlockGui();
		l_hasFocus = false;

		if ( VideoManager::GetSingletonPtr() != NULL )
		{
			VideoManager::GetSingletonPtr()->Update();
		}

		GENLIB_LOCK_MUTEX( m_mutex );
		l_iterator = m_instances.begin();

		while ( l_iterator != m_instances.end() )
		{
			l_ogreApp = l_iterator->second;

			if ( l_ogreApp->IsInitialised() )
			{
				if ( l_ogreApp->IsToDelete() )
				{
					std::cout << "EMUSEController, removing an app " << std::endl;
					m_log->logMessage( "CEMuseController::Removing OgreApplication " + l_iterator->first );
					InstanceMap::iterator j = l_iterator;
					++ j;
					m_instances.erase( l_iterator );
					l_iterator = j;

					if ( m_instances.empty() )
					{
						l_ogreApp->Destroy();
						m_log->logMessage( "CEMuseController::OgreApplication removed, no more apps" );
//						GENLIB_UNLOCK_MUTEX( m_mutex);
						m_stopThread = true;
						break;
					}
					else
					{
						if ( l_ogreApp->IsMain() )
						{
							m_owner->SetMain( false );
							l_ogreApp->Destroy();
							m_log->logMessage( "CEMuseController::Removed the main OgreApplication, " + StringConverter::toString( m_instances.size() ) + " left" );
//							GENLIB_CONDITION_NOTIFY_ONE( m_threadCurrentlyRendering);
							m_stopThread = true;
							break;
						}
						else
						{
							l_ogreApp->Destroy();
						}

						m_log->logMessage( "CEMuseController::Removed OgreApplication, " + StringConverter::toString( m_instances.size() ) + " left" );
					}
				} // end if (ToDelete)
				else
				{
					if ( l_ogreApp->HasFocus() )
					{
						l_hasFocus = true;
					}

					m_owner->GetPlugin()->LockGui();
					l_ogreApp->RenderOneFrame();
					m_owner->GetPlugin()->UnlockGui();
					++ l_iterator;
				}
			} // end if (Initialised)
			else// if (!l_ogreApp->IsDeleted())
			{
				++ l_iterator;
				m_log->logMessage( "CEMuseController::Initialising OgreApplication" );
				l_ogreApp->Initialise();

				if ( ! m_links.empty() )
				{
					InstanceMultiMap::iterator ifind;
					ifind = m_links.find( l_ogreApp->GetName() );

					if ( ifind != m_links.end() )
					{
						String l_name;
						l_name = ifind->first;

						while ( ifind != m_links.end() && l_name == ifind->first )
						{
							ifind->second->LinkTo( l_ogreApp );
							InstanceMultiMap::iterator j = ifind;
							++ j;
							m_links.erase( ifind );
							ifind = j;
						}
					}
				}

				m_log->logMessage( "CEMuseController::End of Initialisation" );
			}
		} // end while (i)

		if ( ! m_stopThread )
		{
			GENLIB_UNLOCK_MUTEX( m_mutex );
		}
	} // end while (!m_stopThread)

//	GENLIB_AUTO_SCOPED_LOCK();
	m_log->logMessage( "CEMuseController::End of MainLoop" );

	if ( ! m_instances.empty() )
	{
//		m_log->logMessage("CEMuseController::MainLoop adopted by "+(m_instances.begin()->first));
		EMUSE_MESSAGE_DEBUG( "EMuseController::Main Loop // ADOPTION" );
		m_owner->SetMain( false );
		GENLIB_CONDITION_NOTIFY_ONE( m_threadCurrentlyRendering );
//		Start((m_instances.begin()->second)->GetThread() , (m_instances.begin()->second) );
//		return;
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "CEMuseController::endeded" );
		DeleteOgre();
	}

	m_status = CS_INITIALISED;
	GENLIB_UNLOCK_MUTEX( m_mutex );
	GENLIB_CONDITION_NOTIFY_ALL( m_threadEnded );
}

void EMuseController::DeleteOgre()
{
	EMUSE_CONSOLE_MESSAGE_RELEASE( "EMuseController::DeleteOgre // Start" );
//	GENLIB_SCOPED_LOCK( m_selfMutex);
	m_status = CS_DESTROYING;

	try
	{
		m_log->logMessage( "CEMuseController::DeleteOgre()" );
		m_log->logMessage( "CEMuseController::\tVideoManager" );
		VideoManager::Destroy();
		m_log->logMessage( "CEMuseController::\tPubManager" );
		PubManager::Destroy();
		m_log->logMessage( "CEMuseController::\tSoundManager" );
		SoundManager::Destroy();
		m_log->logMessage( "CEMuseController::\tWebImageManager" );
		WebImageManager::Destroy();

		if ( m_loadingBar->IsStarted() )
		{
			m_loadingBar->finish();
		}

		delete m_loadingBar;
		m_loadingBar = NULL;

//		ElypseResourceGroupManager::unloadResourceGroup( "core.zip");
//		ElypseResourceGroupManager::destroyResourceGroup( "core.zip");

		if ( m_primaryRenderWindow != NULL )
		{
			m_log->logMessage( "CEMuseController::\tPrimaryWindow" );
//			m_currentOwner->GetPlugin()->LockGui();
			m_primaryRenderWindow->removeAllListeners();
			m_primaryRenderWindow->removeAllViewports();
			m_primaryRenderWindow->destroy();
//			m_currentOwner->GetPlugin()->UnlockGui();
		}
		else
		{
			m_log->logMessage( "CEMuseController::\tNo PrimaryWindow" );
		}

		delete m_downloadManager;
		EMUSE_LOG_MESSAGE_NORMAL( "num scritpNodes : " + StringConverter::toString( ScriptNode::sm_numNodes ) + " nodes left" );
		EMUSE_LOG_MESSAGE_RELEASE( "CEMuseController::Root Shutdown" );
		EMUSE_LOG_MESSAGE_RELEASE( "CEMuseController::Bye" );
		m_log = NULL;
		EMUSE_CONSOLE_MESSAGE_DEBUG( "EMuseController::DeleteOgre // m_root->shutdown();" );
		delete m_root;
		PluginLoader::GetSingletonPtr()->unload();
		EMUSE_CONSOLE_MESSAGE_DEBUG( "EMuseController::DeleteOgre //DELETE( m_root );" );
		m_root = NULL;
	}
	catch ( ... )
	{
		EMUSE_CONSOLE_MESSAGE_RELEASE( "EMuse Controller Caught an Exception" );
		//tant pis
	}

	m_status = CS_DESTROYED;
	EMUSE_CONSOLE_MESSAGE_RELEASE( "EMuseController::DeleteOgre // END" );
}

void EMuseController::LinkInstanceTo( EMuseInstance * p_instance, const String & p_linkedToName )
{
	EMUSE_MESSAGE_DEBUG( "EMuseController::LinkInstanceTo" );
	InstanceMap::iterator ifind;
	EMUSE_MESSAGE_DEBUG( "CEMC link : " + p_linkedToName + String( " from " ) + p_instance->GetName() );
	ifind = m_instances.find( p_linkedToName );

	if ( ifind != m_instances.end() && ifind->second->IsActive() )
	{
		EMUSE_MESSAGE_DEBUG( "EMuseController::LinkInstanceTo : Direct Linkage" );
		p_instance->LinkTo( ifind->second );
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "EMuseController::LinkInstanceTo : Delayed Linkage" );
		m_links.insert( InstanceMultiMap::value_type( p_linkedToName , p_instance ) );
	}
}

void EMuseController::WaitForThreadEnded()
{
	EMUSE_MESSAGE_NORMAL( "EMuseController::WaitForThreadEnded : Wait for thread ended pre lock mutex : " + ToString( ThreadId::GetSelf() ) );
	GENLIB_AUTO_SCOPED_LOCK();
	EMUSE_MESSAGE_NORMAL( "EMuseController::WaitForThreadEnded : Wait for thread ended post lock mutex : " + ToString( ThreadId::GetSelf() ) );

	if ( !GetSingletonPtr() )
	{
		if ( m_status == CS_RENDERING )
		{
			EMUSE_MESSAGE_NORMAL( "EMuseController::WaitForThreadEnded : Wait for thread ended" );
			{
				GENLIB_CONDITION_WAIT( m_threadEnded, m_mutex );
			}
			EMUSE_MESSAGE_NORMAL( "EMuseController::WaitForThreadEnded : End of Waiting for thread ended" );
		}
		else
		{
			EMUSE_MESSAGE_NORMAL( "EMuseController::WaitForThreadEnded : no need to wait" );
		}
	}
}

bool EMuseController::ShowConsole()
{
#if defined( _WIN32 )

	if ( AllocConsole() )
	{
		freopen( "CONOUT$", "w", stdout );
//		freopen( "CONOUT$", "w", stderr);
		return true;
	}

#endif
	return false;
}