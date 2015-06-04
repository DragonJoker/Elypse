#include "PrecompiledHeader.h"

#include "MainFrame.h"

#include "StcTextEditor.hpp"
#include "ElypseCtrl.h"
#include "ElypseFrame.h"
#include "ElypsePanel.h"
#include "main.h"
#include "NewProjectFrame.h"
#include "NewSceneFrame.h"
#include "SceneDependenciesFrame.h"
#include "FunctionsTree.h"
#include "FilesTree.h"
#include "ObjectsTree.h"
#include "TextPanel.h"
#include "LogCtrl.h"
#include "FolderList.h"
#include "AboutFrame.h"
#include "ReplaceDialog.h"
#include "GUI/ObjectProperties/ProjectProperties.h"
#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/ContinuousEvent.h"
#include "Project/Temporal/Sequence.h"
#include "Main/TrollInstance.h"
#include "Script/TrollScriptCompiler.h"
#include "SceneFile/TrollSceneFileParser.h"
#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"
#include "GUI/TimeSequence/LinePanel.h"
#include "GUI/TimeSequence/SequencePanel.h"
#include "GUI/LanguageParser.hpp"
#include "GUI/StcContext.hpp"
#include "TrollLogs.h"

#include "xpm/troll_editor.xpm"
#include "xpm/BMPNew.xpm"
#include "xpm/BMPOpen.xpm"
#include "xpm/BMPSave.xpm"

#include <zip.h>

#include <File.h>
#include <StringUtils.h>

using namespace Troll::Temporal;

namespace Troll
{
	namespace GUI
	{
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

			void WriteBackgroundInMain( Project * p_project, wxTextOutputStream & p_stream )
			{
				if ( p_project->GetBackgroundType() == bgColour )
				{
					wxColour * l_colour = p_project->GetBackgroundColour();
					wxString l_line;
					l_line.Printf( wxT( "background_colour %f %f %f %f\n\n" ),
								   int( l_colour->Red() ) / 255.0,
								   int( l_colour->Green() ) / 255.0,
								   int( l_colour->Blue() ) / 255.0,
								   int( l_colour->Alpha() ) / 255.0 );
					l_line.Replace( wxT( "," ), wxT( "." ), true );
					p_stream.WriteString( l_line );
				}
				else
				{
					p_stream.WriteString( wxT( "background_colour 0.0 0.0 0.0 0.5 \n\n" ) );
				}
			}

			void WriteFilesInMain( TrollFileArray const & p_files, wxString const & p_name, wxString const & p_extension, wxTextOutputStream & p_stream )
			{
				for ( auto && l_file : p_files )
				{
					size_t l_index = min( l_file->FileName.find_last_of( '\\' ) , l_file->FileName.find_last_of( '/' ) );
					wxString l_fileName = l_file->FileName.substr( l_index + 1, l_file->FileName.size() );
					p_stream.WriteString( wxT( "\t" ) + p_name + wxT( " " ) + l_fileName + p_extension + wxT( "\n" ) );
				}
			}

			void WriteSceneInMain( wxString const & p_projectName, TrollScene * p_scene, bool p_writeScripts, bool p_writeMuseFiles, wxTextOutputStream & p_stream )
			{
				wxString l_sceneName = p_scene->GetName();
				p_stream.WriteString( wxT( "scene " ) + l_sceneName + wxT( "\n{\n" ) );

				if ( p_writeMuseFiles )
				{
					if ( p_scene->IsMainScene() )
					{
						p_stream.WriteString( wxT( "\tmuse_file " ) + p_projectName + wxT( ".muse\n" ) );
					}
					else
					{
						p_stream.WriteString( wxT( "\tmuse_file " ) + p_projectName + wxT( "_" ) + p_scene->GetName() + wxT( ".muse\n" ) );
					}
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
					wxLogError( wxT( "%s does not exist!\r\nCan not copy directory" ), sFrom.c_str() );
					return false;
				}

				if ( !wxDirExists( sTo ) )
				{
					if ( !wxFileName::Mkdir( sTo, 0777, wxPATH_MKDIR_FULL ) )
					{
						wxLogError( wxT( "%s could not be created!" ), sTo.c_str() );
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
							wxLogError( wxT( "Could not copy %s to %s !" ), sFileFrom.c_str(), sFileTo.c_str() );
							return false;
						}
					}

					bIsFile = fDir.GetNext( &sNext );
				}

