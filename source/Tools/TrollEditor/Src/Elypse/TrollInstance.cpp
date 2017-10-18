/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
		TrollInstance::TrollInstance( String const & p_installPath, ElypsePlugin * p_plugin )
			: ElypseInstance( p_installPath, p_plugin )
			, m_sceneCreated( false )
		{
			EMUSE_MESSAGE_DEBUG( "TrollInstance::TrollInstance" );
			m_name.clear();
		}

		TrollInstance::~TrollInstance()
		{
		}

		void TrollInstance::CreateScene()
		{
			m_name.clear();
			m_sceneCreated = false;

			if ( m_linked )
			{
				m_frameListener = new TrollFrameListener( *this, static_cast < TrollFrameListener * >( m_frameListener ), m_renderWindow, m_appIndexStr );
			}
			else
			{
				EMUSE_MESSAGE_DEBUG( "TrollInstance::CreateScene : m_filepath : " + m_filePath );
				ElypseController::GetSingletonPtr()->InitialiseRessources( "" );
				m_downManager = ElypseController::GetSingletonPtr()->GetDownloadManager();
				genlib_assert( m_downManager != NULL );
				m_downManager->StartDownloadFile( m_mainUrl );

				//MuseDownloader * l_firstDownloader = m_downManager->GetFirstDownloader();

				//while ( !l_firstDownloader->IsInitialised())
				//{
				//	GENLIB_SLEEP(5);
				//}

				//if (l_firstDownloader->CantDownload())
				//{
				//	m_toDelete = true;
				//	return;
				//}

				EMUSE_MESSAGE_DEBUG( "ElypseInstance::CreateScene : wait for download initialised" );
				m_frameListener = new TrollFrameListener( *this, m_renderWindow, m_mainUrl, m_installationPath, m_appIndexStr );
				m_frameListener->SetStartupScript( m_startupScript );
				m_frameListener->PreInitialise();
				//m_downManager->WaitForFile( "main.emcfg", true);
				EMUSE_MESSAGE_DEBUG( "TrollInstance::CreateScene : next is fl ctor" );
				m_frameListener->Initialise();
			}

			if ( m_showFPS )
			{
				m_frameListener->ShowDebugOverlay( true );
			}

			auto * l_rootNode = wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->AddSceneNode( wxT( "root node" ), wxT( "" ) );
			l_rootNode->SetOgreNode( m_frameListener->GetContext()->sceneManager->getRootSceneNode() );
			m_sceneCreated = true;
			EMUSE_MESSAGE_DEBUG( "TrollInstance::CreateScene : the end" );
		}
	}
}
