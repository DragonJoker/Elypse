
#include "PrecompiledHeader.h"

#include "GUI/SceneDependenciesFrame.h"
#include "GUI/MainFrame.h"
#include "Project/Project.h"
#include "Project/Scene.h"

using namespace Troll;
using namespace Troll::GUI;
extern MainFrame * g_mainFrame;

BEGIN_EVENT_TABLE( SceneDependenciesFrame, wxFrame )
	EVT_CLOSE(	SceneDependenciesFrame::_onClose )
	EVT_CHECKLISTBOX(	sdfSceneDependencies,	SceneDependenciesFrame::_onCheckScene )
	EVT_COMBOBOX(	sdfSceneSelector,		SceneDependenciesFrame::_onSelectScene )
	EVT_BUTTON(	sdfOkBtn,				SceneDependenciesFrame::_onOK )
END_EVENT_TABLE()


SceneDependenciesFrame::SceneDependenciesFrame( wxWindow * p_parent, TrollScene * p_scene,
		const wxString & title, const wxPoint & pos )
	:	wxFrame( p_parent, wxID_ANY, title, pos, wxSize( 200, 300 ), wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW ),
		m_selectedScene( p_scene ),
		m_sceneDependencies( NULL ),
		m_sceneSelector( NULL )
{
	wxArrayString l_values;
	wxString l_value;
	wxPoint l_position( GetClientSize().x / 2, 10 );
	m_sceneSelector = BuildComboBox( this, l_position, sdfSceneSelector, wxT( "Scène" ), l_values, l_value );
	_initialise();
	m_okBtn = new wxButton( this, sdfOkBtn, wxT( "Fermer" ), wxPoint( 65, GetClientSize().y - 35 ), wxSize( 70, 25 ) );
}



void SceneDependenciesFrame::SetScene( TrollScene * p_scene )
{
	m_selectedScene = p_scene;
	_initialise();
}



void SceneDependenciesFrame::_initialise()
{
	if ( !g_mainFrame->GetCurrentProject() || !g_mainFrame->GetCurrentProject()->GetMainScene() )
	{
		return;
	}

	SceneMap l_scenes = g_mainFrame->GetCurrentProject()->GetScenes();

	for ( SceneMap::iterator l_it = l_scenes.begin() ; l_it != l_scenes.end() ; ++l_it )
	{
		m_scenes.insert( SceneMap::value_type( l_it->first, l_it->second ) );
	}

	m_scenes.insert( SceneMap::value_type( g_mainFrame->GetCurrentProject()->GetMainScene()->GetName(), g_mainFrame->GetCurrentProject()->GetMainScene() ) );
	wxArrayString l_values;
	wxString l_value;
	m_sceneSelector->Clear();

	if ( m_selectedScene != NULL )
	{
		l_value = m_selectedScene->GetName();
	}

	SceneMap::iterator l_it;

	for ( l_it = m_scenes.begin() ; l_it != m_scenes.end() ; ++l_it )
	{
		if ( l_value.empty() )
		{
			l_value = l_it->first;
			m_selectedScene = l_it->second;
		}

		l_values.push_back( l_it->first );
		m_sceneSelector->Insert( l_it->first, 0 );
	}

	m_sceneSelector->SetSelection( 0 );
	wxPoint l_position( GetClientSize().x / 2, 10 );
	_populateSceneDependencies();
}



SceneDependenciesFrame::~SceneDependenciesFrame()
{
	m_scenes.clear();
}



void SceneDependenciesFrame::_populateSceneDependencies()
{
	if ( m_sceneDependencies != NULL )
	{
		this->RemoveChild( m_sceneDependencies );
		delete m_sceneDependencies;
		m_sceneDependencies = NULL;
	}

	SceneMap::iterator l_it;
	wxArrayString l_values;
	SceneMap l_scenesThatNeedMe = m_selectedScene->GetScenesThatNeedMe();

	for ( l_it = m_scenes.begin() ; l_it != m_scenes.end() ; l_it++ )
	{
		if ( l_it->second != m_selectedScene && l_scenesThatNeedMe.find( l_it->first ) == l_scenesThatNeedMe.end() )
		{
			l_values.push_back( l_it->first );
		}
	}

	m_sceneDependencies = new wxCheckListBox( this, sdfSceneDependencies, wxPoint( 10, 35 ),
			wxSize( GetClientSize().x - 20, GetClientSize().y - 70 ), l_values,
			wxLB_SINGLE | wxLB_NEEDED_SB );
	unsigned int l_cpt = 0;
	SceneMap l_scenesThatNeedMe2;

	for ( l_it = m_scenes.begin() ; l_it != m_scenes.end() ; l_it++ )
	{
		if ( l_it->second != m_selectedScene && l_scenesThatNeedMe.find( l_it->first ) == l_scenesThatNeedMe.end() )
		{
			l_scenesThatNeedMe2 = l_it->second->GetScenesThatNeedMe();

			if ( l_scenesThatNeedMe2.find( m_selectedScene->GetName() ) != l_scenesThatNeedMe2.end() )
			{
				m_sceneDependencies->Check( l_cpt, true );
			}

			l_cpt++;
		}
	}
}



void SceneDependenciesFrame::_onClose( wxCloseEvent & p_event )
{
	Hide();
}



void SceneDependenciesFrame::_onCheckScene( wxCommandEvent & p_event )
{
	unsigned int l_count = m_sceneDependencies->GetCount();
	wxString l_scene;
	SceneMap::iterator l_it;

	for ( unsigned int i = 0 ; i < l_count ; i++ )
	{
		l_scene = m_sceneDependencies->GetString( i );
		l_it = m_scenes.find( l_scene );

		if ( l_it != m_scenes.end() )
		{
			if ( m_sceneDependencies->IsChecked( i ) )
			{
				m_selectedScene->AddDependency( l_it->second );
			}
			else
			{
				m_selectedScene->RemoveDependency( l_it->second );
			}
		}
	}
}



void SceneDependenciesFrame::_onSelectScene( wxCommandEvent & p_event )
{
	SceneMap::iterator l_it = m_scenes.find( m_sceneSelector->GetValue() );

	if ( l_it != m_scenes.end() && l_it->second != m_selectedScene )
	{
		m_selectedScene = l_it->second;
		_populateSceneDependencies();
	}
}



void SceneDependenciesFrame::_onOK( wxCommandEvent & p_event )
{
	Hide();
}


