//********************************************************************************************************************
#include "PrecompiledHeader.h"

#include "Main/TrollInstance.h"
#include "Main/TrollFrameListener.h"
#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/3D/SceneNode.h"
#include "GUI/MainFrame.h"
/*
#include <Main/EMuseController.h>
#include <Main/EMusePlugin.h>
#include <Main/Context.h>
#include <Download/MuseDownloader.h>
#include <Download/DownloadManager.h>
#include <EMuseLogs.h>

#include <OgreLog.h>
#include <OgreStringConverter.h>

#include <General/Utils.h>
*/
//********************************************************************************************************************
using namespace Troll::Main;
using namespace Troll::GUI;
using namespace Troll::Objects3D;
//********************************************************************************************************************
extern MainFrame * g_mainFrame;
//********************************************************************************************************************

TrollInstance :: TrollInstance( const String & p_installPath , EMusePlugin * p_plugin )
	:	EMuseInstance( p_installPath, p_plugin ),
		m_sceneCreated( false )
{
	EMUSE_MESSAGE_DEBUG( "TrollInstance :: TrollInstance" );
	m_name.clear();
}

//********************************************************************************************************************

TrollInstance :: ~TrollInstance()
{
}

//********************************************************************************************************************

void TrollInstance :: CreateScene()
{
	m_name.clear();
	m_sceneCreated = false;

	if ( m_linked )
	{
		m_frameListener = new TrollFrameListener( this, static_cast <TrollFrameListener *>( m_frameListener ), m_renderWindow, m_appIndexStr );
	}
	else
	{
		EMUSE_MESSAGE_DEBUG( "TrollInstance :: CreateScene : m_filepath : " + m_filePath );
		EMuseController::GetSingletonPtr()->InitialiseRessources( "" );
		m_downManager = EMuseController::GetSingletonPtr()->GetDownloadManager();
		genlib_assert( m_downManager != NULL );
		m_downManager->StartDownloadFile( m_mainUrl );
		/*
		MuseDownloader * l_firstDownloader = m_downManager->GetFirstDownloader();

		while ( ! l_firstDownloader->IsInitialised())
		{
			GENLIB_SLEEP(5);
		}
		if (l_firstDownloader->CantDownload())
		{
			m_toDelete = true;
			return;
		}
		*/
		EMUSE_MESSAGE_DEBUG( "EMuseInstance :: CreateScene : wait for download initialised" );
		m_frameListener = new TrollFrameListener( this, m_renderWindow, m_mainUrl, m_installationPath, m_appIndexStr );
		m_frameListener->SetStartupScript( m_startupScript );
		m_frameListener->PreInitialise();
//		m_downManager->WaitForFile( "main.emcfg", true);
		EMUSE_MESSAGE_DEBUG( "TrollInstance :: CreateScene : next is fl ctor" );
		m_frameListener->Initialise();
	}

	if ( m_showFPS )
	{
		m_frameListener->ShowDebugOverlay( true );
	}

	TrollSceneNode * l_rootNode = g_mainFrame->GetCurrentProject()->GetMainScene()->AddSceneNode( wxT( "root node" ), wxT( "" ) );
	l_rootNode->SetOgreNode( m_frameListener->GetContext()->sceneManager->getRootSceneNode() );
	m_sceneCreated = true;
	EMUSE_MESSAGE_DEBUG( "TrollInstance :: CreateScene : the end" );
}

//********************************************************************************************************************
