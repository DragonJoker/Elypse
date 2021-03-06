/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "ElypseInstance.h"
#include "ElypseLoadingBar.h"
#include "ElypseFrameListener.h"
#include "ElypseController.h"
#include "ElypsePlugin.h"

#include "MuseDownloader.h"
#include "DownloadManager.h"
#include "ElypseResourceGroupManager.h"
//#include "MD5CheckSum.h"

#include <OgreLog.h>
#include <OgreStringConverter.h>
#include <OgreRenderWindow.h>
#include <OgreRenderSystem.h>
#include <OgreRoot.h>
#include <OgreException.h>
#include <OgreTimer.h>

#include "DataFile.h"
#include "ScriptNode.h"

#include <Utils.h>
#include <Exception.h>
#include <PreciseTimer.h>

#include "ElypseLogs.h"

using namespace Ogre;

ElypseInstance::ElypseInstance( const Path & p_installPath, ElypsePlugin * p_plugin )
	: named( "ElypseInstance" )
	, m_installationPath( p_installPath )
	, m_root( nullptr )
	, m_renderWindow( nullptr )
	, m_plugin( p_plugin )
	, m_thread( nullptr )
	, m_downManager( nullptr )
	, m_frameListener( nullptr )
	, m_loadingBar( nullptr )
	, m_antialiasing( 0 )
	, m_height( 0 )
	, m_width( 0 )
	, m_download( true )
	, m_useDirectX( true )
	, m_initialised( false )
	, m_toDelete( false )
	, m_deleted( false )
	, m_hasFocus( false )
	, m_main( false )
	, m_hasPrimaryWindow( false )
	, m_showFPS( false )
	, m_useConsole( false )
	, m_deactivated( false )
	, m_linked( false )
{
	genlib_assert( ! m_installationPath.empty() );
	genlib_assert( m_plugin != nullptr );
	m_name.clear();
	p_plugin->SetInstance( this );
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::ElypseInstance" );
}

bool ElypseInstance::Init( uint32_t p_width, uint32_t p_height, String const & p_linkedTo )
{
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::Init" );
	m_height = p_height;
	m_width = p_width;
	bool ret = false;

	if ( ElypseController::GetSingletonPtr() == nullptr )
	{
		ret = true;
		ElypseController::Create( *this );
		ElypseController::GetSingletonPtr()->PreInit( m_useConsole, m_useDirectX, m_download, m_installationPath );
		EMUSE_MESSAGE_NORMAL( "Instance : " + m_name + " Primary" );
		m_main = true;
	}
	else
	{
		ret = true;
		m_main = false;
		EMUSE_MESSAGE_NORMAL( "Instance : " + m_name + " Secondary" );
	}

	int l_appIndex = ElypseController::GetSingletonPtr()->GetNextAppIndex();
	m_appIndexStr = StringConverter::toString( l_appIndex );
	m_name += m_appIndexStr;
	m_lastFrameTime = 0;
	ElypseController::GetSingletonPtr()->AddApplication( this );

	if ( ! p_linkedTo.empty() )
	{
		m_linked = true;
		ElypseController::GetSingletonPtr()->LinkInstanceTo( this, p_linkedTo );
	}

	if ( ret )
	{
		m_thread = new std::thread{ [this]()
		{
			ElypseController::GetSingletonPtr()->AddThread( *this );
		} };
	}

	Sleep( 50 );
	EMUSE_LOG_MESSAGE_RELEASE( "EMUSE INSTANCE THREAD ID : " + ToString( std::this_thread::get_id() ) );
	return ret;
}

ElypseInstance::~ElypseInstance()
{
	EMUSE_MESSAGE_NORMAL( "ElypseInstance::~ElypseInstance : " + m_name );

	if ( ! m_deleted )
	{
		Destroy();
	}

	auto l_lock = make_unique_lock( m_mutex );

	if ( m_thread )
	{
		m_thread->join();
		delete m_thread;
	}
}

void ElypseInstance::StartThread()
{
	ElypseController::GetSingletonPtr()->AddThread( *this );
}

