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

#include "GUI/SceneDependenciesFrame.h"

#include "GUI/MainFrame.h"

#include "Project/Project.h"
#include "Project/Scene.h"

BEGIN_TROLL_GUI_NAMESPACE
{
	enum SceneDependenciesFrameIDs
	{
		sdfSceneSelector,
		sdfSceneDependencies,
		sdfOkBtn
	};

	BEGIN_EVENT_TABLE( SceneDependenciesFrame, wxFrame )
		EVT_CLOSE( SceneDependenciesFrame::OnClose )
		EVT_CHECKLISTBOX( sdfSceneDependencies, SceneDependenciesFrame::OnCheckScene )
		EVT_COMBOBOX( sdfSceneSelector, SceneDependenciesFrame::OnSelectScene )
		EVT_BUTTON( sdfOkBtn, SceneDependenciesFrame::OnOK )
	END_EVENT_TABLE()

	SceneDependenciesFrame::SceneDependenciesFrame( wxWindow * p_parent, TROLL_PROJECT_NAMESPACE::Scene * p_scene, const wxString & title, const wxPoint & pos )
		: wxFrame( p_parent, wxID_ANY, title, pos, wxSize( 200, 300 ), wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW )
		, m_selectedScene( p_scene )
		, m_sceneDependencies( NULL )
		, m_sceneSelector( NULL )
	{
		wxArrayString l_values;
		wxString l_value;
		wxPoint l_position( GetClientSize().x / 2, 10 );
		m_sceneSelector = BuildComboBox( this, l_position, sdfSceneSelector, _( "Scene" ), l_values, l_value );
		_initialise();
		m_okBtn = new wxButton( this, sdfOkBtn, _( "Close" ), wxPoint( 65, GetClientSize().y - 35 ), wxSize( 70, 25 ) );
	}

	void SceneDependenciesFrame::SetScene( TROLL_PROJECT_NAMESPACE::Scene * p_scene )
	{
		m_selectedScene = p_scene;
		_initialise();
	}

	void SceneDependenciesFrame::_initialise()
	{
		if ( !GUI::MainFrame::GetInstance()->GetCurrentProject() || !GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene() )
		{
			return;
		}

		for ( auto && l_it : GUI::MainFrame::GetInstance()->GetCurrentProject()->GetScenes() )
		{
			m_scenes.insert( std::make_pair( l_it.first, l_it.second ) );
		}

		m_scenes.insert( std::make_pair( GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene()->GetName(), GUI::MainFrame::GetInstance()->GetCurrentProject()->GetMainScene() ) );
		wxArrayString l_values;
		wxString l_value;
		m_sceneSelector->Clear();

		if ( m_selectedScene != NULL )
		{
			l_value = m_selectedScene->GetName();
		}

		for ( auto && l_it : m_scenes )
		{
			if ( l_value.empty() )
			{
				l_value = l_it.first;
				m_selectedScene = l_it.second;
			}

			l_values.push_back( l_it.first );
			m_sceneSelector->Insert( l_it.first, 0 );
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

		wxArrayString l_values;
		auto && l_scenesThatNeedMe = m_selectedScene->GetScenesThatNeedMe();

		for ( auto && l_it : m_scenes )
		{
			if ( l_it.second != m_selectedScene && l_scenesThatNeedMe.find( l_it.first ) == l_scenesThatNeedMe.end() )
			{
				l_values.push_back( l_it.first );
			}
		}

		m_sceneDependencies = new wxCheckListBox( this, sdfSceneDependencies, wxPoint( 10, 35 ), wxSize( GetClientSize().x - 20, GetClientSize().y - 70 ), l_values, wxLB_SINGLE | wxLB_NEEDED_SB );
		unsigned int l_cpt = 0;

		for ( auto && l_it : m_scenes )
		{
			if ( l_it.second != m_selectedScene && l_scenesThatNeedMe.find( l_it.first ) == l_scenesThatNeedMe.end() )
			{
				auto && l_scenesThatNeedMe2 = l_it.second->GetScenesThatNeedMe();

				if ( l_scenesThatNeedMe2.find( m_selectedScene->GetName() ) != l_scenesThatNeedMe2.end() )
				{
					m_sceneDependencies->Check( l_cpt, true );
				}

				l_cpt++;
			}
		}
	}

	void SceneDependenciesFrame::OnClose( wxCloseEvent & p_event )
	{
		Hide();
	}

	void SceneDependenciesFrame::OnCheckScene( wxCommandEvent & p_event )
	{
		unsigned int l_count = m_sceneDependencies->GetCount();
		wxString l_scene;

		for ( unsigned int i = 0 ; i < l_count ; i++ )
		{
			l_scene = m_sceneDependencies->GetString( i );
			auto && l_it = m_scenes.find( l_scene );

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

	void SceneDependenciesFrame::OnSelectScene( wxCommandEvent & p_event )
	{
		auto && l_it = m_scenes.find( m_sceneSelector->GetValue() );

		if ( l_it != m_scenes.end() && l_it->second != m_selectedScene )
		{
			m_selectedScene = l_it->second;
			_populateSceneDependencies();
		}
	}

	void SceneDependenciesFrame::OnOK( wxCommandEvent & p_event )
	{
		Hide();
	}
}
END_TROLL_GUI_NAMESPACE
