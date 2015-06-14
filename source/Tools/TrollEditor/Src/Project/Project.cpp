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

#include "Project.h"

#include "Scene.h"
#include "ProjectFileParser.h"
#include "GUI/FilesTree.h"

BEGIN_TROLL_PROJECT_NAMESPACE
{
	Project::Project()
		: m_shadows( false )
		, m_antiAliasing( aa0 )
		, m_bgImage()
		, m_mainScene( NULL )
		, m_saved( false )
		, m_modified( false )
		, m_bgType( bgColour )
		, m_bgColour()
		, m_showDebug( false )
		, m_showFPS( false )
	{
		m_bgString = m_bgColour.GetAsString();
	}

	Project::Project( const wxString & p_projectName, const wxString & p_mainSceneName, const wxString & p_path, BackgroundType p_backgroundType, const wxString & p_background, bool p_shadows, AntiAliasing p_aa, const wxSize & p_resolution )
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
		m_mainScene = new Scene( shared_from_this(), p_mainSceneName, true );

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
		delete m_mainScene;
		General::Utils::map::deleteAll( m_scenes );
	}

	void Project::FlushObjects()
	{
		if ( m_mainScene )
		{
			m_mainScene->FlushObjects();
		}

		General::Utils::map::cycle( m_scenes, & Scene::FlushObjects );
	}

	Scene * Project::GetScene( const wxString & p_sceneName )
	{
		if ( m_mainScene )
		{
			if ( m_mainScene->GetName() == p_sceneName )
			{
				return m_mainScene;
			}
		}

		SceneMap::iterator l_it = m_scenes.find( p_sceneName );

		if ( l_it != m_scenes.end() )
		{
			return l_it->second;
		}

		return NULL;
	}

	Scene * Project::GetScene( const wxTreeItemId & p_item )
	{
		if ( m_mainScene )
		{
			if ( m_mainScene->HasItem( p_item ) )
			{
				return m_mainScene;
			}
		}

		SceneMap::iterator l_it = m_scenes.begin();

		while ( l_it != m_scenes.end() )
		{
			if ( l_it->second->HasItem( p_item ) )
			{
				LogDebug( wxT( "Project::GetScene - Scene : " ) + l_it->second->GetName() );
				return l_it->second;
			}

			++l_it;
		}

		return NULL;
	}

	void Project::RemoveScene( const wxString & p_sceneName )
	{
		if ( m_mainScene )
		{
			if ( m_mainScene->GetName() == p_sceneName )
			{
				wxMessageBox( _( "Can't remove the main scene" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, NULL );
				return;
			}
		}

		SceneMap::iterator l_it = m_scenes.find( p_sceneName );

		if ( l_it != m_scenes.end() )
		{
			delete l_it->second;
			m_scenes.erase( l_it );
			return;
		}
	}

	void Project::RemoveScene( Scene * p_scene )
	{
		if ( m_mainScene )
		{
			if ( m_mainScene == p_scene )
			{
				wxMessageBox( _( "Can't remove the main scene" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, NULL );
				return;
			}
		}

		SceneMap::iterator l_it = m_scenes.find( p_scene->GetName() );

		if ( l_it != m_scenes.end() )
		{
			delete l_it->second;
			m_scenes.erase( l_it );
			return;
		}
	}



	Scene * Project::CreateScene( wxString const & p_strName )
	{
		Scene * l_pReturn = new Scene( shared_from_this(), p_strName, m_mainScene == NULL );

		if ( !m_mainScene )
		{
			m_mainScene = l_pReturn;
		}
		else
		{
			AddScene( l_pReturn );
		}

		return l_pReturn;
	}

	void Project::AddScene( Scene * p_scene )
	{
		SceneMap::iterator l_it = m_scenes.find( p_scene->GetName() );

		if ( l_it != m_scenes.end() )
		{
			wxMessageBox( _( "Can't add a scene with this name, there is already a scene with this name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, NULL );
			return;
		}

		m_scenes.insert( std::make_pair( p_scene->GetName(), p_scene ) );
	}



	void Project::Save( wxTextOutputStream * p_stream )
	{
		if ( ! p_stream )
		{
			return;
		}

		p_stream->WriteString( wxT( "project " ) + m_name + wxT( "\n{\n" ) );
	//	p_stream->WriteString( m_projectPath + "\n");
		wxString l_res;
		l_res << m_resolution.x << wxT( " " ) << m_resolution.y;
		p_stream->WriteString( wxT( "\tresolution\t\t" ) + l_res + wxT( "\n" ) );

		if ( m_bgType == bgColour )
		{
			l_res.clear();
			l_res << float( m_bgColour.Red() ) / 255.0 << wxT( " " ) << float( m_bgColour.Green() ) / 255.0 << wxT( " " ) << float( m_bgColour.Blue() ) / 255.0;
			p_stream->WriteString( wxT( "\tcolour\t\t\t" ) + l_res + wxT( "\n" ) );
		}
		else
		{
			p_stream->WriteString( wxT( "\timage\t\t\t" ) + m_bgString + wxT( "\n" ) );
		}

		if ( m_shadows )
		{
			p_stream->WriteString( wxT( "\tshadows\t\t\ttrue\n" ) );
		}

		if ( m_antiAliasing == aa2x )
		{
			p_stream->WriteString( wxT( "\tanti_aliasing\t2x\n" ) );
		}
		else if ( m_antiAliasing == aa4x )
		{
			p_stream->WriteString( wxT( "\tanti_aliasing\t4x\n" ) );
		}

		if ( m_showDebug )
		{
			p_stream->WriteString( wxT( "\tshow_debug\t\ttrue\n" ) );
		}

		if ( m_showFPS )
		{
			p_stream->WriteString( wxT( "\tshow_fps\t\ttrue\n" ) );
		}

		if ( ! m_startupScript.empty() )
		{
			p_stream->WriteString( wxT( "\tstartup_script\t" ) + m_startupScript + wxT( "\n" ) );
		}

		if ( m_mainScene )
		{
			m_mainScene->Save( p_stream );
			SceneMap::iterator l_it = m_scenes.begin();

			while ( l_it != m_scenes.end() )
			{
				l_it->second->Save( p_stream );
				++l_it;
			}
		}

		p_stream->WriteString( wxT( "}\n" ) );
	}

	void Project::Load( const wxString & p_path, GUI::FilesTree * p_tree )
	{
		ProjectFileParser l_parser( this, p_tree );
		LogDebug( wxT( "Project::Load - " ) + p_path );

		if ( p_tree )
		{
			p_tree->DeleteAllItems();
		}

		wxFileName l_fileName( p_path );
		m_projectPath = l_fileName.GetPathWithSep();

		if ( l_parser.ParseFile( p_path ) )
		{
			std::clog << make_string( _( "Parsed project file" ) ) << std::endl;

			if ( m_mainScene )
			{
				_fillSceneDependencies( m_mainScene );

				for ( SceneMap::iterator l_it = m_scenes.begin() ; l_it != m_scenes.end() ; l_it++ )
				{
					_fillSceneDependencies( l_it->second );
				}
			}

			m_projectFileName = p_path;
			m_modified = false;
			m_saved = true;
			p_tree->Refresh();
		}
	}

	void Project::Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, const wxString & p_path, GUI::FilesTree * p_tree )
	{
		if ( ! p_stream || ! p_input || p_input->Eof() )
		{
			return;
		}

		wxString l_line = p_stream->ReadLine();

		if ( l_line.empty() || p_input->Eof() )
		{
			return;
		}

		m_name = l_line;
		LogDebug( wxT( "Project::Load - " ) + m_name );

		if ( p_tree != NULL )
		{
			p_tree->DeleteAllItems();
			p_tree->InitProject( m_name );
		}

		m_projectPath = p_path;
		m_projectPath.Replace( wxT( "/" ), wxString() + wxFileName::GetPathSeparator() );
		m_projectPath.Replace( wxT( "\\" ), wxString() + wxFileName::GetPathSeparator() );
		l_line = p_stream->ReadLine();

		if ( l_line.empty() || p_input->Eof() )
		{
			return;
		}

		if ( l_line == wxT( "Resolution" ) )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}

			size_t l_index = l_line.find_first_of( wxT( " " ) );

			if ( l_index != wxNOT_FOUND )
			{
				wxString l_width = l_line.substr( 0, l_index );
				wxString l_height = l_line.substr( l_index + 1 );
				long l_lWidth, l_lHeight;
				l_width.ToLong( & l_lWidth );
				l_height.ToLong( & l_lHeight );
				m_resolution = wxSize( l_lWidth, l_lHeight );
				LogDebug( l_line );
			}
		}

		l_line = p_stream->ReadLine();

		if ( l_line == wxT( "Colour" ) )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}

			_buildColour( l_line );
			LogDebug( l_line + wxT( " - " ) + m_bgColour.GetAsString() );
		}
		else if ( l_line == wxT( "Image" ) )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}

			m_bgImage = wxImage( l_line );
		}
		else
		{
			return;
		}

		m_bgString = l_line;
		l_line = p_stream->ReadLine();

		if ( l_line.empty() || p_input->Eof() )
		{
			return;
		}

		if ( l_line == wxT( "Shadows" ) )
		{
			m_shadows = true;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}

		if ( l_line == wxT( "AntiAliasing2x" ) )
		{
			m_antiAliasing = aa2x;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}
		else if ( l_line == wxT( "AntiAliasing4x" ) )
		{
			m_antiAliasing = aa4x;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}

		if ( l_line == wxT( "ShowDebug" ) )
		{
			m_showDebug = true;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}

		if ( l_line == wxT( "ShowFPS" ) )
		{
			m_showFPS = true;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}

		if ( l_line == wxT( "StartupScript" ) )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}

			m_startupScript = l_line;
			l_line = p_stream->ReadLine();

			if ( l_line.empty() || p_input->Eof() )
			{
				return;
			}
		}

		if ( l_line == wxT( "Scene" ) )
		{
			l_line = p_stream->ReadLine();

			if ( l_line.empty() )
			{
				return;
			}

			m_mainScene = new Scene( shared_from_this(), l_line, true );
			l_line = m_mainScene->Load( p_input, p_stream, p_tree );
			LogDebug( wxT( "Project::Load - Mainscene read" ) );
			Scene * l_scene;

			while ( ! p_input->Eof() )
			{
				LogDebug( wxT( "Project::Load - " ) + l_line );

				if ( l_line == wxT( "Scene" ) )
				{
					LogDebug( wxT( "Project::Load - New scene" ) );
					l_line = p_stream->ReadLine();

					if ( l_line.empty() || p_input->Eof() )
					{
						return;
					}

					l_scene = new Scene( shared_from_this(), l_line );
					l_line = l_scene->Load( p_input, p_stream, p_tree );
					AddScene( l_scene );
					LogDebug( wxT( "Project::Load - New scene read" ) );
				}
			}

			_fillSceneDependencies( m_mainScene );

			for ( SceneMap::iterator l_it = m_scenes.begin() ; l_it != m_scenes.end() ; l_it++ )
			{
				_fillSceneDependencies( l_it->second );
			}
		}
	}

	void Project::Write()
	{
		if ( m_mainScene )
		{
			m_mainScene->Write( m_projectPath );

			for ( auto && l_it : m_scenes )
			{
				l_it.second->Write( m_projectPath );
			}
		}
	}

	void Project::SetBackgroundImage( const wxString & p_imgPath )
	{
		m_bgImage = wxImage( p_imgPath );
		m_bgString = p_imgPath;
		m_modified = true;
	}

	void Project::SetBackgroundColour( const wxString & p_colour )
	{
		m_bgColour.Set( p_colour );
		m_bgString = p_colour;
		m_modified = true;
	}

	bool Project::FindFileInScenes( const wxString & p_fileName, File *& p_file, Scene *& p_scene )
	{
		p_scene = NULL;
		p_file = m_mainScene->GetFileByName( p_fileName );

		if ( !p_file )
		{
			auto && l_it = m_scenes.begin();

			while ( l_it != m_scenes.end() && !p_file )
			{
				p_file = l_it->second->GetFileByName( p_fileName );

				if ( p_file )
				{
					p_scene = l_it->second;
				}

				++l_it;
			}
		}
		else
		{
			p_scene = m_mainScene;
		}

		return p_file != NULL;
	}

	void Project::_fillSceneDependencies( Scene * p_scene )
	{
		wxArrayString l_neededScenes = p_scene->GetNeededScenes();

		for ( auto && l_it : l_neededScenes )
		{
			auto && l_neededSceneIt = m_scenes.find( l_it );
			Scene * l_scene;

			if ( l_neededSceneIt != m_scenes.end() )
			{
				l_scene = l_neededSceneIt->second;
			}
			else if ( l_it == m_mainScene->GetName() )
			{
				l_scene = m_mainScene;
			}

			l_scene->SetNeeded( true );
			l_scene->_addSceneThatNeedMe( p_scene );
		}
	}

	void Project::_buildColour( const wxString & p_infos )
	{
		wxArrayString l_infos = GUI::Split( p_infos, wxT( " " ) );
		double l_red, l_green, l_blue;
		unsigned char l_cred, l_cgreen, l_cblue;
		l_infos[0].ToDouble( & l_red );
		l_infos[1].ToDouble( & l_green );
		l_infos[2].ToDouble( & l_blue );
		l_cred = uint8_t( int( l_red * 255.0 ) );
		l_cgreen = uint8_t( int( l_green * 255.0 ) );
		l_cblue = uint8_t( int( l_blue * 255.0 ) );
		m_bgColour.Set( l_cred, l_cgreen, l_cblue );
	}
}
END_TROLL_PROJECT_NAMESPACE
