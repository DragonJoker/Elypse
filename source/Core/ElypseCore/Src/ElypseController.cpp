/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ElypseController.h"
#include "ElypseLoadingBar.h"
#include "ElypseInstance.h"
#include "ElypsePlugin.h"
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


#include "ElypseLogs.h"

#include <ode/error.h>
#include <ode/collision.h>

#include <PreciseTimer.h>
#include <Utils.h>
#include <File.h>
#include <NeoCurl.h>

#include <OgreResourceBackgroundQueue.h>

#include "OgrePlugins.h"

#pragma warning( disable:4996 )

using namespace Ogre;

namespace
{
	char const * const c_installWebsiteUrl = "http://www.fordev-studio.com/Elypse/Install/";

	std::wstring ToWChar( std::string const & src )
	{
		std::vector< wchar_t > dst;
		dst.resize( src.size() + 1u );
		int length = MultiByteToWideChar( CP_UTF8
			, 0
			, src.c_str()
			, int( src.size() )
			, dst.data()
			, int( dst.size() ) );
		return std::wstring( dst.data(), dst.data() + length + 1 );
	}

	void OdeErrorCallback( int p_errnum, char const * const p_msg, va_list ap )
	{
		EMUSE_MESSAGE_RELEASE( "ERROR( " + StringConverter::toString( p_errnum ) + " ) -> " + String( p_msg ) );
	}
}

GENLIB_INIT_SINGLETON( ElypseController );

ElypseController::ElypseController( ElypseInstance & p_owner )
	: owned_by<ElypseInstance>( p_owner )
	, m_root( NULL )
	, m_log( NULL )
	, m_primaryRenderWindow( NULL )
	, m_downloadManager( NULL )
	, m_direct3D( true )
	, m_oneTimePostWindowInit( false )
	, m_useConsole( false )
	, m_downloadFiles( true )
	, m_initialised( false )
	, m_stopThread( false )
	, m_vsync( true )
	, m_numInstance( 0 )
	, m_status( CS_NULL )
{
	new Elypse::Debug::ElypseLogs();
	GENLIB_SET_SINGLETON();
	/*
		std::locale::global( std::locale(""));
		setlocale( LC_ALL, "");
		std::cout.imbue( std::locale("") );
		std::wcout.imbue( std::locale("") );
	/**/
	m_loadingBar = new ElypseLoadingBar;
	std::cout << std::boolalpha;
	//Optims de cout / cin ...
	//std::ios_base::sync_with_stdio(false);
	//std::cin.tie(0);
	EMUSE_MESSAGE_DEBUG( "ElypseController::ElypseController" );
	dInitODE();
	dSetErrorHandler( OdeErrorCallback );
	dSetDebugHandler( OdeErrorCallback );
	dSetMessageHandler( OdeErrorCallback );
//	ElypseResourceGroupManager::setPrefix( "Miaou");
	srand( uint32_t( time( NULL ) ) );
}

void ElypseController::PreInit( bool p_useConsole, bool p_direct3D, bool p_download, const Path & p_installationDir )
{
	genlib_assert( ! p_installationDir.empty() );
	EMUSE_MESSAGE_DEBUG( "ElypseController::PreInit" );
	m_useConsole = p_useConsole;
	m_direct3D = p_direct3D;
	m_downloadFiles = p_download;
	m_installDir = p_installationDir;
//	CheckForUpdates();
}

ElypseController::~ElypseController()
{
	EMUSE_MESSAGE_DEBUG( "ElypseController::~ElypseController" );
	auto l_lock = make_unique_lock( m_selfMutex );
	dCloseODE();
	EMUSE_MESSAGE_DEBUG( "ElypseController::~ElypseController" );
	delete Elypse::Debug::ElypseLogs::GetSingletonPtr();
}

void ElypseController::AddApplication( ElypseInstance * p_instance )
{
	genlib_assert( p_instance != NULL );
	EMUSE_MESSAGE_DEBUG( "ElypseController::AddApplication" );
	auto l_lock = make_unique_lock( m_mutex );
	m_instances.insert( std::make_pair( p_instance->GetName(), p_instance ) );
}

bool ElypseController::_getNextAvailableRenderer( String & p_pluginName, String & p_rendererName )
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

bool ElypseController::_loadRenderer( String const & p_pluginName, String const & p_rendererName )
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

