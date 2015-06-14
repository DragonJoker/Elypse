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

#include <wx/aui/auibook.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/filepicker.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/menuitem.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textdlg.h>
#include <wx/toolbar.h>

#include "MainFrame.h"

#include "GUI/StcTextEditor.hpp"
#include "GUI/ElypseCtrl.h"
#include "GUI/ElypseFrame.h"
#include "GUI/ElypsePanel.h"
#include "GUI/NewProjectFrame.h"
#include "GUI/NewSceneFrame.h"
#include "GUI/SceneDependenciesFrame.h"
#include "GUI/LogListBox.h"
#include "GUI/FunctionsTree.h"
#include "GUI/FilesTree.h"
#include "GUI/ObjectsTree.h"
#include "GUI/LogCtrl.h"
#include "GUI/FolderList.h"
#include "GUI/AboutFrame.h"
#include "GUI/ReplaceDialog.h"
#include "GUI/ObjectProperties/ProjectProperties.h"
#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"
#include "GUI/TimeSequence/LinePanel.h"
#include "GUI/TimeSequence/SequencePanel.h"
#include "GUI/LanguageParser.hpp"
#include "GUI/StcContext.hpp"

#include "TrollEditorApplication.h"
#include "TrollLogs.h"

#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Temporal/Sequence.h"

#include "Elypse/TrollInstance.h"
#include "Elypse/TrollScriptCompiler.h"
#include "Elypse/TrollSceneFileParser.h"

#include "xpm/troll_editor.xpm"
#include "xpm/BMPNew.xpm"
#include "xpm/BMPOpen.xpm"
#include "xpm/BMPSave.xpm"

#include <zip.h>

#include <File.h>
#include <StringUtils.h>

#include <ScriptEngine.h>
#include <ScriptNode.h>

