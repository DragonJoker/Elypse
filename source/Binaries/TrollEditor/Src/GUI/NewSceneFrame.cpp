//***********************************************************************************************************
#include "PrecompiledHeader.h"

#include "GUI/NewSceneFrame.h"
#include "Project/Scene.h"
#include "Project/Project.h"
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
//***********************************************************************************************************
using namespace Troll;
using namespace Troll::GUI;
//***********************************************************************************************************
BEGIN_EVENT_TABLE( NewSceneFrame, wxDialog )
	EVT_BUTTON(	ID_okBtn,		NewSceneFrame::_onOK )
	EVT_BUTTON(	ID_clBtn,		NewSceneFrame::_onCancel )
END_EVENT_TABLE()
//***********************************************************************************************************
extern MainFrame * g_mainFrame;
//***********************************************************************************************************

NewSceneFrame :: NewSceneFrame( wxWindow * p_parent, const wxString & title, const wxPoint & pos )
	:	wxDialog( p_parent, -1, title, pos, wxSize( 200, 90 ) )
{
	wxSize l_size = GetClientSize();
	wxPoint l_position( l_size.x / 2, 10 );
	m_sceneName = BuildTextCtrl( this, l_position, wxID_ANY, wxT( "Scène" ), wxT( "Nom de la scene" ) );
	l_position.x = 20;
	m_okBtn = new wxButton( this, ID_okBtn, wxT( "Valider" ), l_position += wxPoint( 0, 25 ), wxSize( 70, 20 ) );
	m_cancelBtn = new wxButton( this, ID_clBtn, wxT( "Annuler" ), l_position += wxPoint( 90, 0 ), wxSize( 70, 20 ) );
}

//***********************************************************************************************************

NewSceneFrame :: ~NewSceneFrame()
{
}

//***********************************************************************************************************

wxString NewSceneFrame :: GetSceneName()
{
	return wxEmptyString;
}

//***********************************************************************************************************

void NewSceneFrame :: _onOK( wxCommandEvent & p_event )
{
	wxString l_sceneName = m_sceneName->GetValue();

	if ( l_sceneName.empty() || l_sceneName == wxT( "Nom de la scène" ) )
	{
		wxMessageBox( wxT( "Entrez un nom de scène" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_EXCLAMATION, this );
		return;
	}

	Project * l_project = g_mainFrame->GetCurrentProject();
	TrollScene * l_scene = new TrollScene( l_project, l_sceneName );

	try
	{
		l_scene->m_sceneId = g_mainFrame->m_filesList->AddSceneToProject( l_scene );
		l_scene->m_sceneFileId = g_mainFrame->m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de scène" ) );
		l_scene->m_loadScriptFileId = g_mainFrame->m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script de load" ) );
		l_scene->m_unloadScriptFileId = g_mainFrame->m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script d'unload" ) );
		l_scene->m_dataFileId = g_mainFrame->m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas Graphiques (pour la 3D)" ) );
		l_scene->m_dataFolderId = g_mainFrame->m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas non Graphiques (autres)" ) );
		l_project->AddScene( l_scene );
		wxString l_path = g_mainFrame->GetCurrentProject()->GetPath();
		l_path = l_path + l_sceneName;
		wxFileName::Mkdir( l_path, 0777, wxPATH_MKDIR_FULL );
	}
	catch ( bool )
	{
		g_mainFrame->LogDebugMessage( wxT( "NewSceneFrame :: OnNewScene - Probleme d'ajout" ) );
		return;
	}

	Destroy();
}

//***********************************************************************************************************

void NewSceneFrame :: _onCancel( wxCommandEvent & p_event )
{
	Destroy();
}

//***********************************************************************************************************
