/*
See LICENSE file in root folder
*/
#include "MuseFileWriter.h"

#include <iostream>
#include <cstdio>

#include <wx/zipstrm.h>

#include "Project/Project.h"
#include "Project/Scene.h"

namespace Troll
{
	using namespace ProjectComponents;

	namespace
	{
		class wxDirTraverserSimple
			: public wxDirTraverser
		{
		public:
			wxDirTraverserSimple( wxZipOutputStream & p_stream, wxString const & p_folderPath )
				: m_stream( p_stream )
				, m_folderPath( p_folderPath )
			{
			}

			virtual wxDirTraverseResult OnFile( wxString const & p_filename )
			{
				auto l_index = p_filename.find( m_folderPath );
				wxString l_fileName( p_filename );

				if ( l_index != wxString::npos )
				{
					l_fileName = l_fileName.substr( l_index + m_folderPath.size() + 1 );
				}

				m_stream.PutNextEntry( l_fileName );
				DoDumpFile( l_fileName );
				return wxDIR_CONTINUE;
			}

			virtual wxDirTraverseResult OnDir( wxString const & p_dirname )
			{
				m_stream.PutNextDirEntry( p_dirname );
				return wxDIR_CONTINUE;
			}

		private:
			void DoDumpFile( wxString const & p_fileName )
			{
				wxFile l_file{ p_fileName };
				std::array< uint8_t, 8192 > l_buffer;

				while ( !l_file.Eof() )
				{
					auto l_read = l_file.Read( l_buffer.data(), l_buffer.size() );
					m_stream.Write( l_buffer.data(), l_read );
				}
			}

		private:
			wxZipOutputStream & m_stream;
			wxString const & m_folderPath;
		};

		void WriteBackgroundInMain( Project const & p_project, wxTextOutputStream & p_stream )
		{
			if ( p_project.GetBackgroundType() == bgColour )
			{
				wxColour const & l_colour = p_project.GetBackgroundColour();
				wxString l_line;
				l_line.Printf( wxT( "background_colour %f %f %f %f\n\n" ),
							   int( l_colour.Red() ) / 255.0,
							   int( l_colour.Green() ) / 255.0,
							   int( l_colour.Blue() ) / 255.0,
							   int( l_colour.Alpha() ) / 255.0 );
				l_line.Replace( wxT( "," ), wxT( "." ), true );
				p_stream.WriteString( l_line );
			}
			else
			{
				p_stream.WriteString( wxT( "background_colour 0.0 0.0 0.0 0.5 \n\n" ) );
			}
		}

		void WriteFilesInMain( FileArray const & p_files, wxString const & p_name, wxString const & p_extension, wxTextOutputStream & p_stream )
		{
			for ( auto const & l_file : p_files )
			{
				size_t l_index = min( l_file.FileName.find_last_of( '\\' ), l_file.FileName.find_last_of( '/' ) );
				wxString l_fileName = l_file.FileName.substr( l_index + 1, l_file.FileName.size() );
				p_stream.WriteString( wxT( "\t" ) + p_name + wxT( " " ) + l_fileName + p_extension + wxT( "\n" ) );
			}
		}

		void WriteSceneInMain( wxString const & p_projectName, Scene const & p_scene, bool p_writeScripts, bool p_writeMuseFiles, wxTextOutputStream & p_stream )
		{
			wxString l_sceneName = p_scene.GetName();
			p_stream.WriteString( wxT( "scene " ) + l_sceneName + wxT( "\n{\n" ) );

			if ( p_writeMuseFiles )
			{
				p_stream.WriteString( wxT( "\tmuse_file " ) + p_scene.GetName() + wxT( ".muse\n" ) );
			}

			WriteFilesInMain( p_scene.GetDataFiles(), wxT( "data_file" ), wxT( ".zip" ), p_stream );
			WriteFilesInMain( p_scene.GetSceneFiles(), wxT( "scene_file" ), wxEmptyString, p_stream );

			if ( p_writeScripts )
			{
				WriteFilesInMain( p_scene.GetLoadScriptFiles(), wxT( "load_script_file" ), wxEmptyString, p_stream );
				WriteFilesInMain( p_scene.GetUnloadScriptFiles(), wxT( "unload_script_file" ), wxEmptyString, p_stream );
			}

			if ( p_writeMuseFiles )
			{
				WriteFilesInMain( p_scene.GetMuseFiles(), wxT( "muse_file" ), wxEmptyString, p_stream );
			}

			p_stream.WriteString( wxT( "}\n\n" ) );
		}

		ScenePtr GetStartScene( Project const & p_project )
		{
			auto l_startScene = p_project.GetSplashScene();

			if ( !l_startScene )
			{
				l_startScene = p_project.GetMainScene();
			}

			return l_startScene;
		}