				return true;
			}
		}

		MainFrame * g_mainFrame;

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
			EVT_MENU( CompileFile, MainFrame::OnCompile )
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
			EVT_SPLITTER_SASH_POS_CHANGED( HorizSplitter, MainFrame::OnVerticalSplitterPositionChanged )
			EVT_SPLITTER_SASH_POS_CHANGING( HorizSplitter, MainFrame::OnVerticalSplitterPositionChanging )
			EVT_SPLITTER_DCLICK( HorizSplitter, MainFrame::OnVerticalSplitterDClick )
			EVT_SPLITTER_UNSPLIT( HorizSplitter, MainFrame::OnVerticalSplitterUnsplitEvent )
			EVT_SPLITTER_SASH_POS_CHANGED( VerticSplitter, MainFrame::OnHorizontalSplitterPositionChanged )
			EVT_SPLITTER_SASH_POS_CHANGING( VerticSplitter, MainFrame::OnHorizontalSplitterPositionChanging )
			EVT_SPLITTER_DCLICK( VerticSplitter, MainFrame::OnHorizontalSplitterDClick )
			EVT_SPLITTER_UNSPLIT( VerticSplitter, MainFrame::OnHorizontalSplitterUnsplitEvent )
		END_EVENT_TABLE()

		static const long c_toolbarStyle = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

		MainFrame::MainFrame( const wxString & title, int x, int y, int w, int h )
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
			, m_mainPanel( NULL )
			, m_treeTabsContainer( NULL )
			, m_filesListContainer( NULL )
			, m_filesList( NULL )
			, m_functionsListContainer( NULL )
			, m_functionsList( NULL )
			, m_objectsListContainer( NULL )
			, m_objectsList( NULL )
			, m_mainTabsContainer( NULL )
			, m_editText( NULL )
			, m_elypseContainer( NULL )
			, m_debugLogContainer( NULL )
			, m_timelineEditor( NULL )
			, m_debugLog( NULL )
			, m_compilationLogContainer( NULL )
			, m_compilationLog( NULL )
			, m_propertyDialog( NULL )
			, m_treesWidth( 265 )
			, m_logsHeight( 200 )
			, m_splitterV( NULL )
			, m_splitterH( NULL )
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
		{
			g_mainFrame = this;
			//::AllocConsole();
			//freopen ("CONOUT$", "w", stdout );
			//freopen ("CONERR", "w", stderr );
			wxStandardPathsBase & l_paths = wxStandardPaths::Get();
			Path l_installPath = l_paths.GetExecutablePath().char_str().data();
			Path l_userPath = l_paths.GetUserDataDir().char_str().data();
			l_installPath = l_installPath.GetPath().GetPath() / "share" / "TrollEditor";
			m_appPath = wxString( l_userPath.c_str(), wxConvLibc );
			wxCopyDir( wxString( l_installPath.c_str(), wxConvLibc ), m_appPath );

			//std::cout << "MainFrame::MainFrame - " << m_appPath << "\n";
			LanguageFileParser l_parser( m_pStcContext );
			l_parser.ParseFile( m_appPath + wxFileName::GetPathSeparator() + wxT( "EMSCRIPT.lang" ) );
			l_parser.ParseFile( m_appPath + wxFileName::GetPathSeparator() + wxT( "EMSCENE.lang" ) );
			DoInitialise();
		}

		void MainFrame::DoInitialise()
		{
			SetIcon( wxIcon( troll_editor_xpm ) );
			SetBackgroundColour( wxColour( 255, 255, 255 ) );
			CreateStatusBar( 2 );
			SetStatusText( wxT( "Welcome to Troll Editor!" ) );
			DoSetMenuBar();
			PopulateToolbar( CreateToolBar( c_toolbarStyle, Toolbar ) );
			// main Panel (which contains all panels)
			m_mainPanel = new wxPanel( this );
			m_mainPanel->SetSize( GetClientSize() );
			m_sceneDependencies = new SceneDependenciesFrame( this );
			m_splitterH = new wxSplitterWindow( m_mainPanel, HorizSplitter,
												wxDefaultPosition, GetClientSize(),
												wxBORDER_NONE | wxSP_NOBORDER | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN, wxT( "Horizontal Splitter" ) );
			m_splitterH->SetMinimumPaneSize( 24 );
			m_splitterV = new wxSplitterWindow( m_splitterH, VerticSplitter,
												wxDefaultPosition, GetClientSize(),
												wxBORDER_NONE | wxSP_NOBORDER | wxSP_LIVE_UPDATE | wxCLIP_CHILDREN, wxT( "Vertical Splitter" ) );
			m_splitterV->SetMinimumPaneSize( 170 );
			// listbox for code completion
			m_listBox = new AutoCompletionListBox( m_mainPanel, ListBox, wxDefaultPosition, wxDefaultSize, m_choices );
			m_listBox->Hide();
			// main tabs (contains text editor, test panel...)
			m_mainTabsContainer = new wxAuiNotebook( m_splitterV, MainTabContainer, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE | wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_CLOSE_ON_ALL_TABS );
			//trees tabs (contains files list, objects list...)
			m_treeTabsContainer = new wxNotebook( m_splitterV, TreesTabContainer );
			//files list
			m_filesListContainer = new wxPanel( m_treeTabsContainer, FilesList, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
			m_treeTabsContainer->InsertPage( 0, m_filesListContainer, wxT( "Fichiers" ), true );
			m_filesList = new FilesTree( m_filesListContainer, wxDefaultPosition, m_filesListContainer->GetClientSize(),
										 wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_filesList->InitProjet( wxT( "Projet" ) );
			//functions list
			m_functionsListContainer = new wxPanel( m_treeTabsContainer, FunctionsList, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
			m_treeTabsContainer->InsertPage( 1, m_functionsListContainer, wxT( "Fonctions" ), false );
			m_functionsList = new FunctionsTree( m_functionsListContainer, wxDefaultPosition, m_functionsListContainer->GetClientSize(),
												 wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_functionsList->AddFunctionsToTree();
			TrollScriptCompiler l_compiler( "", true );
			l_compiler.Initialise( NULL );
			//logs tabs (contains debug log, compilation log...)
			m_logTabsContainer = new wxNotebook( m_splitterH, LogsTabContainer );
			// debug log
			m_debugLogContainer = new wxPanel( m_logTabsContainer, DebugLogs, wxPoint( 0, 0 ), m_logTabsContainer->GetClientSize() );
			m_logTabsContainer->InsertPage( 0, m_debugLogContainer, wxT( "Debug" ), true );
			m_debugLog = new wxTextCtrl( m_debugLogContainer, wxID_ANY, wxT( "" ), wxDefaultPosition, m_debugLogContainer->GetClientSize(),
										 wxTE_MULTILINE | wxSUNKEN_BORDER | wxTE_READONLY | wxTE_RICH );
			// compilation log
			m_compilationLogContainer = new wxPanel( m_logTabsContainer, CompilationLogs, wxPoint( 0, 0 ), m_logTabsContainer->GetClientSize() );
			m_logTabsContainer->InsertPage( 1, m_compilationLogContainer, wxT( "Compilation" ), false );
			m_compilationLog = new LogCtrl( m_compilationLogContainer, wxID_ANY, wxT( "" ), wxDefaultPosition, m_compilationLogContainer->GetClientSize(),
											wxTE_MULTILINE | wxSUNKEN_BORDER | wxTE_READONLY | wxTE_RICH );
			DoCreateTreeWithDefStyle();
			//m_menuBar->Check( TreeTest_ToggleImages, false);
			m_logDebug = new wxLogTextCtrl( m_debugLog );
			m_logCompilation = new wxLogTextCtrl( m_compilationLog );
			m_splitterV->SplitVertically( m_treeTabsContainer, m_mainTabsContainer, m_treesWidth );
			m_splitterH->SplitHorizontally( m_splitterV, m_logTabsContainer, 0 - m_logsHeight );
			DoResize();
		}

		MainFrame::~MainFrame()
		{
			delete m_pStcContext;
		}

		void MainFrame::DoSetMenuBar()
		{
			m_menuBar =	new wxMenuBar( wxMB_DOCKABLE );
			m_menuFile = new wxMenu;
			m_menuHelp = new wxMenu;
			m_menuProject = new wxMenu;
			m_menuEdition = new wxMenu;
			m_menuFile->Append( IdNew, wxT( "&Nouveau Projet" ) );
			m_menuFile->Append( OpenProject, wxT( "&Ouvrir Projet" ) );
			m_menuFile->Append( OpenFil, wxT( "O&uvrir Fichier" ) );
			m_menuFile->Append( IdCloseProject, wxT( "&Fermer projet" ) );
			m_menuFile->AppendSeparator();			/******************************************/
			m_menuFile->Append( saveProject, wxT( "&Enregistrer Projet\tCtrl+T" ) );
			m_menuFile->Append( IdOnSaveFile, wxT( "En&registrer Fichier\tCtrl+S" ) );
			m_menuFile->Append( IdOnSaveFileAs, wxT( "Enregistrer Fichier &Sous...Ctrl+Shift+S" ) );
			m_menuFile->AppendSeparator();			/******************************************/
			m_menuFile->Append( Quit, wxT( "&Quitter" ) );
			m_menuEdition->Append( IdGoToLine, wxT( "&Aller à...\tCtrl+G" ) );
			m_menuEdition->Append( IdSearchNext, wxT( "Chercher &suivant...\tF3" ) );
			m_menuEdition->Append( IdSearchPrevious, wxT( "Chercher &précédent...\tShift+F3" ) );
			m_menuEdition->Append( IdReplace, wxT( "&Remplacer...\tCtrl+H" ) );
			m_menuProject->Append( EditSceneDependencies, wxT( "&Editer Dépendances\tF5" ) );
			m_menuProject->Append( CompileFile, wxT( "&Compiler\tF6" ) );
			m_menuProject->Append( IdMusiner, wxT( "&Musiner\tF7" ) );
			m_menuEditTimeLines = m_menuProject->Append( EditTimeLine, wxT( "Editer T&imeline\tF8" ) );
			m_menuEditProject = m_menuProject->Append( IdEditerScene, wxT( "Editer &Scène\tF9" ) );
			m_menuEditOverlays = m_menuProject->Append( IdEditerOverlays, wxT( "Editer &Overlays\tF10" ) );
			m_menuTestProject = m_menuProject->Append( IdTester, wxT( "&Tester Projet\tF11" ) );
			m_menuProject->Append( IdProjectProperties, wxT( "&Propriétés\tCtrl+P" ) );
			m_menuHelp->Append( About, wxT( "&A Propos..." ) );
			m_menuBar->Append( m_menuFile, wxT( "&Fichier" ) );
			m_menuBar->Append( m_menuEdition, wxT( "&Edition" ) );
			m_menuBar->Append( m_menuProject, wxT( "&Projet" ) );
			m_menuBar->Append( m_menuHelp, wxT( "&Aide" ) );
			SetMenuBar( m_menuBar );
			//// Associate the menu bar with the frame
			//m_menuBar->Check( M_TOOLBAR_SHOW_BOTH, true);
			//m_menuBar->Check( M_TOOLBAR_TOGGLETOOLTIPS, true);
			//m_menuBar->Check( M_TOOLBAR_TOP_ORIENTATION, true);
		}

		void MainFrame::DoCreateTreeWithDefStyle()
		{
			long style = wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS;
			wxMenuBar * mbar = GetMenuBar();
			//mbar->Check( TreeTest_TogButtons, (style & wxTR_HAS_BUTTONS)			!= 0);
			//mbar->Check( TreeTest_TogButtons, (style & wxTR_TWIST_BUTTONS)		!= 0);
			//mbar->Check( TreeTest_TogLines, (style & wxTR_NO_LINES)				== 0);
			//mbar->Check( TreeTest_TogRootLines, (style & wxTR_LINES_AT_ROOT)		!= 0);
			//mbar->Check( TreeTest_TogHideRoot, (style & wxTR_HIDE_ROOT)			!= 0);
			//mbar->Check( TreeTest_TogEdit, (style & wxTR_EDIT_LABELS)			!= 0);
			//mbar->Check( TreeTest_TogBorder, (style & wxTR_ROW_LINES)			!= 0);
			//mbar->Check( TreeTest_TogFullHighlight, (style & wxTR_FULL_ROW_HIGHLIGHT)	!= 0);
		}

		void MainFrame::DoResizeTrees()
		{
			wxSize l_size = m_splitterV->GetWindow1()->GetClientSize();

			if ( m_treeTabsContainer != NULL )
			{
				m_treeTabsContainer->SetSize( 0, 0, l_size.x, l_size.y - 2 );
				wxSize l_tabSize = m_treeTabsContainer->GetClientSize();
				wxSize l_contSize;

				if ( m_filesListContainer != NULL )
				{
					m_filesListContainer->SetSize( 0, 22, l_tabSize.x - 2, l_tabSize.y - 22 );
					l_contSize = m_filesListContainer->GetClientSize();

					if ( m_filesList != NULL )
					{
						m_filesList->SetSize( 0, 0, l_contSize.x, l_contSize.y );
					}
				}

				if ( m_functionsListContainer != NULL )
				{
					m_functionsListContainer->SetSize( 0, 22, l_tabSize.x - 2, l_tabSize.y - 22 );
					l_contSize = m_functionsListContainer->GetClientSize();

					if ( m_functionsList != NULL )
					{
						m_functionsList->SetSize( 0, 0, l_contSize.x, l_contSize.y );
					}
				}

				if ( m_objectsListContainer != NULL )
				{
					m_objectsListContainer->SetSize( 0, 22, l_tabSize.x - 2, l_tabSize.y - 22 );
					l_contSize = m_objectsListContainer->GetClientSize();

					if ( m_objectsList != NULL )
					{
						m_objectsList->SetSize( 0, 0, l_contSize.x, l_contSize.y / 2 );
					}

					if ( m_objectInfosContainer != NULL )
					{
						m_objectInfosContainer->SetSize( 0, l_contSize.y / 2, l_contSize.x, l_contSize.y / 2 );

						if ( m_viewedObject != NULL )
						{
							m_objectInfosContainer->SetScrollbars( 20, 20, 0, m_viewedObject->GetPanel()->GetSize().y / 20, 0, 0 );
						}
						else
						{
							m_objectInfosContainer->SetScrollbars( 20, 20, 0, 0, 0, 0 );
						}
					}
				}
			}
		}

		void MainFrame::DoResizePanels()
		{
			if ( m_mainTabsContainer == NULL )
			{
				return;
			}

			wxSize l_size = m_splitterV->GetWindow2()->GetClientSize();
			m_mainTabsContainer->SetSize( m_splitterV->GetWindow2()->GetPosition().x, 0, l_size.x, l_size.y );
			wxSize l_tabSize = m_mainTabsContainer->GetClientSize();

			if ( m_mainTabsContainer->GetPageCount() > 0 )
			{
				wxWindow * l_page;

				for ( unsigned int i = 0 ; i < m_mainTabsContainer->GetPageCount() ; i++ )
				{
					l_page = m_mainTabsContainer->GetPage( i );

					if ( l_page == m_elypseContainer )
					{
						m_elypseContainer->SetSize( 0, 22, l_tabSize.x, l_tabSize.y - 22 );
						wxSize l_size = m_elypseContainer->GetClientSize();
						wxPoint l_pos( 0, 0 );

						if ( m_editingTimeLines )
						{
							l_size.y -= 120;
						}

						if ( l_size.x > 4 * l_size.y / 3 )
						{
							int l_width = l_size.x;
							l_size.x = 4 * l_size.y / 3;
							l_pos.x = ( l_width - l_size.x ) / 2;
						}
						else if ( l_size.y > 3 * l_size.x / 4 )
						{
							int l_height = l_size.y;
							l_size.y = 3 * l_size.x / 4;
							l_pos.y = ( l_height - l_size.y ) / 2;
						}

						if ( m_editingTimeLines )
						{
							l_pos.x = 0;
							l_pos.y = 0;
							m_timePanel->SetSize( 0, m_elypseContainer->GetClientSize().y - 120, m_elypseContainer->GetClientSize().x, 20 );
							m_timeLineContainer->SetSize( 0, m_elypseContainer->GetClientSize().y - 100, m_elypseContainer->GetClientSize().x, 100 );
							//m_timelineEditor->SetSize( 0, 0, m_timeLineContainer->GetClientSize().x, m_timeLineContainer->GetClientSize().y);
							//m_timeLineInfos->SetSize( l_size.x, 0, m_elypseContainer->GetClientSize().x - l_size.x, l_size.y);
							m_timeLineContainer->SetScrollbars( 20, 20, m_timelineEditor->GetSize().x / 20, m_timelineEditor->GetSize().y / 20, 0, 0 );
						}

						//m_elypseContainer->SetSize( l_pos.x, l_pos.y, l_size.x, l_size.y);
					}
					else
					{
						l_page->SetSize( 0, 22, l_tabSize.x, l_tabSize.y - 22 );
						( * l_page->GetChildren().begin() )->SetSize( 0, 0, l_page->GetClientSize().x, l_page->GetClientSize().y );
					}
				}
			}
		}

		void MainFrame::DoResizeLogs()
		{
			wxSize l_size = m_splitterH->GetWindow2()->GetClientSize();

			if ( m_logTabsContainer != NULL )
			{
				m_logTabsContainer->SetSize( 0, 0, l_size.x, l_size.y - 2 );
				wxSize l_tabSize = m_logTabsContainer->GetClientSize();

				if ( m_debugLogContainer != NULL )
				{
					m_debugLogContainer->SetSize( 0, 22, l_tabSize.x, l_tabSize.y - 22 );

					if ( m_debugLog != NULL )
					{
						m_debugLog->SetSize( 0, 0, m_debugLogContainer->GetClientSize().x, m_debugLogContainer->GetClientSize().y );
					}
				}

				if ( m_compilationLogContainer != NULL )
				{
					m_compilationLogContainer->SetSize( 0, 22, l_tabSize.x, l_tabSize.y - 22 );

					if ( m_compilationLog != NULL )
					{
						m_compilationLog->SetSize( 0, 0, m_compilationLogContainer->GetClientSize().x, m_compilationLogContainer->GetClientSize().y );
					}
				}
			}
		}

		void MainFrame::DoResize()
		{
			m_resizing = true;

			if ( m_splitterV != NULL && m_splitterH != NULL )
			{
				wxSize l_size = GetClientSize();
				//		std::cout << "MainFrame::DoResize - Width : " << l_size.x << " - Height : " << l_size.y << "\n";
				m_splitterH->SetSize( l_size );
				m_splitterH->SetSashPosition( m_splitterH->GetClientSize().y - m_logsHeight );
				m_splitterV->SetSize( l_size.x, l_size.y - m_logsHeight );
				m_splitterV->SetSashPosition( m_treesWidth );
				DoResizeLogs();
				DoResizeTrees();
				DoResizePanels();
			}

			m_resizing = false;
		}

		void MainFrame::DoAddSceneToMuse( Project * p_project, bool p_createMain, bool p_withScripts, TrollScene * p_scene, wxString const & p_mainFile )
		{
			wxString l_projectPath = p_project->GetPath();
			wxString l_scenePath = l_projectPath;

			if ( p_scene != p_project->GetMainScene() )
			{
				l_scenePath += p_scene->GetName() + wxFileName::GetPathSeparator();
			}

			DataWriter l_dataWriter( m_compilationLog );

			for ( auto && l_folder : p_scene->m_dataFolders )
			{
				LogDebugMessage( wxT( "MainFrame::Musiner - Data Folder - " ) + l_scenePath + l_folder->FileName );
				l_dataWriter.AddFolder( l_scenePath + l_folder->FileName, EM_BLOCK_ZIPSNDDATA );
			}

			for ( auto && l_file : p_scene->m_sceneFiles )
			{
				LogDebugMessage( wxT( "MainFrame::Musiner - Scene File - " ) + l_scenePath + l_file->FileName );
				l_dataWriter.AddFile( l_scenePath + l_file->FileName );
			}

			if ( p_withScripts )
			{
				for ( auto && l_file : p_scene->m_loadScriptFiles )
				{
					LogDebugMessage( wxT( "MainFrame::Musiner - Load Script File - " ) + l_scenePath + l_file->FileName );
					l_dataWriter.AddFile( l_scenePath + l_file->FileName );
				}


				for ( auto && l_file : p_scene->m_unloadScriptFiles )
				{
					LogDebugMessage( wxT( "MainFrame::Musiner - Unload Script File - " ) + l_scenePath + l_file->FileName );
					l_dataWriter.AddFile( l_scenePath + l_file->FileName );
				}
			}

			for ( auto && l_folder : p_scene->m_dataFiles )
			{
				LogDebugMessage( wxT( "MainFrame::Musiner - Data file - " ) + l_scenePath + l_folder->FileName );
				ZipFolder( l_scenePath + l_folder->FileName, l_scenePath + l_folder->FileName + wxT( ".zip" ) );
				l_dataWriter.AddFile( l_scenePath + l_folder->FileName + wxT( ".zip" ) );
			}

			if ( p_scene == p_project->GetMainScene() )
			{
				if ( p_createMain )
				{
					l_dataWriter.AddFile( p_mainFile );
				}

				l_dataWriter.Write( l_projectPath + p_project->GetName() + wxT( ".muse" ) );
			}
			else
			{
				l_dataWriter.Write( l_projectPath + p_project->GetName() + wxT( "_" ) + p_scene->GetName() + wxT( ".muse" ) );
			}
		}

		void MainFrame::DoMusinate( Project * p_project, bool p_createMain, bool p_withScripts )
		{
			if ( p_project && p_project->GetMainScene() )
			{
				wxString l_mainFile;

				if ( p_createMain )
				{
					l_mainFile = CreateMain( p_project, p_withScripts );
				}

				DoAddSceneToMuse( p_project, p_createMain, p_withScripts, p_project->GetMainScene(), l_mainFile );

				for ( auto && l_scene : p_project->GetScenes() )
				{
					DoAddSceneToMuse( p_project, p_createMain, p_withScripts, l_scene.second, l_mainFile );
				}
			}
		}

		void MainFrame::DoMusinateSceneEditor( Project * p_project )
		{
			if ( p_project && p_project->GetMainScene() )
			{
				wxString l_mainFile = CreateSceneEditorMain( p_project );
				DoAddSceneToMuse( p_project, true, true, p_project->GetMainScene(), l_mainFile );
			}
		}

		void MainFrame::DoMusinateOverlayEditor( Project * p_project )
		{
			if ( p_project && p_project->GetMainScene() )
			{
				TrollScene * l_scene = p_project->GetMainScene();
				wxString l_mainFile = CreateOverlayEditorMain( p_project );
				wxString l_path = p_project->GetPath();
				DataWriter l_dataWriter( m_compilationLog );

				for ( auto && l_file : l_scene->m_sceneFiles )
				{
					LogDebugMessage( wxT( "MainFrame::Musiner - TrollScene File - " ) + l_path + l_file->FileName );
					l_dataWriter.AddFile( l_path + l_file->FileName );
				}

				for ( auto && l_file : l_scene->m_loadScriptFiles )
				{
					LogDebugMessage( wxT( "MainFrame::Musiner - Load Script File - " ) + l_path + l_file->FileName );
					l_dataWriter.AddFile( l_path + l_file->FileName );
				}

				l_dataWriter.AddFile( l_mainFile );
				l_dataWriter.Write( l_path + p_project->GetName() + wxT( ".muse" ) );
			}
		}

		void MainFrame::OnOpenSelectedFile( wxCommandEvent & p_event )
		{
			OpenSelectedFile();
		}

		void MainFrame::OnOpenFile( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( wxT( "Choisissez un fichier à ouvrir" ), wxEmptyString, wxEmptyString, wxEmptyString,
												  wxT( "Fichiers de TrollScene (*.emscene)|*.emscene|Fichiers de Script (*.emscript)|*.emscript" ) );

			if ( l_fileName.empty() )
			{
				return;
			}
		}

		void MainFrame::OnOpenProject( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( wxT( "Choisissez un projet à ouvrir" ), wxEmptyString, wxEmptyString, wxEmptyString,
												  wxT( "Projets Troll Editor (*.teproj)|*.teproj" ) );
			OpenProjectFile( l_fileName );
		}

		void MainFrame::OnWriteSceneFiles( wxCommandEvent & p_event )
		{
			if ( m_currentProject != NULL )
			{
				m_currentProject->Write();
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
			SaveSceneFiles( m_currentProject->GetMainScene() );
			SceneMap l_scenes = m_currentProject->GetScenes();
			SceneMap::iterator l_it = l_scenes.begin();

			for ( ; l_it != l_scenes.end() ; ++l_it )
			{
				SaveSceneFiles( l_it->second );
			}
		}

		void MainFrame::OnQuit( wxCommandEvent & p_event )
		{
			delete m_logDebug;
			m_logDebug = NULL;
			delete m_logCompilation;
			m_logCompilation = NULL;
			//	Close( true);
			p_event.Skip();
		}

		void MainFrame::OnClose( wxCloseEvent & p_event )
		{
			wxLog::SetActiveTarget( NULL );
			delete m_logDebug;
			m_logDebug = NULL;
			delete m_logCompilation;
			m_logCompilation = NULL;
			m_listBox = NULL;
			m_mainPanel = NULL;
			m_treeTabsContainer = NULL;
			m_filesListContainer = NULL;
			m_filesList = NULL;
			m_functionsListContainer = NULL;
			m_functionsList = NULL;
			m_objectsListContainer = NULL;
			m_objectsList = NULL;
			m_mainTabsContainer = NULL;
			m_editText = NULL;
			m_elypseContainer = NULL;
			m_elypseContainer = NULL;
			m_logTabsContainer = NULL;
			m_debugLogContainer = NULL;
			m_timelineEditor = NULL;
			m_debugLog = NULL;
			m_compilationLogContainer = NULL;
			m_compilationLog = NULL;
			m_propertyDialog = NULL;
			m_splitterV = NULL;
			m_splitterH = NULL;
			m_timePanel = NULL;
			m_objectInfos = NULL;

			if ( m_currentProject != NULL )
			{
				CloseProject( true );
			}

			p_event.Skip();
		}

		void MainFrame::OnCompile( wxCommandEvent & p_event )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			//if ( ! m_filesList->GetSelectedScene())
			//{
			//	LogDebugMessage( "Aucune scène sélectionnée");
			//	return;
			//}

			std::cout << m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ) << "\n";
			TrollScene * l_scene;
			TrollFile * l_file;

			if ( !m_currentProject->FindFileInScenes( m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ), l_file, l_scene ) )
			{
				LogDebugMessage( wxT( "Aucun fichier sélectionné" ) );
				return;
			}

			if ( ! l_file->FileName.Contains( wxT( ".emscript" ) ) )
			{
				LogDebugMessage( wxT( "Impossible de compiler ce fichier" ) );
				return;
			}

			try
			{
				Compile( l_file, l_scene );
			}
			catch ( ... )
			{
				LogDebugMessage( wxT( "Erreur fatale rencontrée lors de la compilation de ce fichier" ) );
			}
		}

		void MainFrame::OnAbout( wxCommandEvent & p_event )
		{
			AboutFrame * l_frame = new AboutFrame( this );
			l_frame->Show();
		}

		void MainFrame::OnAdd( wxCommandEvent & p_event )
		{
			if ( ! m_currentProject )
			{
				return;
			}

			wxTreeItemId l_folderId = m_filesList->GetSelected();
			wxString l_str;
			FileType l_fileType = m_filesList->GetSelectedScene()->GetType( l_folderId );
			TETreeItemType l_titype;
			TrollScene * l_scene = m_filesList->GetSelectedScene();

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
			//std::cout<< l_str << std::endl;
		}

		void MainFrame::OnRename( wxCommandEvent & p_event )
		{
			m_filesList->EditLabel( m_filesList->GetSelected() );
		}

		void MainFrame::OnAddExistingFile( wxCommandEvent & p_event )
		{
			if ( m_currentProject == NULL )
			{
				return;
			}

			wxTreeItemId l_folderId = m_filesList->GetSelected();
			wxString l_str;
			FileType l_fileType = m_filesList->GetSelectedScene()->GetType( l_folderId );
			TETreeItemType l_titype;
			wxString filename;

			if ( l_fileType == dataFolder )
			{
				l_titype = TITDataFolder;
				wxDirDialog l_dialog( NULL, wxT( "Sélectionnez un dossier" ), m_currentProject->GetPath() );

				if ( l_dialog.ShowModal() == wxID_OK )
				{
					filename = l_dialog.GetPath();
				}
			}

			if ( l_fileType == sceneFile )
			{
				l_titype = TITSceneFile;
				filename = wxFileSelector( wxT( "Choisissez un fichier à ouvrir" ), m_currentProject->GetPath(), wxEmptyString, wxEmptyString, wxT( "Fichiers de TrollScene (*.emscene)|*.emscene" ) );
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

				filename = wxFileSelector( wxT( "Choisissez un fichier à ouvrir" ), m_currentProject->GetPath(), wxEmptyString, wxEmptyString, wxT( "Fichiers de Script (*.emscript)|*.emscript" ) );
			}
			else if ( l_fileType == dataFile )
			{
				l_titype = TITDataFile;
				wxDirDialog l_dialog( NULL, wxT( "Sélectionnez un dossier" ), m_currentProject->GetPath() );

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

		void MainFrame::OnSaveFile( wxCommandEvent & p_event )
		{
			if ( m_editText != NULL )
			{
				int l_selectedPage = m_mainTabsContainer->GetSelection();
				wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );

				if ( l_fileName != wxT( "Edition de la Timeline" ) && l_fileName != wxT( "Edition de TrollScene" ) && l_fileName != wxT( "Edition des overlays" ) && l_fileName != wxT( "Test de TrollScene" ) )
				{
					m_editText->SaveFile();
					//TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
					////std::cout << "MainFrame::OnSaveFile - Selected Page : " << l_selectedPage << " - " << l_fileName << "\n";
					//SaveFile( l_editCtrl->GetFile(), l_fileName );
				}
			}
		}

		void MainFrame::OnSaveProject( wxCommandEvent & p_event )
		{
			SaveProject( m_currentProject );
		}

		void MainFrame::OnSaveSelectedFile( wxCommandEvent & p_event )
		{
			wxTreeItemId l_itemId = m_filesList->GetSelected();
			TrollScene * l_scene = m_filesList->GetSelectedScene();

			if ( l_scene == NULL )
			{
				return;
			}

			TrollFile * l_file = l_scene->GetFile( l_itemId );

			if ( l_file == NULL )
			{
				return;
			}

			if ( l_file->Saved )
			{
				//std::cout << "MainFrame::OnSaveSelectedFile" << l_file->FileName << "\n";
				l_file->EditPanel->SaveFile( m_currentProject->GetPath() + l_file->FileName );
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
			TrollScene * l_scene = m_filesList->GetSelectedScene();
			TrollFile * l_file = l_scene->GetFileByName( l_fileName );
			SaveFileAs( l_file );
		}

		void MainFrame::OnMusinate( wxCommandEvent & )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			DoMusinate( m_currentProject );
		}

		void MainFrame::OnRemoveFile( wxCommandEvent & )
		{
			wxTreeItemId l_itemId = m_filesList->GetSelected();
			TrollFile * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );
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
			if ( wxMessageBox( wxT( "Voulez vous vraiment supprimer ce fichier?" ), wxT( "Confirmation" ), wxYES_NO | wxICON_EXCLAMATION, this ) == wxYES )
			{
				wxTreeItemId l_itemId = m_filesList->GetSelected();
				TrollFile * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );
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

				//		std::cout << "MainFrame::OnDeleteFile - " << remove( m_currentProject->GetPath() + l_file->FileName) << "\n";
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
			if ( m_currentProject )
			{
				TrollScene * l_scene = new TrollScene( m_currentProject, wxT( "Nouvelle Scène" ) );
				wxString l_sceneName = l_scene->GetName();

				try
				{
					l_scene->m_sceneId				= m_filesList->AddSceneToProject( l_scene );
					l_scene->m_sceneFileId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de scène" ) );
					l_scene->m_loadScriptFileId		= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script de load" ) );
					l_scene->m_unloadScriptFileId	= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script d'unload" ) );
					l_scene->m_dataFileId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas Graphiques (pour la 3D)" ) );
					l_scene->m_dataFolderId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas non Graphiques (autres)" ) );
					m_currentProject->AddScene( l_scene );
				}
				catch ( bool )
				{
					LogDebugMessage( wxT( "MainFrame::OnNewScene - Probleme d'ajout" ) );
				}
			}
		}

		void MainFrame::OnRemoveScene( wxCommandEvent & p_event )
		{
			if ( ! m_currentProject )
			{
				return;
			}

			TrollScene * l_scene = m_filesList->GetSelectedScene();
			wxTreeItemId l_item = m_filesList->GetSelected();
			m_filesList->DeleteChildren( l_item );
			m_filesList->Delete( l_item );
			m_currentProject->RemoveScene( l_scene );
		}

		void MainFrame::OnEditProject( wxCommandEvent & p_event )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			CloseElypse();
			m_editingProject = ! m_editingProject;

			if ( m_editingProject )
			{
				DoInitEditLists();

				if ( m_editingTimeLines )
				{
					m_menuEditTimeLines->SetItemLabel( wxT( "Editer TimeLine\tF8" ) );
					m_editingTimeLines = false;
				}

				if ( m_editingOverlays )
				{
					m_menuEditOverlays->SetItemLabel( wxT( "Editer Overlays\tF10" ) );
					m_editingOverlays = false;
				}

				if ( m_testingProject )
				{
					m_menuTestProject->SetItemLabel( wxT( "Tester\tF11" ) );
					m_testingProject = false;
				}

				if ( m_currentProject == NULL )
				{
					return;
				}

				m_menuEditProject->SetItemLabel( wxT( "Stopper l'édition\tF9" ) );
				wxString l_fileName = m_appPath + wxT( "SceneEditor/Editeur.teproj" );
				wxFileInputStream * l_input = new wxFileInputStream( l_fileName );
				wxTextInputStream * l_textStream = new wxTextInputStream( * l_input );
				Project * l_project = new Project;
				l_project->Load( l_input, l_textStream, m_appPath + wxT( "SceneEditor/" ) );
				l_project->SetResolution( m_currentProject->GetResolution() );
				l_project->SetBackgroundImage( m_currentProject->GetBackgroundImage() );
				l_project->SetBackgroundColour( m_currentProject->GetBackgroundColour() );
				l_project->SetProjectFileName( l_fileName );
				l_project->SetSaved( true );
				delete l_textStream;
				delete l_input;
				DoMusinate( m_currentProject, false, false );
				DoMusinateSceneEditor( l_project );
				DoInitElypse( l_project, _T( "Edition de TrollScene" ), true );
				//		delete l_project;
			}
			else
			{
				m_menuEditProject->SetItemLabel( _T( "Editer Scène\tF9" ) );
			}
		}

		void MainFrame::OnEditOverlays( wxCommandEvent & p_event )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			CloseElypse();
			m_editingOverlays = ! m_editingOverlays;

			if ( m_editingOverlays )
			{
				DoInitEditLists();

				if ( m_editingTimeLines )
				{
					m_menuEditTimeLines->SetItemLabel( _T( "Editer TimeLine\tF8" ) );
					m_editingTimeLines = false;
				}

				if ( m_editingProject )
				{
					m_menuEditProject->SetItemLabel( _T( "Editer Scène\tF9" ) );
					m_editingProject = false;
				}

				if ( m_testingProject )
				{
					m_menuTestProject->SetItemLabel( _T( "Tester\tF11" ) );
					m_testingProject = false;
				}

				if ( m_currentProject == NULL )
				{
					return;
				}

				m_menuEditOverlays->SetItemLabel( _T( "Stopper l'édition\tF10" ) );
				wxString l_fileName = m_appPath + wxT( "OverlayEditor/Editeur.teproj" );
				wxFileInputStream * l_input = new wxFileInputStream( l_fileName );
				wxTextInputStream * l_textStream = new wxTextInputStream( * l_input );
				Project * l_project = new Project;
				l_project->Load( l_input, l_textStream, m_appPath + wxT( "OverlayEditor/" ) );
				l_project->SetResolution( m_currentProject->GetResolution() );
				l_project->SetBackgroundImage( m_currentProject->GetBackgroundImage() );
				l_project->SetBackgroundColour( m_currentProject->GetBackgroundColour() );
				l_project->SetProjectFileName( l_fileName );
				l_project->SetSaved( true );
				delete l_textStream;
				delete l_input;
				DoMusinate( m_currentProject, false, false );
				DoMusinateOverlayEditor( l_project );
				DoInitElypse( l_project, _T( "Edition des overlays" ), false );
				delete l_project;
			}
			else
			{
				m_menuEditOverlays->SetItemLabel( _T( "Editer Overlays\tF10" ) );
				m_currentProject->GetMainScene()->FlushObjects();
			}
		}

		void MainFrame::OnTestProject( wxCommandEvent & p_event )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			CloseElypse();
			m_testingProject = ! m_testingProject;

			if ( m_testingProject )
			{
				if ( m_editingTimeLines )
				{
					m_menuEditTimeLines->SetItemLabel( _T( "Editer TimeLine\tF8" ) );
					m_editingTimeLines = false;
				}

				if ( m_editingProject )
				{
					m_menuEditProject->SetItemLabel( _T( "Editer Scène\tF9" ) );
					m_editingProject = false;
				}

				if ( m_editingOverlays )
				{
					m_menuEditOverlays->SetItemLabel( _T( "Editer Overlays\tF10" ) );
					m_editingOverlays = false;
				}

				if ( ! m_currentProject )
				{
					return;
				}

				m_menuTestProject->SetItemLabel( _T( "Stopper le test\tF11" ) );
				DoMusinate( m_currentProject );
				DoInitElypse( m_currentProject, _T( "Test de " ) + m_currentProject->GetName(), false );
			}
			else
			{
				m_menuTestProject->SetItemLabel( _T( "Tester Projet\tF11" ) );
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

			DoResize();
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

			//	std::cout << "MainFrame::OnNotebook - " << m_currentPage << std::endl;

			if ( l_book->GetName() == wxT( "Test de TrollScene" ) )
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
						TrollFile * l_file = m_filesList->GetSelectedScene()->GetFileByName( m_currentPage );

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
			TrollFile * l_file = m_filesList->GetSelectedScene()->GetFile( m_filesList->GetSelected() );

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

		void MainFrame::OnEditTimeLine( wxCommandEvent & p_event )
		{
			//	std::cout << "MainFrame::OnEditTimeLine\n";
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			CloseElypse();
			m_editingTimeLines = ! m_editingTimeLines;

			if ( m_editingTimeLines )
			{
				DoInitEditLists();

				if ( m_editingProject )
				{
					m_menuEditProject->SetItemLabel( _T( "Editer Scène\tF9" ) );
					m_editingProject = false;
				}

				if ( m_editingOverlays )
				{
					m_menuEditOverlays->SetItemLabel( _T( "Editer Overlays\tF10" ) );
					m_editingOverlays = false;
				}

				if ( m_testingProject )
				{
					m_menuTestProject->SetItemLabel( _T( "Tester Projet\tF11" ) );
					m_testingProject = false;
				}

				if ( ! m_currentProject )
				{
					return;
				}

				m_menuEditTimeLines->SetItemLabel( _T( "Stopper Edition\tF8" ) );
				DoMusinate( m_currentProject, true );
				DoInitElypse( m_currentProject, _T( "Edition de la Timeline" ), false, -120 );
				m_timePanel = new TimePanel( m_elypseContainer, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 120 ), wxSize( m_elypseContainer->GetClientSize().x - 20, 20 ) );
				m_timeLineContainer = new TimeLineContainer( m_elypseContainer, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 100 ), wxSize( m_elypseContainer->GetClientSize().x, 100 ) );
				m_timeLineContainer->Show();
				m_timeLineContainer->SetScrollbars( 20, 20, 0, 0, 0, 0 );
				m_timelineEditor = new TimeLinePanel( m_timeLineContainer, IdTimeLinePanel, wxPoint( 0, 0 ), m_timeLineContainer->GetClientSize() );
				m_timelineEditor->Show();
				//		m_timeLineInfos = new wxPanel(m_elypseContainer, wxID_ANY, wxPoint( l_size.x, 0), wxSize( m_elypseContainer->GetClientSize().x - l_size.x, l_size.y), wxBORDER_SIMPLE);
				m_timeLineContainer->SetScrollbars( 20, 20, m_timelineEditor->GetSize().x / 20, m_timelineEditor->GetSize().y / 20, 0, 0 );
			}
			else
			{
				m_menuEditTimeLines->SetItemLabel( _T( "Editer la Timeline\tF8" ) );
				m_currentProject->GetMainScene()->FlushObjects();
			}
		}

		void MainFrame::OnVerticalSplitterPositionChanged( wxSplitterEvent & p_event )
		{
			if ( m_resizing )
			{
				p_event.Skip();
				return;
			}

			m_logsHeight = GetClientSize().y - p_event.GetSashPosition();
			DoResizeLogs();
			DoResizeTrees();
			DoResizePanels();
			p_event.Skip();
		}

		void MainFrame::OnVerticalSplitterPositionChanging( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame::OnVerticalSplitterDClick( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame::OnVerticalSplitterUnsplitEvent( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame::OnHorizontalSplitterPositionChanged( wxSplitterEvent & p_event )
		{
			if ( m_resizing )
			{
				p_event.Skip();
				return;
			}

			m_treesWidth = p_event.GetSashPosition();
			DoResizeTrees();
			DoResizePanels();
			p_event.Skip();
		}

		void MainFrame::OnHorizontalSplitterPositionChanging( wxSplitterEvent & p_event )
		{
			/*
				m_treesWidth = p_event.GetSashPosition();

				DoResizeTrees();
				DoResizePanels();
			/**/
			p_event.Skip();
		}

		void MainFrame::OnHorizontalSplitterDClick( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame::OnHorizontalSplitterUnsplitEvent( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}


		void MainFrame::OnListFolder( wxCommandEvent & p_event )
		{
			ListFolder();
		}

		void MainFrame::OnProjectProperties( wxCommandEvent & p_event )
		{
			ProjectProperties * l_properties = new ProjectProperties( m_currentProject, this );
			l_properties->Show();
		}

		void MainFrame::OnGoToLine( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer->GetPageCount() <= 0 )
			{
				return;
			}

			int l_page = m_mainTabsContainer->GetSelection();
			TextPanel * l_panel = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_page )->GetChildren().GetFirst()->GetData() );

			if ( l_panel == NULL || !l_panel->GetText() )
			{
				return;
			}

			int l_maxLines = l_panel->GetText()->GetNumberOfLines();
			wxString l_nbMaxLines;
			l_nbMaxLines << l_maxLines;
			wxTextEntryDialog * l_dialog = new wxTextEntryDialog( this, wxT( "Choisissez la ligne (1 - " ) + l_nbMaxLines + wxT( ")" ), wxT( "Aller à..." ) );

			if ( l_dialog->ShowModal() == wxID_OK )
			{
				wxString l_line = l_dialog->GetValue();
				int l_res = 0;

				if ( GetInt( l_line, l_res ) && l_res <= l_maxLines && l_res > 0 )
				{
					l_panel->GoToLine( l_res );
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
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->Search( m_search->GetValue() );
		}

		void MainFrame::OnSearchNext( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchNext();
		}

		void MainFrame::OnSearchPrevious( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchPrevious();
		}

		void MainFrame::OnReplace( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			ReplaceDialog * l_dialog = new ReplaceDialog( this, l_editCtrl );
			l_dialog->Show();
		}

		void MainFrame::DoInitEditLists()
		{
			m_objectsListContainer = new wxPanel( m_treeTabsContainer, ObjectsList, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
			m_treeTabsContainer->InsertPage( 2, m_objectsListContainer, _T( "Objets" ), false );
			wxSize l_size = m_objectsListContainer->GetClientSize();
			l_size.y /= 2;
			m_objectsList = new ObjectsTree( m_objectsListContainer, wxDefaultPosition, l_size, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_objectsList->InitObjectList();
			m_objectInfosContainer = new wxScrolledWindow( m_objectsListContainer, PropertiesList, wxPoint( 5, l_size.y + 5 ), wxSize( l_size.x - 10, l_size.y - 10 ) );
		}

		void MainFrame::DoInitElypse( Project * p_project, const wxString & p_title, bool p_edit, int p_adjustY )
		{
			wxSize l_projectSize = p_project->GetResolution();
			wxSize l_size = m_mainTabsContainer->GetClientSize();

			if ( l_size.x < l_projectSize.x || l_size.y < l_projectSize.y && m_testingProject )
			{
				l_size = l_projectSize;
				m_elypseContainer = new ElypseFrame( this, wxPoint( 0, 0 ), l_size + wxSize( 0, 30 ) );
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

				m_mainPanelContainer = new wxPanel( m_mainTabsContainer, wxID_ANY, wxPoint( 0, 0 ), m_mainTabsContainer->GetClientSize() );
				m_elypseContainer = new ElypsePanel( m_mainPanelContainer, wxPoint( 0, 0 ), l_size );
				m_mainPanelContainer->SetBackgroundColour( wxColour( 212, 208, 200 ) );
				m_mainTabsContainer->InsertPage( 0, m_mainPanelContainer, p_title, true );
				m_isFrame = false;
				/*
						l_size.y += p_adjustY;
						wxPoint l_pos( 0, 0);
						if (l_size.x > 4 * l_size.y / 3)
						{
							int l_width = l_size.x;
							l_size.x = 4 * l_size.y / 3;
							l_pos.x = (l_width - l_size.x) / 2;
						}
						else if (l_size.y > 3 * l_size.x / 4)
						{
							int l_height = l_size.y;
							l_size.y = 3 * l_size.x / 4;
							l_pos.y = (l_height - l_size.y) / 2;
						}
				*/
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
			toolBar->AddTool( IdNew, wxT( "Nouveau" ), toolBarBitmaps[Tool_new], wxT( "Nouveau projet" ) );
			toolBar->AddTool( OpenProject, wxT( "Ouvrir" ), toolBarBitmaps[Tool_open], wxT( "Ouvrir un projet" ) );
			toolBar->AddTool( saveProject, wxT( "Enregistrer" ), toolBarBitmaps[Tool_save], wxT( "Enregistrer le projet" ) );
			//	toolBar->AddTool( wxID_COPY, wxT( "Copier" ), toolBarBitmaps[Tool_copy], wxT( "Copier") );
			//	toolBar->AddTool( wxID_CUT, wxT( "Couper" ), toolBarBitmaps[Tool_cut], wxT( "Couper") );
			//	toolBar->AddTool( wxID_PASTE, wxT( "Coller" ), toolBarBitmaps[Tool_paste], wxT( "Coller") );
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
			if ( m_currentProject )
			{
				delete m_currentProject;
			}

			m_currentProject = new Project( p_name, p_mainSceneName, p_path, p_backgroundType, p_background, p_shadows,
											p_aa, p_resolution );
			m_currentProject->SetSaved( false );
			TrollScene * l_mainScene = m_currentProject->GetMainScene();
			m_filesList->DeleteAllItems();
			m_filesList->InitProjet( p_name );

			try
			{
				l_mainScene->m_sceneId = m_filesList->AddSceneToProject( l_mainScene );
				l_mainScene->m_sceneFileId = m_filesList->AddFolderToScene( p_mainSceneName, wxT( "Fichiers de scène" ) );
				l_mainScene->m_loadScriptFileId = m_filesList->AddFolderToScene( p_mainSceneName, wxT( "Fichiers de script de load" ) );
				l_mainScene->m_unloadScriptFileId = m_filesList->AddFolderToScene( p_mainSceneName, wxT( "Fichiers de script d'unload" ) );
				l_mainScene->m_dataFileId = m_filesList->AddFolderToScene( p_mainSceneName, wxT( "Datas Graphiques (pour la 3D)" ) );
				l_mainScene->m_dataFolderId = m_filesList->AddFolderToScene( p_mainSceneName, wxT( "Datas non Graphiques (autres)" ) );
			}
			catch ( bool )
			{
				LogDebugMessage( wxT( "MainFrame::SetProject - Probleme d'ajout" ) );
				return;
			}

			if ( m_editText )
			{
				m_editText->GetText().Clear();
			}
		}

		wxString MainFrame::CreateMain( Project * p_project, bool p_writeScripts )
		{
			wxString l_mainFileName;

			if ( p_project )
			{
				l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
				wxFileOutputStream l_fileOutput( l_mainFileName );
				wxTextOutputStream l_textStream( l_fileOutput );
				WriteBackgroundInMain( p_project, l_textStream );

				for ( auto && l_scene : p_project->GetScenes() )
				{
					WriteSceneInMain( p_project->GetName(), l_scene.second, p_writeScripts, true, l_textStream );
				}

				TrollScene * l_scene = p_project->GetMainScene();

				if ( l_scene )
				{
					WriteSceneInMain( p_project->GetName(), l_scene, p_writeScripts, true, l_textStream );
					l_textStream.WriteString( wxT( "start_scene " ) + l_scene->GetName() );
				}

				l_fileOutput.Close();
			}

			return l_mainFileName;
		}

		wxString MainFrame::CreateSceneEditorMain( Project * p_project )
		{
			wxString l_mainFileName;

			if ( p_project )
			{
				l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
				wxFileOutputStream l_fileOutput( l_mainFileName );
				wxTextOutputStream l_textStream( l_fileOutput );
				WriteBackgroundInMain( p_project, l_textStream );
				TrollScene * l_scene = p_project->GetMainScene();

				if ( l_scene )
				{
					WriteSceneInMain( p_project->GetName(), l_scene, true, false, l_textStream );

					l_textStream.WriteString( wxT( "scene " ) + l_scene->GetName() + wxT( "\n{\n" ) );
					l_textStream.WriteString( wxT( "\tmuse_file file://" ) + m_currentProject->GetPath() + m_currentProject->GetName() + wxT( ".muse\n" ) );
					WriteFilesInMain( l_scene->m_dataFiles, wxT( "data_file" ), wxT( ".zip" ), l_textStream );
					WriteFilesInMain( l_scene->m_sceneFiles, wxT( "scene_file" ), wxEmptyString, l_textStream );
					l_textStream.WriteString( wxT( "}\n\n" ) );

					l_textStream.WriteString( wxT( "start_scene " ) + m_currentProject->GetMainScene()->GetName() + wxT( "\n" ) );
					l_textStream.WriteString( wxT( "start_scene " ) + p_project->GetMainScene()->GetName() );
				}

				l_fileOutput.Close();
			}

			return l_mainFileName;
		}

		wxString MainFrame::CreateOverlayEditorMain( Project * p_project )
		{
			wxString l_mainFileName;

			if ( p_project )
			{
				l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
				wxFileOutputStream l_fileOutput( l_mainFileName );
				wxTextOutputStream l_textStream( l_fileOutput );
				WriteBackgroundInMain( p_project, l_textStream );
				TrollScene * l_scene = p_project->GetMainScene();

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
						LogDebugMessage( wxT( "MainFrame::CreateOverlayEditorMain - Current Project has no main scene" ) );
					}

					l_textStream.WriteString( wxT( "start_scene " ) + p_project->GetMainScene()->GetName() );
				}
				else
				{
					LogDebugMessage( wxT( "MainFrame::CreateOverlayEditorMain - Given Project has no main scene" ) );
				}

				l_fileOutput.Close();
			}

			return l_mainFileName;
		}

		void MainFrame::OpenSelectedFile()
		{
			wxTreeItemId l_itemId = m_filesList->GetSelected();
			TrollFile * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );

			if ( l_file && l_file->Saved && !l_file->Open )
			{
				wxPanel * l_editTextContainer = new wxPanel( m_mainTabsContainer, wxID_ANY, wxPoint( 0, 0 ) );
				m_mainTabsContainer->AddPage( l_editTextContainer, l_file->FileName, true );
				l_editTextContainer->SetSize( 0, 22, m_mainTabsContainer->GetClientSize().x, m_mainTabsContainer->GetClientSize().y - 22 );
				m_editText = new wxStcTextEditor( m_pStcContext, l_editTextContainer, wxID_ANY, wxPoint( 0, 0 ), l_editTextContainer->GetClientSize() );
				wxString l_path = GetCurrentProject()->GetPath() + ( l_file->m_scene->IsMainScene() ? wxString() : l_file->m_scene->GetName() + wxFileName::GetPathSeparator() );
				m_editText->LoadFile( l_path + l_file->FileName );
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

		void MainFrame::LogDebugMessage( const wxString & p_message )
		{
			//	std::cout << p_message << "\n";
			wxLog::SetActiveTarget( m_logDebug );
			wxLogMessage( p_message );
			m_debugLog->SetSelection( m_debugLog->GetLastPosition() - 2, m_debugLog->GetLastPosition() - 1 );
		}

		void MainFrame::LogOutMessage( const wxString & p_message )
		{
			//	std::cout << p_message << "\n";
			wxLog::SetActiveTarget( m_logCompilation );
			wxLogMessage( p_message );
			m_compilationLog->SetSelection( m_compilationLog->GetLastPosition() - 2, m_compilationLog->GetLastPosition() - 1 );
		}

		void MainFrame::CloseProject( bool p_closingApp )
		{
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
				m_mainPanel = NULL;
				m_treeTabsContainer = NULL;
				m_filesListContainer = NULL;
				m_filesList	 = NULL;
				m_functionsListContainer = NULL;
				m_functionsList = NULL;
				m_objectsListContainer = NULL;
				m_objectsList = NULL;
				m_mainTabsContainer = NULL;
				m_editText = NULL;
				m_elypseContainer = NULL;
				m_elypseContainer = NULL;
				m_logTabsContainer = NULL;
				m_debugLogContainer = NULL;
				m_debugLog = NULL;
				m_compilationLogContainer = NULL;
				m_compilationLog = NULL;
				m_propertyDialog = NULL;
				m_splitterV = NULL;
				m_splitterH = NULL;
				m_toolbarPosition = TOOLBAR_TOP;

				if ( !p_closingApp )
				{
					DoInitialise();
				}
			}

			if ( m_currentProject )
			{
				if ( m_currentProject->IsModified() )
				{
					int l_answer = wxMessageBox( wxT( "Enregistrer les modifications?" ), wxT( "Confirmer" ), wxYES_NO | wxCANCEL, this );

					if ( l_answer == wxYES )
					{
						SaveProject( m_currentProject );
					}
					else if ( l_answer == wxCANCEL )
					{
						return;
					}
				}

				delete m_currentProject;
				m_currentProject = NULL;
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

		void MainFrame::ShowObjectInfos( TrollObject * p_object )
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

		void MainFrame::CreateNewContinuousEventFrame( TrollContinuousEvent * p_event )
		{
		}

		void MainFrame::AddPonctualEvent( TrollSequence * p_sequence, float p_fireTime )
		{
			StringArray l_tmp;
			EMuse::Sequences::BasePonctualEvent * l_museEvent = ScriptEngine::GetContext()->sequenceManager->CreatePonctualEvent( "Overlay_Hide", l_tmp );
			TrollPonctualEvent * l_event = new TrollPonctualEvent( p_sequence, l_museEvent, wxT( "overlay" ), wxT( "" ), p_fireTime, wxT( "" ), wxT( "Overlay_Hide" ), wxT( "" ) );
			p_sequence->AddPonctualEvent( l_event );
			SequencePanel * l_panel = reinterpret_cast <SequencePanel *>( m_timelineEditor->GetSequencePanel( p_sequence->TrollObject::GetName() )->GetParent() );
			l_panel->AddPonctualEvent( l_event, p_fireTime );
		}

		void MainFrame::AddContinuousEvent( TrollSequence * p_sequence, float p_fireTime )
		{
			StringArray l_tmp;
			Overlay_Translate * l_museEvent = reinterpret_cast <Overlay_Translate *>( ScriptEngine::GetContext()->sequenceManager->CreateContinuousEvent( "Overlay_Translate" ) );
			l_museEvent->SetInterpolator( ScriptEngine::GetContext()->sequenceManager->GetInterpolator_V3( "linear" ) );
			l_museEvent->AddFrame( p_fireTime, Vector3( 0.0, 0.0, 0.0 ) );
			TrollContinuousEvent * l_event = new TrollContinuousEvent( p_sequence, l_museEvent, wxT( "Overlay_Translate" ), wxT( "" ) );
			p_sequence->AddContinuousEvent( l_event );
			l_event->AddKeyFrame( p_fireTime, wxT( "0.0 0.0 0.0" ) );
			l_event->SetTargetType( wxT( "overlay" ) );
			l_event->SetInterpolation( wxT( "linear" ) );
			l_event->SetActionName( wxT( "Overlay_Translate" ) );
			SequencePanel * l_panel = reinterpret_cast <SequencePanel *>( m_timelineEditor->GetSequencePanel( p_sequence->TrollObject::GetName() )->GetParent() );
			l_panel->AddContinuousEvent( l_event );
		}

		void MainFrame::OpenProjectFile( const wxString & p_fileName )
		{
			if ( ! p_fileName.empty() )
			{
				wxString l_fileName = p_fileName;
				l_fileName.Replace( wxT( "/" ), wxString( wxFileName::GetPathSeparator() ) );
				l_fileName.Replace( wxT( "\\" ), wxString( wxFileName::GetPathSeparator() ) );
				size_t l_index = l_fileName.find_last_of( wxFileName::GetPathSeparator() );
				wxString l_path = l_fileName.substr( 0, l_index + 1 );

				if ( m_currentProject == NULL )
				{
					wxTreeItemId l_root = m_filesList->GetRootItem();
					m_currentProject = new Project();
					//wxFileInputStream * l_input = new wxFileInputStream( p_fileName);
					//wxTextInputStream * l_textStream = new wxTextInputStream( * l_input);
					//m_currentProject->Load( l_input, l_textStream, l_path, m_filesList);
					//m_currentProject->SetProjectFileName( p_fileName);
					//m_currentProject->SetSaved( true);
					//delete l_textStream;
					//delete l_input;
					m_currentProject->Load( p_fileName, m_filesList );
					//m_currentProject->SetProjectPath( l_path);
				}
				else if ( p_fileName != m_currentProject->GetProjectFileName() )
				{
					wxTreeItemId l_root = m_filesList->GetRootItem();
					delete m_currentProject;
					m_currentProject = new Project();
					//wxFileInputStream * l_input = new wxFileInputStream( p_fileName);
					//wxTextInputStream * l_textStream = new wxTextInputStream( * l_input);
					//m_currentProject->Load(l_input, l_textStream, l_path, m_filesList);
					//m_currentProject->SetProjectFileName( p_fileName);
					//m_currentProject->SetSaved( true);
					//delete l_textStream;
					//delete l_input;
					m_currentProject->Load( p_fileName, m_filesList );
					//m_currentProject->SetProjectPath( l_path);
				}
				else
				{
					wxMessageBox( _T( "Impossible d'ajouter ce projet, un projet avec le même nom existe déjà" ),
								  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
				}
			}
		}

		void MainFrame::SaveSceneFiles( TrollScene * p_scene )
		{
			TrollFile * l_currentFile;
			unsigned int l_number = p_scene->GetSceneFilesNumber();

			for ( unsigned int i = 0 ; i < l_number ; i++ )
			{
				l_currentFile = p_scene->GetSceneFile( i );

				if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
				{
					l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
				}
			}

			l_number = p_scene->GetLoadScriptFilesNumber();

			for ( unsigned int i = 0 ; i < l_number ; i++ )
			{
				l_currentFile = p_scene->GetLoadScriptFile( i );

				if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
				{
					l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
				}
			}

			l_number = p_scene->GetUnloadScriptFilesNumber();

			for ( unsigned int i = 0 ; i < l_number ; i++ )
			{
				l_currentFile = p_scene->GetUnloadScriptFile( i );

				if ( l_currentFile->EditPanel != NULL && l_currentFile->EditPanel->IsModified() )
				{
					l_currentFile->EditPanel->SaveFile( m_currentProject->GetPath() + l_currentFile->FileName );
				}
			}
		}

		void MainFrame::SaveScene( TrollScene * p_scene )
		{
			TrollFile * l_file;

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

			/*
				for (unsigned int i = 0 ; i < p_scene->m_dataFiles.size() ; i++)
				{
					l_file = p_scene->m_dataFiles[i];
					if ( ! l_file->Saved)
					{
						SaveFileAs( l_file);
					}
					else if (l_file->EditPanel != NULL && l_file->EditPanel->IsModified())
					{
						SaveFile( l_file, l_file->FileName);
					}
				}
			*/
		}

		void MainFrame::SaveProject( Project * p_project )
		{
			if ( p_project == NULL )
			{
				return;
			}

			wxString l_fileName;

			if ( p_project->IsSaved() )
			{
				l_fileName = p_project->GetProjectFileName();
			}
			else
			{
				l_fileName = wxFileSelector( wxT( "Enregistrez sous ..." ), p_project->GetPath(), p_project->GetName() + wxT( ".teproj" ), wxEmptyString,
											 wxT( "Projets Troll Editor (*.teproj)|*.teproj" ), wxFD_SAVE );
				LogDebugMessage( l_fileName );
				p_project->SetProjectFileName( l_fileName );
			}

			LogDebugMessage( wxT( "MainFrame::SaveProject - " ) + l_fileName );

			if ( ! l_fileName.empty() )
			{
				LogDebugMessage( wxT( "MainFrame::OnSaveProject - " ) + l_fileName );
				wxFileOutputStream fileOutput( l_fileName );
				wxTextOutputStream * l_textStream = new wxTextOutputStream( fileOutput );
				TrollScene * l_scene = p_project->GetMainScene();
				SaveScene( l_scene );
				SceneMap l_scenes = p_project->GetScenes();
				SceneMap::iterator l_it = l_scenes.begin();

				while ( l_it != l_scenes.end() )
				{
					l_scene = l_it->second;

					if ( l_scene )
					{
						SaveScene( l_scene );
					}

					++l_it;
				}

				p_project->Save( l_textStream );
				p_project->SetSaved( true );
				p_project->SetModified( false );
				delete l_textStream;
				fileOutput.Close();
			}
		}

		void MainFrame::SaveFileAs( TrollFile * p_file )
		{
			if ( p_file == NULL )
			{
				LogDebugMessage( wxT( "MainFrame::SaveFileAs - Nothing to save" ) );
				return;
			}

			wxString l_fullFileName;

			if ( p_file->Filetype == sceneFile )
			{
				l_fullFileName = wxFileSelector( wxT( "Enregistrez sous ..." ), m_currentProject->GetPath(),
												 wxEmptyString, wxEmptyString, wxT( "Fichier .emscene (*.emscene)|*.emscene" ), wxFD_SAVE );
			}
			else if ( p_file->Filetype == loadScriptFile || p_file->Filetype == unloadScriptFile )
			{
				l_fullFileName = wxFileSelector( wxT( "Enregistrez sous ..." ), m_currentProject->GetPath(),
												 wxEmptyString, wxEmptyString, wxT( "Fichier .emscript (*.emscript)|*.emscript" ), wxFD_SAVE );
			}
			else if ( p_file->Filetype == dataFile )
			{
				l_fullFileName = wxFileSelector( wxT( "Enregistrez sous ..." ), m_currentProject->GetPath(),
												 wxEmptyString, wxEmptyString, wxT( "Fichier .zip (*.zip)|*.zip" ), wxFD_SAVE );
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
				p_file->Folder = l_fullFileName.substr( 0, l_index );
			}
		}

		void MainFrame::SaveFile( TrollFile * p_file, const wxString & p_fileName )
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

		void MainFrame::CompileNeededScenes( const wxArrayString & p_scenes, TrollScriptCompiler * p_compiler, wxArrayString & p_compiledScenes )
		{
			TrollScene * l_scene;
			wxArrayString l_neededScenes;
			unsigned int l_nbFiles;
			TrollFile * l_file;
			ScriptNode * l_scriptNode;
			bool l_found;
			unsigned int k;

			for ( size_t i = 0 ; i < p_scenes.size() ; i++ )
			{
				l_scene = m_currentProject->GetScene( p_scenes[i] );
				l_neededScenes = l_scene->GetNeededScenes();
				CompileNeededScenes( l_neededScenes, p_compiler, p_compiledScenes );
				l_found = false;
				k = 0;

				while ( k < p_compiledScenes.size() && ! l_found )
				{
					if ( p_compiledScenes[k] == p_scenes[i] )
					{
						l_found = true;
					}

					k++;
				}

				if ( l_found )
				{
					continue;
				}

				l_nbFiles = l_scene->GetLoadScriptFilesNumber();
				wxString l_compilationResult;

				for ( unsigned int j = 0 ; j < l_nbFiles ; j++ )
				{
					l_file = l_scene->GetLoadScriptFile( j );
					wxString l_fileName = ( l_file->m_scene->IsMainScene() ? wxString() : l_file->m_scene->GetName() + wxFileName::GetPathSeparator() ) + l_file->FileName;
					wxString l_path = GetCurrentProject()->GetPath() + l_fileName;
					LogOutMessage( wxString( wxT( "Compiling " ) ) << l_fileName );
					l_scriptNode = p_compiler->CompileScript( DumpFile( l_path.char_str().data() ) );
					LogOutMessage( l_fileName << wxString( wxT( " compiled - " ) ) << p_compiler->GetNumWarnings() << wxT( " warnings - " ) << p_compiler->GetNumErrors() << wxT( " errors" ) );
					l_file->m_compiled = true;

					if ( l_scriptNode != NULL )
					{
						l_scriptNode->Use();
						l_scriptNode->Delete();
					}
				}

				p_compiledScenes.push_back( p_scenes[i] );
			}
		}

		void MainFrame::Compile( TrollFile * p_file, TrollScene * p_scene )
		{
			m_compilationLog->Clear();
			m_logTabsContainer->SetSelection( 1 );

			if ( m_elypseContainer != NULL )
			{
				LogOutMessage( wxT( "Impossible de compiler pendant un test ou une édition" ) );
				return;
			}

			wxString l_wxPath = m_currentProject->GetPath() + ( p_file->m_scene->IsMainScene() ? wxString() : wxString( p_file->m_scene->GetName() ) + wxFileName::GetPathSeparator() );
			p_file->EditPanel->SaveFile();

			if ( ! p_file->Saved || ( p_file->EditPanel != NULL && p_file->EditPanel->IsModified() ) )
			{
				LogOutMessage( wxT( "Sauvegardez le fichier avant de le compiler" ) );
				return;
			}

			String l_path = m_currentProject->GetPath().char_str().data();

			if ( p_file->EditPanel )
			{
				p_file->EditPanel->ReinitContext( m_pStcContext );
			}

			TrollLogs * l_logs = new TrollLogs();
			TrollScriptCompiler * l_compiler = new TrollScriptCompiler( l_path, false );
			l_compiler->Initialise( p_file->EditPanel );
			ScriptNode * l_scriptNode;
			l_compiler->CompileScript( m_currentProject->GetStartupScript().char_str().data() );
			wxArrayString l_scenesThatNeedMe = p_scene->GetNeededScenes();
			wxArrayString l_compiledScenes;
			CompileNeededScenes( l_scenesThatNeedMe, l_compiler, l_compiledScenes );
			unsigned int l_nbFiles = p_scene->GetLoadScriptFilesNumber();
			unsigned int i = 0;
			bool l_found = false;
			TrollFile * l_file = NULL;
			wxStcTextEditor * l_textCtrl = NULL;
			wxString l_compilationResult;

			while ( i < l_nbFiles && ! l_found )
			{
				l_file = p_scene->GetLoadScriptFile( i );

				if ( l_file == p_file )
				{
					l_found = true;
					l_textCtrl = l_file->EditPanel;
				}
				else
				{
					l_textCtrl = m_editText;
				}

				wxString l_fileName = ( l_file->m_scene->IsMainScene() ? wxString() : l_file->m_scene->GetName() + wxFileName::GetPathSeparator() ) + l_file->FileName;
				wxString l_path = GetCurrentProject()->GetPath() + l_fileName;
				LogOutMessage( wxString( wxT( "Compiling " ) ) << l_fileName );
				l_scriptNode = l_compiler->CompileScript( DumpFile( l_path.char_str().data() ) );
				LogOutMessage( l_fileName << wxString( wxT( " compiled - " ) ) << l_compiler->GetNumWarnings() << wxT( " warnings - " ) << l_compiler->GetNumErrors() << wxT( " errors" ) );

				if ( l_scriptNode != NULL )
				{
					l_scriptNode->Use();
					l_scriptNode->Delete();
				}

				i++;
			}

			if ( ! l_found )
			{
				l_nbFiles = p_scene->GetUnloadScriptFilesNumber();
				i = 0;
				l_file = NULL;
				l_textCtrl = NULL;

				while ( i < l_nbFiles && ! l_found )
				{
					l_file = p_scene->GetUnloadScriptFile( i );

					if ( l_file == p_file )
					{
						l_found = true;
						l_textCtrl = l_file->EditPanel;
					}
					else
					{
						l_textCtrl = m_editText;
					}

					wxString l_fileName = ( l_file->m_scene->IsMainScene() ? wxString() : l_file->m_scene->GetName() + wxFileName::GetPathSeparator() ) + l_file->FileName;
					wxString l_path = GetCurrentProject()->GetPath() + l_fileName;
					LogOutMessage( wxString( wxT( "Compiling " ) ) << l_fileName );
					l_scriptNode = l_compiler->CompileScript( DumpFile( l_path.char_str().data() ) );
					LogOutMessage( l_fileName << wxString( wxT( " compiled - " ) ) << l_compiler->GetNumWarnings() << wxT( " warnings - " ) << l_compiler->GetNumErrors() << wxT( " errors" ) );

					if ( l_scriptNode != NULL )
					{
						l_scriptNode->Use();
						l_scriptNode->Delete();
					}

					i++;
				}
			}

			delete l_compiler;
			delete l_logs;
			SceneMap l_scenes = m_currentProject->GetScenes();
			TrollScene * l_scene;

			for ( SceneMap::iterator l_it = l_scenes.begin() ; l_it != l_scenes.end() ; ++l_it )
			{
				l_scene = l_it->second;
				l_nbFiles = p_scene->GetLoadScriptFilesNumber();

				for ( i = 0 ; i < l_nbFiles ; i++ )
				{
					p_scene->GetLoadScriptFile( i )->m_compiled = false;
				}
			}
		}

		void MainFrame::ListFolder()
		{
			FolderList * l_list = new FolderList( m_currentProject->GetPath() + m_filesList->GetSelectedScene()->GetFile( m_filesList->GetSelected() )->Folder, this );
			l_list->Show();
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

				//		m_elypseCtrl->Destroy();
				m_elypseCtrl = NULL;
				m_elypseContainer = NULL;
				m_viewedObject = NULL;

				//		m_currentProject->GetMainScene()->FlushObjects();

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
	}
}
