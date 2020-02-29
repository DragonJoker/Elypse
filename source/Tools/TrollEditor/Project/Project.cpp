/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Project.h"

#include "Scene.h"
#include "ProjectFileParser.h"
#include "GUI/FilesTree.h"

namespace Troll
{
	namespace ProjectComponents
	{
		Project::Project()
			: m_shadows( false )
			, m_antiAliasing( aa0 )
			, m_bgImage()
			, m_mainScene( nullptr )
			, m_saved( false )
			, m_modified( false )
			, m_bgType( bgColour )
			, m_bgColour()
			, m_showDebug( false )
			, m_showFPS( false )
		{
			m_bgString = m_bgColour.GetAsString();
		}

		Project::Project( wxString const & p_projectName
			, wxString const & p_mainSceneName
			, wxString const & p_path
			, BackgroundType p_backgroundType
			, wxString const & p_background
			, bool p_shadows
			, AntiAliasing p_aa
			, wxSize const & p_resolution )
			: m_shadows( p_shadows )
			, m_antiAliasing( p_aa )
			, m_projectPath( p_path )
			, m_name( p_projectName )
			, m_saved( false )
			, m_bgImage()
			, m_bgType( p_backgroundType )
			, m_bgString( p_background )
			, m_bgColour()
			, m_resolution( p_resolution )
		{
			m_mainScene = CreateScene( p_mainSceneName );

			if ( p_backgroundType == bgColour )
			{
				m_bgColour.Set( p_background );
			}
			else
			{
				m_bgImage = wxImage( p_background );
			}
		}

		Project::~Project()
		{
		}

		void Project::FlushObjects()
		{
			if ( m_mainScene )
			{
				m_mainScene->FlushObjects();
			}

			for ( auto & l_scene : m_scenes )
			{
				l_scene->FlushObjects();
			}
		}

		ScenePtr Project::GetScene( wxString const & p_sceneName )
		{
			if ( m_mainScene 
				&& m_mainScene->GetName() == p_sceneName )
			{
				return m_mainScene;
			}

			auto l_it = std::find_if( m_scenes.begin()
				, m_scenes.end()
				, [&p_sceneName]( ScenePtr p_scene )
				{
					return p_scene->GetName() == p_sceneName;
				} );

			if ( l_it == m_scenes.end() )
			{
				GENLIB_EXCEPTION( "No Scene with this name." );
			}

			return *l_it;
		}

