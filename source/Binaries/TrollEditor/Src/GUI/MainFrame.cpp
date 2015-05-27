﻿#include "PrecompiledHeader.h"

#include "GUI/MainFrame.h"
#include "GUI/StcTextEditor.hpp"
#include "GUI/ElypseCtrl.h"
#include "GUI/ElypseFrame.h"
#include "GUI/ElypsePanel.h"
#include "GUI/main.h"
#include "GUI/NewProjectFrame.h"
#include "GUI/NewSceneFrame.h"
#include "GUI/SceneDependenciesFrame.h"
#include "GUI/FunctionsTree.h"
#include "GUI/FilesTree.h"
#include "GUI/ObjectsTree.h"
#include "GUI/TextPanel.h"
#include "GUI/LogCtrl.h"
#include "GUI/FolderList.h"
#include "GUI/AboutFrame.h"
#include "GUI/ReplaceDialog.h"
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
/*
#include <Data/ConfigFile.h>
#include <ScriptNode.h>
#include <TrollSceneFile/SceneFileParser.h>
#include <Sequences/SequenceManager.h>
#include <Sequences/BaseEvent.h>
#include <Sequences/Events.h>
#include <Main/Context.h>

#include <General/Memory.h>

#include <CkZip.h>

#include <wx/dnd.h>
#include <wx/event.h>
#include <wx/listbox.h>
#include <wx/splitter.h>
#include <wx/stattext.h>
#include <wx/listctrl.h>
#include <wx/scrolwin.h>
#include <wx/stdpaths.h>
#include <wx/textdlg.h>
*/
using namespace Troll::Temporal;

namespace Troll
{
	namespace GUI
	{
		MainFrame * g_mainFrame;

		BEGIN_EVENT_TABLE( MainFrame, wxFrame )
			EVT_MENU(	IdNew,					MainFrame::_onNew	)
			EVT_MENU(	OpenProject,			MainFrame::_onOpenProject	)
			EVT_MENU(	saveModifs,				MainFrame::_onWriteSceneFiles	)
			EVT_MENU(	saveProject,			MainFrame::_onSaveProject	)
			EVT_MENU(	IdCloseProject,			MainFrame::_onCloseProject	)
			EVT_MENU(	IdOnSaveFile,			MainFrame::_onSaveFile	)
			EVT_MENU(	IdOnSaveFileAs,			MainFrame::_onSaveFileAs	)
			EVT_MENU(	IdOnSaveSelectedFile,	MainFrame::_onSaveSelectedFile	)
			EVT_MENU(	Quit,					MainFrame::_onQuit	)
			EVT_MENU(	OpenFil,				MainFrame::_onOpenFile	)
			EVT_MENU(	About,					MainFrame::_onAbout	)
			EVT_MENU(	Add,					MainFrame::_onAdd	)
			EVT_MENU(	Rename,					MainFrame::_onRename	)
			EVT_MENU(	IdOuvrirFichier,		MainFrame::_onOpenSelectedFile	)
			EVT_MENU(	CompileFile,			MainFrame::_onCompile	)
			EVT_MENU(	IdAddExistingFile,		MainFrame::_onAddExistingFile	)
			EVT_MENU(	IdMusiner,				MainFrame::_onMusinate	)
			EVT_MENU(	IdRemove,				MainFrame::_onRemoveFile	)
			EVT_MENU(	IdDelete,				MainFrame::_onDeleteFile	)
			EVT_MENU(	IdListFolder,			MainFrame::_onListFolder	)
			EVT_MENU(	Tool_copy,				MainFrame::_onFunctionCopy	)
			EVT_MENU(	Tool_cut,				MainFrame::_onFunctionCut	)
			EVT_MENU(	Tool_paste,				MainFrame::_onFunctionPaste	)
			EVT_MENU(	EditTimeLine,			MainFrame::_onEditTimeLine	)
			EVT_MENU(	NewScene,				MainFrame::_onNewScene	)
			EVT_MENU(	RenameScene,			MainFrame::_onRename	)
			EVT_MENU(	DeleteScene,			MainFrame::_onRemoveScene	)
			EVT_MENU(	IdEditerScene,			MainFrame::_onEditProject	)
			EVT_MENU(	IdEditerOverlays,		MainFrame::_onEditOverlays	)
			EVT_MENU(	IdTester,				MainFrame::_onTestProject	)
			EVT_MENU(	IdFermer,				MainFrame::_onCloseFile	)
			EVT_MENU(	IdProjectProperties,	MainFrame::_onProjectProperties	)
			EVT_MENU(	IdGoToLine,				MainFrame::_onGoToLine	)
			EVT_MENU(	IdSearchNext,			MainFrame::_onSearchNext	)
			EVT_MENU(	IdSearchPrevious,		MainFrame::_onSearchPrevious	)
			EVT_MENU(	IdReplace,				MainFrame::_onReplace	)
			EVT_MENU(	TreeNewScene,			MainFrame::_onTreeNewScene	)
			EVT_MENU(	EditSceneDependencies,	MainFrame::_onEditSceneDependencies	)
			EVT_CLOSE(	MainFrame::_onClose	)
			EVT_TEXT_ENTER(	IdSearch,				MainFrame::_onSearch	)
			EVT_SHOW(	MainFrame::_onShow	)
			EVT_SET_FOCUS(	MainFrame::_onSetFocus	)
			EVT_KILL_FOCUS(	MainFrame::_onKillFocus	)
			EVT_ACTIVATE(	MainFrame::_onActivate	)
			EVT_SIZE(	MainFrame::_onSize	)
			EVT_NOTEBOOK_PAGE_CHANGED(	wxID_ANY,				MainFrame::_onNotebook	)
			EVT_SPLITTER_SASH_POS_CHANGED(	HorizSplitter,			MainFrame::_onVerticalSplitterPositionChanged	)
			EVT_SPLITTER_SASH_POS_CHANGING( HorizSplitter,			MainFrame::_onVerticalSplitterPositionChanging	)
			EVT_SPLITTER_DCLICK(	HorizSplitter,			MainFrame::_onVerticalSplitterDClick	)
			EVT_SPLITTER_UNSPLIT(	HorizSplitter,			MainFrame::_onVerticalSplitterUnsplitEvent	)
			EVT_SPLITTER_SASH_POS_CHANGED(	VerticSplitter,			MainFrame::_onHorizontalSplitterPositionChanged	)
			EVT_SPLITTER_SASH_POS_CHANGING( VerticSplitter,			MainFrame::_onHorizontalSplitterPositionChanging	)
			EVT_SPLITTER_DCLICK(	VerticSplitter,			MainFrame::_onHorizontalSplitterDClick	)
			EVT_SPLITTER_UNSPLIT(	VerticSplitter,			MainFrame::_onHorizontalSplitterUnsplitEvent	)
		END_EVENT_TABLE()

		static const long c_toolbarStyle = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

