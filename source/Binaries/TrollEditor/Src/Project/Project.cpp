
#include "PrecompiledHeader.h"

#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/ProjectFileParser.h"
#include "GUI/MainFrame.h"
#include "GUI/FilesTree.h"
/*
#include <wx/colour.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
*/

using namespace Troll;
using namespace Troll::GUI;


Project::Project()
	:	m_shadows( false ),
		m_antiAliasing( aa0 ),
		m_bgImage( NULL ),
		m_mainScene( NULL ),
		m_saved( false ),
		m_modified( false ),
		m_bgType( bgColour ),
		m_bgColour( new wxColour( ( unsigned long )0 ) ),
		m_showDebug( false ),
		m_showFPS( false )
{
	m_bgString = m_bgColour->GetAsString();
}



Project::Project( const wxString & p_projectName, const wxString & p_mainSceneName,
				  const wxString & p_path, BackgroundType p_backgroundType,
				  const wxString & p_background, bool p_shadows, AntiAliasing p_aa,
				  const wxSize & p_resolution )
	:	m_shadows( p_shadows ),
		m_antiAliasing( p_aa ),
		m_projectPath( p_path ),
		m_name( p_projectName ),
		m_saved( false ),
		m_bgImage( NULL ),
		m_bgType( p_backgroundType ),
		m_bgString( p_background ),
		m_bgColour( new wxColour() ),
		m_resolution( p_resolution )
{
	m_mainScene = new TrollScene( this, p_mainSceneName, true );

	if ( p_backgroundType == bgColour )
	{
		m_bgColour->Set( p_background );
	}
	else
	{
		m_bgImage = new wxImage( p_background );
	}
}



Project::~Project()
{
	if ( m_mainScene != NULL )
	{
		delete m_mainScene;
	}

	General::Utils::map::deleteAll( m_scenes );

	if ( m_bgImage != NULL )
	{
		delete m_bgImage;
	}

	delete m_bgColour;
}



void Project::FlushObjects()
{
	if ( m_mainScene != NULL )
	{
		m_mainScene->FlushObjects();
	}

	General::Utils::map::cycle( m_scenes, & TrollScene::FlushObjects );
}



TrollScene * Project::GetScene( const wxString & p_sceneName )
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