		wxString DoCreateMain( ScenePtr p_scene
			, Project const & p_project
			, bool p_writeScripts )
		{
			auto l_startScene = GetStartScene( p_project );
			wxString l_mainFileName = p_project.GetPath() + p_scene->GetName() + wxT( "/main.emcfg" );

			if ( l_startScene == p_scene )
			{
				l_mainFileName = p_project.GetPath() + wxT( "main.emcfg" );
			}

			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			WriteBackgroundInMain( p_project, l_textStream );
			WriteSceneInMain( p_project.GetName(), *p_scene, p_writeScripts, true, l_textStream );

			if ( l_startScene == p_scene )
			{
				if ( !p_project.GetStartupScript().empty() )
				{
					l_textStream.WriteString( wxT( "<startup_script>\n" ) + p_project.GetStartupScript() + wxT( "\n</startup_script>\n\n" ) );
				}

				l_textStream.WriteString( wxT( "start_scene " ) + p_scene->GetName() + "\n" );
			}

			l_fileOutput.Close();
			return l_mainFileName;
		}

		wxString DoCreateMain( Project const & p_project, bool p_writeScripts )
		{
			wxString l_mainFileName;
			l_mainFileName = p_project.GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			WriteBackgroundInMain( p_project, l_textStream );
			auto l_startScene = GetStartScene( p_project );

			for ( auto const & l_scene : p_project.GetScenes() )
			{
				if ( l_startScene != l_scene )
				{
					WriteSceneInMain( p_project.GetName(), *l_scene, p_writeScripts, true, l_textStream );
				}
			}

			if ( l_startScene )
			{
				WriteSceneInMain( p_project.GetName(), *l_startScene, p_writeScripts, true, l_textStream );
			}

			if ( !p_project.GetStartupScript().empty() )
			{
				l_textStream.WriteString( wxT( "<startup_script>\n" ) + p_project.GetStartupScript() + wxT( "\n</startup_script>\n\n" ) );
			}

			if ( l_startScene )
			{
				l_textStream.WriteString( wxT( "start_scene " ) + l_startScene->GetName() + "\n" );
			}

			l_fileOutput.Close();
			return l_mainFileName;
		}

		wxString DoCreateSceneEditorMain( Project const & p_current, ProjectComponents::Project const & p_editor )
		{
			wxString l_mainFileName;
			l_mainFileName = p_editor.GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			WriteBackgroundInMain( p_editor, l_textStream );

			if ( p_editor.HasMainScene() )
			{
				Scene const & l_scene = *p_editor.GetMainScene();
				WriteSceneInMain( p_editor.GetName(), l_scene, true, false, l_textStream );

				if ( p_current.HasMainScene() )
				{
					Scene const & l_mainScene = *p_current.GetMainScene();
					WriteSceneInMain( p_editor.GetName(), l_mainScene, false, true, l_textStream );
					l_textStream.WriteString( wxT( "start_scene " ) + l_mainScene.GetName() + wxT( "\n" ) );
				}
				else
				{
					LogDebug( _( "MainFrame::DoCreateSceneEditorMain - Current Project has no main scene" ) );
				}

				l_textStream.WriteString( wxT( "start_scene " ) + p_editor.GetMainScene()->GetName() );
			}

			l_fileOutput.Close();
			return l_mainFileName;
		}

		wxString DoCreateOverlayEditorMain( Project const & p_current, ProjectComponents::Project const & p_editor )
		{
			wxString l_mainFileName;
			l_mainFileName = p_editor.GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			WriteBackgroundInMain( p_editor, l_textStream );

			if ( p_editor.HasMainScene() )
			{
				Scene const & l_scene = *p_editor.GetMainScene();
				WriteSceneInMain( p_editor.GetName(), l_scene, true, false, l_textStream );

				if ( p_current.HasMainScene() )
				{
					Scene const & l_mainScene = *p_current.GetMainScene();
					WriteSceneInMain( p_editor.GetName(), l_mainScene, false, true, l_textStream );
					l_textStream.WriteString( wxT( "start_scene " ) + l_mainScene.GetName() + wxT( "\n" ) );
				}
				else
				{
					LogDebug( _( "MainFrame::CreateOverlayEditorMain - Current Project has no main scene" ) );
				}

				l_textStream.WriteString( wxT( "start_scene " ) + p_editor.GetMainScene()->GetName() );
			}
			else
			{
				LogDebug( _( "MainFrame::CreateOverlayEditorMain - Given Project has no main scene" ) );
			}

			l_fileOutput.Close();
			return l_mainFileName;
		}

		void DoZipFolder( wxString const & p_folderPath, wxString const & p_zipFileName )
		{
			auto l_path = wxGetCwd();
			{
				wxSetWorkingDirectory( p_folderPath );
				wxFFileOutputStream l_out( p_zipFileName );
				wxZipOutputStream l_zip( l_out );
				wxDirTraverserSimple traverser( l_zip, p_folderPath );
				wxDir dir( p_folderPath );
				dir.Traverse( traverser );
			}
			wxSetWorkingDirectory( l_path );
		}

