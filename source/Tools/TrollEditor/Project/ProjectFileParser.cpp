/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "ProjectFileParser.h"

#include "Project.h"
#include "Scene.h"
#include "GUI/FilesTree.h"
#include "GUI/ProjectFrame.h"

using namespace Troll::GUI;

#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4100 )
#endif

namespace Troll
{
	namespace ProjectComponents
	{
		ProjectFileContext::ProjectFileContext( FILE * p_pFile )
			: FileParserContext( p_pFile )
			, pProject( nullptr )
		{
		}

		//*************************************************************************************************

		ProjectFileParser::ProjectFileParser( Project * p_project )
			: FileParser{ ePROJECT_SECTION_ROOT, ePROJECT_SECTION_COUNT }
			, m_pProject{ p_project }
		{
		}

		ProjectFileParser::~ProjectFileParser()
		{
		}

		void ProjectFileParser::DoInitialiseParser( FILE * p_file )
		{
			AddParser( ePROJECT_SECTION_ROOT, wxT( "project" ), Root_Project );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "splash_scene" ), Project_SplashScene );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "main_scene" ), Project_MainScene );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "resolution" ), Project_Resolution );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "colour" ), Project_BgColour );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "image" ), Project_BgImage );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "shadows" ), Project_Shadows );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "antialiasing" ), Project_AntiAliasing );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "startup_script" ), Project_StartupScript );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "show_debug" ), Project_ShowDebug );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "show_fps" ), Project_ShowFPS );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "separable" ), Project_Separable );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "scene" ), Project_Scene );
			AddParser( ePROJECT_SECTION_PROJECT, wxT( "}" ), Project_End );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "scene_files" ), Scene_SceneFiles );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "load_script_files" ), Scene_LoadScriptFiles );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "unload_script_files" ), Scene_UnloadScriptFiles );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "data_files_3d" ), Scene_3DDataFiles );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "data_files_other" ), Scene_OtherDataFiles );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "dependencies" ), Scene_Dependencies );
			AddParser( ePROJECT_SECTION_SCENE, wxT( "}" ), Scene_End );
			AddParser( ePROJECT_SECTION_SCENEFILES, wxT( "}" ), SceneFiles_End );
			AddParser( ePROJECT_SECTION_LOADSCRIPTFILES, wxT( "}" ), LoadScriptFiles_End );
			AddParser( ePROJECT_SECTION_UNLOADSCRIPTFILES, wxT( "}" ), UnloadScriptFiles_End );
			AddParser( ePROJECT_SECTION_3DDATAFILES, wxT( "}" ), DataFiles3D_End );
			AddParser( ePROJECT_SECTION_OTHERDATAFILES, wxT( "}" ), OtherDataFiles_End );
			AddParser( ePROJECT_SECTION_DEPENDENCIES, wxT( "}" ), Dependecies_End );
			ProjectFileContextPtr l_pContext( new ProjectFileContext( p_file ) );
			m_pParsingContext = l_pContext;
			l_pContext->pProject = m_pProject;
		}

		void ProjectFileParser::DoCleanupParser()
		{
			std::static_pointer_cast< ProjectFileContext >( m_pParsingContext )->pProject = nullptr;
		}

		void ProjectFileParser::DoDiscardParser( wxString const & p_strLine )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( m_pParsingContext );
			FileType l_type = FileTypeCount;

			switch ( m_pParsingContext->stackSections.top() )
			{
			case ePROJECT_SECTION_SCENEFILES:
				l_type = sceneFile;
				break;

			case ePROJECT_SECTION_LOADSCRIPTFILES:
				l_type = loadScriptFile;
				break;

			case ePROJECT_SECTION_UNLOADSCRIPTFILES:
				l_type = unloadScriptFile;
				break;

			case ePROJECT_SECTION_3DDATAFILES:
				l_type = dataFile;
				break;

			case ePROJECT_SECTION_OTHERDATAFILES:
				l_type = dataFolder;
				break;

			case ePROJECT_SECTION_DEPENDENCIES:
				l_pContext->pScene->AddDependency( p_strLine );
				break;

			default:
				break;
			}

			if ( l_type != FileTypeCount )
			{
				l_pContext->pScene->AddFile( l_type, p_strLine, true, false );
			}
		}

		void ProjectFileParser::DoValidate()
		{
		}

		//*************************************************************************************************

		IMPLEMENT_ATTRIBUTE_PARSER( Root_Project )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pProject->SetName( p_strParams );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_PROJECT );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_SplashScene )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pProject->SetSplashScene( p_strParams );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_PROJECT );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_MainScene )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pProject->SetMainScene( p_strParams );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_PROJECT );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_Resolution )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			std::size_t l_index = p_strParams.find_first_of( wxT( " " ) );

			if ( l_index != wxNOT_FOUND )
			{
				wxString l_width = p_strParams.substr( 0, l_index );
				wxString l_height = p_strParams.substr( l_index + 1 );
				long l_lWidth, l_lHeight;
				l_width.ToLong( &l_lWidth );
				l_height.ToLong( &l_lHeight );
				l_pContext->pProject->SetResolution( wxSize( l_lWidth, l_lHeight ) );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_BgColour )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			wxArrayString l_infos = GUI::Split( p_strParams, wxT( " " ) );
			double l_red, l_green, l_blue;
			unsigned char l_cred, l_cgreen, l_cblue;
			l_infos[0].ToDouble( &l_red );
			l_infos[1].ToDouble( &l_green );
			l_infos[2].ToDouble( &l_blue );
			l_cred = static_cast< unsigned char >( static_cast< int >( l_red * 255.0 ) );
			l_cgreen = static_cast< unsigned char >( static_cast< int >( l_green * 255.0 ) );
			l_cblue = static_cast< unsigned char >( static_cast< int >( l_blue * 255.0 ) );
			l_pContext->pProject->SetBackgroundColour( wxColour( l_cred, l_cgreen, l_cblue ) );
			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_BgImage )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pProject->SetBackgroundImage( p_strParams );
			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_Shadows )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );

			if ( p_strParams.Lower() == wxT( "true" ) )
			{
				l_pContext->pProject->SetShadows( true );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_AntiAliasing )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );

			if ( p_strParams.Lower() == wxT( "2x" ) )
			{
				l_pContext->pProject->SetFSAA( aa2x );
			}
			else if ( p_strParams.Lower() == wxT( "4x" ) )
			{
				l_pContext->pProject->SetFSAA( aa4x );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_StartupScript )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pProject->SetStartupScript( p_strParams );
			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_ShowDebug )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );

			if ( p_strParams.Lower() == wxT( "true" ) )
			{
				l_pContext->pProject->SetShowDebug( true );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_ShowFPS )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );

			if ( p_strParams.Lower() == wxT( "true" ) )
			{
				l_pContext->pProject->SetShowFPS( true );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_Separable )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );

			if ( p_strParams.Lower() == wxT( "true" ) )
			{
				l_pContext->pProject->SetSeparable( true );
			}

			ATTRIBUTE_END();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_Scene )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pScene = l_pContext->pProject->CreateScene( p_strParams );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_SCENE );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Project_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_SceneFiles )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_SCENEFILES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_LoadScriptFiles )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_LOADSCRIPTFILES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_UnloadScriptFiles )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_UNLOADSCRIPTFILES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_3DDataFiles )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_3DDATAFILES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_OtherDataFiles )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_OTHERDATAFILES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_Dependencies )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			ATTRIBUTE_END_PUSH( ePROJECT_SECTION_DEPENDENCIES );
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Scene_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->pScene = nullptr;
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( SceneFiles_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( LoadScriptFiles_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( UnloadScriptFiles_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( DataFiles3D_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( OtherDataFiles_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}

		IMPLEMENT_ATTRIBUTE_PARSER( Dependecies_End )
		{
			ProjectFileContextPtr l_pContext = std::static_pointer_cast< ProjectFileContext >( p_pContext );
			l_pContext->arrayStrings.clear();
			ATTRIBUTE_END_POP();
		}
	}
}