TrollScene * Project::GetScene( const wxTreeItemId & p_item )
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
			g_mainFrame->LogDebugMessage( wxT( "Project::GetScene - TrollScene : " ) + l_it->second->GetName() );
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
			wxMessageBox( wxT( "Impossible de supprimer la scène principale" ),
						  wxT( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
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



void Project::RemoveScene( TrollScene * p_scene )
{
	if ( m_mainScene )
	{
		if ( m_mainScene == p_scene )
		{
			wxMessageBox( wxT( "Impossible de supprimer la scène principale" ),
						  wxT( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
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



TrollScene * Project::CreateScene( wxString const & p_strName )
{
	TrollScene * l_pReturn = new TrollScene( this, p_strName, m_mainScene == NULL );

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

void Project::AddScene( TrollScene * p_scene )
{
	SceneMap::iterator l_it = m_scenes.find( p_scene->GetName() );

	if ( l_it != m_scenes.end() )
	{
		wxMessageBox( wxT( "Impossible d'ajouter cette scène, une scène avec le même nom existe déjà" ),
					  wxT( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
		return;
	}

	m_scenes.insert( SceneMap::value_type( p_scene->GetName(), p_scene ) );
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

	if ( m_bgColour )
	{
		l_res.clear();
		l_res << float( m_bgColour->Red() ) / 255.0 << wxT( " " ) << float( m_bgColour->Green() ) / 255.0 << wxT( " " ) << float( m_bgColour->Blue() ) / 255.0;
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



void Project::Load( const wxString & p_path, FilesTree * p_tree )
{
	ProjectFileParser l_parser( this, p_tree );
	g_mainFrame->LogDebugMessage( wxT( "Project::Load - " ) + m_name );

	if ( p_tree )
	{
		p_tree->DeleteAllItems();
	}

	wxFileName l_fileName( p_path );
	m_projectPath = l_fileName.GetPathWithSep();

	if ( l_parser.ParseFile( p_path ) )
	{
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
	}
}

void Project::Load( wxFileInputStream * p_input, wxTextInputStream * p_stream, const wxString & p_path,
					FilesTree * p_tree )
{
//	std::cout << "Project::Load - Path : " << p_path << "\n";
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
	g_mainFrame->LogDebugMessage( wxT( "Project::Load - " ) + m_name );

	if ( p_tree != NULL )
	{
		p_tree->DeleteAllItems();
		p_tree->InitProjet( m_name );
	}

	m_projectPath = p_path;
	m_projectPath.Replace( wxT( '/' ), wxFileName::GetPathSeparator() );
	m_projectPath.Replace( wxT( '\\' ), wxFileName::GetPathSeparator() );
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
			g_mainFrame->LogOutMessage( l_line );
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
		g_mainFrame->LogOutMessage( l_line + wxT( " - " ) + m_bgColour->GetAsString() );
	}
	else if ( l_line == wxT( "Image" ) )
	{
		l_line = p_stream->ReadLine();

		if ( l_line.empty() || p_input->Eof() )
		{
			return;
		}

		m_bgImage = new wxImage( l_line );
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

		m_mainScene = new TrollScene( this, l_line, true );
		l_line = m_mainScene->Load( p_input, p_stream, p_tree );
		/*
				std::cout << m_mainScene->GetSceneFilesNumber() << "\n";
				std::cout << m_mainScene->GetLoadScriptFilesNumber() << "\n";
				std::cout << m_mainScene->GetUnloadScriptFilesNumber() << "\n";
				std::cout << m_mainScene->GetDataFilesNumber() << "\n";
				std::cout << m_mainScene->GetDataFoldersNumber() << "\n";
		*/
		g_mainFrame->LogDebugMessage( wxT( "Project::Load - Mainscene read" ) );
		TrollScene * l_scene;

		while ( ! p_input->Eof() )
		{
			g_mainFrame->LogDebugMessage( wxT( "Project::Load - " ) + l_line );

			if ( l_line == wxT( "Scene" ) )
			{
				g_mainFrame->LogDebugMessage( wxT( "Project::Load - New scene" ) );
				l_line = p_stream->ReadLine();

				if ( l_line.empty() || p_input->Eof() )
				{
					return;
				}

				l_scene = new TrollScene( this, l_line );
				l_line = l_scene->Load( p_input, p_stream, p_tree );
				AddScene( l_scene );
				g_mainFrame->LogDebugMessage( wxT( "Project::Load - New scene read" ) );
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
	if ( m_mainScene != NULL )
	{
		m_mainScene->Write( m_projectPath );
		SceneMap::iterator l_it = m_scenes.begin();

		while ( l_it != m_scenes.end() )
		{
			l_it->second->Write( m_projectPath );
			++l_it;
		}
	}
}



void Project::SetBackgroundImage( const wxString & p_imgPath )
{
	m_bgImage = new wxImage( p_imgPath );
	m_bgString = p_imgPath;
	m_modified = true;
}



void Project::SetBackgroundColour( const wxString & p_colour )
{
	m_bgColour->Set( p_colour );
	m_bgString = p_colour;
	m_modified = true;
}



bool Project::FindFileInScenes( const wxString & p_fileName, TrollFile *& p_file, TrollScene *& p_scene )
{
	p_scene = NULL;
	p_file = m_mainScene->GetFileByName( p_fileName );

	if ( p_file == NULL )
	{
		SceneMap::iterator l_it = m_scenes.begin();

		while ( l_it != m_scenes.end() && p_file == NULL )
		{
			p_file = l_it->second->GetFileByName( p_fileName );

			if ( p_file != NULL )
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



void Project::_fillSceneDependencies( TrollScene * p_scene )
{
	wxArrayString l_neededScenes = p_scene->GetNeededScenes();
	TrollScene * l_neededScene;
	SceneMap::iterator l_neededSceneIt;

	for ( size_t i = 0 ; i < l_neededScenes.size() ; i++ )
	{
		l_neededSceneIt = m_scenes.find( l_neededScenes[i] );

		if ( l_neededSceneIt != m_scenes.end() )
		{
			l_neededScene = l_neededSceneIt->second;
		}
		else if ( l_neededScenes[i] == m_mainScene->GetName() )
		{
			l_neededScene = m_mainScene;
		}

		l_neededScene->SetNeeded( true );
		l_neededScene->_addSceneThatNeedMe( p_scene );
	}
}



void Project::_buildColour( const wxString & p_infos )
{
	wxArrayString l_infos = Split( p_infos,  wxT( " " ) );
	double l_red, l_green, l_blue;
	unsigned char l_cred, l_cgreen, l_cblue;
	l_infos[0].ToDouble( & l_red );
	l_infos[1].ToDouble( & l_green );
	l_infos[2].ToDouble( & l_blue );
	l_cred = static_cast <unsigned char>( static_cast <int>( l_red * 255.0 ) );
	l_cgreen = static_cast <unsigned char>( static_cast <int>( l_green * 255.0 ) );
	l_cblue = static_cast <unsigned char>( static_cast <int>( l_blue * 255.0 ) );
	m_bgColour->Set( l_cred, l_cgreen, l_cblue );
}


