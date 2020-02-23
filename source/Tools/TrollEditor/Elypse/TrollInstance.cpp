/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Elypse/TrollInstance.h"

#include "Elypse/TrollFrameListener.h"

#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/3D/SceneNode.h"

#include "TrollEditorApplication.h"
#include "GUI/MainFrame.h"

namespace Troll
{
	namespace ElypseRW
	{
		TrollInstance::TrollInstance( Path const & p_installPath, ElypsePlugin * p_plugin )
			: ElypseInstance( p_installPath, p_plugin )
			, m_sceneCreated( false )
		{
			EMUSE_MESSAGE_DEBUG( "TrollInstance::TrollInstance" );
		}

		TrollInstance::~TrollInstance()
		{
		}

		void TrollInstance::CreateScene()
		{
			EMUSE_MESSAGE_DEBUG( "TrollInstance::CreateScene : opening" );
			ElypseInstance::CreateScene();
			m_sceneCreated = false;
			auto * l_rootNode = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddSceneNode( wxT( "root node" ), wxT( "" ) );
			l_rootNode->SetOgreNode( m_frameListener->GetContext()->sceneManager->getRootSceneNode() );
			m_sceneCreated = true;
			EMUSE_MESSAGE_DEBUG( "TrollInstance::CreateScene : the end" );
		}

		void TrollInstance::_createNewFramelistener()
		{
			m_downManager->StartDownloadFile( m_mainUrl );
			m_frameListener = new TrollFrameListener( *this, m_renderWindow, m_mainUrl, m_installationPath, m_appIndexStr );
			m_frameListener->SetStartupScript( m_startupScript );
			m_frameListener->PreInitialise();
		}

		void TrollInstance::_createLinkedFramelistener()
		{
			m_frameListener = new ElypseFrameListener( *this, m_frameListener, m_renderWindow, m_appIndexStr );
		}
	}
}