RenderWindow * ElypseController::CreateRenderWindow( String const & p_name, uint32_t p_width, uint32_t p_height, String const & p_handle, uint32_t p_antialiasing, bool & p_mainWindow )
{
	NameValuePairList list;
	list["externalWindowHandle"] = p_handle;
	list["parentWindowHandle"] = p_handle;
	list["vsync"] = StringConverter::toString( m_vsync );
	list["FSAA"] = StringConverter::toString( p_antialiasing );
	list["FSAAQUALITY"] = StringConverter::toString( p_antialiasing * p_antialiasing );
	RenderWindow * l_rend = NULL;
	bool l_windowCreate = false;
	GetOwner()->GetPlugin()->LockGui();
	l_rend = m_root->getRenderSystem()->_createRenderWindow( p_name, p_width, p_height, false, & list );
	l_windowCreate = true;
	GetOwner()->GetPlugin()->UnlockGui();

	if ( ! m_oneTimePostWindowInit )
	{
		m_log->logMessage( "CElypseController::Main window created" );
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

SceneManager * ElypseController::CreateSceneManager( String const & p_name )
{
	genlib_assert( ! p_name.empty() );
	EMUSE_MESSAGE_DEBUG( "ElypseController::CreateSceneManager" );
	auto l_sceneManager = m_root->createSceneManager( ST_GENERIC, p_name );

	if ( m_overlaySystem )
	{
		l_sceneManager->addRenderQueueListener( m_overlaySystem );
	}

	return l_sceneManager;
}

void ElypseController::AddThread( ElypseInstance & p_owner )
{
	EMUSE_MESSAGE_DEBUG( "ElypseController::AddThread" );

	if ( m_status == CS_RENDERING )
	{
		auto l_lock = make_unique_lock( m_mutex );
		EMUSE_MESSAGE_DEBUG( "ElypseController::AddThread // OK, WAITING" );
		m_threadCurrentlyRendering.wait( l_lock );
		EMUSE_MESSAGE_DEBUG( "ElypseController::AddThread // OK, END WAITING" );
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "ElypseController::AddThread // OK, NOT WAITING" );
	}

	EMUSE_MESSAGE_DEBUG( "ElypseController::AddThread // OK, STARTED" );

	if ( !GetSingletonPtr() )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseController::AddThread // Problem : sm_singleton == NULL" );
		return;
	}

	EMUSE_MESSAGE_RELEASE( "ElypseController::AddThread // Pre check m_status" );

	if ( m_status != CS_DESTROYING && m_status != CS_DESTROYED )
	{
		ChangeOwner( p_owner );
		//m_currentOwner = p_owner;
		GetOwner()->SetMain( true );
		MainLoop();
	}
}