		void Project::RemoveScene( wxString const & p_sceneName )
		{
			if ( m_mainScene && m_mainScene->GetName() == p_sceneName )
			{
				wxMessageBox( _( "Can't remove the main scene" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, nullptr );
			}
			else
			{
				auto l_it = std::find_if( m_scenes.begin()
					, m_scenes.end()
					, [&p_sceneName]( ScenePtr p_scene )
					{
						return p_scene->GetName() == p_sceneName;
					} );

				if ( l_it != m_scenes.end() )
				{
					m_scenes.erase( l_it );
				}
			}
		}

		void Project::RemoveScene( ScenePtr p_scene )
		{
			RemoveScene( p_scene->GetName() );
		}

		ScenePtr Project::CreateScene( wxString const & p_strName )
		{
			ScenePtr result = std::make_shared< Scene >( *this, p_strName, m_mainScene == nullptr );

			if ( AddScene( result ) )
			{
				if ( !m_mainScene )
				{
					m_mainScene = result;
				}

				return m_scenes.back();
			}

			GENLIB_EXCEPTION( "Can't create an already existing scene." );
		}

		bool Project::AddScene( ScenePtr p_scene )
		{
			auto l_it = std::find_if( m_scenes.begin()
				, m_scenes.end()
				, [&p_scene]( ScenePtr p_lookup )
				{
					return p_lookup->GetName() == p_scene->GetName();
				} );

			bool l_return = l_it == m_scenes.end();

			if ( !l_return )
			{
				wxMessageBox( _( "Can't add a scene with this name, there is already a scene with this name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, nullptr );
			}
			else
			{
				m_scenes.push_back( p_scene );
			}

			return l_return;
		}

		void Project::Load( wxString const & p_path )
		{
			ProjectFileParser l_parser{ this };
			LogDebug( wxT( "Project::Load - " ) + p_path );

			wxFileName l_fileName( p_path );
			m_projectPath = l_fileName.GetPathWithSep();

			if ( l_parser.ParseFile( p_path ) )
			{
				std::clog << make_string( _( "Parsed project file" ) ) << std::endl;

				if ( m_mainScene )
				{
					DoFillSceneDependencies( *m_mainScene );

					for ( auto const & l_scene : m_scenes )
					{
						DoFillSceneDependencies( *l_scene );
					}
				}

				m_projectFileName = p_path;
				m_modified = false;
				m_saved = true;
			}
		}

		void Project::SaveScripts()
		{
			if ( m_mainScene )
			{
				m_mainScene->SaveScripts( m_projectPath );

				for ( auto const & l_scene : m_scenes )
				{
					l_scene->SaveScripts( m_projectPath );
				}
			}
		}

		void Project::SetBackgroundImage( wxString const & p_imgPath )
		{
			m_bgImage = wxImage( p_imgPath );
			m_bgString = p_imgPath;
			m_modified = true;
		}

		void Project::SetBackgroundColour( wxString const & p_colour )
		{
			m_bgColour.Set( p_colour );
			m_bgString = p_colour;
			m_modified = true;
		}

		bool Project::FindFileInScene( wxString const & p_fileName, wxString const & p_sceneName, File *& p_file, ScenePtr & p_scene )
		{
			try
			{
				p_scene = GetScene( p_sceneName );
			}
			catch ( std::exception & exc )
			{
				std::cerr << "Couldn't find scene [" << p_sceneName << "]" << std::endl;
				return false;
			}

			if ( p_scene )
			{
				try
				{
					p_file = &p_scene->GetFile( p_fileName );
				}
				catch ( std::exception & exc )
				{
					std::cerr << "Couldn't find file [" << p_fileName << "] in scene [" << p_sceneName << "]" << std::endl;
					return false;
				}
			}

			return true;
		}

		bool Project::FindFileInScenes( wxString const & p_fileName, File *& p_file, ScenePtr & p_scene )
		{
			bool l_return = false;

			try
			{
				p_file = &m_mainScene->GetFile( p_fileName );
				l_return = true;
			}
			catch ( General::Utils::GenException & )
			{
				// File not found in main scene, no need to warn user.
			}

			if ( !l_return )
			{
				auto && l_it = m_scenes.begin();

				while ( l_it != m_scenes.end() && !l_return )
				{
					try
					{
						p_file = &( *l_it )->GetFile( p_fileName );
						l_return = true;
					}
					catch ( General::Utils::GenException & )
					{
						// File not found in this scene, no need to warn user.
					}

					if ( l_return )
					{
						p_scene = *l_it;
					}

					++l_it;
				}
			}
			else
			{
				p_scene = m_mainScene;
			}

			return l_return;
		}

		void Project::DoFillSceneDependencies( Scene const & p_scene )
		{
			wxArrayString l_neededScenes = p_scene.GetNeededScenes();

			for ( auto && l_name : l_neededScenes )
			{
				auto l_it = std::find_if( m_scenes.begin()
					, m_scenes.end()
					, [&l_name]( ScenePtr l_scene )
					{
						return l_scene->GetName() == l_name;
					} );

				if ( l_it != m_scenes.end() )
				{
					( *l_it )->DoAddSceneThatNeedMe( p_scene );
					( *l_it )->SetNeeded( true );
				}
				else if ( l_name == m_mainScene->GetName() )
				{
					m_mainScene->DoAddSceneThatNeedMe( p_scene );
					m_mainScene->SetNeeded( true );
				}
			}
		}

		void Project::DoBuildColour( wxString const & p_infos )
		{
			wxArrayString l_infos = GUI::Split( p_infos, wxT( " " ) );
			double l_red, l_green, l_blue;
			unsigned char l_cred, l_cgreen, l_cblue;
			l_infos[0].ToDouble( &l_red );
			l_infos[1].ToDouble( &l_green );
			l_infos[2].ToDouble( &l_blue );
			l_cred = uint8_t( int( l_red * 255.0 ) );
			l_cgreen = uint8_t( int( l_green * 255.0 ) );
			l_cblue = uint8_t( int( l_blue * 255.0 ) );
			m_bgColour.Set( l_cred, l_cgreen, l_cblue );
		}
	}
}
