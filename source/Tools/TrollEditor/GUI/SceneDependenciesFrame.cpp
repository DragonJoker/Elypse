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
		namespace
		{
			int constexpr MarginSize = 10;
		}

		enum SceneDependenciesFrameIDs
		{
			sdfSceneSelector,
			sdfSceneDependencies,
		};

		SceneDependenciesFrame::SceneDependenciesFrame( wxWindow * parent
			, ProjectComponents::ScenePtr scene
			, wxString const & title
			, wxPoint const & pos )
			: wxDialog{ parent, wxID_ANY, title, pos, wxSize{ 200, 300 }, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT | wxFRAME_TOOL_WINDOW }
			, m_selectedScene{ scene }
		{
			int const WindowWidth = GetClientSize().x;
			int const WindowHeight = GetClientSize().y;
			int const WindowHalfWidth = WindowWidth / 2;
			int const ButtonWidth = 70;
			int const ButtonHeight = 25;
			int EditWidth = 40;
			int const EditHeight = 25;
			int ComboWidth = WindowWidth - EditWidth - ( 3 * MarginSize );
			int const ListWidth = WindowWidth - ( 2 * MarginSize );
			int const ListHeight = WindowHeight - ( 5 * MarginSize ) - ButtonHeight - EditHeight;

			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			auto top = MarginSize;
			auto left = MarginSize;

			wxStaticText * text = new wxStaticText{ this
				, wxID_ANY
				, _( "Scene" )
				, wxPoint{ left, top }
				, wxSize{ EditWidth, EditHeight }
				, wxTE_MULTILINE };
			text->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			text->SetForegroundColour( GuiCommon::PanelForegroundColour );
			auto textExtent = text->GetTextExtent( text->GetLabel() );
			auto textH = textExtent.y;
			EditWidth = textExtent.x;
			ComboWidth = WindowWidth - EditWidth - ( 3 * MarginSize );
			// Adjust vertical position, since no vertical alignment stlye for wxStaticText.
			top += ( EditHeight - textH ) / 2;
			text->SetPosition( wxPoint{ left, top } );
			text->SetSize( wxSize{ EditWidth, EditHeight } );
			top -= ( EditHeight - textH ) / 2;
			left += EditWidth + MarginSize;
			wxString value;
			wxArrayString values;
			m_sceneSelector = new wxComboBox{ this
				, sdfSceneSelector
				, value
				, wxPoint{ left, top }
				, wxSize{ ComboWidth, EditHeight }
				, values
				, wxBORDER_SIMPLE | wxCB_READONLY };
			m_sceneSelector->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_sceneSelector->SetForegroundColour( GuiCommon::PanelForegroundColour );
			top += EditHeight + MarginSize;

			left = MarginSize;
			m_sceneDependencies = new wxListBox{ this
				, sdfSceneDependencies
				, wxPoint{ left, top }
				, wxSize{ ListWidth, ListHeight }
				, wxArrayString{}
				, wxLB_EXTENDED };
			m_sceneDependencies->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_sceneDependencies->SetForegroundColour( GuiCommon::PanelForegroundColour );
			top += ListHeight + MarginSize;

			left = WindowHalfWidth - ButtonWidth / 2;
			new GuiCommon::GradientButton{ this
				, wxID_OK
				, _( "Close" )
				, wxPoint{ left, top }
				, wxSize{ ButtonWidth, ButtonHeight } };
			top += ButtonHeight + MarginSize;
			SetClientSize( wxSize{ WindowWidth, top } );

			DoInitialise();
		}

		void SceneDependenciesFrame::SetScene( ProjectComponents::ScenePtr scene )
		{
			m_selectedScene = scene;
			DoInitialise();
		}

		void SceneDependenciesFrame::DoInitialise()
		{
			auto mainFrame = wxGetApp().GetMainFrame();

			if ( !mainFrame->GetProject() || !mainFrame->GetProject()->HasMainScene() )
			{
				return;
			}

			auto project = mainFrame->GetProject();

			for ( auto & scene : project->GetScenes() )
			{
				m_scenes.emplace( scene->GetName(), scene );
			}

			m_scenes.emplace( project->GetMainScene()->GetName(), project->GetMainScene() );
			m_sceneSelector->Clear();

			if ( !m_selectedScene && !m_scenes.empty() )
			{
				m_selectedScene = m_scenes.begin()->second;
			}

			for ( auto it : m_scenes )
			{
				m_sceneSelector->AppendString( it.first );
			}

			m_sceneSelector->SetSelection( 0 );
			DoPopulateSceneDependencies();
		}

		SceneDependenciesFrame::~SceneDependenciesFrame()
		{
			m_scenes.clear();
		}

		void SceneDependenciesFrame::DoPopulateSceneDependencies()
		{
			wxArrayString values;
			auto scenesThatNeedMe = m_selectedScene->GetScenesThatNeedMe();

			for ( auto it : m_scenes )
			{
				if ( it.second != m_selectedScene
					&& scenesThatNeedMe.end() == std::find( scenesThatNeedMe.begin()
						, scenesThatNeedMe.end()
						, it.first ) )
				{
					values.push_back( it.first );
				}
			}

			m_sceneDependencies->Clear();
			m_sceneDependencies->Append( values );
			uint32_t cpt = 0;

			for ( auto it : m_scenes )
			{
				if ( it.second != m_selectedScene
					&& scenesThatNeedMe.end() == std::find( scenesThatNeedMe.begin()
						, scenesThatNeedMe.end()
						, it.first ) )
				{
					auto scenesThatNeedMe2 = it.second->GetScenesThatNeedMe();

					if ( scenesThatNeedMe2.end() != std::find( scenesThatNeedMe2.begin()
						, scenesThatNeedMe2.end()
						, m_selectedScene->GetName() ) )
					{
						m_sceneDependencies->Select( cpt );
					}

					cpt++;
				}
			}
		}

		BEGIN_EVENT_TABLE( SceneDependenciesFrame, wxDialog )
			EVT_CLOSE( SceneDependenciesFrame::OnClose )
			EVT_LISTBOX( sdfSceneDependencies, SceneDependenciesFrame::OnCheckScene )
			EVT_COMBOBOX( sdfSceneSelector, SceneDependenciesFrame::OnSelectScene )
			EVT_BUTTON( wxID_OK, SceneDependenciesFrame::OnOK )
		END_EVENT_TABLE()

		void SceneDependenciesFrame::OnClose( wxCloseEvent & event )
		{
			Hide();
		}

		void SceneDependenciesFrame::OnCheckScene( wxCommandEvent & event )
		{
			uint32_t count = m_sceneDependencies->GetCount();

			for ( uint32_t i = 0; i < count; i++ )
			{
				auto scene = m_sceneDependencies->GetString( i );
				auto it = m_scenes.find( scene );

				if ( it != m_scenes.end() )
				{
					if ( m_sceneDependencies->IsSelected( i ) )
					{
						m_selectedScene->AddDependency( it->second );
					}
					else
					{
						m_selectedScene->RemoveDependency( it->second );
					}
				}
			}
		}

		void SceneDependenciesFrame::OnSelectScene( wxCommandEvent & event )
		{
			auto it = m_scenes.find( m_sceneSelector->GetValue() );

			if ( it != m_scenes.end() && it->second != m_selectedScene )
			{
				m_selectedScene = it->second;
				DoPopulateSceneDependencies();
			}
		}

		void SceneDependenciesFrame::OnOK( wxCommandEvent & event )
		{
			Hide();
		}
	}
}