BEGIN_TROLL_GUI_NAMESPACE
{
	using namespace TROLL_PROJECT_NAMESPACE;
	using namespace TROLL_PROJECT_2D_NAMESPACE;

	namespace
	{
		String DumpFile( String const & p_file )
		{
			String l_return;
			std::ifstream l_file( p_file );

			if ( l_file.is_open() )
			{
				String l_line;

				while ( std::getline( l_file, l_line ) )
				{
					l_return += l_line + "\n";
				}
			}

			return l_return;
		}

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
			for ( auto && l_file : p_files )
			{
				size_t l_index = min( l_file->FileName.find_last_of( '\\' ) , l_file->FileName.find_last_of( '/' ) );
				wxString l_fileName = l_file->FileName.substr( l_index + 1, l_file->FileName.size() );
				p_stream.WriteString( wxT( "\t" ) + p_name + wxT( " " ) + l_fileName + p_extension + wxT( "\n" ) );
			}
		}

		void WriteSceneInMain( wxString const & p_projectName, Scene * p_scene, bool p_writeScripts, bool p_writeMuseFiles, wxTextOutputStream & p_stream )
		{
			wxString l_sceneName = p_scene->GetName();
			p_stream.WriteString( wxT( "scene " ) + l_sceneName + wxT( "\n{\n" ) );

			if ( p_writeMuseFiles )
			{
				p_stream.WriteString( wxT( "\tmuse_file " ) + p_scene->GetName() + wxT( ".muse\n" ) );
			}

			WriteFilesInMain( p_scene->m_dataFiles, wxT( "data_file" ), wxT( ".zip" ), p_stream );
			WriteFilesInMain( p_scene->m_sceneFiles, wxT( "scene_file" ), wxEmptyString, p_stream );

			if ( p_writeScripts )
			{
				WriteFilesInMain( p_scene->m_loadScriptFiles, wxT( "load_script_file" ), wxEmptyString, p_stream );
				WriteFilesInMain( p_scene->m_unloadScriptFiles, wxT( "unload_script_file" ), wxEmptyString, p_stream );
			}

			if ( p_writeMuseFiles )
			{
				WriteFilesInMain( p_scene->m_museFiles, wxT( "muse_file" ), wxEmptyString, p_stream );
			}

			p_stream.WriteString( wxT( "}\n\n" ) );
		}

		bool wxCopyDir( wxString sFrom, wxString sTo )
		{
			if ( sFrom[sFrom.Len() - 1] != '\\' && sFrom[sFrom.Len() - 1] != '/' )
			{
				sFrom += wxFileName::GetPathSeparator();
			}

			if ( sTo[sTo.Len() - 1] != '\\' && sTo[sTo.Len() - 1] != '/' )
			{
				sTo += wxFileName::GetPathSeparator();
			}

			if ( !wxDirExists( sFrom ) )
			{
				std::cerr << make_string( _( "Can not copy directory" ) ) << " " << sFrom << ", " << make_string( _( "it does not exist." ) ) << std::endl;
				return false;
			}

			if ( !wxDirExists( sTo ) )
			{
				if ( !wxFileName::Mkdir( sTo, 0777, wxPATH_MKDIR_FULL ) )
				{
					std::cerr << make_string( _( "Could not create" ) ) << " " << sFrom << std::endl;
					return false;
				}
			}

			wxDir fDir( sFrom );
			wxString sNext = wxEmptyString;
			bool bIsFile = fDir.GetFirst( &sNext );

			while ( bIsFile )
			{
				const wxString sFileFrom = sFrom + sNext;
				const wxString sFileTo = sTo + sNext;

				if ( wxDirExists( sFileFrom ) )
				{
					wxCopyDir( sFileFrom, sFileTo );
				}
				else
				{
					if ( wxFileExists( sFileTo ) )
					{
						wxRemoveFile( sFileTo );
					}

					if ( !wxCopyFile( sFileFrom, sFileTo ) )
					{
						std::cerr << make_string( _( "Can not copy" ) ) << " " << sFileFrom << ", " << make_string( _( "to" ) ) << " " << sFileTo << std::endl;
						return false;
					}
				}

				bIsFile = fDir.GetNext( &sNext );
			}

			return true;
		}

		class LogStreambuf
			: public std::streambuf
		{
		public:
			LogStreambuf( wxString const & p_prefix, wxLog * p_log )
				: m_prefix( p_prefix )
				, m_log( p_log )
			{
			}

			int_type overflow( int_type c = traits_type::eof() ) override
			{
				if ( traits_type::eq_int_type( c, traits_type::eof() ) )
				{
					sync();
				}
				else if ( traits_type::eq_int_type( c, '\n' ) )
				{
					sync();
				}
				else
				{
					m_buffer << traits_type::to_char_type( c );
				}

				return c;
			}

			int sync() override
			{
				if ( !m_buffer.empty() )
				{
					if ( wxIsMainThread() )
					{
						if ( m_buffer != wxT( "\n" ) )
						{
							wxLog::SetActiveTarget( m_log );
							wxLogMessage( wxString() << m_prefix << m_buffer );
						}
					}
					else
					{
						printf( "%s\n", make_string( m_buffer ).c_str() );
					}

					m_buffer.clear();
				}

				return 0;
			}

		private:
			wxString m_buffer;
			wxString m_prefix;
			wxLog * m_log;
		};

	}

	const wxString TE_NO_SELECTED_PROJECT = _( "No selected project" );
	const wxString TE_NEW_PROJECT = _( "New Project" );
	const wxString TE_OPEN_PROJECT = _( "Open Project" );
	const wxString TE_SAVE_PROJECT = _( "Save Project" );

	const wxString TE_TIMELINE_EDITION = _( "TimeLine edition" );
	const wxString TE_OVERLAYS_EDITION = _( "Overlays edition" );
	const wxString TE_SCENE_EDITION = _( "Scene edition" );
	const wxString TE_PROJECT_TESTING = _( "Testing Project" );

	const wxString TE_EDIT_TIMELINE = _( "Edit TimeLine" );
	const wxString TE_EDIT_OVERLAYS = _( "Edit Overlays" );
	const wxString TE_EDIT_SCENE = _( "Edit Scene" );
	const wxString TE_TEST_PROJECT = _( "Test Project" );
	const wxString TE_END_EDITION = _( "End Edition" );
	const wxString TE_END_PROJECT_TEST = _( "End Project Test" );

	MainFrame * MainFrame::s_mainFrame;

	BEGIN_EVENT_TABLE( MainFrame, wxFrame )
		EVT_MENU( IdNew, MainFrame::OnNew )
		EVT_MENU( OpenProject, MainFrame::OnOpenProject )
		EVT_MENU( saveModifs, MainFrame::OnWriteSceneFiles )
		EVT_MENU( saveProject, MainFrame::OnSaveProject )
		EVT_MENU( IdCloseProject, MainFrame::OnCloseProject )
		EVT_MENU( IdOnSaveFile, MainFrame::OnSaveFile )
		EVT_MENU( IdOnSaveFileAs, MainFrame::OnSaveFileAs )
		EVT_MENU( IdOnSaveSelectedFile, MainFrame::OnSaveSelectedFile )
		EVT_MENU( Quit, MainFrame::OnQuit )
		EVT_MENU( OpenFil, MainFrame::OnOpenFile )
		EVT_MENU( About, MainFrame::OnAbout )
		EVT_MENU( Add, MainFrame::OnAdd )
		EVT_MENU( Rename, MainFrame::OnRename )
		EVT_MENU( IdOuvrirFichier, MainFrame::OnOpenSelectedFile )
		EVT_MENU( IdCompileFile, MainFrame::OnCompile )
		EVT_MENU( IdAddExistingFile, MainFrame::OnAddExistingFile )
		EVT_MENU( IdMusiner, MainFrame::OnMusinate )
		EVT_MENU( IdRemove, MainFrame::OnRemoveFile )
		EVT_MENU( IdDelete, MainFrame::OnDeleteFile )
		EVT_MENU( IdListFolder, MainFrame::OnListFolder )
		EVT_MENU( Tool_copy, MainFrame::OnFunctionCopy )
		EVT_MENU( Tool_cut, MainFrame::OnFunctionCut )
		EVT_MENU( Tool_paste, MainFrame::OnFunctionPaste )
		EVT_MENU( EditTimeLine, MainFrame::OnEditTimeLine )
		EVT_MENU( NewScene, MainFrame::OnNewScene )
		EVT_MENU( RenameScene, MainFrame::OnRename )
		EVT_MENU( DeleteScene, MainFrame::OnRemoveScene )
		EVT_MENU( IdEditerScene, MainFrame::OnEditProject )
		EVT_MENU( IdEditerOverlays, MainFrame::OnEditOverlays )
		EVT_MENU( IdTester, MainFrame::OnTestProject )
		EVT_MENU( IdFermer, MainFrame::OnCloseFile )
		EVT_MENU( IdProjectProperties, MainFrame::OnProjectProperties )
		EVT_MENU( IdGoToLine, MainFrame::OnGoToLine )
		EVT_MENU( IdSearchNext, MainFrame::OnSearchNext )
		EVT_MENU( IdSearchPrevious, MainFrame::OnSearchPrevious )
		EVT_MENU( IdReplace, MainFrame::OnReplace )
		EVT_MENU( TreeNewScene, MainFrame::OnTreeNewScene )
		EVT_MENU( EditSceneDependencies, MainFrame::OnEditSceneDependencies )
		EVT_CLOSE( MainFrame::OnClose )
		EVT_TEXT_ENTER( IdSearch, MainFrame::OnSearch )
		EVT_SHOW( MainFrame::OnShow )
		EVT_SET_FOCUS( MainFrame::OnSetFocus )
		EVT_KILL_FOCUS( MainFrame::OnKillFocus )
		EVT_ACTIVATE( MainFrame::OnActivate )
		EVT_SIZE( MainFrame::OnSize )
		EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, MainFrame::OnNotebook )
	END_EVENT_TABLE()

	static const long c_toolbarStyle = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

	MainFrame::MainFrame( bool p_debug, const wxString & title, int x, int y, int w, int h )
		: wxFrame( NULL, wxID_ANY, title, wxPoint( x, y ), wxSize( w, h ) )
		, m_tbar( NULL )
		, m_numSceneFile( 1 )
		, m_numScriptFile( 1 )
		, m_numDataFile( 1 )
		, m_rows( 1 )
		, m_nPrint( 1 )
		, m_smallToolbar( true )
		, m_horzText( false )
		, m_useCustomDisabled( false )
		, m_showTooltips( true )
		, m_testingProject( false )
		, m_currentProject( NULL )
		, m_listBox( NULL )
		, m_treeTabsContainer( NULL )
		, m_filesList( NULL )
		, m_functionsList( NULL )
		, m_objectsListContainer( NULL )
		, m_objectsList( NULL )
		, m_mainTabsContainer( NULL )
		, m_editText( NULL )
		, m_elypseContainer( NULL )
		, m_timelineEditor( NULL )
		, m_debugLog( NULL )
		, m_messageLog( NULL )
		, m_errorLog( NULL )
		, m_propertyDialog( NULL )
		, m_treesWidth( 265 )
		, m_logsHeight( 200 )
		, m_timePanel( NULL )
		, m_toolbarPosition( TOOLBAR_TOP )
		, m_objectInfos( NULL )
		, m_viewedObject( NULL )
		, m_editingProject( false )
		, m_editingOverlays( false )
		, m_editingTimeLines( false )
		, m_logTabsContainer( NULL )
		, m_elypseCtrl( NULL )
		, m_resizing( false )
		, m_pStcContext( new StcContext )
		, m_oldClog( NULL )
		, m_debug( p_debug )
	{
		s_mainFrame = this;
		wxStandardPathsBase & l_paths = wxStandardPaths::Get();
		Path l_installPath = make_string( l_paths.GetExecutablePath() );
		Path l_userPath = make_string( l_paths.GetUserDataDir() );
		l_installPath = l_installPath.GetPath().GetPath() / "share" / "TrollEditor";
		m_appPath = make_wxString( l_userPath );
		wxCopyDir( make_wxString( l_installPath ), m_appPath );

		LanguageFileParser l_parser( m_pStcContext );
		l_parser.ParseFile( m_appPath + wxFileName::GetPathSeparator() + wxT( "EMSCRIPT.lang" ) );
		l_parser.ParseFile( m_appPath + wxFileName::GetPathSeparator() + wxT( "EMSCENE.lang" ) );

		DoInitialise();
		
		wxLog::SetActiveTarget( m_logMessage );
		m_oldCout = std::cout.rdbuf( new LogStreambuf( wxT( "" ), m_logMessage ) );
		m_oldCerr = std::cerr.rdbuf( new LogStreambuf( wxT( "" ), m_logError ) );
		
		if ( m_debug )
		{
			m_oldClog = std::clog.rdbuf( new LogStreambuf( wxT( "" ), m_logDebug ) );
		}
	}

	MainFrame::~MainFrame()
	{
		delete m_pStcContext;
		s_mainFrame = NULL;
	}

	void MainFrame::DoInitialiseMainContainers()
	{
		// main tabs (contains text editor, test panel...)
		m_mainTabsContainer = new wxAuiNotebook( this, MainTabContainer, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_CLOSE_ON_ALL_TABS );
		//trees tabs (contains files list, objects list...)
		m_treeTabsContainer = new wxNotebook( this, TreesTabContainer );
		//logs tabs (contains debug log, compilation log...)
		m_logTabsContainer = new wxNotebook( this, LogsTabContainer );

		m_manager.AddPane( m_treeTabsContainer, wxAuiPaneInfo().LeftDockable().CloseButton( false ).MinSize( 200, 500 ) );
		m_manager.AddPane( m_mainTabsContainer, wxAuiPaneInfo().Center().CloseButton( false ).MinSize( 600, 500 ) );
		m_manager.AddPane( m_logTabsContainer, wxAuiPaneInfo().Bottom().CloseButton( false ).MinSize( 800, 100 ) );
	}

	void MainFrame::DoInitialiseTrees()
	{
		//files list
		m_filesList = new FilesTree( m_treeTabsContainer, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
		m_treeTabsContainer->AddPage( m_filesList, _( "Project Files" ), true );
		m_filesList->InitProject( _( "Project" ) );
		//functions list
		m_functionsList = new FunctionsTree( m_treeTabsContainer, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
		m_treeTabsContainer->AddPage( m_functionsList, _( "Functions" ), false );
		m_functionsList->AddFunctionsToTree();

		TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler l_compiler( m_pStcContext, "", true );
		l_compiler.Initialise( NULL );
	}

	void MainFrame::DoInitialiseLogs()
	{
		auto && l_creator = [this]( wxString const & p_name, LogCtrl *& p_ctrl, wxLog *& p_log )
		{
			p_ctrl = new LogCtrl( m_logTabsContainer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
			p_ctrl->SetFont( wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
			p_log = new wxLogListBox( p_ctrl );
			m_logTabsContainer->AddPage( p_ctrl, p_name, true );
		};

		if ( m_debug )
		{
			// debug log
			l_creator( _( "Debug" ), m_debugLog, m_logDebug );
		}
		
		l_creator( _( "Messages" ), m_messageLog, m_logMessage );
		l_creator( _( "Errors" ), m_errorLog, m_logError );
	}

	void MainFrame::DoInitialise()
	{
#if wxCHECK_VERSION( 2, 9, 0 )
		SetMinClientSize( wxSize( 800, 600 ) );
#else
		SetMinSize( wxSize( 800, 600 ) );
#endif

		m_manager.SetManagedWindow( this );
		
		SetIcon( wxIcon( troll_editor_xpm ) );
		SetBackgroundColour( wxColour( 255, 255, 255 ) );
		CreateStatusBar( 2 );
		SetStatusText( _( "Welcome to Troll Editor!" ) );
		DoSetMenuBar();
		PopulateToolbar( CreateToolBar( c_toolbarStyle, Toolbar ) );
			
		m_sceneDependencies = new SceneDependenciesFrame( this );
			
		DoInitialiseMainContainers();
		DoInitialiseTrees();
		DoInitialiseLogs();

		// listbox for code completion
		m_listBox = new AutoCompletionListBox( this, ListBox, wxDefaultPosition, wxDefaultSize, m_choices );
		m_listBox->Hide();

		DoCreateTreeWithDefStyle();

		m_manager.Update();
	}

	void MainFrame::DoSetMenuBar()
	{
		m_menuBar = new wxMenuBar( wxMB_DOCKABLE );
		m_menuFile = new wxMenu;
		m_menuHelp = new wxMenu;
		m_menuProject = new wxMenu;
		m_menuEdition = new wxMenu;
		m_menuFile->Append( IdNew, wxString() << TE_NEW_PROJECT );
		m_menuFile->Append( OpenProject, wxString() << TE_OPEN_PROJECT );
		m_menuFile->Append( OpenFil, wxString() << _( "Open File" ) );
		m_menuFile->Append( IdCloseProject, wxString() << _( "Close Project" ) );
		m_menuFile->AppendSeparator();			/******************************************/
		m_menuFile->Append( saveProject, wxString() << TE_SAVE_PROJECT << wxT( "\tCtrl+T" ) );
		m_menuFile->Append( IdOnSaveFile, wxString() << _( "Save File" ) << wxT( "\tCtrl+S" ) );
		m_menuFile->Append( IdOnSaveFileAs, wxString() << _( "Save File As..." ) << wxT( "\tCtrl+Shift+S" ) );
		m_menuFile->AppendSeparator();			/******************************************/
		m_menuFile->Append( Quit, wxString() << _( "Exit" ) << wxT( "\tCtrl+Q" ) );
		m_menuEdition->Append( IdGoToLine, wxString() << _( "Go to line..." ) << wxT( "\tCtrl+G" ) );
		m_menuEdition->Append( IdSearchNext, wxString() << _( "Find next..." ) << wxT( "\tF3" ) );
		m_menuEdition->Append( IdSearchPrevious, wxString() << _( "Find previous..." ) << wxT( "\tShift+F3" ) );
		m_menuEdition->Append( IdReplace, wxString() << _( "Replace..." ) << wxT( "\tCtrl+H" ) );
		m_menuProject->Append( EditSceneDependencies, wxString() << _( "Edit dependencies" ) << wxT( "\tF5" ) );
		m_menuProject->Append( IdCompileFile, wxString() << _( "Compile" ) << wxT( "\tF6" ) );
		m_menuProject->Append( IdMusiner, wxString() << _( "Musinate" ) << wxT( "\tF7" ) );
		m_menuEditTimeLines = m_menuProject->Append( EditTimeLine, wxString() << TE_EDIT_TIMELINE << wxT( "\tF8" ) );
		m_menuEditProject = m_menuProject->Append( IdEditerScene, wxString() << TE_EDIT_SCENE << wxT( "\tF9" ) );
		m_menuEditOverlays = m_menuProject->Append( IdEditerOverlays, wxString() << TE_EDIT_OVERLAYS << wxT( "\tF10" ) );
		m_menuTestProject = m_menuProject->Append( IdTester, wxString() << TE_TEST_PROJECT << wxT( "\tF11" ) );
		m_menuProject->Append( IdProjectProperties, wxString() << _( "Properties" ) << wxT( "\tCtrl+P" ) );
		m_menuHelp->Append( About, wxString() << _( "About..." ) );
		m_menuBar->Append( m_menuFile, wxString() << _( "File" ) );
		m_menuBar->Append( m_menuEdition, wxString() << _( "Edit" ) );
		m_menuBar->Append( m_menuProject, wxString() << _( "Project" ) );
		m_menuBar->Append( m_menuHelp, wxString() << _( "Help" ) );
		SetMenuBar( m_menuBar );
	}

	void MainFrame::DoCreateTreeWithDefStyle()
	{
		long style = wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS;
		wxMenuBar * mbar = GetMenuBar();
	}

	void MainFrame::DoAddSceneToMuse( Project const & p_project, bool p_createMain, bool p_withScripts, Scene * p_scene, wxString const & p_mainFile )
	{
		wxString l_projectPath = p_project.GetPath();
		wxString l_scenePath = l_projectPath + p_scene->GetName() + wxFileName::GetPathSeparator();
		DataWriter l_dataWriter;

		for ( auto && l_folder : p_scene->m_dataFolders )
		{
			LogDebug( wxT( "MainFrame::Musiner - Data Folder - " ) + l_scenePath + l_folder->FileName );
			l_dataWriter.AddFolder( make_string( l_scenePath + l_folder->FileName ), EM_BLOCK_ZIPSNDDATA );
		}

		for ( auto && l_file : p_scene->m_sceneFiles )
		{
			LogDebug( wxT( "MainFrame::Musiner - Scene File - " ) + l_scenePath + l_file->FileName );
			l_dataWriter.AddFile( make_string( l_scenePath + l_file->FileName ) );
		}

		if ( p_withScripts )
		{
			for ( auto && l_file : p_scene->m_loadScriptFiles )
			{
				LogDebug( wxT( "MainFrame::Musiner - Load Script File - " ) + l_scenePath + l_file->FileName );
				l_dataWriter.AddFile( make_string( l_scenePath + l_file->FileName ) );
			}


			for ( auto && l_file : p_scene->m_unloadScriptFiles )
			{
				LogDebug( wxT( "MainFrame::Musiner - Unload Script File - " ) + l_scenePath + l_file->FileName );
				l_dataWriter.AddFile( make_string( l_scenePath + l_file->FileName ) );
			}
		}

		for ( auto && l_folder : p_scene->m_dataFiles )
		{
			LogDebug( wxT( "MainFrame::Musiner - Data file - " ) + l_scenePath + l_folder->FileName );
			ZipFolder( l_scenePath + l_folder->FileName, l_scenePath + l_folder->FileName + wxT( ".zip" ) );
			l_dataWriter.AddFile( make_string( l_scenePath + l_folder->FileName + wxT( ".zip" ) ) );
		}

		if ( p_scene == p_project.GetMainScene() && p_createMain )
		{
			l_dataWriter.AddFile( make_string( p_mainFile ) );
		}

		l_dataWriter.Write( make_string( l_projectPath + p_scene->GetName() + wxT( ".muse" ) ) );
	}

	void MainFrame::DoMusinate( Project const & p_project, bool p_createMain, bool p_withScripts )
	{
		if ( p_project.GetMainScene() )
		{
			wxString l_mainFile;

			if ( p_createMain )
			{
				l_mainFile = CreateMain( p_project, p_withScripts );
			}

			DoAddSceneToMuse( p_project, p_createMain, p_withScripts, p_project.GetMainScene(), l_mainFile );

			for ( auto && l_scene : p_project.GetScenes() )
			{
				DoAddSceneToMuse( p_project, p_createMain, p_withScripts, l_scene.second, l_mainFile );
			}
		}
	}

	void MainFrame::DoMusinateSceneEditor( Project const & p_project )
	{
		if ( p_project.GetMainScene() )
		{
			wxString l_mainFile = CreateSceneEditorMain( p_project );
			DoAddSceneToMuse( p_project, true, true, p_project.GetMainScene(), l_mainFile );
		}
	}

	void MainFrame::DoMusinateOverlayEditor( Project const & p_project )
	{
		if ( p_project.GetMainScene() )
		{
			Scene * l_scene = p_project.GetMainScene();
			wxString l_mainFile = CreateOverlayEditorMain( p_project );
			wxString l_path = p_project.GetPath();
			DataWriter l_dataWriter;

			for ( auto && l_file : l_scene->m_sceneFiles )
			{
				LogDebug( wxT( "MainFrame::Musiner - Scene File - " ) + l_path + l_file->FileName );
				l_dataWriter.AddFile( make_string( l_path + l_file->FileName ) );
			}

			for ( auto && l_file : l_scene->m_loadScriptFiles )
			{
				LogDebug( wxT( "MainFrame::Musiner - Load Script File - " ) + l_path + l_file->FileName );
				l_dataWriter.AddFile( make_string( l_path + l_file->FileName ) );
			}

			l_dataWriter.AddFile( make_string( l_mainFile ) );
			l_dataWriter.Write( make_string( l_path + p_project.GetName() + wxT( ".muse" ) ) );
		}
	}

	void MainFrame::OnOpenSelectedFile( wxCommandEvent & p_event )
	{
		OpenSelectedFile();
	}

	void MainFrame::OnOpenFile( wxCommandEvent & p_event )
	{
		wxString l_fileName = wxFileSelector( _( "Choose a file to open" ), wxEmptyString, wxEmptyString, wxEmptyString,
												wxString() << _( "Elypse Scene files" ) << wxT( "(*.emscene)|*.emscene|" ) << _( "Elypse Sciprt files" ) << wxT( " (*.emscript)|*.emscript" ) );

		if ( l_fileName.empty() )
		{
			return;
		}
	}

	void MainFrame::OnOpenProject( wxCommandEvent & p_event )
	{
		wxString l_fileName = wxFileSelector( _( "Choose a project to open" ), wxEmptyString, wxEmptyString, wxEmptyString,
												wxString() << _( "Troll Editor Project files" ) << wxT( " (*.teproj)|*.teproj" ) );
		OpenProjectFile( l_fileName );
	}

	void MainFrame::OnWriteSceneFiles( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			GetCurrentProject()->Write();
		}
	}

	void MainFrame::OnNew( wxCommandEvent & p_event )
	{
		NewProjectFrame * l_frame = new NewProjectFrame( this );
		l_frame->ShowModal();
	}

	void MainFrame::OnCloseProject( wxCommandEvent & p_event )
	{
		CloseProject( false );
	}

	void MainFrame::OnSaveAll( wxCommandEvent & p_event )
	{
		SaveSceneFiles( GetCurrentProject()->GetMainScene() );

		for ( auto && l_it : GetCurrentProject()->GetScenes() )
		{
			SaveSceneFiles( l_it.second );
		}
	}

	void MainFrame::OnQuit( wxCommandEvent & p_event )
	{
		delete m_logDebug;
		m_logDebug = NULL;
		delete m_logMessage;
		m_logMessage = NULL;
		delete m_logError;
		m_logError = NULL;
		p_event.Skip();
	}

	void MainFrame::OnClose( wxCloseEvent & p_event )
	{
		delete std::cout.rdbuf( m_oldCout );
		delete std::cerr.rdbuf( m_oldCerr );

		if ( m_oldClog )
		{
			delete std::clog.rdbuf( m_oldClog );
		}

		CloseElypse();
		m_manager.UnInit();
		wxLog::SetActiveTarget( NULL );
		delete m_logDebug;
		m_logDebug = NULL;
		delete m_logMessage;
		m_logMessage = NULL;
		delete m_logError;
		m_logError = NULL;
		m_listBox = NULL;
		m_treeTabsContainer = NULL;
		m_filesList = NULL;
		m_functionsList = NULL;
		m_objectsListContainer = NULL;
		m_objectsList = NULL;
		m_mainTabsContainer = NULL;
		m_editText = NULL;
		m_elypseContainer = NULL;
		m_elypseContainer = NULL;
		m_logTabsContainer = NULL;
		m_timelineEditor = NULL;
		m_debugLog = NULL;
		m_messageLog = NULL;
		m_errorLog = NULL;
		m_propertyDialog = NULL;
		m_timePanel = NULL;
		m_objectInfos = NULL;

		if ( GetCurrentProject() )
		{
			CloseProject( true );
		}

		p_event.Skip();
	}

	void MainFrame::OnCompile( wxCommandEvent & p_event )
	{
		if ( !GetCurrentProject() )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
			return;
		}

		//if ( ! m_filesList->GetSelectedScene())
		//{
		//	LogDebugMessage( "Aucune scène sélectionnée");
		//	return;
		//}

		std::clog << m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ) << "\n";
		Scene * l_scene;
		File * l_file;

		if ( !GetCurrentProject()->FindFileInScenes( m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ), l_file, l_scene ) )
		{
			LogDebug( _( "No selected file" ) );
			return;
		}

		if ( ! l_file->FileName.Contains( wxT( ".emscript" ) ) )
		{
			LogDebug( _( "Can't compile this file type" ) );
			return;
		}

		try
		{
			Compile( l_file, l_scene );
		}
		catch ( ... )
		{
			LogDebug( _( "Fatal error encountered while compiling this file" ) );
		}
	}

	void MainFrame::OnAbout( wxCommandEvent & p_event )
	{
		AboutFrame * l_frame = new AboutFrame( this );
		l_frame->Show();
	}

	void MainFrame::OnAdd( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			wxTreeItemId l_folderId = m_filesList->GetSelected();
			wxString l_str;
			FileType l_fileType = m_filesList->GetSelectedScene()->GetType( l_folderId );
			TETreeItemType l_titype;
			Scene * l_scene = m_filesList->GetSelectedScene();

			if ( l_fileType == sceneFile )
			{
				l_titype = TITSceneFile;
				l_str.Printf( wxT( "NewFile%d.emscene" ), m_numSceneFile++ );
			}
			else if ( l_fileType == loadScriptFile || l_fileType == unloadScriptFile )
			{
				if ( l_fileType == loadScriptFile )
				{
					l_titype = TITLoadScriptFile;
				}
				else
				{
					l_titype = TITUnloadScriptFile;
				}

				l_str.Printf( wxT( "NewFile%d.emscript" ), m_numScriptFile++ );
			}
			else if ( l_fileType == dataFile || l_fileType == dataFolder )
			{
				return;
			}

			wxTreeItemId l_itemId = m_filesList->AddFileToFolder( l_folderId, l_str, l_titype, false );
			m_filesList->GetSelectedScene()->AddFile( l_folderId, l_itemId, l_str, true );
		}
	}

	void MainFrame::OnRename( wxCommandEvent & p_event )
	{
		m_filesList->EditLabel( m_filesList->GetSelected() );
	}

	void MainFrame::OnAddExistingFile( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			wxTreeItemId l_folderId = m_filesList->GetSelected();
			wxString l_str;
			FileType l_fileType = m_filesList->GetSelectedScene()->GetType( l_folderId );
			TETreeItemType l_titype;
			wxString filename;

			if ( l_fileType == dataFolder )
			{
				l_titype = TITDataFolder;
				wxDirDialog l_dialog( NULL, _( "Select a directory" ), m_currentProject->GetPath() );

				if ( l_dialog.ShowModal() == wxID_OK )
				{
					filename = l_dialog.GetPath();
				}
			}

			if ( l_fileType == sceneFile )
			{
				l_titype = TITSceneFile;
				filename = wxFileSelector( _( "Choose a file to open" ), m_currentProject->GetPath(), wxEmptyString, wxEmptyString, wxString() << _( "Elypse Scene files" ) << wxT( " (*.emscene)|*.emscene" ) );
			}
			else if ( l_fileType == loadScriptFile || l_fileType == unloadScriptFile )
			{
				if ( l_fileType == loadScriptFile )
				{
					l_titype = TITLoadScriptFile;
				}
				else
				{
					l_titype = TITUnloadScriptFile;
				}

				filename = wxFileSelector( _( "Choose a file to open" ), m_currentProject->GetPath(), wxEmptyString, wxEmptyString, wxString() << _( "Elypse Script Files" ) << wxT( " (*.emscript)|*.emscript" ) );
			}
			else if ( l_fileType == dataFile )
			{
				l_titype = TITDataFile;
				wxDirDialog l_dialog( NULL, _( "Choose a folder" ), m_currentProject->GetPath() );

				if ( l_dialog.ShowModal() == wxID_OK )
				{
					filename = l_dialog.GetPath();
				}
			}

			size_t posPoint = 0;

			if ( ! filename.empty() )
			{
				posPoint = filename.find_last_of( wxFileName::GetPathSeparator() ) + 1;
				wxString text = filename.substr( posPoint, filename.size() - posPoint );
				wxTreeItemId l_folderId = m_filesList->GetSelected();
				wxTreeItemId l_itemId = m_filesList->AddExistingFileToProjet( l_folderId, l_titype, text );

				if ( ! m_filesList->GetSelectedScene()->AddFile( l_folderId, l_itemId, text, true ) )
				{
					m_filesList->Delete( l_itemId );
				}
			}
		}
	}

	void MainFrame::OnSaveFile( wxCommandEvent & p_event )
	{
		if ( m_editText != NULL )
		{
			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );

			if ( l_fileName != TE_TIMELINE_EDITION && l_fileName != TE_SCENE_EDITION && l_fileName != TE_OVERLAYS_EDITION && l_fileName != TE_PROJECT_TESTING )
			{
				m_editText->SaveFile( GetCurrentProject()->GetPath() );
			}
		}
	}

	void MainFrame::OnSaveProject( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			SaveProject( *GetCurrentProject() );
		}
	}

	void MainFrame::OnSaveSelectedFile( wxCommandEvent & p_event )
	{
		wxTreeItemId l_itemId = m_filesList->GetSelected();
		Scene * l_scene = m_filesList->GetSelectedScene();

		if ( l_scene == NULL )
		{
			return;
		}

		File * l_file = l_scene->GetFile( l_itemId );

		if ( l_file == NULL )
		{
			return;
		}

		if ( l_file->Saved )
		{
			//std::cout << "MainFrame::OnSaveSelectedFile" << l_file->FileName << "\n";
			l_file->EditPanel->SaveFile( GetCurrentProject()->GetPath() );
		}
		else
		{
			OnSaveFileAs( p_event );
		}
	}

	void MainFrame::OnSaveFileAs( wxCommandEvent & p_event )
	{
		//std::cout << "MainFrame::OnSaveFileAs\n";
		int l_selectedPage = m_mainTabsContainer->GetSelection();
		wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );
		Scene * l_scene = m_filesList->GetSelectedScene();
		File * l_file = l_scene->GetFileByName( l_fileName );
		SaveFileAs( l_file );
	}

	void MainFrame::OnMusinate( wxCommandEvent & )
	{
		if ( !GetCurrentProject() )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
		}
		else
		{
			DoMusinate( *GetCurrentProject() );
		}
	}

	void MainFrame::OnRemoveFile( wxCommandEvent & )
	{
		wxTreeItemId l_itemId = m_filesList->GetSelected();
		File * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );
		bool l_found = false;
		size_t i = 0;

		while ( i <= m_mainTabsContainer->GetPageCount() && ! l_found )
		{
			if ( m_mainTabsContainer->GetPageText( i ) == l_file->FileName )
			{
				l_file->Open = false;
				m_mainTabsContainer->DeletePage( i );
				l_found = true;
			}

			i++;
		}

		m_filesList->GetSelectedScene()->RemoveFile( l_itemId );
		m_filesList->Delete( l_itemId );
	}

	void MainFrame::OnDeleteFile( wxCommandEvent & p_event )
	{
		if ( wxMessageBox( _( "Do you really want to remove this file?" ), _( "Confirm" ), wxYES_NO | wxICON_EXCLAMATION, this ) == wxYES )
		{
			wxTreeItemId l_itemId = m_filesList->GetSelected();
			File * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );
			bool l_found = false;
			size_t i = 0;

			while ( i <= m_mainTabsContainer->GetPageCount() && ! l_found )
			{
				if ( m_mainTabsContainer->GetPageText( i ) == l_file->FileName )
				{
					l_file->Open = false;
					m_mainTabsContainer->DeletePage( i );
					l_found = true;
				}

				i++;
			}

			std::clog << "MainFrame::OnDeleteFile - " << wxRemoveFile( GetCurrentProject()->GetPath() + l_file->FileName ) << "\n";
			m_filesList->GetSelectedScene()->RemoveFile( l_itemId );
			m_filesList->Delete( l_itemId );
		}
	}

	void MainFrame::OnFunctionCopy( wxCommandEvent & )
	{
	}

	void MainFrame::OnFunctionCut( wxCommandEvent & )
	{
	}

	void MainFrame::OnFunctionPaste( wxCommandEvent & )
	{
	}

	void MainFrame::OnNewScene( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			Scene * l_scene = new Scene( GetCurrentProject(), _( "New Scene" ) );
			wxString l_sceneName = l_scene->GetName();

			try
			{
				l_scene->m_sceneId = m_filesList->AddSceneToProject( l_scene );
				l_scene->m_sceneFileId = m_filesList->AddFolderToScene( l_sceneName, SCENE_FILES );
				l_scene->m_loadScriptFileId = m_filesList->AddFolderToScene( l_sceneName, LOAD_SCRIPT_FILES );
				l_scene->m_unloadScriptFileId = m_filesList->AddFolderToScene( l_sceneName, UNLOAD_SCRIPT_FILES );
				l_scene->m_dataFileId = m_filesList->AddFolderToScene( l_sceneName, GRAPHIC_DATA_FOLDERS );
				l_scene->m_dataFolderId = m_filesList->AddFolderToScene( l_sceneName, OTHER_DATA_FOLDERS );
				GetCurrentProject()->AddScene( l_scene );
			}
			catch ( std::exception const & p_exc )
			{
				wxMessageBox( _( "New scene - Add problem:\n" ) + make_wxString( p_exc.what() ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
			}
		}
	}

	void MainFrame::OnRemoveScene( wxCommandEvent & p_event )
	{
		if ( ! m_currentProject )
		{
			return;
		}

		Scene * l_scene = m_filesList->GetSelectedScene();
		wxTreeItemId l_item = m_filesList->GetSelected();
		m_filesList->DeleteChildren( l_item );
		m_filesList->Delete( l_item );
		m_currentProject->RemoveScene( l_scene );
	}

	void MainFrame::OnEditTimeLine( wxCommandEvent & p_event )
	{
		//	std::cout << "MainFrame::OnEditTimeLine\n";
		if ( !m_currentProject )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
			return;
		}

		CloseElypse();
		m_editingTimeLines = ! m_editingTimeLines;

		if ( m_editingTimeLines )
		{
			DoInitEditLists();

			if ( m_editingProject )
			{
				m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
				m_editingProject = false;
			}

			if ( m_editingOverlays )
			{
				m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
				m_editingOverlays = false;
			}

			if ( m_testingProject )
			{
				m_menuTestProject->SetItemLabel( TE_TEST_PROJECT + wxT( "\tF11" ) );
				m_testingProject = false;
			}

			if ( ! m_currentProject )
			{
				return;
			}

			m_menuEditTimeLines->SetItemLabel( TE_END_EDITION + wxT( "\tF8" ) );
			DoMusinate( *GetCurrentProject(), true );
			DoInitElypse( GetCurrentProject(), TE_TIMELINE_EDITION, false, -120 );
			m_timePanel = new Time::TimePanel( m_elypseContainer, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 120 ), wxSize( m_elypseContainer->GetClientSize().x - 20, 20 ) );
			m_timeLineContainer = new Time::TimeLineContainer( m_elypseContainer, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 100 ), wxSize( m_elypseContainer->GetClientSize().x, 100 ) );
			m_timeLineContainer->Show();
			m_timeLineContainer->SetScrollbars( 20, 20, 0, 0, 0, 0 );
			m_timelineEditor = new Time::TimeLinePanel( m_timeLineContainer, IdTimeLinePanel, wxPoint( 0, 0 ), m_timeLineContainer->GetClientSize() );
			m_timelineEditor->Show();
			//m_timeLineInfos = new wxPanel(m_elypseContainer, wxID_ANY, wxPoint( l_size.x, 0), wxSize( m_elypseContainer->GetClientSize().x - l_size.x, l_size.y), wxBORDER_SIMPLE);
			m_timeLineContainer->SetScrollbars( 20, 20, m_timelineEditor->GetSize().x / 20, m_timelineEditor->GetSize().y / 20, 0, 0 );
		}
		else
		{
			m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
			m_currentProject->GetMainScene()->FlushObjects();
		}
	}

	void MainFrame::OnEditProject( wxCommandEvent & p_event )
	{
		if ( !m_currentProject )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
			return;
		}

		CloseElypse();
		m_editingProject = ! m_editingProject;

		if ( m_editingProject )
		{
			DoInitEditLists();

			if ( m_editingTimeLines )
			{
				m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
				m_editingTimeLines = false;
			}

			if ( m_editingOverlays )
			{
				m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
				m_editingOverlays = false;
			}

			if ( m_testingProject )
			{
				m_menuTestProject->SetItemLabel( TE_TEST_PROJECT + wxT( "\tF11" ) );
				m_testingProject = false;
			}

			if ( m_currentProject == NULL )
			{
				return;
			}

			m_menuEditProject->SetItemLabel( TE_END_EDITION + wxT( "\tF9" ) );
			wxString l_fileName = m_appPath + wxT( "SceneEditor/Editeur.teproj" );
			wxFileInputStream * l_input = new wxFileInputStream( l_fileName );
			wxTextInputStream * l_textStream = new wxTextInputStream( * l_input );
			ProjectSPtr l_project = std::make_shared< Project >();
			l_project->Load( l_input, l_textStream, m_appPath + wxT( "SceneEditor/" ) );
			l_project->SetResolution( m_currentProject->GetResolution() );
			l_project->SetBackgroundImage( m_currentProject->GetBackgroundImage() );
			l_project->SetBackgroundColour( m_currentProject->GetBackgroundColour() );
			l_project->SetProjectFileName( l_fileName );
			l_project->SetSaved( true );
			delete l_textStream;
			delete l_input;
			DoMusinate( *GetCurrentProject(), false, false );
			DoMusinateSceneEditor( *l_project );
			DoInitElypse( l_project, TE_SCENE_EDITION, true );
		}
		else
		{
			m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
		}
	}

	void MainFrame::OnEditOverlays( wxCommandEvent & p_event )
	{
		if ( !m_currentProject )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
			return;
		}

		CloseElypse();
		m_editingOverlays = ! m_editingOverlays;

		if ( m_editingOverlays )
		{
			DoInitEditLists();

			if ( m_editingTimeLines )
			{
				m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
				m_editingTimeLines = false;
			}

			if ( m_editingProject )
			{
				m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
				m_editingProject = false;
			}

			if ( m_testingProject )
			{
				m_menuTestProject->SetItemLabel( TE_TEST_PROJECT + wxT( "\tF11" ) );
				m_testingProject = false;
			}

			if ( m_currentProject == NULL )
			{
				return;
			}

			m_menuEditOverlays->SetItemLabel( TE_END_EDITION + wxT( "\tF10" ) );
			wxString l_fileName = m_appPath + wxT( "OverlayEditor/Editeur.teproj" );
			wxFileInputStream * l_input = new wxFileInputStream( l_fileName );
			wxTextInputStream * l_textStream = new wxTextInputStream( * l_input );
			ProjectSPtr l_project = std::make_shared< Project >();
			l_project->Load( l_input, l_textStream, m_appPath + wxT( "OverlayEditor/" ) );
			l_project->SetResolution( m_currentProject->GetResolution() );
			l_project->SetBackgroundImage( m_currentProject->GetBackgroundImage() );
			l_project->SetBackgroundColour( m_currentProject->GetBackgroundColour() );
			l_project->SetProjectFileName( l_fileName );
			l_project->SetSaved( true );
			delete l_textStream;
			delete l_input;
			DoMusinate( *GetCurrentProject(), false, false );
			DoMusinateOverlayEditor( *l_project );
			DoInitElypse( l_project, TE_OVERLAYS_EDITION, false );
		}
		else
		{
			m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
			m_currentProject->GetMainScene()->FlushObjects();
		}
	}

	void MainFrame::OnTestProject( wxCommandEvent & p_event )
	{
		if ( !m_currentProject )
		{
			LogDebug( TE_NO_SELECTED_PROJECT );
			return;
		}

		CloseElypse();
		m_testingProject = ! m_testingProject;

		if ( m_testingProject )
		{
			if ( m_editingTimeLines )
			{
				m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
				m_editingTimeLines = false;
			}

			if ( m_editingProject )
			{
				m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
				m_editingProject = false;
			}

			if ( m_editingOverlays )
			{
				m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
				m_editingOverlays = false;
			}

			if ( ! m_currentProject )
			{
				return;
			}

			m_menuTestProject->SetItemLabel( TE_END_PROJECT_TEST + wxT( "\tF11" ) );
			DoMusinate( *GetCurrentProject() );
			DoInitElypse( GetCurrentProject(), TE_PROJECT_TESTING + wxT( " " ) + m_currentProject->GetName(), false );
		}
		else
		{
			m_menuTestProject->SetItemLabel( TE_TEST_PROJECT + wxT( "\tF11" ) );
		}
	}

	void MainFrame::OnSize( wxSizeEvent & p_event )
	{
		if ( m_tbar )
		{
			LayoutChildren();
		}
		else
		{
			p_event.Skip();
		}
	}

	void MainFrame::OnShow( wxShowEvent & p_event )
	{
		if ( m_elypseCtrl != NULL )
		{
			m_elypseCtrl->OnShow( p_event );
		}
	}

	void MainFrame::OnSetFocus( wxFocusEvent & p_event )
	{
		if ( m_elypseCtrl != NULL )
		{
			m_elypseCtrl->OnSetFocus( p_event );
		}
	}

	void MainFrame::OnKillFocus( wxFocusEvent & p_event )
	{
		if ( m_elypseCtrl != NULL )
		{
			m_elypseCtrl->OnKillFocus( p_event );
		}
	}

	void MainFrame::OnActivate( wxActivateEvent & p_event )
	{
		if ( m_elypseCtrl != NULL )
		{
			m_elypseCtrl->OnActivate( p_event );
		}
	}

	void MainFrame::OnNotebook( wxNotebookEvent & p_event )
	{
		int l_idx = p_event.GetSelection();
		wxBookCtrlBase  * l_book = wx_static_cast( wxBookCtrlBase *, p_event.GetEventObject() );
		m_currentPage = l_book->GetPageText( l_idx );

		if ( l_book->GetName().find( TE_PROJECT_TESTING ) == 0 )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->SetCtrlFocus();
			}
		}
		else
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->KillCtrlFocus();
			}

			if ( l_book->GetName() == wxT( "Editeur" ) )
			{
				if ( m_filesList->GetSelectedScene() != NULL )
				{
					File * l_file = m_filesList->GetSelectedScene()->GetFileByName( m_currentPage );

					if ( l_file != NULL )
					{
						m_editText = l_file->EditPanel;
						return;
					}
				}
			}
		}
	}

	void MainFrame::OnNotebookMiddleDown( wxMouseEvent & p_event )
	{
		std::cout << "MainFrame::OnNotebookMiddleDown\n";
	}

	void MainFrame::OnCloseFile( wxCommandEvent & p_event )
	{
		File * l_file = m_filesList->GetSelectedScene()->GetFile( m_filesList->GetSelected() );

		if ( l_file->Open )
		{
			for ( size_t i = 0; i <= m_mainTabsContainer->GetPageCount(); i++ )
			{
				if ( m_mainTabsContainer->GetPageText( i ) == l_file->FileName )
				{
					l_file->Open = false;
					l_file->EditPanel = NULL;
					m_mainTabsContainer->DeletePage( i );
					return;
				}
			}
		}
	}

	void MainFrame::OnListFolder( wxCommandEvent & p_event )
	{
		ListFolder();
	}

	void MainFrame::OnProjectProperties( wxCommandEvent & p_event )
	{
		if ( GetCurrentProject() )
		{
			Properties::ProjectProperties l_properties( *GetCurrentProject(), this );
			l_properties.ShowModal();
		}
	}

	void MainFrame::OnGoToLine( wxCommandEvent & p_event )
	{
		if ( m_mainTabsContainer->GetPageCount() <= 0 )
		{
			return;
		}

		int l_page = m_mainTabsContainer->GetSelection();
		wxStcTextEditor * l_panel = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_page )->GetChildren().GetFirst()->GetData() );

		if ( l_panel == NULL )
		{
			return;
		}

		int l_maxLines = l_panel->GetLineCount();
		wxString l_text;
		l_text << _( "Choose the line" ) << wxT( " (1 - " ) << l_maxLines << wxT( ")" );
		wxTextEntryDialog * l_dialog = new wxTextEntryDialog( this, l_text, _( "Go to..." ) );

		if ( l_dialog->ShowModal() == wxID_OK )
		{
			wxString l_line = l_dialog->GetValue();
			int l_res = 0;

			if ( GetInt( l_line, l_res ) && l_res <= l_maxLines && l_res > 0 )
			{
				l_panel->GotoLine( l_res );
			}
		}

		l_dialog->Destroy();
	}

	void MainFrame::OnSearch( wxCommandEvent & p_event )
	{
		if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
		{
			return;
		}

		int l_selectedPage = m_mainTabsContainer->GetSelection();
		wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
		l_editCtrl->SearchNext( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
	}

	void MainFrame::OnSearchNext( wxCommandEvent & p_event )
	{
		if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
		{
			return;
		}

		int l_selectedPage = m_mainTabsContainer->GetSelection();
		wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
		l_editCtrl->SearchNext( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
	}

	void MainFrame::OnSearchPrevious( wxCommandEvent & p_event )
	{
		if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
		{
			return;
		}

		int l_selectedPage = m_mainTabsContainer->GetSelection();
		wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
		l_editCtrl->SearchPrev( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
	}

	void MainFrame::OnReplace( wxCommandEvent & p_event )
	{
		if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
		{
			return;
		}

		int l_selectedPage = m_mainTabsContainer->GetSelection();
		wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
		ReplaceDialog * l_dialog = new ReplaceDialog( this, l_editCtrl );
		l_dialog->Show();
	}

	void MainFrame::DoInitEditLists()
	{
		m_objectsListContainer = new wxPanel( m_treeTabsContainer, ObjectsList, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
		m_treeTabsContainer->InsertPage( 2, m_objectsListContainer, _( "Objects" ), false );
		wxSize l_size = m_objectsListContainer->GetClientSize();
		l_size.y /= 2;
		m_objectsList = new ObjectsTree( m_objectsListContainer, wxDefaultPosition, l_size, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
		m_objectsList->InitObjectList();
		m_objectInfosContainer = new wxScrolledWindow( m_objectsListContainer, PropertiesList, wxPoint( 5, l_size.y + 5 ), wxSize( l_size.x - 10, l_size.y - 10 ) );
	}

	void MainFrame::DoInitElypse( ProjectSPtr p_project, const wxString & p_title, bool p_edit, int p_adjustY )
	{
		wxSize l_projectSize = p_project->GetResolution();
		wxSize l_size = m_mainTabsContainer->GetClientSize();

		if ( l_size.x < l_projectSize.x || l_size.y < l_projectSize.y && m_testingProject )
		{
			l_size = l_projectSize;
			m_elypseContainer = new ElypseFrame( this, wxPoint( 0, 0 ), l_size );
			m_elypseContainer->SetBackgroundColour( wxColour( 212, 208, 200 ) );
			m_elypseContainer->Show();
			m_isFrame = true;
		}
		else
		{
			if ( m_testingProject )
			{
				l_size = l_projectSize;
			}

			wxPanel * l_panel = new wxPanel( m_mainTabsContainer );
			l_panel->SetBackgroundColour( p_project->GetBackgroundColour() );
			m_elypseContainer = new ElypsePanel( l_panel, wxPoint( 0, 0 ), l_size );
			wxBoxSizer * l_sizer = new wxBoxSizer( wxVERTICAL );
			l_sizer->AddStretchSpacer();
			l_sizer->Add( m_elypseContainer, wxSizerFlags().Align( wxALIGN_CENTRE ) );
			l_sizer->AddStretchSpacer();
			l_sizer->SetSizeHints( l_panel );
			l_panel->SetSizer( l_sizer );
			m_mainTabsContainer->InsertPage( 0, l_panel, p_title, true );
			m_isFrame = false;
		}

		m_elypseCtrl = new ElypseCtrl( m_elypseContainer, p_project, this, p_edit );
		m_elypseCtrl->Init();
	}

	void MainFrame::OnTreeNewScene( wxCommandEvent & p_event )
	{
		if ( m_currentProject == NULL )
		{
			return;
		}

		NewSceneFrame * l_frame = new NewSceneFrame( this );
		l_frame->Show();
	}

	void MainFrame::OnEditSceneDependencies( wxCommandEvent & p_event )
	{
		if ( m_currentProject == NULL )
		{
			return;
		}

		m_sceneDependencies->SetScene( m_filesList->GetSelectedScene() );
		m_sceneDependencies->Show();
	}

	void MainFrame::LayoutChildren()
	{
		if ( m_tbar )
		{
			m_tbar->SetSize( 0, 0, wxDefaultCoord, GetClientSize().y );
			return;
		}
	}

	void MainFrame::PopulateToolbar( wxToolBarBase * toolBar )
	{
		wxBitmap toolBarBitmaps[Tool_Max];
		toolBarBitmaps[Tool_new] = wxBitmap( new_xpm );
		toolBarBitmaps[Tool_open] = wxBitmap( open_xpm );
		toolBarBitmaps[Tool_save] = wxBitmap( save_xpm );
		int l_width = toolBarBitmaps[Tool_new].GetWidth();
		int l_height = toolBarBitmaps[Tool_new].GetHeight();

		if ( ! m_smallToolbar )
		{
			l_width *= 2;
			l_height *= 2;

			for ( size_t n = Tool_new ; n < WXSIZEOF( toolBarBitmaps ) ; n ++ )
			{
				toolBarBitmaps[n] = wxBitmap( toolBarBitmaps[n].ConvertToImage().Scale( l_width, l_height ) );
			}
		}

		toolBar->SetToolBitmapSize( wxSize( l_width, l_height ) );
		toolBar->AddTool( IdNew, TE_NEW, toolBarBitmaps[Tool_new], TE_NEW_PROJECT );
		toolBar->AddTool( OpenProject, TE_OPEN, toolBarBitmaps[Tool_open], TE_OPEN_PROJECT );
		toolBar->AddTool( saveProject, TE_SAVE, toolBarBitmaps[Tool_save], TE_SAVE_PROJECT );
		//toolBar->AddTool( wxID_COPY, wxT( "Copier" ), toolBarBitmaps[Tool_copy], wxT( "Copier") );
		//toolBar->AddTool( wxID_CUT, wxT( "Couper" ), toolBarBitmaps[Tool_cut], wxT( "Couper") );
		//toolBar->AddTool( wxID_PASTE, wxT( "Coller" ), toolBarBitmaps[Tool_paste], wxT( "Coller") );
		toolBar->AddSeparator();		/*****************************************************/
		m_search = new wxSearchCtrl( toolBar, IdSearch, wxEmptyString, wxDefaultPosition, wxSize( 120, wxDefaultCoord ), wxBORDER_SIMPLE );
		toolBar->AddControl( m_search );
		toolBar->Realize();
		toolBar->SetRows( !( toolBar->IsVertical() ) ? m_rows : 10 / m_rows );
	}

	void MainFrame::SetProject( const wxString & p_name, const wxString & p_path, const wxString & p_mainSceneName,
								BackgroundType p_backgroundType, const wxString & p_background, bool p_shadows,
								AntiAliasing p_aa, const wxSize & p_resolution )
	{
		m_currentProject = std::make_shared< Project >( p_name, p_mainSceneName, p_path, p_backgroundType, p_background, p_shadows, p_aa, p_resolution );
		m_currentProject->SetSaved( false );
		Scene * l_mainScene = m_currentProject->GetMainScene();
		m_filesList->DeleteAllItems();
		m_filesList->InitProject( p_name );

		try
		{
			l_mainScene->m_sceneId = m_filesList->AddSceneToProject( l_mainScene );
			l_mainScene->m_sceneFileId = m_filesList->AddFolderToScene( p_mainSceneName, SCENE_FILES );
			l_mainScene->m_loadScriptFileId = m_filesList->AddFolderToScene( p_mainSceneName, LOAD_SCRIPT_FILES );
			l_mainScene->m_unloadScriptFileId = m_filesList->AddFolderToScene( p_mainSceneName, UNLOAD_SCRIPT_FILES );
			l_mainScene->m_dataFileId = m_filesList->AddFolderToScene( p_mainSceneName, GRAPHIC_DATA_FOLDERS );
			l_mainScene->m_dataFolderId = m_filesList->AddFolderToScene( p_mainSceneName, OTHER_DATA_FOLDERS );
		}
		catch ( std::exception const & p_exc )
		{
			wxMessageBox( _( "Set project - Add problem:\n" ) +make_wxString( p_exc.what() ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
		}

		if ( m_editText )
		{
			m_editText->GetText().Clear();
		}
	}

	wxString MainFrame::CreateMain( Project const & p_project, bool p_writeScripts )
	{
		wxString l_mainFileName;
		l_mainFileName = p_project.GetPath() + wxT( "main.emcfg" );
		wxFileOutputStream l_fileOutput( l_mainFileName );
		wxTextOutputStream l_textStream( l_fileOutput );
		WriteBackgroundInMain( p_project, l_textStream );

		for ( auto && l_scene : p_project.GetScenes() )
		{
			WriteSceneInMain( p_project.GetName(), l_scene.second, p_writeScripts, true, l_textStream );
		}

		Scene * l_scene = p_project.GetMainScene();

		if ( l_scene )
		{
			WriteSceneInMain( p_project.GetName(), l_scene, p_writeScripts, true, l_textStream );
			l_textStream.WriteString( wxT( "start_scene " ) + l_scene->GetName() );
		}

		l_fileOutput.Close();
		return l_mainFileName;
	}

	wxString MainFrame::CreateSceneEditorMain( Project const & p_project )
	{
		wxString l_mainFileName;
		l_mainFileName = p_project.GetPath() + wxT( "main.emcfg" );
		wxFileOutputStream l_fileOutput( l_mainFileName );
		wxTextOutputStream l_textStream( l_fileOutput );
		WriteBackgroundInMain( p_project, l_textStream );
		Scene * l_scene = p_project.GetMainScene();

		if ( l_scene )
		{
			WriteSceneInMain( p_project.GetName(), l_scene, true, false, l_textStream );

			l_textStream.WriteString( wxT( "scene " ) + l_scene->GetName() + wxT( "\n{\n" ) );
			l_textStream.WriteString( wxT( "\tmuse_file file://" ) + m_currentProject->GetPath() + m_currentProject->GetName() + wxT( ".muse\n" ) );
			WriteFilesInMain( l_scene->m_dataFiles, wxT( "data_file" ), wxT( ".zip" ), l_textStream );
			WriteFilesInMain( l_scene->m_sceneFiles, wxT( "scene_file" ), wxEmptyString, l_textStream );
			l_textStream.WriteString( wxT( "}\n\n" ) );

			l_textStream.WriteString( wxT( "start_scene " ) + m_currentProject->GetMainScene()->GetName() + wxT( "\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + p_project.GetMainScene()->GetName() );
		}

		l_fileOutput.Close();
		return l_mainFileName;
	}

	wxString MainFrame::CreateOverlayEditorMain( Project const & p_project )
	{
		wxString l_mainFileName;
		l_mainFileName = p_project.GetPath() + wxT( "main.emcfg" );
		wxFileOutputStream l_fileOutput( l_mainFileName );
		wxTextOutputStream l_textStream( l_fileOutput );
		WriteBackgroundInMain( p_project, l_textStream );
		Scene * l_scene = p_project.GetMainScene();

		if ( l_scene )
		{
			l_textStream.WriteString( wxT( "scene " ) + l_scene->GetName() + wxT( "\n{\n" ) );
			WriteFilesInMain( l_scene->m_sceneFiles, wxT( "scene_file" ), wxEmptyString, l_textStream );
			WriteFilesInMain( l_scene->m_loadScriptFiles, wxT( "load_script_file" ), wxEmptyString, l_textStream );
			l_textStream.WriteString( wxT( "}\n\n" ) );

			l_scene = m_currentProject->GetMainScene();

			if ( l_scene )
			{
				l_textStream.WriteString( wxT( "scene " ) + l_scene->GetName() + wxT( "\n{\n" ) );
				l_textStream.WriteString( wxT( "\tmuse_file file://" ) + m_currentProject->GetPath() + m_currentProject->GetName() + wxT( ".muse\n" ) );
				WriteFilesInMain( l_scene->m_dataFiles, wxT( "data_file" ), wxT( ".zip" ), l_textStream );
				WriteFilesInMain( l_scene->m_sceneFiles, wxT( "scene_file" ), wxEmptyString, l_textStream );
				l_textStream.WriteString( wxT( "}\n\n" ) );
				l_textStream.WriteString( wxT( "start_scene " ) + m_currentProject->GetMainScene()->GetName() + wxT( "\n" ) );
			}
			else
			{
				LogDebug( _( "MainFrame::CreateOverlayEditorMain - Current Project has no main scene" ) );
			}

			l_textStream.WriteString( wxT( "start_scene " ) + p_project.GetMainScene()->GetName() );
		}
		else
		{
			LogDebug( _( "MainFrame::CreateOverlayEditorMain - Given Project has no main scene" ) );
		}

		l_fileOutput.Close();
		return l_mainFileName;
	}

	void MainFrame::OpenSelectedFile()
	{
		wxTreeItemId l_itemId = m_filesList->GetSelected();
		File * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );

		if ( l_file && l_file->Saved && !l_file->Open )
		{
			wxPanel * l_editTextContainer = new wxPanel( m_mainTabsContainer, wxID_ANY, wxPoint( 0, 0 ) );
			m_mainTabsContainer->AddPage( l_editTextContainer, l_file->FileName, true );
			l_editTextContainer->SetSize( 0, 22, m_mainTabsContainer->GetClientSize().x, m_mainTabsContainer->GetClientSize().y - 22 );
			m_editText = new wxStcTextEditor( l_file, *m_pStcContext, l_editTextContainer, wxID_ANY, wxPoint( 0, 0 ), l_editTextContainer->GetClientSize() );
			wxString l_path = GetCurrentProject()->GetPath() + l_file->m_scene->GetName() + wxFileName::GetPathSeparator();
			m_editText->LoadFile( GetCurrentProject()->GetPath() );
			l_file->Open = true;
			l_file->EditPanel = m_editText;

			for ( size_t i = 0 ; i < m_mainTabsContainer->GetPageCount() ; i++ )
			{
				if ( m_mainTabsContainer->GetPageText( i ) == wxT( "Editeur" ) )
				{
					m_mainTabsContainer->RemovePage( i );
					return;
				}
			}

			if ( l_file->Filetype == loadScriptFile || l_file->Filetype == unloadScriptFile )
			{
				Compile( l_file, m_filesList->GetSelectedScene() );
			}
		}
	}

	void MainFrame::LogDebug( const wxString & p_message )
	{
		std::clog << p_message << std::endl;
	}

	void MainFrame::LogMessage( const wxString & p_message )
	{
		std::cout << p_message << std::endl;
	}

	void MainFrame::LogError( const wxString & p_message )
	{
		std::cerr << p_message << std::endl;
	}

	void MainFrame::CloseProject( bool p_closingApp )
	{
		bool l_continue = true;

		if ( m_currentProject )
		{
			if ( m_currentProject->IsModified() )
			{
				int l_answer = wxMessageBox( _( "Would you like to save your modifications" ), _( "Confirm" ), wxYES_NO | wxCANCEL, this );

				if ( l_answer == wxYES )
				{
					SaveProject( *GetCurrentProject() );
				}
				else if ( l_answer == wxCANCEL )
				{
					bool l_continue = false;
				}
			}
		}

		if ( l_continue )
		{
			m_currentProject.reset();

			m_elypseContainer = NULL;
			m_tbar = NULL;
			m_numSceneFile = 1;
			m_numScriptFile = 1;
			m_numDataFile = 1;
			m_rows = 1;
			m_nPrint = 1;
			m_smallToolbar = true;
			m_horzText = false;
			m_useCustomDisabled = false;
			m_showTooltips = true;
			m_testingProject = false;
			m_editingProject = false;
			m_editingOverlays = false;

			// main Panel (which contains all panels)
			if ( DestroyChildren() )
			{
				m_listBox = NULL;
				m_treeTabsContainer = NULL;
				m_filesList = NULL;
				m_functionsList = NULL;
				m_objectsListContainer = NULL;
				m_objectsList = NULL;
				m_mainTabsContainer = NULL;
				m_editText = NULL;
				m_elypseContainer = NULL;
				m_elypseContainer = NULL;
				m_logTabsContainer = NULL;
				m_debugLog = NULL;
				m_messageLog = NULL;
				m_errorLog = NULL;
				m_propertyDialog = NULL;
				m_toolbarPosition = TOOLBAR_TOP;

				if ( !p_closingApp )
				{
					DoInitialise();
				}
			}
		}
	}

	void MainFrame::HideObjectInfos()
	{
		if ( m_viewedObject != NULL )
		{
			m_viewedObject->HideProperties();
			m_viewedObject = NULL;
		}
	}

	void MainFrame::ShowObjectInfos( Object * p_object )
	{
		//	std::cout << "MainFrame::ShowObjectInfos\n";
		HideObjectInfos();
		int l_height = p_object->BuildPanel( m_objectInfosContainer, m_objectInfosContainer->GetClientSize().x - 20 );

		if ( l_height > m_objectInfosContainer->GetClientSize().y )
		{
			int l_displayedLines = ( m_objectInfosContainer->GetClientSize().y - 10 ) / 20;
			m_objectInfosContainer->SetScrollbars( 20, 20, 0, l_height / 20, 0, 0 );
		}
		else
		{
			m_objectInfosContainer->SetScrollbars( 20, 20, 0, 0, 0, 0 );
		}

		m_viewedObject = p_object;
	}

	float MainFrame::GetMuseHeight()
	{
		if ( m_elypseCtrl != NULL )
		{
			return m_elypseCtrl->GetMuseHeight();
		}

		return 0;
	}

	float MainFrame::GetMuseWidth()
	{
		if ( m_elypseCtrl != NULL )
		{
			return m_elypseCtrl->GetMuseWidth();
		}

		return 0;
	}

	void MainFrame::CreateNewContinuousEventFrame( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
	}

	void MainFrame::AddPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence, float p_fireTime )
	{
		StringArray l_tmp;
		Elypse::Sequences::BasePonctualEvent * l_museEvent = ScriptEngine::GetContext()->sequenceManager->CreatePonctualEvent( "Overlay_Hide", l_tmp );
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * l_event = new TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent( p_sequence, l_museEvent, wxT( "overlay" ), wxT( "" ), p_fireTime, wxT( "" ), wxT( "Overlay_Hide" ), wxT( "" ) );
		p_sequence->AddPonctualEvent( l_event );
		Time::SequencePanel * l_panel = reinterpret_cast< TROLL_GUI_TIME_NAMESPACE::SequencePanel * >( m_timelineEditor->GetSequencePanel( p_sequence->GetObjectName() )->GetParent() );
		l_panel->AddPonctualEvent( l_event, p_fireTime );
	}

	void MainFrame::AddContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence, float p_fireTime )
	{
		StringArray l_tmp;
		Overlay_Translate * l_museEvent = reinterpret_cast <Overlay_Translate *>( ScriptEngine::GetContext()->sequenceManager->CreateContinuousEvent( "Overlay_Translate" ) );
		l_museEvent->SetInterpolator( ScriptEngine::GetContext()->sequenceManager->GetInterpolator_V3( "linear" ) );
		l_museEvent->AddFrame( p_fireTime, Vector3( 0.0, 0.0, 0.0 ) );
		TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * l_event = new TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent( p_sequence, l_museEvent, wxT( "Overlay_Translate" ), wxT( "" ) );
		p_sequence->AddContinuousEvent( l_event );
		l_event->AddKeyFrame( p_fireTime, wxT( "0.0 0.0 0.0" ) );
		l_event->SetTargetType( wxT( "overlay" ) );
		l_event->SetInterpolation( wxT( "linear" ) );
		l_event->SetActionName( wxT( "Overlay_Translate" ) );
		Time::SequencePanel * l_panel = reinterpret_cast< TROLL_GUI_TIME_NAMESPACE::SequencePanel * >( m_timelineEditor->GetSequencePanel( p_sequence->GetObjectName() )->GetParent() );
		l_panel->AddContinuousEvent( l_event );
	}

	void MainFrame::OpenProjectFile( const wxString & p_fileName )
	{
		if ( !p_fileName.empty() )
		{
			wxString l_fileName = p_fileName;
			l_fileName.Replace( wxT( "/" ), wxString( wxFileName::GetPathSeparator() ) );
			l_fileName.Replace( wxT( "\\" ), wxString( wxFileName::GetPathSeparator() ) );
			size_t l_index = l_fileName.find_last_of( wxFileName::GetPathSeparator() );
			wxString l_path = l_fileName.substr( 0, l_index + 1 );

			if ( !m_currentProject )
			{
				std::clog << make_string( _( "New project while no current" ) ) << std::endl;
				wxTreeItemId l_root = m_filesList->GetRootItem();
				m_currentProject = std::make_shared< Project >();
				m_currentProject->Load( p_fileName, m_filesList );
			}
			else if ( p_fileName != m_currentProject->GetProjectFileName() )
			{
				std::clog << make_string( _( "New project with a current" ) ) << std::endl;
				wxTreeItemId l_root = m_filesList->GetRootItem();
				m_currentProject = std::make_shared< Project >();
				m_currentProject->Load( p_fileName, m_filesList );
			}
			else
			{
				wxMessageBox( _( "Can't add a project with this name, there is already one with the same name" ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, NULL );
			}
		}
	}

	void MainFrame::SaveSceneFiles( Scene * p_scene )
	{
		File * l_currentFile;
		unsigned int l_number = p_scene->GetSceneFilesCount();

		for ( unsigned int i = 0 ; i < l_number ; i++ )
		{
			l_currentFile = p_scene->GetSceneFile( i );

			if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
			{
				l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
			}
		}

		l_number = p_scene->GetLoadScriptFilesCount();

		for ( unsigned int i = 0 ; i < l_number ; i++ )
		{
			l_currentFile = p_scene->GetLoadScriptFile( i );

			if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
			{
				l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
			}
		}

		l_number = p_scene->GetUnloadScriptFilesCount();

		for ( unsigned int i = 0 ; i < l_number ; i++ )
		{
			l_currentFile = p_scene->GetUnloadScriptFile( i );

			if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
			{
				l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
			}
		}
	}

	void MainFrame::SaveScene( Scene * p_scene )
	{
		File * l_file;

		for ( unsigned int i = 0 ; i < p_scene->m_sceneFiles.size() ; i++ )
		{
			l_file = p_scene->m_sceneFiles[i];

			if ( ! l_file->Saved )
			{
				SaveFileAs( l_file );
			}
			else if ( l_file->EditPanel != NULL && l_file->EditPanel->IsModified() )
			{
				SaveFile( l_file, l_file->FileName );
			}
		}

		for ( unsigned int i = 0 ; i < p_scene->m_loadScriptFiles.size() ; i++ )
		{
			l_file = p_scene->m_loadScriptFiles[i];

			if ( ! l_file->Saved )
			{
				SaveFileAs( l_file );
			}
			else if ( l_file->EditPanel != NULL && l_file->EditPanel->IsModified() )
			{
				SaveFile( l_file, l_file->FileName );
			}
		}

		for ( unsigned int i = 0 ; i < p_scene->m_unloadScriptFiles.size() ; i++ )
		{
			l_file = p_scene->m_unloadScriptFiles[i];

			if ( ! l_file->Saved )
			{
				SaveFileAs( l_file );
			}
			else if ( l_file->EditPanel != NULL && l_file->EditPanel->IsModified() )
			{
				SaveFile( l_file, l_file->FileName );
			}
		}
	}

	void MainFrame::SaveProject( Project & p_project )
	{
		wxString l_fileName;

		if ( p_project.IsSaved() )
		{
			l_fileName = p_project.GetProjectFileName();
		}
		else
		{
			l_fileName = wxFileSelector( _( "Save as ..." ), p_project.GetPath(), p_project.GetName() + wxT( ".teproj" ), wxEmptyString,
											_( "TrollEditor Project file" ) + wxString( wxT( " (*.teproj)|*.teproj" ) ), wxFD_SAVE );
			LogDebug( l_fileName );
			p_project.SetProjectFileName( l_fileName );
		}

		LogDebug( wxT( "MainFrame::SaveProject - " ) + l_fileName );

		if ( ! l_fileName.empty() )
		{
			LogDebug( wxT( "MainFrame::OnSaveProject - " ) + l_fileName );
			wxFileOutputStream fileOutput( l_fileName );
			wxTextOutputStream * l_textStream = new wxTextOutputStream( fileOutput );
			Scene * l_scene = p_project.GetMainScene();
			SaveScene( l_scene );

			for ( auto && l_it : p_project.GetScenes() )
			{
				if ( l_it.second )
				{
					SaveScene( l_it.second );
				}
			}

			p_project.Save( l_textStream );
			p_project.SetSaved( true );
			p_project.SetModified( false );
			delete l_textStream;
			fileOutput.Close();
		}
	}

	void MainFrame::SaveFileAs( File * p_file )
	{
		if ( p_file == NULL )
		{
			LogDebug( _( "MainFrame::SaveFileAs - Nothing to save" ) );
			return;
		}

		wxString l_fullFileName;

		if ( p_file->Filetype == sceneFile )
		{
			l_fullFileName = wxFileSelector( _( "Save as ..." ), m_currentProject->GetPath(),
												wxEmptyString, wxEmptyString, _( "Elypse Scene file" ) + wxString( wxT( " (*.emscene)|*.emscene" ) ), wxFD_SAVE );
		}
		else if ( p_file->Filetype == loadScriptFile || p_file->Filetype == unloadScriptFile )
		{
			l_fullFileName = wxFileSelector( _( "Save as ..." ), m_currentProject->GetPath(),
												wxEmptyString, wxEmptyString, _( "Elypse Script file" ) + wxString( wxT( " (*.emscript)|*.emscript" ) ), wxFD_SAVE );
		}
		else if ( p_file->Filetype == dataFile )
		{
			l_fullFileName = wxFileSelector( _( "Save as ..." ), m_currentProject->GetPath(),
												wxEmptyString, wxEmptyString, _( "Zip file" ) + wxString( wxT( " (*.zip)|*.zip" ) ), wxFD_SAVE );
		}

		if ( ! l_fullFileName.empty() )
		{
			size_t l_index = min( l_fullFileName.find_last_of( '\\' ) , l_fullFileName.find_last_of( '/' ) );
			wxString l_filename = l_fullFileName.substr( l_index + 1, l_fullFileName.size() );
			m_filesList->SetItemText( p_file->ItemId, l_filename );

			if ( p_file->EditPanel != NULL )
			{
				p_file->EditPanel->SaveFile( l_filename );
			}

			p_file->FileName = l_filename;
		}
	}

	void MainFrame::SaveFile( File * p_file, const wxString & p_fileName )
	{
		if ( p_file != NULL && p_file->EditPanel != NULL )
		{
			wxString l_filePath = m_currentProject->GetPath();

			if ( p_file->m_scene != m_currentProject->GetMainScene() )
			{
				l_filePath += p_file->m_scene->GetName() + wxT( "/" );
			}

			l_filePath += p_fileName;
			p_file->EditPanel->SaveFile( l_filePath );
		}
	}

	bool MainFrame::ZipFolder( const wxString & p_folderPath, const wxString & p_zipFileName )
	{
		//CkZip l_zip;
		//if ( ! l_zip.UnlockComponent("Anything for 30-day trial."))
		//{
		//	LogDebugMessage( wxString( "MainFrame::ZipFolder - ") + l_zip.lastErrorText());
		//	return false;
		//}

		//l_zip.NewZip( p_zipFileName.c_str());

		//if ( ! l_zip.AppendFiles( p_folderPath.c_str(), true, 0))
		//{
		//	LogDebugMessage( wxString( "MainFrame::ZipFolder - ") + l_zip.lastErrorText());
		//	return false;
		//}

		//l_zip.SetCompressionLevel( 9);

		//if ( ! l_zip.WriteZipAndClose( 0))
		//{
		//	LogDebugMessage( wxString( "MainFrame::ZipFolder - ") + l_zip.lastErrorText());
		//	return false;
		//}

		return true;
	}

	void MainFrame::CompileFile( TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler & p_compiler, File * p_file )
	{
		wxString l_fileName = p_file->m_scene->GetName() + wxFileName::GetPathSeparator() + p_file->FileName;
		wxString l_path = GetCurrentProject()->GetPath() + l_fileName;
		LogMessage( wxString( _( "Compiling " ) ) << l_fileName );
		ScriptNode * l_scriptNode = p_compiler.CompileScriptFile( p_file->CfgFile.get() );
		LogMessage( l_fileName << wxString( _( " compiled - " ) ) << p_compiler.GetNumWarnings() << _( " warnings - " ) << p_compiler.GetNumErrors() << _( " errors" ) );

		if ( l_scriptNode )
		{
			l_scriptNode->Use();
			l_scriptNode->Delete();
		}
	}

	void MainFrame::CompileNeededScenes( const wxArrayString & p_scenes, TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler & p_compiler, wxArrayString & p_compiledScenes )
	{
		for ( auto && l_it : p_scenes )
		{
			auto && l_scene = m_currentProject->GetScene( l_it );
			wxArrayString l_neededScenes = l_scene->GetNeededScenes();
			CompileNeededScenes( l_neededScenes, p_compiler, p_compiledScenes );

			if ( std::find( p_compiledScenes.begin(), p_compiledScenes.end(), l_it ) == p_compiledScenes.end() )
			{
				for ( auto && l_itFiles : l_scene->GetLoadScriptFiles() )
				{
					CompileFile( p_compiler, l_itFiles );
				}

				p_compiledScenes.push_back( l_it );
			}
		}
	}

	void MainFrame::Compile( File * p_file, Scene * p_scene )
	{
		m_logTabsContainer->SetSelection( 1 );

		if ( m_elypseContainer != NULL )
		{
			LogMessage( _( "Can't compile during a test or a scene edition" ) );
		}
		else
		{
			p_file->EditPanel->SaveFile( m_currentProject->GetPath() );

			if ( !p_file->Saved || ( p_file->EditPanel != NULL && p_file->EditPanel->IsModified() ) )
			{
				LogMessage( _( "Please save the file before compiling it" ) );
			}
			else
			{
				String l_path = make_string( m_currentProject->GetPath() );

				if ( p_file->EditPanel )
				{
					p_file->EditPanel->ReinitContext( *m_pStcContext );
				}

				{
					TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler l_compiler( m_pStcContext, l_path, false );
					l_compiler.Initialise( p_file->EditPanel );
					l_compiler.CompileScript( make_string( m_currentProject->GetStartupScript() ) );
					wxArrayString l_compiledScenes;
					CompileNeededScenes( p_scene->GetNeededScenes(), l_compiler, l_compiledScenes );
					auto && l_it = p_scene->GetLoadScriptFiles().begin();
					auto && l_end = p_scene->GetLoadScriptFiles().end();
					bool l_found = false;

					while ( l_it != l_end && !l_found )
					{
						File * l_file = *l_it;
						CompileFile( l_compiler, l_file );
						l_found = p_file == l_file;
						++l_it;
					}

					if ( !l_found )
					{
						for ( auto && l_itFiles : p_scene->GetUnloadScriptFiles() )
						{
							CompileFile( l_compiler, l_itFiles );
						}
					}
				}

				for ( auto && l_itScenes : m_currentProject->GetScenes() )
				{
					for ( auto && l_itFiles : l_itScenes.second->GetLoadScriptFiles() )
					{
						l_itFiles->m_compiled = false;
					}
				}
			}
		}
	}

	void MainFrame::ListFolder()
	{
		Scene * l_scene = m_filesList->GetSelectedScene();

		if ( l_scene )
		{
			File * l_file = l_scene->GetFile( m_filesList->GetSelected() );

			if ( l_file )
			{
				FolderList l_list( m_currentProject->GetPath() + l_file->m_scene->GetName() + wxFileName::GetPathSeparator() + l_file->FileName, this );
				l_list.ShowModal();
			}
		}
	}

	void MainFrame::CloseElypse()
	{
		if ( m_elypseCtrl != NULL && m_elypseCtrl->GetTrollInstance()->IsSceneCreated() )
		{
			m_elypseCtrl->Close();
			m_elypseContainer->Destroy();

			if ( ! m_isFrame )
			{
				m_mainTabsContainer->DeletePage( 0 );
			}

			//m_elypseCtrl->Destroy();
			m_elypseCtrl = NULL;
			m_elypseContainer = NULL;
			m_viewedObject = NULL;

			//m_currentProject->GetMainScene()->FlushObjects();

			if ( m_objectsListContainer != NULL )
			{
				m_treeTabsContainer->DeletePage( 2 );
				m_objectsListContainer = NULL;
				m_objectsList = NULL;
				m_objectInfosContainer = NULL;
			}
		}

#if defined( _WIN32 )

		if ( m_currentProject && m_currentProject->GetShowDebug() )
		{
			FreeConsole();
		}

#endif
	}

	void MainFrame::CreateFunctionsGroup( const wxString & p_name )
	{
		if ( m_functionsList )
		{
			m_functionsList->CreateTitreFunction( p_name );
		}
	}

	void MainFrame::CreateFreeFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
	{
		if ( m_functionsList )
		{
			m_functionsList->CreaFunc( p_funcName, p_returnValue, p_params );
		}
	}

	void MainFrame::CreateClassFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
	{
		if ( m_functionsList )
		{
			m_functionsList->ClassFunc( p_funcName, p_returnValue, p_params );
		}
	}

	void MainFrame::CreateConstantsGroup( const wxString & p_name )
	{
		if ( m_functionsList )
		{
			m_functionsList->CreateTitreConstant( p_name );
		}
	}

	void MainFrame::CreateConstant( const wxString & p_constName, VariableBaseType p_type )
	{
		if ( m_functionsList )
		{
			m_functionsList->CreateConstant( p_constName, p_type );
		}
	}

	void MainFrame::ClosePropertyDialog()
	{
		if ( m_propertyDialog )
		{
			m_propertyDialog->Close();
			delete m_propertyDialog;
			m_propertyDialog = NULL;
		}
	}

	void MainFrame::UpdatePageLabel( wxString const & p_old, wxString const & p_new )
	{
		for ( size_t i = 0 ; i <= m_mainTabsContainer->GetPageCount() ; i++ )
		{
			if ( m_mainTabsContainer->GetPageText( i ) == p_old )
			{
				m_mainTabsContainer->SetPageText( i, p_new );
			}
		}
	}

	File * MainFrame::GetFile( wxString const & p_name )
	{
		return m_filesList->GetSelectedScene()->GetFile( m_filesList->GetItemByName( p_name ) );
	}

	void MainFrame::SelectFileTab( int p_tab )
	{
		m_mainTabsContainer->SetFocus();
		m_mainTabsContainer->SetSelection( p_tab );
	}

	void MainFrame::AddOverlay( TrollOverlay * p_overlay, EMOverlay * p_parent )
	{
		GetCurrentProject()->GetMainScene()->AddOverlay( p_overlay );
		m_objectsList->AddOverlay( p_overlay, ( p_parent ? make_wxString( p_parent->GetName() ) : wxT( "" ) ) );
	}

	void MainFrame::IncrementTimePanel( int p_diff )
	{
		m_timePanel->IncrementFirst( 0.2f * p_diff );
		m_timePanel->IncrementTickPosition( ( 0 - p_diff ) * 20 );
	}

	void MainFrame::UpdateSequence( int p_width, int p_height )
	{
		float l_first = m_timePanel->GetFirst();
		m_timeLineContainer->Scroll( 0, 0 );
		SetSize( p_width + 20, p_height );
		int l_verticalScroll = m_timeLineContainer->GetScrollPos( wxVERTICAL );
		//std::cout << "TimeLinePanel::UpdateSequence - " << l_verticalScroll << "\n";
		m_timeLineContainer->SetScrollbars( 20, 20, ( p_width + 20 ) / 20, p_height / 20, 0, 0 );
		m_timeLineContainer->Scroll( int( l_first >= 0.1 ? l_first * 5 + 1 : l_first * 5 ), l_verticalScroll );
	}
}
END_TROLL_GUI_NAMESPACE