void ElypseInstance::Destroy()
{
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::Destroy : " + m_name );
	auto l_lock = make_unique_lock( m_mutex );

	if ( m_deleted )
	{
		return;
	}

	ElypseResourceGroupManager::setPrefix( m_name );

	try
	{
		if ( m_root != nullptr )
		{
			_destroyFrameListener();
			ElypseController::GetSingletonPtr()->UninitialiseRessources( m_name );

			if ( ! m_instancesLinkedToSelf.empty() )
			{
				General::Utils::set::cycle( m_instancesLinkedToSelf, & ElypseInstance::_destroyFrameListener );
			}

			if ( m_renderWindow != nullptr )
			{
				EMUSE_MESSAGE_RELEASE( "ElypseInstance::Destroy : Removing viewports" );
				m_renderWindow->removeAllViewports();
				m_renderWindow->removeAllListeners();

				if ( ! m_hasPrimaryWindow )
				{
					EMUSE_MESSAGE_RELEASE( "ElypseInstance::Destroy : Destroying window" );
					m_root->getRenderSystem()->destroyRenderWindow( m_renderWindow->getName() );
				}
				else
				{
					EMUSE_MESSAGE_RELEASE( "ElypseInstance::Destroy : Deactivating primary window" );
					m_renderWindow->setActive( false );
					m_renderWindow->setAutoUpdated( false );
				}
			}
		}
		else
		{
			EMUSE_MESSAGE_RELEASE( "ElypseInstance::Destroy : ERROR : Main : Root already null" );
		}
	}
	catch ( ... )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Destroy : exception caught when trying to delete the instance" );
	}

	m_toDelete = false;
	m_initialised = false;
	m_deleted = true;
	m_condition.notify_one();
}

void ElypseInstance::Initialise()
{
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::Initialise" );
	m_initialised = true;

	try
	{
		m_root = Root::getSingletonPtr();

		if ( m_handle.empty() || m_handle == "0" )
		{
			EMUSE_MESSAGE_RELEASE( "Main : Initialisation failed, no handle" );
			m_toDelete = true;
			return;
		}

		if ( m_mainUrl.empty() )
		{
			EMUSE_MESSAGE_RELEASE( "Main : Initialisation failed, no file name" );
			m_toDelete = true;
			return;
		}

		EMUSE_MESSAGE_NORMAL( "Main : Initialisation started for instance " + m_name );

		if ( m_renderWindow == nullptr )
		{
			m_renderWindow = ElypseController::GetSingletonPtr()->CreateRenderWindow( m_name, m_width, m_height, m_handle, m_antialiasing, m_hasPrimaryWindow );
			m_renderWindow->setAutoUpdated( false );
		}

		EMUSE_MESSAGE_NORMAL( "Main : RenderWindow created" );
		m_loadingBar = ElypseController::GetSingletonPtr()->GetLoadingBar();
		m_loadingBar->SetWindow( m_renderWindow );

		if ( ! m_linked )
		{
//			m_sceneMgr = ElypseController::GetSingletonPtr()->CreateSceneManager( "ElypsePlayer" + m_appIndexStr);
			EMUSE_MESSAGE_DEBUG( "Main : SceneManager created : " + m_name );
			CreateScene();
//			Sleep( 5000);
			EMUSE_MESSAGE_DEBUG( "Main : Initialisation finished" );
		}

		m_initialised = true;
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (General) --> " + p_exception.GetDescription() );
		m_toDelete = true;
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}
	catch ( Exception & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (Ogre) --> " + p_exception.getDescription() );
		m_toDelete = true;
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}
	catch ( std::exception & p_e )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (Stl) --> " + String( p_e.what() ) );
		m_toDelete = true;
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}
	catch ( ... )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::Initialise, exception (unknown)\n" );
		m_toDelete = true;
		m_plugin->SetGraphicalStatus( StatusErrorOgre );
	}

	if ( ! m_toDelete )
	{
		m_plugin->SetGraphicalStatus( StatusNone );
	}
}

void ElypseInstance::CreateScene()
{
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : opening" );

	if ( m_linked )
	{
		_createLinkedFramelistener();
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : m_filepath : " + m_mainUrl );
		ElypseController::GetSingletonPtr()->InitialiseRessources( m_name );
		m_downManager = ElypseController::GetSingletonPtr()->GetDownloadManager();
		genlib_assert( m_downManager != nullptr );
		EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : wait for download initialised" );
		_createNewFramelistener();
		EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : next is fl ctor" );
		m_frameListener->Initialise();
	}

	if ( m_showFPS )
	{
		m_frameListener->ShowDebugOverlay( true );
	}

	EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : the end" );
}

void ElypseInstance::_createNewFramelistener()
{
	m_frameListener = new ElypseFrameListener( *this, m_renderWindow, m_mainUrl, m_installationPath, m_appIndexStr );
	m_frameListener->SetStartupScript( m_startupScript );
	m_frameListener->PreInitialise();
}

void ElypseInstance::_createLinkedFramelistener()
{
	m_frameListener = new ElypseFrameListener( *this, m_frameListener, m_renderWindow, m_appIndexStr );
}

void ElypseInstance::SetToDelete( bool p_toDelete )
{
	m_toDelete = p_toDelete;
	EMUSE_MESSAGE_NORMAL( "ElypseInstance::SetToDelete - " + m_appIndexStr );
}