		MainFrame :: MainFrame( const wxString & title, int x, int y, int w, int h )
			:	wxFrame( NULL, wxID_ANY, title, wxPoint( x, y ), wxSize( w, h )	)
			,	m_tbar( NULL	),	m_numSceneFile( 1	)
			,	m_numScriptFile( 1	),	m_numDataFile( 1	)
			,	m_rows( 1	),	m_nPrint( 1	)
			,	m_smallToolbar( true	),	m_horzText( false	)
			,	m_useCustomDisabled( false	),	m_showTooltips( true	)
			,	m_testingProject( false	),	m_currentProject( NULL	)
			,	m_listBox( NULL	),	m_mainPanel( NULL	)
			,	m_treeTabsContainer( NULL	),	m_filesListContainer( NULL	)
			,	m_filesList( NULL	),	m_functionsListContainer( NULL	)
			,	m_functionsList( NULL	),	m_objectsListContainer( NULL	)
			,	m_objectsList( NULL	),	m_mainTabsContainer( NULL	)
			,	m_editText( NULL	),	m_elypseContainer( NULL	)
			,	m_debugLogContainer( NULL	),	m_timelineEditor( NULL	)
			,	m_debugLog( NULL	),	m_compilationLogContainer( NULL	)
			,	m_compilationLog( NULL	),	m_propertyDialog( NULL	)
			,	m_treesWidth( 265	),	m_logsHeight( 200	)
			,	m_splitterV( NULL	),	m_splitterH( NULL	)
			,	m_timePanel( NULL	),	m_toolbarPosition( TOOLBAR_TOP	)
			,	m_objectInfos( NULL	),	m_viewedObject( NULL	)
			,	m_editingProject( false	),	m_editingOverlays( false	)
			,	m_editingTimeLines( false	),	m_logTabsContainer( NULL	)
			,	m_elypseCtrl( NULL	),	m_resizing( false	)
			,	m_pStcContext( new StcContext	)
		{
			g_mainFrame = this;
			/*
				::AllocConsole();
				freopen ("CONOUT$", "w", stdout );
				freopen ("CONERR", "w", stderr );
			*/
			m_appPath = wxStandardPaths::Get().GetExecutablePath();
			m_appPath.Replace( wxT( "/" ), wxString( wxFileName::GetPathSeparator() ) );
			m_appPath.Replace( wxT( "\\" ), wxString( wxFileName::GetPathSeparator() ) );
			size_t l_index = m_appPath.find_last_of( wxFileName::GetPathSeparator() );
			m_appPath = m_appPath.substr( 0, l_index + 1 );
			//	std::cout << "MainFrame :: MainFrame - " << m_appPath << "\n";
			LanguageFileParser l_parser( m_pStcContext );
			l_parser.ParseFile( m_appPath + wxT( "EMSCRIPT.lang" ) );
			l_parser.ParseFile( m_appPath + wxT( "EMSCENE.lang" ) );
			_initialise();
		}

		void MainFrame :: _initialise()
		{
			SetIcon( wxIcon( troll_editor_xpm ) );
			SetBackgroundColour( wxColour( 255, 255, 255 ) );
			CreateStatusBar( 2 );
			SetStatusText( wxT( "Welcome to Troll Editor!" ) );
			_setMenuBar();
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
			TrollScriptCompiler * l_compiler = new TrollScriptCompiler( "", true );
			l_compiler->Initialise( NULL );
			delete l_compiler;
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
			_createTreeWithDefStyle();
			//m_menuBar->Check( TreeTest_ToggleImages, false);
			m_logDebug = new wxLogTextCtrl( m_debugLog );
			m_logCompilation = new wxLogTextCtrl( m_compilationLog );
			m_splitterV->SplitVertically( m_treeTabsContainer, m_mainTabsContainer, m_treesWidth );
			m_splitterH->SplitHorizontally( m_splitterV, m_logTabsContainer, 0 - m_logsHeight );
			_resize();
		}

		void MainFrame :: _setMenuBar()
		{
			m_menuBar =	new wxMenuBar( wxMB_DOCKABLE );
			m_menuFile = new wxMenu;
			m_menuHelp = new wxMenu;
			m_menuProject = new wxMenu;
			m_menuEdition = new wxMenu;
			m_menuFile->Append( IdNew,				wxT( "&Nouveau Projet" )	);
			m_menuFile->Append( OpenProject,		wxT( "&Ouvrir Projet" )	);
			m_menuFile->Append( OpenFil,			wxT( "O&uvrir Fichier" )	);
			m_menuFile->Append( IdCloseProject,		wxT( "&Fermer projet" )	);
			m_menuFile->AppendSeparator();			/******************************************/
			m_menuFile->Append( saveProject,		wxT( "&Enregistrer Projet\tCtrl+T" )	);
			m_menuFile->Append( IdOnSaveFile,		wxT( "En&registrer Fichier\tCtrl+S" )	);
			m_menuFile->Append( IdOnSaveFileAs,		wxT( "Enregistrer Fichier &Sous...Ctrl+Shift+S" )	);
			m_menuFile->AppendSeparator();			/******************************************/
			m_menuFile->Append( Quit,				wxT( "&Quitter" )	);
			m_menuEdition->Append( IdGoToLine,			wxT( "&Aller à...\tCtrl+G" )	);
			m_menuEdition->Append( IdSearchNext,		wxT( "Chercher &suivant...\tF3" )	);
			m_menuEdition->Append( IdSearchPrevious,	wxT( "Chercher &précédent...\tShift+F3" )	);
			m_menuEdition->Append( IdReplace,			wxT( "&Remplacer...\tCtrl+H" )	);
			m_menuProject->Append( EditSceneDependencies,					wxT( "&Editer Dépendances\tF5" )	);
			m_menuProject->Append( CompileFile,								wxT( "&Compiler\tF6" )	);
			m_menuProject->Append( IdMusiner,								wxT( "&Musiner\tF7" )	);
			m_menuEditTimeLines = m_menuProject->Append( EditTimeLine,		wxT( "Editer T&imeline\tF8" )	);
			m_menuEditProject = m_menuProject->Append( IdEditerScene,		wxT( "Editer &Scène\tF9" )	);
			m_menuEditOverlays = m_menuProject->Append( IdEditerOverlays,	wxT( "Editer &Overlays\tF10" )	);
			m_menuTestProject = m_menuProject->Append( IdTester,			wxT( "&Tester Projet\tF11" )	);
			m_menuProject->Append( IdProjectProperties,						wxT( "&Propriétés\tCtrl+P" )	);
			m_menuHelp->Append( About,										wxT( "&A Propos..." ) );
			m_menuBar->Append( m_menuFile,		wxT( "&Fichier" )	);
			m_menuBar->Append( m_menuEdition,	wxT( "&Edition" )	);
			m_menuBar->Append( m_menuProject,	wxT( "&Projet" )	);
			m_menuBar->Append( m_menuHelp,		wxT( "&Aide" )	);
			SetMenuBar( m_menuBar );
			/*
				// Associate the menu bar with the frame
				m_menuBar->Check( M_TOOLBAR_SHOW_BOTH, true);
				m_menuBar->Check( M_TOOLBAR_TOGGLETOOLTIPS, true);
				m_menuBar->Check( M_TOOLBAR_TOP_ORIENTATION, true);
			*/
		}

