/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/SceneDependenciesFrame.h"

#include "TrollEditorApplication.h"

#include "GUI/MainFrame.h"

#include "Project/Project.h"
#include "Project/Scene.h"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		enum SceneDependenciesFrameIDs
		{
			sdfSceneSelector,
			sdfSceneDependencies,
		};

		SceneDependenciesFrame::SceneDependenciesFrame( wxWindow * p_parent
			, ProjectComponents::ScenePtr p_scene
			, wxString const & title
			, wxPoint const & pos )
			: wxDialog{ p_parent, wxID_ANY, title, pos, wxSize( 200, 300 ), wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW }
			, m_selectedScene{ p_scene }
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			wxArrayString l_values;
			wxString l_value;
			wxPoint l_position( GetClientSize().x / 2, 10 );
			m_sceneSelector = BuildComboBox( this, l_position, sdfSceneSelector, _( "Scene" ), l_values, l_value );
			m_sceneSelector->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_sceneSelector->SetForegroundColour( GuiCommon::PanelForegroundColour );

			m_sceneDependencies = new wxCheckListBox( this
				, sdfSceneDependencies
				, wxPoint( 10, 35 )
				, wxSize( GetClientSize().x - 20, GetClientSize().y - 70 )
				, wxArrayString{}
				, wxLB_SINGLE | wxLB_NEEDED_SB );
			m_sceneDependencies->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_sceneDependencies->SetForegroundColour( GuiCommon::PanelForegroundColour );

			DoInitialise();

			auto l_sizerBtn = new wxBoxSizer( wxHORIZONTAL );
			l_sizerBtn->AddStretchSpacer();
			l_sizerBtn->Add( new GuiCommon::GradientButton( this, wxID_OK, _( "Close" ), wxPoint( 65, GetClientSize().y - 35 ), wxSize( 70, 25 ) ) );
			l_sizerBtn->AddStretchSpacer();

			auto l_sizerAll = new wxBoxSizer( wxVERTICAL );
			l_sizerAll->Add( m_sceneSelector, wxSizerFlags().Border( wxALL, 10 ) );
			l_sizerAll->Add( m_sceneDependencies, wxSizerFlags().Border( wxLEFT | wxRIGHT | wxBOTTOM, 10 ) );
			l_sizerAll->Add( l_sizerBtn, wxSizerFlags().Border( wxBOTTOM, 10 ) );

			l_sizerAll->SetSizeHints( this );
			SetSizer( l_sizerAll );
		}

		void SceneDependenciesFrame::SetScene( ProjectComponents::ScenePtr p_scene )
		{
			m_selectedScene = p_scene;
			DoInitialise();
		}

		void SceneDependenciesFrame::DoInitialise()
		{
			if ( !wxGetApp().GetMainFrame()->GetProject() || !wxGetApp().GetMainFrame()->GetProject()->HasMainScene() )
			{
				return;
			}

			for ( auto & l_scene : wxGetApp().GetMainFrame()->GetProject()->GetScenes() )
			{
				m_scenes.insert( { l_scene->GetName(), l_scene } );
			}

			m_scenes.insert( { wxGetApp().GetMainFrame()->GetProject()->GetMainScene()->GetName(), wxGetApp().GetMainFrame()->GetProject()->GetMainScene() } );
			wxArrayString l_values;
			wxString l_value;
			m_sceneSelector->Clear();

			if ( m_selectedScene )
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
			DoPopulateSceneDependencies();
		}

		SceneDependenciesFrame::~SceneDependenciesFrame()
		{
			m_scenes.clear();
		}

		void SceneDependenciesFrame::DoPopulateSceneDependencies()
		{
			wxArrayString l_values;
			auto && l_scenesThatNeedMe = m_selectedScene->GetScenesThatNeedMe();

			for ( auto && l_it : m_scenes )
			{
				if ( l_it.second != m_selectedScene && std::find( l_scenesThatNeedMe.begin(), l_scenesThatNeedMe.end(), l_it.first ) == l_scenesThatNeedMe.end() )
				{
					l_values.push_back( l_it.first );
				}
			}

			m_sceneDependencies->Clear();
			m_sceneDependencies->Append( l_values );
			uint32_t l_cpt = 0;

			for ( auto && l_it : m_scenes )
			{
				if ( l_it.second != m_selectedScene && std::find( l_scenesThatNeedMe.begin(), l_scenesThatNeedMe.end(), l_it.first ) == l_scenesThatNeedMe.end() )
				{
					auto && l_scenesThatNeedMe2 = l_it.second->GetScenesThatNeedMe();

					if ( std::find( l_scenesThatNeedMe2.begin(), l_scenesThatNeedMe2.end(), m_selectedScene->GetName() ) != l_scenesThatNeedMe2.end() )
					{
						m_sceneDependencies->Check( l_cpt, true );
					}

					l_cpt++;
				}
			}
		}

		BEGIN_EVENT_TABLE( SceneDependenciesFrame, wxDialog )
			EVT_CLOSE( SceneDependenciesFrame::OnClose )
			EVT_CHECKLISTBOX( sdfSceneDependencies, SceneDependenciesFrame::OnCheckScene )
			EVT_COMBOBOX( sdfSceneSelector, SceneDependenciesFrame::OnSelectScene )
			EVT_BUTTON( wxID_OK, SceneDependenciesFrame::OnOK )
		END_EVENT_TABLE()

		void SceneDependenciesFrame::OnClose( wxCloseEvent & p_event )
		{
			Hide();
		}

		void SceneDependenciesFrame::OnCheckScene( wxCommandEvent & p_event )
		{
			uint32_t l_count = m_sceneDependencies->GetCount();
			wxString l_scene;

			for ( uint32_t i = 0; i < l_count; i++ )
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
				DoPopulateSceneDependencies();
			}
		}

		void SceneDependenciesFrame::OnOK( wxCommandEvent & p_event )
		{
			Hide();
		}
	}
}