		void DoAddSceneToMuse( Project const & p_project
			, bool p_createMain
			, bool p_withScripts
			, Scene const & p_scene
			, Scene const & p_startScene
			, wxString const & p_mainFile )
		{
			wxString l_projectPath = p_project.GetPath();
			wxString l_scenePath = l_projectPath + p_scene.GetName() + wxFileName::GetPathSeparator();
			DataWriter l_dataWriter;

			for ( auto const & l_folder : p_scene.GetDataFolders() )
			{
				LogDebug( wxT( "MuseFileWriter::Musiner - Data Folder - " ) + l_scenePath + l_folder.FileName );
				l_dataWriter.AddFolder( make_string( l_scenePath + l_folder.FileName ), EM_BLOCK_ZIPSNDDATA );
			}

			for ( auto const & l_file : p_scene.GetSceneFiles() )
			{
				LogDebug( wxT( "MuseFileWriter::Musiner - Scene File - " ) + l_scenePath + l_file.FileName );
				l_dataWriter.AddFile( make_string( l_scenePath + l_file.FileName ) );
			}

			if ( p_withScripts )
			{
				for ( auto const & l_file : p_scene.GetLoadScriptFiles() )
				{
					LogDebug( wxT( "MuseFileWriter::Musiner - Load Script File - " ) + l_scenePath + l_file.FileName );
					l_dataWriter.AddFile( make_string( l_scenePath + l_file.FileName ) );
				}


				for ( auto const & l_file : p_scene.GetUnloadScriptFiles() )
				{
					LogDebug( wxT( "MuseFileWriter::Musiner - Unload Script File - " ) + l_scenePath + l_file.FileName );
					l_dataWriter.AddFile( make_string( l_scenePath + l_file.FileName ) );
				}
			}

			for ( auto const & l_folder : p_scene.GetDataFiles() )
			{
				LogDebug( wxT( "MuseFileWriter::Musiner - Data file - " ) + l_scenePath + l_folder.FileName );
				DoZipFolder( l_scenePath + l_folder.FileName, l_scenePath + l_folder.FileName + wxT( ".zip" ) );
				l_dataWriter.AddFile( make_string( l_scenePath + l_folder.FileName + wxT( ".zip" ) ) );
			}

			if ( p_createMain
				&& p_scene.GetName() == p_startScene.GetName() )
			{
				l_dataWriter.AddFile( make_string( p_mainFile ) );
			}

			l_dataWriter.Write( make_string( l_projectPath + p_scene.GetName() + wxT( ".muse" ) ) );
		}
	}

	void MuseFileWriter::Musinate( Project const & p_project, bool p_createMain, bool p_withScripts )
	{
		auto l_startScene = p_project.GetSplashScene();

		if ( !l_startScene )
		{
			l_startScene = p_project.GetMainScene();
		}

		if ( l_startScene )
		{
			if ( p_project.GetSeparable() )
			{
				for ( auto l_scene : p_project.GetScenes() )
				{
					auto l_mainFile = DoCreateMain( l_scene, p_project, p_withScripts );
					DoAddSceneToMuse( p_project, p_createMain, p_withScripts, *l_scene, *l_scene, l_mainFile );
				}

				return;
			}

			wxString l_mainFile;

			if ( p_createMain )
			{
				l_mainFile = DoCreateMain( p_project, p_withScripts );
			}

			DoAddSceneToMuse( p_project, p_createMain, p_withScripts, *l_startScene, *p_project.GetMainScene(), l_mainFile );

			for ( auto const & l_scene : p_project.GetScenes() )
			{
				if ( l_startScene != l_scene )
				{
					DoAddSceneToMuse( p_project, p_createMain, p_withScripts, *l_scene, *p_project.GetMainScene(), l_mainFile );
				}
			}
		}
	}

	void MuseFileWriter::MusinateSceneEditor( Project const & p_current
		, Project const & p_editor )
	{
		if ( p_editor.HasMainScene() )
		{
			wxString l_mainFile = DoCreateSceneEditorMain( p_current, p_editor );
			DoAddSceneToMuse( p_editor, true, true, *p_editor.GetMainScene(), *p_editor.GetMainScene(), l_mainFile );
		}
	}

	void MuseFileWriter::MusinateOverlayEditor( Project const & p_current
		, Project const & p_editor )
	{
		if ( p_editor.HasMainScene() )
		{
			Scene const & l_scene = *p_editor.GetMainScene();
			wxString l_mainFile = DoCreateOverlayEditorMain( p_current, p_editor );
			wxString l_path = p_editor.GetPath();
			DataWriter l_dataWriter;

			for ( auto const & l_file : l_scene.GetSceneFiles() )
			{
				LogDebug( wxT( "MuseFileWriter::Musiner - Scene File - " ) + l_path + l_file.FileName );
				l_dataWriter.AddFile( make_string( l_path + l_file.FileName ) );
			}

			for ( auto const & l_file : l_scene.GetLoadScriptFiles() )
			{
				LogDebug( wxT( "MuseFileWriter::Musiner - Load Script File - " ) + l_path + l_file.FileName );
				l_dataWriter.AddFile( make_string( l_path + l_file.FileName ) );
			}

			l_dataWriter.AddFile( make_string( l_mainFile ) );
			l_dataWriter.Write( make_string( l_path + p_editor.GetName() + wxT( ".muse" ) ) );
		}
	}
}
