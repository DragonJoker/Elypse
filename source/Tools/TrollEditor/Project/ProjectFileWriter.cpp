#include "ProjectFileWriter.h"

#include "Project.h"
#include "Scene.h"

using namespace Troll::GUI;

namespace Troll
{
	namespace ProjectComponents
	{
		namespace ProjectFileWriter
		{
			namespace
			{
				template< typename ArrayType, typename Function >
				void WriteFiles( ArrayType const & p_array, wxString const & p_type, wxTextOutputStream & p_stream, Function p_function )
				{
					if ( p_array.size() > 0 )
					{
						p_stream.WriteString( wxT( "\t\t" ) + p_type + wxT( "\n\t\t{\n" ) );

						for ( auto const & l_it : p_array )
						{
							LogDebug( wxT( "Scene::Save - " ) + p_function( l_it ) );
							p_stream.WriteString( wxT( "\t\t\t" ) + p_function( l_it ) + wxT( "\n" ) );
						}

						p_stream.WriteString( wxT( "\t\t}\n" ) );
					}
				}
			}

			bool WriteProject( Project const & p_project, wxTextOutputStream & p_stream )
			{
				p_stream.WriteString( wxT( "project " ) + p_project.GetName() + wxT( "\n{\n" ) );
				wxString l_res;
				l_res << p_project.GetResolution().x << wxT( " " ) << p_project.GetResolution().y;
				p_stream.WriteString( wxT( "\tresolution " ) + l_res + wxT( "\n" ) );

				if ( p_project.GetBackgroundType() == bgColour )
				{
					l_res.clear();
					l_res << float( p_project.GetBackgroundColour().Red() ) / 255.0 << wxT( " " ) << float( p_project.GetBackgroundColour().Green() ) / 255.0 << wxT( " " ) << float( p_project.GetBackgroundColour().Blue() ) / 255.0;
					p_stream.WriteString( wxT( "\tcolour " ) + l_res + wxT( "\n" ) );
				}
				else
				{
					p_stream.WriteString( wxT( "\timage " ) + p_project.GetBackground() + wxT( "\n" ) );
				}

				if ( p_project.GetShadows() )
				{
					p_stream.WriteString( wxT( "\tshadows true\n" ) );
				}

				if ( p_project.GetFSAA() == aa2x )
				{
					p_stream.WriteString( wxT( "\tanti_aliasing 2x\n" ) );
				}
				else if ( p_project.GetFSAA() == aa4x )
				{
					p_stream.WriteString( wxT( "\tanti_aliasing 4x\n" ) );
				}

				if ( p_project.GetShowDebug() )
				{
					p_stream.WriteString( wxT( "\tshow_debug true\n" ) );
				}

				if ( p_project.GetShowFPS() )
				{
					p_stream.WriteString( wxT( "\tshow_fps true\n" ) );
				}

				if ( p_project.GetSeparable() )
				{
					p_stream.WriteString( wxT( "\tseparable true\n" ) );
				}

				if ( !p_project.GetStartupScript().empty() )
				{
					p_stream.WriteString( wxT( "\tstartup_script " ) + p_project.GetStartupScript() + wxT( "\n" ) );
				}

				if ( p_project.HasMainScene() )
				{
					auto l_mainScene = p_project.GetMainScene();
					WriteScene( *l_mainScene, p_stream );

					for ( auto const & l_scene : p_project )
					{
						if ( l_mainScene != l_scene )
						{
							WriteScene( *l_scene, p_stream );
						}
					}
				}

				p_stream.WriteString( wxT( "}\n" ) );
				return true;
			}

			bool WriteScene( Scene const & p_scene, wxTextOutputStream & p_stream )
			{
				auto && l_fileNamer = []( File const & p_file )
				{
					return p_file.FileName;
				};

				auto && l_depNamer = []( wxString const & p_name )
				{
					return p_name;
				};

				p_stream.WriteString( wxT( "\tscene " ) + p_scene.GetName() + wxT( "\n\t{\n" ) );
				WriteFiles( p_scene.GetSceneFiles(), wxT( "scene_files" ), p_stream, l_fileNamer );
				WriteFiles( p_scene.GetLoadScriptFiles(), wxT( "load_script_files" ), p_stream, l_fileNamer );
				WriteFiles( p_scene.GetUnloadScriptFiles(), wxT( "unload_script_files" ), p_stream, l_fileNamer );
				WriteFiles( p_scene.GetDataFiles(), wxT( "data_files_3d" ), p_stream, l_fileNamer );
				WriteFiles( p_scene.GetDataFolders(), wxT( "data_files_other" ), p_stream, l_fileNamer );
				WriteFiles( p_scene.GetNeededScenes(), wxT( "dependencies" ), p_stream, l_depNamer );
				p_stream.WriteString( wxT( "\t}\n" ) );
				return true;
			}
		}
	}
}