void ElypseController::Initialise()
{
	EMUSE_MESSAGE_DEBUG( "ElypseController::Initialise" );
	std::cout << "ElypseController::Initialise()" << std::endl;
	ElypseResourceGroupManager::setPrefix( EMPTY_STRING );
	auto l_lock = make_unique_lock( m_mutex );

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

	EMUSE_MESSAGE_DEBUG( "ElypseController::Root::getSingletonPtr" );

	if ( Root::getSingletonPtr() == NULL )
	{
		Path l_directory = m_installDir / "cfg";
		EMUSE_MESSAGE_DEBUG( "ElypseController::Initialisation 1" );

		try
		{
			m_root = new Root( EMPTY_STRING, EMPTY_STRING, l_directory / "Ogre.log" );
			m_overlaySystem = new Ogre::OverlaySystem();
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
			GetOwner()->GetPlugin()->SetGraphicalStatus( StatusErrorOgre );
		}

#if ELYPSE_WINDOWS
		m_root->setRenderSystem( m_root->getRenderSystemByName( "OpenGL Rendering Subsystem" ) );
		//m_root->setRenderSystem( m_root->getRenderSystemByName( "Direct3D11 Rendering Subsystem" ) );
#else
		m_root->setRenderSystem( m_root->getRenderSystemByName( "OpenGL Rendering Subsystem" ) );
#endif

		if ( m_root->getRenderSystem() )
		{
			std::cout << "named : " << m_root->getRenderSystem()->getName() << std::endl;
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 7" );
			EMUSE_MESSAGE_DEBUG( "ElypseController::Initialisation 6" );
			m_root->initialise( false );
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 8" );
			EMUSE_LOG_MESSAGE_RELEASE( "miaou 9" );
			m_log->logMessage( "CElypseController::Root Initialisation Started" );
			EMUSE_MESSAGE_DEBUG( "ElypseController::Initialisation 7" );
			new SoundManager( m_installDir / "rsc" );
			new VideoManager();
			new PubManager();
			new WebImageManager();
			EMUSE_MESSAGE_DEBUG( "ElypseController::Initialisation	 8" );
			m_log->logMessage( "CElypseController::Media Managers created" );
			Ogre::Animation::setDefaultInterpolationMode( Ogre::Animation::IM_LINEAR );
			Ogre::Animation::setDefaultRotationInterpolationMode( Ogre::Animation::RIM_LINEAR );
//	ElypseResourceGroupManager::setPrefix( EMPTY_STRING);
			EMUSE_MESSAGE_DEBUG( "ElypseController::InitialiseRessources , next is get win dir for fonts" );
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
			EMUSE_MESSAGE_DEBUG( "ElypseController::Initialise , the end" );
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
		EMUSE_MESSAGE_DEBUG( "ElypseController::Root::getSingletonPtr already exists" );
		m_initialised = true;
		m_status = CS_INITIALISED;
	}
}

void ElypseController::InitialiseRessources( String const & p_prefix )
{
	ElypseResourceGroupManager::setPrefix( p_prefix );
	EMUSE_MESSAGE_DEBUG( "ElypseController::Initialise , next is get core.zip" );

	if ( ! FileExists( m_installDir / "cfg" / "core.zip" ) )
	{
#if ELYPSE_WINDOWS
		MessageBoxW( NULL
			, ToWChar( "Le player ne trouve pas certains fichiers nécessaires\nVeuillez réinstaller le player" ).c_str()
			, ToWChar( "Player mal installé" ).c_str()
			, 0 );
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

void ElypseController::UninitialiseRessources( String const & p_prefix )
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

void ElypseController::MainLoop()
{
	auto l_lock = make_unique_lock( m_selfMutex );
	EMUSE_MESSAGE_DEBUG( "ElypseController::MainLoop() : start" );
	ElypseInstance * l_ogreApp;

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

		m_mutex.lock();
		l_iterator = m_instances.begin();

		while ( l_iterator != m_instances.end() )
		{
			l_ogreApp = l_iterator->second;

			if ( l_ogreApp->IsInitialised() )
			{
				if ( l_ogreApp->IsToDelete() )
				{
					m_log->logMessage( "CElypseController::Removing OgreApplication " + l_iterator->first );
					InstanceMap::iterator j = l_iterator;
					++ j;
					m_instances.erase( l_iterator );
					l_iterator = j;

					if ( m_instances.empty() )
					{
						l_ogreApp->Destroy();
						m_log->logMessage( "CElypseController::OgreApplication removed, no more apps" );
						//m_mutex.unlock();
						m_stopThread = true;
						break;
					}
					else
					{
						if ( l_ogreApp->IsMain() )
						{
							GetOwner()->SetMain( false );
							l_ogreApp->Destroy();
							m_log->logMessage( "CElypseController::Removed the main OgreApplication, " + StringConverter::toString( m_instances.size() ) + " left" );
//							m_threadCurrentlyRendering.notify_one();
							m_stopThread = true;
							break;
						}
						else
						{
							l_ogreApp->Destroy();
						}

						m_log->logMessage( "CElypseController::Removed OgreApplication, " + StringConverter::toString( m_instances.size() ) + " left" );
					}
				} // end if (ToDelete)
				else
				{
					if ( l_ogreApp->HasFocus() )
					{
						l_hasFocus = true;
					}

					GetOwner()->GetPlugin()->LockGui();
					l_ogreApp->RenderOneFrame();
					GetOwner()->GetPlugin()->UnlockGui();
					++ l_iterator;
				}
			} // end if (Initialised)
			else// if (!l_ogreApp->IsDeleted())
			{
				++ l_iterator;
				m_log->logMessage( "CElypseController::Initialising OgreApplication" );
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

				m_log->logMessage( "CElypseController::End of Initialisation" );
			}
		} // end while (i)

		if ( !m_stopThread )
		{
			m_mutex.unlock();
		}
	} // end while (!m_stopThread)

	//auto l_lock = make_unique_lock( m_mutex );
	m_log->logMessage( "CElypseController::End of MainLoop" );

	if ( ! m_instances.empty() )
	{
//		m_log->logMessage("CElypseController::MainLoop adopted by "+(m_instances.begin()->first));
		EMUSE_MESSAGE_DEBUG( "ElypseController::Main Loop // ADOPTION" );
		GetOwner()->SetMain( false );
		m_threadCurrentlyRendering.notify_one();
//		Start((m_instances.begin()->second)->GetThread() , (m_instances.begin()->second) );
//		return;
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "CElypseController::endeded" );
		DeleteOgre();
	}

	m_status = CS_INITIALISED;
	m_mutex.unlock();
	m_threadEnded.notify_all();
}

void ElypseController::DeleteOgre()
{
	EMUSE_CONSOLE_MESSAGE_RELEASE( "ElypseController::DeleteOgre // Start" );
	//auto l_lock = make_unique_lock( m_selfMutex );
	m_status = CS_DESTROYING;

	try
	{
		m_log->logMessage( "CElypseController::DeleteOgre()" );
		m_log->logMessage( "CElypseController::\tVideoManager" );
		VideoManager::Destroy();
		m_log->logMessage( "CElypseController::\tPubManager" );
		PubManager::Destroy();
		m_log->logMessage( "CElypseController::\tSoundManager" );
		SoundManager::Destroy();
		m_log->logMessage( "CElypseController::\tWebImageManager" );
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
			m_log->logMessage( "CElypseController::\tPrimaryWindow" );
//			m_currentOwner->GetPlugin()->LockGui();
			m_primaryRenderWindow->removeAllListeners();
			m_primaryRenderWindow->removeAllViewports();
			m_primaryRenderWindow->destroy();
//			m_currentOwner->GetPlugin()->UnlockGui();
		}
		else
		{
			m_log->logMessage( "CElypseController::\tNo PrimaryWindow" );
		}

		delete m_downloadManager;
		EMUSE_LOG_MESSAGE_NORMAL( "num scritpNodes : " + StringConverter::toString( ScriptNode::sm_numNodes ) + " nodes left" );
		EMUSE_LOG_MESSAGE_RELEASE( "CElypseController::Root Shutdown" );
		EMUSE_LOG_MESSAGE_RELEASE( "CElypseController::Bye" );
		m_log = NULL;
		EMUSE_CONSOLE_MESSAGE_DEBUG( "ElypseController::DeleteOgre // m_root->shutdown();" );
		delete m_overlaySystem;
		m_overlaySystem = nullptr;
		delete m_root;
		m_root = nullptr;
		PluginLoader::GetSingletonPtr()->unload();
		EMUSE_CONSOLE_MESSAGE_DEBUG( "ElypseController::DeleteOgre //DELETE( m_root );" );
	}
	catch ( ... )
	{
		EMUSE_CONSOLE_MESSAGE_RELEASE( "Elypse Controller Caught an Exception" );
		//tant pis
	}

	m_status = CS_DESTROYED;
	EMUSE_CONSOLE_MESSAGE_RELEASE( "ElypseController::DeleteOgre // END" );
}