		void MainFrame :: _createTreeWithDefStyle()
		{
			long style = wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS;
			wxMenuBar * mbar = GetMenuBar();
			/*
				mbar->Check( TreeTest_TogButtons,		(style & wxTR_HAS_BUTTONS)			!= 0);
				mbar->Check( TreeTest_TogButtons,		(style & wxTR_TWIST_BUTTONS)		!= 0);
				mbar->Check( TreeTest_TogLines,			(style & wxTR_NO_LINES)				== 0);
				mbar->Check( TreeTest_TogRootLines,		(style & wxTR_LINES_AT_ROOT)		!= 0);
				mbar->Check( TreeTest_TogHideRoot,		(style & wxTR_HIDE_ROOT)			!= 0);
				mbar->Check( TreeTest_TogEdit,			(style & wxTR_EDIT_LABELS)			!= 0);
				mbar->Check( TreeTest_TogBorder,		(style & wxTR_ROW_LINES)			!= 0);
				mbar->Check( TreeTest_TogFullHighlight,	(style & wxTR_FULL_ROW_HIGHLIGHT)	!= 0);
			*/
		}

		void MainFrame :: _resizeTrees()
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

		void MainFrame :: _resizePanels()
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

						//				m_elypseContainer->SetSize( l_pos.x, l_pos.y, l_size.x, l_size.y);
					}
					else
					{
						l_page->SetSize( 0, 22, l_tabSize.x, l_tabSize.y - 22 );
						( * l_page->GetChildren().begin() )->SetSize( 0, 0, l_page->GetClientSize().x, l_page->GetClientSize().y );
					}
				}
			}
		}

		void MainFrame :: _resizeLogs()
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

		void MainFrame :: _resize()
		{
			m_resizing = true;

			if ( m_splitterV != NULL && m_splitterH != NULL )
			{
				wxSize l_size = GetClientSize();
				//		std::cout << "MainFrame :: _resize - Width : " << l_size.x << " - Height : " << l_size.y << "\n";
				m_splitterH->SetSize( l_size );
				m_splitterH->SetSashPosition( m_splitterH->GetClientSize().y - m_logsHeight );
				m_splitterV->SetSize( l_size.x, l_size.y - m_logsHeight );
				m_splitterV->SetSashPosition( m_treesWidth );
				_resizeLogs();
				_resizeTrees();
				_resizePanels();
			}

			m_resizing = false;
		}

		void MainFrame :: _musinate( Project * p_project, bool p_createMain, bool p_withScripts )
		{
			if ( ! p_project )
			{
				return;
			}

			wxString l_mainFile;

			if ( p_createMain )
			{
				l_mainFile = CreateMain( p_project, p_withScripts );
			}

			TrollScene * l_scene = p_project->GetMainScene();

			if ( l_scene )
			{
				wxString l_path = p_project->GetPath();
				DataWriter * l_dataWriter = new DataWriter( m_compilationLog );
				TrollFileArray l_files = l_scene->m_dataFolders;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - Data Folder - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFolder( l_path + l_files[i]->FileName, EM_BLOCK_ZIPSNDDATA );
				}

				l_files = l_scene->m_sceneFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - TrollScene File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				if ( p_withScripts )
				{
					l_files = l_scene->m_loadScriptFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						LogDebugMessage( wxT( "MainFrame :: Musiner - Load Script File - " ) + l_path + l_files[i]->FileName );
						l_dataWriter->AddFile( l_path + l_files[i]->FileName );
					}

					l_files = l_scene->m_unloadScriptFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						LogDebugMessage( wxT( "MainFrame :: Musiner - Unload Script File - " ) + l_path + l_files[i]->FileName );
						l_dataWriter->AddFile( l_path + l_files[i]->FileName );
					}
				}

				l_files = l_scene->m_dataFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					//LogDebugMessage(wxT( "MainFrame :: Musiner - Data File - " ) + l_path + l_files[i]->FileName);
					//l_dataWriter->AddFolder( l_path + l_files[i]->FileName, EM_BLOCK_GZIPDATA );
					ZipFolder( l_path + l_files[i]->FileName, l_path + l_files[i]->FileName + wxT( ".zip" ) );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName + wxT( ".zip" ) );
				}

				if ( p_createMain )
				{
					l_dataWriter->AddFile( l_mainFile );
				}

				SceneMap l_scenes = p_project->GetScenes();
				SceneMap::iterator l_it = l_scenes.begin();
				DataWriter * l_sceneDataWriter;

				while ( l_it != l_scenes.end() )
				{
					l_sceneDataWriter = new DataWriter( m_compilationLog );
					l_scene = l_it->second;
					l_files = l_scene->m_dataFolders;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						LogDebugMessage( wxT( "MainFrame :: Musiner - Data Folder - " ) + l_path + l_files[i]->FileName );
						l_sceneDataWriter->AddFolder( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName, EM_BLOCK_ZIPSNDDATA );
					}

					l_files = l_scene->m_sceneFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						LogDebugMessage( wxT( "MainFrame :: Musiner - TrollScene File - " ) + l_path + l_files[i]->FileName );
						l_sceneDataWriter->AddFile( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName );
					}

					if ( p_withScripts )
					{
						l_files = l_scene->m_loadScriptFiles;

						for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
						{
							LogDebugMessage( wxT( "MainFrame :: Musiner - Load Script File - " ) + l_path + l_files[i]->FileName );
							l_sceneDataWriter->AddFile( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName );
						}

						l_files = l_scene->m_unloadScriptFiles;

						for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
						{
							LogDebugMessage( wxT( "MainFrame :: Musiner - Unload Script File - " ) + l_path + l_files[i]->FileName );
							l_sceneDataWriter->AddFile( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName );
						}
					}

					l_files = l_scene->m_dataFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						//LogDebugMessage(wxT( "MainFrame :: Musiner - Data File - " ) + l_path + l_files[i]->FileName);
						//l_sceneDataWriter->AddFolder( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName, EM_BLOCK_GZIPDATA );
						ZipFolder( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName, l_path + l_files[i]->FileName + wxT( ".zip" ) );
						l_sceneDataWriter->AddFile( l_path + l_scene->GetName() + wxT( "/" ) + l_files[i]->FileName + wxT( ".zip" ) );
					}

					l_sceneDataWriter->Write( l_path + l_scene->GetName() + wxT( ".muse" ) );
					delete l_sceneDataWriter;
					++l_it;
				}

				l_dataWriter->Write( l_path + p_project->GetName() + wxT( ".muse" ) );
				delete l_dataWriter;
			}
		}

		void MainFrame :: _musinateSceneEditor( Project * p_project )
		{
			if ( ! p_project )
			{
				return;
			}

			wxString l_mainFile = CreateSceneEditorMain( p_project );
			TrollScene * l_scene = p_project->GetMainScene();

			if ( l_scene )
			{
				wxString l_path = p_project->GetPath();
				DataWriter * l_dataWriter = new DataWriter( m_compilationLog );
				TrollFileArray l_files = l_scene->m_dataFolders;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - Data Folder - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFolder( l_path + l_files[i]->FileName, EM_BLOCK_ZIPSNDDATA );
				}

				l_files = l_scene->m_sceneFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - TrollScene File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				l_files = l_scene->m_loadScriptFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - Load Script File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				l_files = l_scene->m_unloadScriptFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - Unload Script File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				l_files = l_scene->m_dataFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					//LogDebugMessage(wxT( "MainFrame :: Musiner - Data File - ") + l_path + l_files[i]->FileName);
					//l_dataWriter->AddFolder( l_path + l_files[i]->FileName, EM_BLOCK_GZIPDATA );
					ZipFolder( l_path + l_files[i]->FileName, l_path + l_files[i]->FileName + wxT( ".zip" ) );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName + wxT( ".zip" ) );
				}

				l_dataWriter->AddFile( l_mainFile );
				l_dataWriter->Write( l_path + p_project->GetName() + wxT( ".muse" ) );
				delete l_dataWriter;
			}
		}

		void MainFrame :: _musinateOverlayEditor( Project * p_project )
		{
			if ( ! p_project )
			{
				return;
			}

			wxString l_mainFile = CreateOverlayEditorMain( p_project );
			TrollScene * l_scene = p_project->GetMainScene();

			if ( l_scene )
			{
				wxString l_path = p_project->GetPath();
				DataWriter * l_dataWriter = new DataWriter( m_compilationLog );
				TrollFileArray l_files = l_scene->m_sceneFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - TrollScene File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				l_files = l_scene->m_loadScriptFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					LogDebugMessage( wxT( "MainFrame :: Musiner - Load Script File - " ) + l_path + l_files[i]->FileName );
					l_dataWriter->AddFile( l_path + l_files[i]->FileName );
				}

				l_dataWriter->AddFile( l_mainFile );
				l_dataWriter->Write( l_path + p_project->GetName() + wxT( ".muse" ) );
				delete l_dataWriter;
			}
		}

		void MainFrame :: _onOpenSelectedFile( wxCommandEvent & p_event )
		{
			OpenSelectedFile();
		}

		void MainFrame :: _onOpenFile( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( wxT( "Choisissez un fichier à ouvrir" ), wxEmptyString, wxEmptyString, wxEmptyString,
												  wxT( "Fichiers de TrollScene (*.emscene)|*.emscene|Fichiers de Script (*.emscript)|*.emscript" ) );

			if ( l_fileName.empty() )
			{
				return;
			}
		}

		void MainFrame :: _onOpenProject( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( wxT( "Choisissez un projet à ouvrir" ), wxEmptyString, wxEmptyString, wxEmptyString,
												  wxT( "Projets Troll Editor (*.teproj)|*.teproj" ) );
			OpenProjectFile( l_fileName );
		}

		void MainFrame :: _onWriteSceneFiles( wxCommandEvent & p_event )
		{
			if ( m_currentProject != NULL )
			{
				m_currentProject->Write();
			}
		}

		void MainFrame :: _onNew( wxCommandEvent & p_event )
		{
			NewProjectFrame * l_frame = new NewProjectFrame( this );
			l_frame->ShowModal();
		}

		void MainFrame :: _onCloseProject( wxCommandEvent & p_event )
		{
			CloseProject();
		}

		void MainFrame :: _onSaveAll( wxCommandEvent & p_event )
		{
			SaveSceneFiles( m_currentProject->GetMainScene() );
			SceneMap l_scenes = m_currentProject->GetScenes();
			SceneMap::iterator l_it = l_scenes.begin();

			for ( ; l_it != l_scenes.end() ; ++l_it )
			{
				SaveSceneFiles( l_it->second );
			}
		}

		void MainFrame :: _onQuit( wxCommandEvent & p_event )
		{
			delete m_logDebug;
			m_logDebug = NULL;
			delete m_logCompilation;
			m_logCompilation = NULL;
			//	Close( true);
			p_event.Skip();
		}

		void MainFrame :: _onClose( wxCloseEvent & p_event )
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
				CloseProject();
			}

			p_event.Skip();
		}

		void MainFrame :: _onCompile( wxCommandEvent & p_event )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			/*
				if ( ! m_filesList->GetSelectedScene())
				{
					LogDebugMessage( "Aucune scène sélectionnée");
					return;
				}
			*/
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

		void MainFrame :: _onAbout( wxCommandEvent & p_event )
		{
			AboutFrame * l_frame = new AboutFrame( this );
			l_frame->Show();
		}

		void MainFrame :: _onAdd( wxCommandEvent & p_event )
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

		void MainFrame :: _onRename( wxCommandEvent & p_event )
		{
			m_filesList->EditLabel( m_filesList->GetSelected() );
		}

		void MainFrame :: _onAddExistingFile( wxCommandEvent & p_event )
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

		void MainFrame :: _onSaveFile( wxCommandEvent & p_event )
		{
			if ( m_editText != NULL )
			{
				int l_selectedPage = m_mainTabsContainer->GetSelection();
				wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );

				if ( l_fileName != wxT( "Edition de la Timeline" ) && l_fileName != wxT( "Edition de TrollScene" ) && l_fileName != wxT( "Edition des overlays" ) && l_fileName != wxT( "Test de TrollScene" ) )
				{
					TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
					//			std::cout << "MainFrame :: _onSaveFile - Selected Page : " << l_selectedPage << " - " << l_fileName << "\n";
					SaveFile( l_editCtrl->GetFile(), l_fileName );
				}
			}
		}

		void MainFrame :: _onSaveProject( wxCommandEvent & p_event )
		{
			SaveProject( m_currentProject );
		}

		void MainFrame :: _onSaveSelectedFile( wxCommandEvent & p_event )
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
				//		std::cout << "MainFrame :: _onSaveSelectedFile" << l_file->FileName << "\n";
				l_file->EditPanel->SaveFile( m_currentProject->GetPath() + l_file->FileName );
			}
			else
			{
				_onSaveFileAs( p_event );
			}
		}

		void MainFrame :: _onSaveFileAs( wxCommandEvent & p_event )
		{
			//	std::cout << "MainFrame :: _onSaveFileAs\n";
			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );
			TrollScene * l_scene = m_filesList->GetSelectedScene();
			TrollFile * l_file = l_scene->GetFileByName( l_fileName );
			SaveFileAs( l_file );
		}

		void MainFrame :: _onMusinate( wxCommandEvent & )
		{
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			_musinate( m_currentProject );
		}

		void MainFrame :: _onRemoveFile( wxCommandEvent & )
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

		void MainFrame :: _onDeleteFile( wxCommandEvent & p_event )
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

				//		std::cout << "MainFrame :: _onDeleteFile - " << remove( m_currentProject->GetPath() + l_file->FileName) << "\n";
				m_filesList->GetSelectedScene()->RemoveFile( l_itemId );
				m_filesList->Delete( l_itemId );
			}
		}

		void MainFrame :: _onFunctionCopy( wxCommandEvent & )
		{
		}

		void MainFrame :: _onFunctionCut( wxCommandEvent & )
		{
		}

		void MainFrame :: _onFunctionPaste( wxCommandEvent & )
		{
		}

		void MainFrame :: _onNewScene( wxCommandEvent & p_event )
		{
			if ( m_currentProject )
			{
				TrollScene * l_scene = new TrollScene( m_currentProject, wxT( "Nouvelle Scène" ) );
				wxString l_sceneName = l_scene->GetName();

				try
				{
					l_scene->m_sceneId				= m_filesList->AddSceneToProject( l_scene );
					l_scene->m_sceneFileId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de scène"	) );
					l_scene->m_loadScriptFileId		= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script de load"	) );
					l_scene->m_unloadScriptFileId	= m_filesList->AddFolderToScene( l_sceneName, wxT( "Fichiers de script d'unload"	) );
					l_scene->m_dataFileId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas Graphiques (pour la 3D)"	) );
					l_scene->m_dataFolderId			= m_filesList->AddFolderToScene( l_sceneName, wxT( "Datas non Graphiques (autres)"	) );
					m_currentProject->AddScene( l_scene );
				}
				catch ( bool )
				{
					LogDebugMessage( wxT( "MainFrame :: OnNewScene - Probleme d'ajout" ) );
				}
			}
		}

		void MainFrame :: _onRemoveScene( wxCommandEvent & p_event )
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

		void MainFrame :: _onEditProject( wxCommandEvent & p_event )
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
				_initEditLists();

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
				_musinate( m_currentProject, false, false );
				_musinateSceneEditor( l_project );
				_initElypse( l_project, _T( "Edition de TrollScene" ), true );
				//		delete l_project;
			}
			else
			{
				m_menuEditProject->SetItemLabel( _T( "Editer Scène\tF9" ) );
			}
		}

		void MainFrame :: _onEditOverlays( wxCommandEvent & p_event )
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
				_initEditLists();

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
				_musinate( m_currentProject, false, false );
				_musinateOverlayEditor( l_project );
				_initElypse( l_project, _T( "Edition des overlays" ), false );
				delete l_project;
			}
			else
			{
				m_menuEditOverlays->SetItemLabel( _T( "Editer Overlays\tF10" ) );
				m_currentProject->GetMainScene()->FlushObjects();
			}
		}

		void MainFrame :: _onTestProject( wxCommandEvent & p_event )
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
				_musinate( m_currentProject );
				_initElypse( m_currentProject, _T( "Test de " ) + m_currentProject->GetName(), false );
			}
			else
			{
				m_menuTestProject->SetItemLabel( _T( "Tester Projet\tF11" ) );
			}
		}

		void MainFrame :: _onSize( wxSizeEvent & p_event )
		{
			if ( m_tbar )
			{
				LayoutChildren();
			}
			else
			{
				p_event.Skip();
			}

			_resize();
		}

		void MainFrame :: _onShow( wxShowEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnShow( p_event );
			}
		}

		void MainFrame :: _onSetFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnSetFocus( p_event );
			}
		}

		void MainFrame :: _onKillFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnKillFocus( p_event );
			}
		}

		void MainFrame :: _onActivate( wxActivateEvent & p_event )
		{
			if ( m_elypseCtrl != NULL )
			{
				m_elypseCtrl->OnActivate( p_event );
			}
		}

		void MainFrame :: _onNotebook( wxNotebookEvent & p_event )
		{
			int l_idx = p_event.GetSelection();
			wxBookCtrlBase  * l_book = wx_static_cast( wxBookCtrlBase *, p_event.GetEventObject() );
			m_currentPage = l_book->GetPageText( l_idx );

			//	std::cout << "MainFrame :: _onNotebook - " << m_currentPage << std::endl;

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

		void MainFrame :: _onNotebookMiddleDown( wxMouseEvent & p_event )
		{
			std::cout << "MainFrame :: _onNotebookMiddleDown\n";
		}

		void MainFrame :: _onCloseFile( wxCommandEvent & p_event )
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

		void MainFrame :: _onEditTimeLine( wxCommandEvent & p_event )
		{
			//	std::cout << "MainFrame :: _onEditTimeLine\n";
			if ( !m_currentProject )
			{
				LogDebugMessage( wxT( "Aucun projet sélectionné" ) );
				return;
			}

			CloseElypse();
			m_editingTimeLines = ! m_editingTimeLines;

			if ( m_editingTimeLines )
			{
				_initEditLists();

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
				_musinate( m_currentProject, true );
				_initElypse( m_currentProject, _T( "Edition de la Timeline" ), false, -120 );
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

		void MainFrame :: _onVerticalSplitterPositionChanged( wxSplitterEvent & p_event )
		{
			if ( m_resizing )
			{
				p_event.Skip();
				return;
			}

			m_logsHeight = GetClientSize().y - p_event.GetSashPosition();
			_resizeLogs();
			_resizeTrees();
			_resizePanels();
			p_event.Skip();
		}

		void MainFrame :: _onVerticalSplitterPositionChanging( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame :: _onVerticalSplitterDClick( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame :: _onVerticalSplitterUnsplitEvent( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame :: _onHorizontalSplitterPositionChanged( wxSplitterEvent & p_event )
		{
			if ( m_resizing )
			{
				p_event.Skip();
				return;
			}

			m_treesWidth = p_event.GetSashPosition();
			_resizeTrees();
			_resizePanels();
			p_event.Skip();
		}

		void MainFrame :: _onHorizontalSplitterPositionChanging( wxSplitterEvent & p_event )
		{
			/*
				m_treesWidth = p_event.GetSashPosition();

				_resizeTrees();
				_resizePanels();
			/**/
			p_event.Skip();
		}

		void MainFrame :: _onHorizontalSplitterDClick( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}

		void MainFrame :: _onHorizontalSplitterUnsplitEvent( wxSplitterEvent & p_event )
		{
			p_event.Skip();
		}


		void MainFrame :: _onListFolder( wxCommandEvent & p_event )
		{
			ListFolder();
		}

		void MainFrame :: _onProjectProperties( wxCommandEvent & p_event )
		{
			ProjectProperties * l_properties = new ProjectProperties( m_currentProject, this );
			l_properties->Show();
		}

		void MainFrame :: _onGoToLine( wxCommandEvent & p_event )
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

		void MainFrame :: _onSearch( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->Search( m_search->GetValue() );
		}

		void MainFrame :: _onSearchNext( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchNext();
		}

		void MainFrame :: _onSearchPrevious( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == NULL || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			TextPanel * l_editCtrl = static_cast <TextPanel *>( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchPrevious();
		}

		void MainFrame :: _onReplace( wxCommandEvent & p_event )
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

		void MainFrame :: _initEditLists()
		{
			m_objectsListContainer = new wxPanel( m_treeTabsContainer, ObjectsList, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
			m_treeTabsContainer->InsertPage( 2, m_objectsListContainer, _T( "Objets" ), false );
			wxSize l_size = m_objectsListContainer->GetClientSize();
			l_size.y /= 2;
			m_objectsList = new ObjectsTree( m_objectsListContainer, wxDefaultPosition, l_size, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_objectsList->InitObjectList();
			m_objectInfosContainer = new wxScrolledWindow( m_objectsListContainer, PropertiesList, wxPoint( 5, l_size.y + 5 ), wxSize( l_size.x - 10, l_size.y - 10 ) );
		}

		void MainFrame :: _initElypse( Project * p_project, const wxString & p_title, bool p_edit, int p_adjustY )
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

		void MainFrame :: _onTreeNewScene( wxCommandEvent & p_event )
		{
			if ( m_currentProject == NULL )
			{
				return;
			}

			NewSceneFrame * l_frame = new NewSceneFrame( this );
			l_frame->Show();
		}

		void MainFrame :: _onEditSceneDependencies( wxCommandEvent & p_event )
		{
			if ( m_currentProject == NULL )
			{
				return;
			}

			m_sceneDependencies->SetScene( m_filesList->GetSelectedScene() );
			m_sceneDependencies->Show();
		}

		void MainFrame :: LayoutChildren()
		{
			if ( m_tbar )
			{
				m_tbar->SetSize( 0, 0, wxDefaultCoord, GetClientSize().y );
				return;
			}
		}

		void MainFrame :: PopulateToolbar( wxToolBarBase * toolBar )
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
			toolBar->AddTool( IdNew,		wxT( "Nouveau" ),		toolBarBitmaps[Tool_new],	wxT( "Nouveau projet" ) );
			toolBar->AddTool( OpenProject,	wxT( "Ouvrir" ),		toolBarBitmaps[Tool_open],	wxT( "Ouvrir un projet" ) );
			toolBar->AddTool( saveProject,	wxT( "Enregistrer" ),	toolBarBitmaps[Tool_save],	wxT( "Enregistrer le projet" ) );
			//	toolBar->AddTool( wxID_COPY,	wxT( "Copier" ),		toolBarBitmaps[Tool_copy],	wxT( "Copier") );
			//	toolBar->AddTool( wxID_CUT,		wxT( "Couper" ),		toolBarBitmaps[Tool_cut],	wxT( "Couper") );
			//	toolBar->AddTool( wxID_PASTE,	wxT( "Coller" ),		toolBarBitmaps[Tool_paste], wxT( "Coller") );
			toolBar->AddSeparator();		/*****************************************************/
			m_search = new wxSearchCtrl( toolBar, IdSearch, wxEmptyString, wxDefaultPosition, wxSize( 120, wxDefaultCoord ), wxBORDER_SIMPLE );
			toolBar->AddControl( m_search );
			toolBar->Realize();
			toolBar->SetRows( !( toolBar->IsVertical() ) ? m_rows : 10 / m_rows );
		}

		void MainFrame :: SetProject( const wxString & p_name, const wxString & p_path, const wxString & p_mainSceneName,
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
				LogDebugMessage( wxT( "MainFrame :: SetProject - Probleme d'ajout" ) );
				return;
			}

			if ( m_editText )
			{
				m_editText->GetText().Clear();
			}
		}

		wxString MainFrame :: CreateMain( Project * p_project, bool p_writeScripts )
		{
			wxString l_mainFileName;

			if ( p_project == NULL )
			{
				return l_mainFileName;
			}

			l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			wxString l_line;

			if ( p_project->GetBackgroundType() == bgColour )
			{
				wxColour * l_colour = p_project->GetBackgroundColour();
				LogOutMessage( l_colour->GetAsString() );
				l_line.Printf( wxT( "background_colour %f %f %f %f\n\n" ),
							   static_cast <int>( l_colour->Red() ) / 255.0,
							   static_cast <int>( l_colour->Green() ) / 255.0,
							   static_cast <int>( l_colour->Blue() ) / 255.0,
							   static_cast <int>( l_colour->Alpha() ) / 255.0 );
				l_line.Replace( wxT( "," ), wxT( "." ), true );
			}
			else
			{
				l_line = wxT( "background_colour 0.0 0.0 0.0 0.5 \n\n" );
			}

			l_textStream.WriteString( l_line );
			SceneMap l_scenes = p_project->GetScenes();
			SceneMap::iterator l_it = l_scenes.begin();
			wxString l_sceneName;
			TrollScene * l_scene;
			size_t l_index;
			wxString l_fileName;
			TrollFileArray l_files;
			wxArrayString l_neededScenes;

			while ( l_it != l_scenes.end() )
			{
				l_scene = l_it->second;
				l_sceneName = l_scene->GetName();
				l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
				l_textStream.WriteString( l_line );
				//		if ( ! l_scene->IsNeeded())
				//		{
				l_textStream.WriteString( wxT( "\tmuse_file " ) + l_scene->GetName() + wxT( ".muse\n" ) );
				//		}
				l_files = l_scene->m_dataFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
					l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
					l_textStream.WriteString( wxT( "\tdata_file " ) + l_fileName + wxT( ".zip\n" ) );
				}

				l_files = l_scene->m_sceneFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
					l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
					l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
				}

				if ( p_writeScripts )
				{
					l_files = l_scene->m_loadScriptFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
						l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
						l_textStream.WriteString( wxT( "\tload_script_file " ) + l_fileName + wxT( "\n" ) );
					}

					l_files = l_scene->m_unloadScriptFiles;

					for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
					{
						l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
						l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
						l_textStream.WriteString( wxT( "\tunload_script_file " ) + l_fileName + wxT( "\n" ) );
					}
				}

				l_files = l_scene->m_museFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
					l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
					l_textStream.WriteString( wxT( "\tmuse_file " ) + l_fileName + wxT( "\n" ) );
				}

				/*
						l_neededScenes = l_scene->GetNeededScenes();
						for (size_t i = 0 ; i < l_neededScenes.size() ; i++)
						{
							l_textStream.WriteString( wxT( "\tmuse_file " ) + l_neededScenes[i] + wxT( ".muse\n"));
						}
				*/
				l_textStream.WriteString( wxT( "}\n\n" ) );
				++l_it;
			}

			l_scene = p_project->GetMainScene();

			if ( ! l_scene )
			{
				l_fileOutput.Close();
				return l_mainFileName;
			}

			l_sceneName = l_scene->GetName();
			l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
			l_textStream.WriteString( l_line );
			l_textStream.WriteString( wxT( "\tmuse_file " ) + p_project->GetName() + wxT( ".muse\n" ) );
			l_files = l_scene->m_dataFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tdata_file " ) + l_fileName + wxT( ".zip\n" ) );
			}

			l_files = l_scene->m_sceneFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
			}

			if ( p_writeScripts )
			{
				l_files = l_scene->m_loadScriptFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
					std::cout << "l_index : " << l_index << std::endl;
					l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
					l_textStream.WriteString( wxT( "\tload_script_file " ) + l_fileName + wxT( "\n" ) );
				}

				l_files = l_scene->m_unloadScriptFiles;

				for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
				{
					l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
					l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
					l_textStream.WriteString( wxT( "\tunload_script_file " ) + l_fileName + wxT( "\n" ) );
				}
			}

			l_files = l_scene->m_museFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tmuse_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_textStream.WriteString( wxT( "}\n\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + p_project->GetMainScene()->GetName() );
			l_fileOutput.Close();
			return l_mainFileName;
		}

		wxString MainFrame :: CreateSceneEditorMain( Project * p_project )
		{
			wxString l_mainFileName;

			if ( p_project == NULL )
			{
				return l_mainFileName;
			}

			l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			wxString l_line;

			if ( p_project->GetBackgroundType() == bgColour )
			{
				wxColour * l_colour = p_project->GetBackgroundColour();
				l_line.Printf( wxT( "background_colour %f %f %f %f\n\n" ),
							   int( l_colour->Red()	) / 255.0,
							   int( l_colour->Green()	) / 255.0,
							   int( l_colour->Blue()	) / 255.0,
							   int( l_colour->Alpha()	) / 255.0 );
				l_line.Replace( wxT( "," ), wxT( "." ), true );
			}
			else
			{
				l_line = wxT( "background_colour 0.0 0.0 0.0 0.5 \n\n" );
			}

			l_textStream.WriteString( l_line );
			size_t l_index;
			wxString l_fileName;
			TrollFileArray l_files;
			TrollScene * l_scene = p_project->GetMainScene();

			if ( l_scene == NULL )
			{
				l_fileOutput.Close();
				return l_mainFileName;
			}

			wxString l_sceneName = l_scene->GetName();
			l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
			l_textStream.WriteString( l_line );
			l_files = l_scene->m_dataFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tdata_file " ) + l_fileName + wxT( ".zip\n" ) );
			}

			l_files = l_scene->m_sceneFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_files = l_scene->m_loadScriptFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				std::cout << "l_index : " << l_index << std::endl;
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tload_script_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_files = l_scene->m_unloadScriptFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tunload_script_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_textStream.WriteString( wxT( "}\n\n" ) );
			l_scene = m_currentProject->GetMainScene();
			l_sceneName = l_scene->GetName();
			l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
			l_textStream.WriteString( l_line );
			l_textStream.WriteString( wxT( "\tmuse_file file://" ) + m_currentProject->GetPath() + m_currentProject->GetName() + wxT( ".muse\n" ) );
			l_files = l_scene->m_dataFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tdata_file " ) + l_fileName + wxT( ".zip\n" ) );
			}

			l_files = l_scene->m_sceneFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_textStream.WriteString( wxT( "}\n\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + m_currentProject->GetMainScene()->GetName() + wxT( "\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + p_project->GetMainScene()->GetName() );
			l_fileOutput.Close();
			return l_mainFileName;
		}

		wxString MainFrame :: CreateOverlayEditorMain( Project * p_project )
		{
			wxString l_mainFileName;

			if ( p_project == NULL )
			{
				return l_mainFileName;
			}

			l_mainFileName = p_project->GetPath() + wxT( "main.emcfg" );
			wxFileOutputStream l_fileOutput( l_mainFileName );
			wxTextOutputStream l_textStream( l_fileOutput );
			wxString l_line;

			if ( p_project->GetBackgroundType() == bgColour )
			{
				wxColour * l_colour = p_project->GetBackgroundColour();
				l_line.Printf( wxT( "background_colour %f %f %f %f\n\n" ),
							   static_cast <int>( l_colour->Red() ) / 255.0,
							   static_cast <int>( l_colour->Green() ) / 255.0,
							   static_cast <int>( l_colour->Blue() ) / 255.0,
							   static_cast <int>( l_colour->Alpha() ) / 255.0 );
				l_line.Replace( wxT( "," ), wxT( "." ), true );
			}
			else
			{
				l_line = wxT( "background_colour 0.0 0.0 0.0 0.5 \n\n" );
			}

			l_textStream.WriteString( l_line );
			size_t l_index;
			wxString l_fileName;
			TrollFileArray l_files;
			TrollScene * l_scene = p_project->GetMainScene();

			if ( l_scene == NULL )
			{
				l_fileOutput.Close();
				return l_mainFileName;
			}

			wxString l_sceneName = l_scene->GetName();
			l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
			l_textStream.WriteString( l_line );
			l_files = l_scene->m_sceneFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				std::cout << "l_index : " << l_index << std::endl;
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_files = l_scene->m_loadScriptFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				std::cout << "l_index : " << l_index << std::endl;
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tload_script_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_textStream.WriteString( wxT( "}\n\n" ) );
			l_scene = m_currentProject->GetMainScene();
			l_sceneName = l_scene->GetName();
			l_line = wxT( "scene " ) + l_sceneName + wxT( "\n{\n" );
			l_textStream.WriteString( l_line );
			l_textStream.WriteString( wxT( "\tmuse_file file://" ) + m_currentProject->GetPath() + m_currentProject->GetName() + wxT( ".muse\n" ) );
			l_files = l_scene->m_dataFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tdata_file " ) + l_fileName + wxT( ".zip\n" ) );
			}

			l_files = l_scene->m_sceneFiles;

			for ( unsigned int i = 0 ; i < l_files.size() ; i++ )
			{
				l_index = min( l_files[i]->FileName.find_last_of( '\\' ) , l_files[i]->FileName.find_last_of( '/' ) );
				l_fileName = l_files[i]->FileName.substr( l_index + 1, l_files[i]->FileName.size() );
				l_textStream.WriteString( wxT( "\tscene_file " ) + l_fileName + wxT( "\n" ) );
			}

			l_textStream.WriteString( wxT( "}\n\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + m_currentProject->GetMainScene()->GetName() + wxT( "\n" ) );
			l_textStream.WriteString( wxT( "start_scene " ) + p_project->GetMainScene()->GetName() );
			l_fileOutput.Close();
			return l_mainFileName;
		}

		void MainFrame :: OpenSelectedFile()
		{
			wxTreeItemId l_itemId = m_filesList->GetSelected();
			TrollFile * l_file = m_filesList->GetSelectedScene()->GetFile( l_itemId );

			if ( l_file && l_file->Saved /*&& !l_file->Open*/ )
			{
				wxPanel * l_editTextContainer = new wxPanel( m_mainTabsContainer, wxID_ANY, wxPoint( 0, 0 ) );
				m_mainTabsContainer->AddPage( l_editTextContainer, l_file->FileName, true );
				l_editTextContainer->SetSize( 0, 22, m_mainTabsContainer->GetClientSize().x, m_mainTabsContainer->GetClientSize().y - 22 );
				m_editText = new wxStcTextEditor( m_pStcContext, l_editTextContainer, wxID_ANY, wxPoint( 0, 0 ), l_editTextContainer->GetClientSize() );
				wxString l_path = GetCurrentProject()->GetPath() + ( l_file->m_scene->IsMainScene() ? wxString() : wxString( l_file->m_scene->GetName() ) + wxFileName::GetPathSeparator() );
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

		void MainFrame :: LogDebugMessage( const wxString & p_message )
		{
			//	std::cout << p_message << "\n";
			wxLog::SetActiveTarget( m_logDebug );
			wxLogMessage( p_message );
			m_debugLog->SetSelection( m_debugLog->GetLastPosition() - 2, m_debugLog->GetLastPosition() - 1 );
		}

		void MainFrame :: LogOutMessage( const wxString & p_message )
		{
			//	std::cout << p_message << "\n";
			wxLog::SetActiveTarget( m_logCompilation );
			wxLogMessage( p_message );
			m_compilationLog->SetSelection( m_compilationLog->GetLastPosition() - 2, m_compilationLog->GetLastPosition() - 1 );
		}

		void MainFrame :: CloseProject()
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
				_initialise();
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

		void MainFrame :: HideObjectInfos()
		{
			if ( m_viewedObject != NULL )
			{
				m_viewedObject->HideProperties();
				m_viewedObject = NULL;
			}
		}

		void MainFrame :: ShowObjectInfos( TrollObject * p_object )
		{
			//	std::cout << "MainFrame :: ShowObjectInfos\n";
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

		float MainFrame :: GetMuseHeight()
		{
			if ( m_elypseCtrl != NULL )
			{
				return m_elypseCtrl->GetMuseHeight();
			}

			return 0;
		}

		float MainFrame :: GetMuseWidth()
		{
			if ( m_elypseCtrl != NULL )
			{
				return m_elypseCtrl->GetMuseWidth();
			}

			return 0;
		}

		void MainFrame :: CreateNewContinuousEventFrame( TrollContinuousEvent * p_event )
		{
		}

		void MainFrame :: AddPonctualEvent( TrollSequence * p_sequence, float p_fireTime )
		{
			StringArray l_tmp;
			EMuse::Sequences::BasePonctualEvent * l_museEvent = ScriptEngine::GetContext()->sequenceManager->CreatePonctualEvent( "Overlay_Hide", l_tmp );
			TrollPonctualEvent * l_event = new TrollPonctualEvent( p_sequence, l_museEvent, wxT( "overlay" ), wxT( "" ), p_fireTime, wxT( "" ), wxT( "Overlay_Hide" ), wxT( "" ) );
			p_sequence->AddPonctualEvent( l_event );
			SequencePanel * l_panel = reinterpret_cast <SequencePanel *>( m_timelineEditor->GetSequencePanel( p_sequence->TrollObject::GetName() )->GetParent() );
			l_panel->AddPonctualEvent( l_event, p_fireTime );
		}

		void MainFrame :: AddContinuousEvent( TrollSequence * p_sequence, float p_fireTime )
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

		void MainFrame :: OpenProjectFile( const wxString & p_fileName )
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
					//			wxFileInputStream * l_input = new wxFileInputStream( p_fileName);
					//			wxTextInputStream * l_textStream = new wxTextInputStream( * l_input);
					//			m_currentProject->Load( l_input, l_textStream, l_path, m_filesList);
					//			m_currentProject->SetProjectFileName( p_fileName);
					//			m_currentProject->SetSaved( true);
					//			delete l_textStream;
					//			delete l_input;
					m_currentProject->Load( p_fileName, m_filesList );
					//			m_currentProject->SetProjectPath( l_path);
				}
				else if ( p_fileName != m_currentProject->GetProjectFileName() )
				{
					wxTreeItemId l_root = m_filesList->GetRootItem();
					delete m_currentProject;
					m_currentProject = new Project();
					//			wxFileInputStream * l_input = new wxFileInputStream( p_fileName);
					//			wxTextInputStream * l_textStream = new wxTextInputStream( * l_input);
					//			m_currentProject->Load(l_input, l_textStream, l_path, m_filesList);
					//			m_currentProject->SetProjectFileName( p_fileName);
					//			m_currentProject->SetSaved( true);
					//			delete l_textStream;
					//			delete l_input;
					m_currentProject->Load( p_fileName, m_filesList );
					//			m_currentProject->SetProjectPath( l_path);
				}
				else
				{
					wxMessageBox( _T( "Impossible d'ajouter ce projet, un projet avec le même nom existe déjà" ),
								  _T( "Troll Editor Error" ), wxOK | wxICON_ERROR, NULL );
				}
			}
		}

		void MainFrame :: SaveSceneFiles( TrollScene * p_scene )
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

		void MainFrame :: SaveScene( TrollScene * p_scene )
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

		void MainFrame :: SaveProject( Project * p_project )
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

			LogDebugMessage( wxT( "MainFrame :: SaveProject - " ) + l_fileName );

			if ( ! l_fileName.empty() )
			{
				LogDebugMessage( wxT( "MainFrame :: OnSaveProject - " ) + l_fileName );
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

		void MainFrame :: SaveFileAs( TrollFile * p_file )
		{
			if ( p_file == NULL )
			{
				LogDebugMessage( wxT( "MainFrame :: SaveFileAs - Nothing to save" ) );
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

		void MainFrame :: SaveFile( TrollFile * p_file, const wxString & p_fileName )
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

		bool MainFrame :: ZipFolder( const wxString & p_folderPath, const wxString & p_zipFileName )
		{
			/*
				CkZip l_zip;
				if ( ! l_zip.UnlockComponent("Anything for 30-day trial."))
				{
					LogDebugMessage( wxString( "MainFrame :: ZipFolder - ") + l_zip.lastErrorText());
					return false;
				}

				l_zip.NewZip( p_zipFileName.c_str());

				if ( ! l_zip.AppendFiles( p_folderPath.c_str(), true, 0))
				{
					LogDebugMessage( wxString( "MainFrame :: ZipFolder - ") + l_zip.lastErrorText());
					return false;
				}

				l_zip.SetCompressionLevel( 9);

				if ( ! l_zip.WriteZipAndClose( 0))
				{
					LogDebugMessage( wxString( "MainFrame :: ZipFolder - ") + l_zip.lastErrorText());
					return false;
				}
			*/
			return true;
		}

		void MainFrame :: CompileNeededScenes( const wxArrayString & p_scenes, TrollScriptCompiler * p_compiler, wxArrayString & p_compiledScenes )
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
					l_scriptNode = p_compiler->CompileScriptFile( l_file->CfgFile );
					l_compilationResult = wxString( l_file->CfgFile->GetName().c_str(), wxMBConvLibc() );
					l_compilationResult << wxT( " compiled - " ) << p_compiler->GetNumWarnings() << wxT( " warnings - " ) << p_compiler->GetNumErrors() << wxT( " errors" );
					LogOutMessage( l_compilationResult );
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

		void MainFrame :: Compile( TrollFile * p_file, TrollScene * p_scene )
		{
			m_compilationLog->Clear();
			m_logTabsContainer->SetSelection( 1 );

			if ( m_elypseContainer != NULL )
			{
				LogOutMessage( wxT( "Impossible de compiler pendant un test ou une édition" ) );
				return;
			}

			wxString l_wxPath = m_currentProject->GetPath() + ( p_file->m_scene->IsMainScene() ? wxString() : wxString( p_file->m_scene->GetName() ) + wxFileName::GetPathSeparator() );
			p_file->EditPanel->SaveFile( l_wxPath + p_file->FileName );

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

				l_scriptNode = l_compiler->CompileScriptFile( l_file->CfgFile );
				l_compilationResult = wxString( l_file->CfgFile->GetName().c_str(), wxMBConvLibc() );
				l_compilationResult << wxT( " compiled - " ) << l_compiler->GetNumWarnings() << wxT( " warnings - " ) << l_compiler->GetNumErrors() << wxT( " errors" );
				LogOutMessage( l_compilationResult );

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

					l_scriptNode = l_compiler->CompileScriptFile( l_file->CfgFile );
					l_compilationResult = wxString( l_file->CfgFile->GetName().c_str(), wxMBConvLibc() );
					l_compilationResult << wxT( " compiled - " ) << l_compiler->GetNumWarnings() << wxT( " warnings - " ) << l_compiler->GetNumErrors() << wxT( " errors" );
					LogOutMessage( l_compilationResult );

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

		void MainFrame :: ListFolder()
		{
			FolderList * l_list = new FolderList( m_currentProject->GetPath() + m_filesList->GetSelectedScene()->GetFile( m_filesList->GetSelected() )->Folder, this );
			l_list->Show();
		}

		void MainFrame :: CloseElypse()
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

		void MainFrame :: CreateFunctionsGroup( const wxString & p_name )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateTitreFunction( p_name );
			}
		}

		void MainFrame :: CreateFreeFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreaFunc( p_funcName, p_returnValue, p_params );
			}
		}

		void MainFrame :: CreateClassFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			if ( m_functionsList )
			{
				m_functionsList->ClassFunc( p_funcName, p_returnValue, p_params );
			}
		}

		void MainFrame :: CreateConstantsGroup( const wxString & p_name )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateTitreConstant( p_name );
			}
		}

		void MainFrame :: CreateConstant( const wxString & p_constName, VariableBaseType p_type )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateConstant( p_constName, p_type );
			}
		}
	}
}