void ElypseInstance::RenderOneFrame()
{
	//auto l_lock = make_unique_lock( m_mutex );
	if ( m_toDelete )
	{
		return;
	}

	EMUSE_CONSOLE_MESSAGE_DEBUG( "RenderOneFrame" );
	Timer * l_timer = Root::getSingletonPtr()->getTimer();
	auto l_frameTime = uint32_t( l_timer->getMilliseconds() );
	auto l_timeSinceLastFrame = l_frameTime - m_lastFrameTime;
	Real l_time = Real( l_timeSinceLastFrame / 1000.0 );
	m_lastFrameTime = l_frameTime;

//	PreciseTimer l_preciseTimer;

	try
	{
		if ( m_frameListener != nullptr )
		{
			if ( ! m_deactivated )
			{
				m_frameListener->frameStarted( l_time );
			}
		}
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(pre), Exception (General) --> " + p_exception.GetDescription() );
	}
	catch ( Exception & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(pre), Exception (Ogre) --> " + p_exception.getDescription() );
	}
	catch ( std::exception & p_e )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(pre), Exception (Stl) --> " + String( p_e.what() ) );
	}
	catch ( ... )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(pre), Exception (unkown)" );
	}

//	EMUSE_CONSOLE_MESSAGE_RELEASE("Time Preframe : " + StringConverter::toString( static_cast<Real>(l_preciseTimer.Time())));

	try
	{
		if ( m_renderWindow != nullptr )
		{
			m_renderWindow->update();
		}
		else
		{
//			EMUSE_SLEEP(5);
		}
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(rendering), Exception (General) --> " + p_exception.GetDescription() );
	}
	catch ( Exception & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(rendering), Exception (Ogre) --> " + p_exception.getDescription() );
	}
	catch ( std::exception & p_e )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(rendering), Exception (Stl) --> " + String( p_e.what() ) );
	}
	catch ( ... )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(rendering), Exception (unkown)" );
	}

	try
	{
		if ( m_frameListener != nullptr )
		{
			if ( ! m_deactivated )
			{
				m_frameListener->frameEnded( l_time );
			}
		}
	}
	catch ( GenException & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(post), Exception (General) --> " + p_exception.GetDescription() );
	}
	catch ( Exception & p_exception )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(post), Exception (Ogre) --> " + p_exception.getDescription() );
	}
	catch ( std::exception & p_e )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(post), Exception (Stl) --> " + String( p_e.what() ) );
	}
	catch ( ... )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::RenderOneFrame(post), Exception (unkown)" );
	}
}

void ElypseInstance::LinkTo( ElypseInstance * p_originalInstance )
{
	genlib_assert( p_originalInstance != nullptr );
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::LinkTo from " + m_instanceName + String( " to " ) + p_originalInstance->GetName() );

	if ( ! m_initialised )
	{
		EMUSE_MESSAGE_DEBUG( "ElypseInstance::LinkTo forceful initialisation" );
		Initialise();
	}

	m_frameListener = p_originalInstance->m_frameListener;
	p_originalInstance->_isOriginLinkFor( this );
	CreateScene();
}

void ElypseInstance::_isOriginLinkFor( ElypseInstance * p_instance )
{
	m_instancesLinkedToSelf.insert( p_instance );
}

void ElypseInstance::_destroyFrameListener()
{
	if ( m_frameListener != nullptr )
	{
		EMUSE_MESSAGE_RELEASE( "Main : Deleting framelistener" );
		delete m_frameListener;
		m_frameListener = nullptr;
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "ElypseInstance::_destroyFrameListener while no FL" );
	}
}

bool ElypseInstance::IsActive()
{
	if ( this != nullptr )
	{
		auto l_lock = make_unique_lock( m_mutex );
		return ( m_initialised && m_frameListener != nullptr && m_frameListener->IsReady() );
	}

	return false;
}

void ElypseInstance::WaitForDeletion()
{
	EMUSE_MESSAGE_DEBUG( "ElypseInstance::WaitForDeletion" );
	m_toDelete = true;

	if ( m_deleted )
	{
		return;
	}

	if ( ! m_initialised )
	{
		EMUSE_MESSAGE_RELEASE( "ElypseInstance::WaitForDeletion -> Already deleted or never initialised" );
		return;
	}

	auto l_lock = make_unique_lock( m_mutex );
	EMUSE_MESSAGE_RELEASE( "ElypseInstance::WaitForDeletion -> wait for deletion" );
	m_condition.wait( l_lock );
	EMUSE_MESSAGE_RELEASE( "ElypseInstance::WaitForDeletion -> deleted" );
}

void ElypseInstance::SetStartupScript( String const & p_script )
{
	m_startupScript = p_script;

	if ( m_frameListener )
	{
		m_frameListener->SetStartupScript( m_startupScript );
	}
}