void ElypseController::LinkInstanceTo( ElypseInstance * p_instance, String const & p_linkedToName )
{
	EMUSE_MESSAGE_DEBUG( "ElypseController::LinkInstanceTo" );
	InstanceMap::iterator ifind;
	EMUSE_MESSAGE_DEBUG( "CEMC link : " + p_linkedToName + String( " from " ) + p_instance->GetName() );
	ifind = m_instances.find( p_linkedToName );

	if ( ifind != m_instances.end() && ifind->second->IsActive() )
	{
		EMUSE_MESSAGE_DEBUG( "ElypseController::LinkInstanceTo : Direct Linkage" );
		p_instance->LinkTo( ifind->second );
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "ElypseController::LinkInstanceTo : Delayed Linkage" );
		m_links.insert( std::make_pair( p_linkedToName , p_instance ) );
	}
}

void ElypseController::WaitForThreadEnded()
{
	EMUSE_MESSAGE_NORMAL( "ElypseController::WaitForThreadEnded : Wait for thread ended pre lock mutex : " + ToString( std::this_thread::get_id() ) );
	auto l_lock = make_unique_lock( m_mutex );
	EMUSE_MESSAGE_NORMAL( "ElypseController::WaitForThreadEnded : Wait for thread ended post lock mutex : " + ToString( std::this_thread::get_id() ) );

	if ( !GetSingletonPtr() )
	{
		if ( m_status == CS_RENDERING )
		{
			EMUSE_MESSAGE_NORMAL( "ElypseController::WaitForThreadEnded : Wait for thread ended" );
			{
				m_threadEnded.wait( l_lock );
			}
			EMUSE_MESSAGE_NORMAL( "ElypseController::WaitForThreadEnded : End of Waiting for thread ended" );
		}
		else
		{
			EMUSE_MESSAGE_NORMAL( "ElypseController::WaitForThreadEnded : no need to wait" );
		}
	}
}

bool ElypseController::ShowConsole()
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
