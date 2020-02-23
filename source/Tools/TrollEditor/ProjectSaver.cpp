/*
See LICENSE file in root folder
*/
#include "ProjectSaver.h"

#include <iostream>
#include <cstdio>

#include <wx/zipstrm.h>

#include "Project/Project.h"
#include "Project/Scene.h"
#include "Project/File.h"

#include "Project/ProjectFileWriter.h"

#include "GUI/StcTextEditor.h"

namespace Troll
{
	using namespace ProjectComponents;

	namespace
	{
		wxString const TE_SAVE_AS = _( "Save as ..." );
	}

	void ProjectSaver::SaveProject( Project & p_project )
	{
		wxString l_fileName = p_project.GetProjectFileName();

		if ( l_fileName.empty() )
		{
			l_fileName = wxFileSelector( TE_SAVE_AS
				, p_project.GetPath()
				, p_project.GetName() + wxT( ".teproj" )
				, wxEmptyString
				, _( "TrollEditor Project file" ) + wxString( wxT( " (*.teproj)|*.teproj" ) )
				, wxFD_SAVE );
			LogDebug( l_fileName );
			p_project.SetProjectFileName( l_fileName );
		}

		LogDebug( wxT( "ProjectSaver::SaveProject - " ) + l_fileName );

		if ( !l_fileName.empty() )
		{
			LogDebug( wxT( "ProjectSaver::OnSaveProject - " ) + l_fileName );
			wxFileOutputStream fileOutput( l_fileName );
			wxTextOutputStream l_textStream( fileOutput );
			auto l_mainScene = p_project.GetMainScene();
			SaveScene( p_project, *l_mainScene );

			for ( auto const & l_scene : p_project.GetScenes() )
			{
				if ( l_scene != l_mainScene )
				{
					SaveScene( p_project, *l_scene );
				}
			}

			ProjectFileWriter::WriteProject( p_project, l_textStream );
			p_project.SetSaved( true );
			p_project.SetModified( false );
			l_textStream.Flush();
			fileOutput.Close();
		}
	}

	void ProjectSaver::SaveScene( Project & p_project, Scene const & p_scene )
	{
		auto l_func = [&p_project]( File const & p_file )
		{
			if ( !p_file.Saved )
			{
				SaveFileAs( p_project, p_file );
			}
			else if ( p_file.EditPanel && p_file.EditPanel->IsModified() )
			{
				SaveFile( p_project, p_file, p_file.FileName );
			}
		};

		for ( auto const & l_file : p_scene.GetSceneFiles() )
		{
			l_func( l_file );
		}

		for ( auto const & l_file : p_scene.GetLoadScriptFiles() )
		{
			l_func( l_file );
		}

		for ( auto const & l_file : p_scene.GetUnloadScriptFiles() )
		{
			l_func( l_file );
		}
	}

	wxString ProjectSaver::SaveFileAs( Project & p_project, File const & p_file )
	{
		wxString l_fileName;

		switch ( p_file.Filetype )
		{
		case sceneFile:
			l_fileName = wxFileSelector( TE_SAVE_AS
				, p_project.GetPath()
				, wxEmptyString
				, wxEmptyString
				, _( "Elypse Scene file" ) + wxString( wxT( " (*.emscene)|*.emscene" ) )
				, wxFD_SAVE );
			break;

		case loadScriptFile:
		case unloadScriptFile:
			l_fileName = wxFileSelector( TE_SAVE_AS
				, p_project.GetPath()
				, wxEmptyString
				, wxEmptyString
				, _( "Elypse Script file" ) + wxString( wxT( " (*.emscript)|*.emscript" ) )
				, wxFD_SAVE );
			break;

		case dataFile:
			l_fileName = wxFileSelector( TE_SAVE_AS
				, p_project.GetPath()
				, wxEmptyString
				, wxEmptyString
				, _( "Zip file" ) + wxString( wxT( " (*.zip)|*.zip" ) )
				, wxFD_SAVE );
			break;

		default:
			break;
		}

		if ( !l_fileName.empty() )
		{
			size_t l_index = min( l_fileName.find_last_of( '\\' ), l_fileName.find_last_of( '/' ) );
			l_fileName = l_fileName.substr( l_index + 1, l_fileName.size() );

			if ( p_file.EditPanel )
			{
				p_file.EditPanel->SaveFile( l_fileName );
			}

			const_cast< File & >( p_file ).FileName = l_fileName;
		}

		return l_fileName;
	}

	void ProjectSaver::SaveFile( Project & p_project, File const & p_file, wxString const & p_fileName )
	{
		if ( p_file.EditPanel )
		{
			wxString l_filePath = p_project.GetPath();

			if ( p_file.m_scene->GetName() != p_project.GetMainScene()->GetName() )
			{
				l_filePath += p_file.m_scene->GetName() + wxT( "/" );
			}

			l_filePath += p_fileName;
			p_file.EditPanel->SaveFile( l_filePath );
		}
	}
}
