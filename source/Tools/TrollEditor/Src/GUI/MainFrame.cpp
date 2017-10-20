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

#include <wx/aui/auibar.h>
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

#include "GUI/AboutFrame.h"
#include "GUI/LogCtrl.h"
#include "GUI/LogListBox.h"
#include "GUI/NewProjectFrame.h"
#include "GUI/ProjectFrame.h"
#include "GUI/PropertyDialog.h"

#include "GUI/Aui/AuiDockArt.h"
#include "GUI/Aui/AuiTabArt.h"
#include "GUI/Aui/AuiToolBarArt.h"

#include "ProjectSaver.h"
#include "TrollEditorApplication.h"
#include "TrollLogs.h"

#include "Project/Project.h"
#include "Project/File.h"

#include "xpm/troll_editor.xpm"
#include "xpm/BMPNew.xpm"
#include "xpm/BMPOpen.xpm"
#include "xpm/BMPSave.xpm"

#include <File.h>
#include <StringUtils.h>

namespace Troll
{
	using namespace ProjectComponents;
	using namespace ProjectComponents::Objects2D;

	namespace GUI
	{
		namespace
		{
			wxString const TE_TEXT_WELCOME = _( "Welcome to Troll Editor!" );
			wxString const TE_TEXT_NO_SELECTED_PROJECT = _( "No selected project" );
			wxString const TE_TEXT_SELECT_PROJECT_FILE = _( "Choose a project to open" );
			wxString const TE_TEXT_PROJECT_FILES = _( "Troll Editor Project files" );
			wxString const TE_TEXT_NEW_PROJECT = _( "New Project" );
			wxString const TE_TEXT_CANT_COPY_FOLDER = _( "Can not copy directory" );
			wxString const TE_TEXT_FOLDER_DOES_NOT_EXIST = _( "it does not exist." );
			wxString const TE_TEXT_CANT_CREATE = _( "Could not create" );
			wxString const TE_TEXT_TO = _( "to" );
			wxString const TE_TEXT_CANT_COPY_FILE = _( "Can not copy file" );

			wxString const TE_EXIT = _( "Exit" );
			wxString const TE_ABOUT = _( "About..." );

			wxString const TE_MENU_FILE = _( "File" );
			wxString const TE_MENU_PROJECT = _( "Project" );
			wxString const TE_MENU_EDIT = _( "Edit" );
			wxString const TE_MENU_HELP = _( "Help" );

			wxString const TE_PROJECT_NEW = _( "New Project" );
			wxString const TE_PROJECT_OPEN = _( "Open Project" );
			wxString const TE_PROJECT_SAVE = _( "Save Project" );
			wxString const TE_PROJECT_CLOSE = _( "Close Project" );
			wxString const TE_PROJECT_TEST = _( "Test Project" );
			wxString const TE_PROJECT_COMPILE = _( "Compile" );
			wxString const TE_PROJECT_MUSINATE = _( "Musinate" );
			wxString const TE_PROJECT_PROPERTIES = _( "Properties" );
			wxString const TE_PROJECT_TEST_END = _( "End Project Test" );
			wxString const TE_PROJECT_TESTING = _( "Project Testing" );

			wxString const TE_FILE_OPEN = _( "Open File..." );
			wxString const TE_FILE_SAVE = _( "Save File" );
			wxString const TE_FILE_SAVE_AS = _( "Save File As..." );
			wxString const TE_FILE_GOTO_LINE = _( "Go To Line..." );
			wxString const TE_FILE_FIND_NEXT = _( "Find Next..." );
			wxString const TE_FILE_FIND_PREVIOUS = _( "Find Previous..." );
			wxString const TE_FILE_REPLACE = _( "Replace..." );

			wxString const TE_SCENE_DEPENDENCIES = _( "Edit Dependencies..." );

			wxString const TE_EDIT_TIMELINE = _( "Edit TimeLine" );
			wxString const TE_EDIT_OVERLAYS = _( "Edit Overlays" );
			wxString const TE_EDIT_SCENE = _( "Edit Scene" );
			wxString const TE_EDIT_END = _( "End Edition" );

			wxString const TE_LOG_DEBUG = _( "Debug" );
			wxString const TE_LOG_MESSAGES = _( "Messages" );
			wxString const TE_LOG_ERRORS = _( "Errors" );

			wxString const TE_SAVE_CONFIRM = _( "Would you like to save your modifications" );
			wxString const TE_CONFIRM = _( "Confirm" );

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
					std::cerr << make_string( TE_TEXT_CANT_COPY_FOLDER ) << " " << sFrom << ", " << make_string( TE_TEXT_FOLDER_DOES_NOT_EXIST ) << std::endl;
					return false;
				}

				if ( !wxDirExists( sTo ) )
				{
					if ( !wxFileName::Mkdir( sTo, 0777, wxPATH_MKDIR_FULL ) )
					{
						std::cerr << make_string( TE_TEXT_CANT_CREATE ) << " " << sFrom << std::endl;
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
							std::cerr << make_string( TE_TEXT_CANT_COPY_FILE ) << " " << sFileFrom << ", " << make_string( TE_TEXT_TO ) << " " << sFileTo << std::endl;
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

		enum ToolBoxImages
		{
			Tool_new,
			Tool_open,
			Tool_save,
			Tool_copy,
			Tool_cut,
			Tool_paste,
			Tool_Max
		};

		long const c_toolbarStyle = wxTB_FLAT | wxTB_DOCKABLE | wxTB_TEXT;

		MainFrame::MainFrame( bool p_debug, wxString const & title, int x, int y, int w, int h )
			: wxFrame{ nullptr, wxID_ANY, title, wxPoint( x, y ), wxSize( w, h ) }
			, m_debug{ p_debug }
		{
			wxStandardPathsBase & l_paths = wxStandardPaths::Get();
			Path l_installPath = make_string( l_paths.GetExecutablePath() );
			Path l_userPath = make_string( l_paths.GetUserDataDir() );
			l_installPath = l_installPath.GetPath().GetPath() / "share" / "TrollEditor";
			m_appPath = make_wxString( l_userPath );
			wxCopyDir( make_wxString( l_installPath ), m_appPath );

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
		}

		void MainFrame::SetProject( ProjectSPtr p_project )
		{
			m_projectFrame = new ProjectFrame{ m_projectFrameContainer, p_project };
			auto l_sizer = new wxBoxSizer{ wxHORIZONTAL };
			l_sizer->Add( m_projectFrame, wxSizerFlags( 1 ).Expand() );
			l_sizer->SetSizeHints( m_projectFrameContainer );
			m_projectFrameContainer->SetSizer( l_sizer );
		}

		void MainFrame::LogDebug( wxString const & p_message )
		{
			std::clog << p_message << std::endl;
		}

		void MainFrame::LogMessage( wxString const & p_message )
		{
			std::cout << p_message << std::endl;
		}

		void MainFrame::LogError( wxString const & p_message )
		{
			std::cerr << p_message << std::endl;
		}

		void MainFrame::CloseProject()
		{
			if ( m_projectFrame )
			{
				m_projectFrame->Close();
				delete m_projectFrame;
				m_projectFrame = nullptr;
			}
		}

		void MainFrame::OpenProjectFile( wxString const & p_fileName )
		{
			if ( !p_fileName.empty() )
			{
				CloseProject();
				m_projectFrame = new ProjectFrame{ m_projectFrameContainer, p_fileName };
				auto l_sizer = new wxBoxSizer{ wxHORIZONTAL };
				l_sizer->Add( m_projectFrame, wxSizerFlags( 1 ).Expand() );
				l_sizer->SetSizeHints( m_projectFrameContainer );
				m_projectFrameContainer->SetSizer( l_sizer );
			}
		}

		ProjectSPtr MainFrame::GetProject()const
		{
			ProjectSPtr l_return;

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->GetProject();
			}

			return l_return;
		}

		bool MainFrame::IsTestingScene()const
		{
			bool l_return{ false };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->IsTestingScene();
			}

			return l_return;
		}

		bool MainFrame::IsEditingScene()const
		{
			bool l_return{ false };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->IsEditingScene();
			}

			return l_return;
		}

		bool MainFrame::IsEditingOverlays()const
		{
			bool l_return{ false };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->IsEditingOverlays();
			}

			return l_return;
		}

		bool MainFrame::IsEditingTimeLine()const
		{
			bool l_return{ false };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->IsEditingTimeLine();
			}

			return l_return;
		}

		ObjectsTree * MainFrame::GetObjectsList()const
		{
			ObjectsTree * l_return{ nullptr };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->GetObjectsList();
			}

			return l_return;
		}

		Time::TimeLinePanel * MainFrame::GetTimelineEditor()const
		{
			Time::TimeLinePanel * l_return{ nullptr };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->GetTimelineEditor();
			}

			return l_return;
		}

		Time::TimeLineContainer * MainFrame::GetTimeLineContainer()const
		{
			Time::TimeLineContainer * l_return{ nullptr };

			if ( m_projectFrame )
			{
				l_return = m_projectFrame->GetTimeLineContainer();
			}

			return l_return;
		}

		void MainFrame::GotoFileLine( wxString const & p_file, uint32_t p_line )
		{
			if ( m_projectFrame )
			{
				auto l_file = m_projectFrame->GetFile( p_file );

				if ( l_file )
				{
					m_projectFrame->SelectFileTab( l_file->IdOnglet );
					std::clog << "LogCtrl::OnDoubleClick - " << l_file->IdOnglet << "\n";
					wxStcTextEditor * l_edit = l_file->EditPanel;
					l_edit->SetFocus();
					l_edit->GotoLine( p_line );
				}
			}
		}

		void MainFrame::DoPopulateToolbar()
		{
			std::array< wxBitmap, Tool_Max > l_bitmaps
			{
				wxBitmap( new_xpm ),
				wxBitmap( open_xpm ),
				wxBitmap( save_xpm ),
			};
			int l_width = l_bitmaps[Tool_new].GetWidth();
			int l_height = l_bitmaps[Tool_new].GetHeight();

			if ( !m_smallToolbar )
			{
				l_width *= 2;
				l_height *= 2;

				for ( auto & l_bitmap : l_bitmaps )
				{
					if ( l_bitmap.IsOk() )
					{
						l_bitmap = wxBitmap( l_bitmap.ConvertToImage().Scale( l_width, l_height ) );
					}
				}
			}

			auto toolBar = new wxAuiToolBar{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_PLAIN_BACKGROUND | wxAUI_TB_HORIZONTAL };
			toolBar->SetArtProvider( new AuiToolBarArt );
			toolBar->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			toolBar->SetForegroundColour( GuiCommon::PanelForegroundColour );
			toolBar->SetToolBitmapSize( wxSize( l_width, l_height ) );
			toolBar->AddTool( Menu_NewProject, TE_NEW, l_bitmaps[Tool_new], TE_PROJECT_NEW );
			toolBar->AddTool( Menu_OpenProject, TE_OPEN, l_bitmaps[Tool_open], TE_PROJECT_OPEN );
			toolBar->AddTool( Menu_SaveProject, TE_SAVE, l_bitmaps[Tool_save], TE_PROJECT_SAVE );
			//toolBar->AddTool( wxID_COPY, wxT( "Copier" ), l_bitmaps[Tool_copy], wxT( "Copier") );
			//toolBar->AddTool( wxID_CUT, wxT( "Couper" ), l_bitmaps[Tool_cut], wxT( "Couper") );
			//toolBar->AddTool( wxID_PASTE, wxT( "Coller" ), l_bitmaps[Tool_paste], wxT( "Coller") );
			toolBar->AddSeparator();		/*****************************************************/
			toolBar->Realize();
			//toolBar->SetRows( !( toolBar->IsVertical() ) ? m_rows : 10 / m_rows );
			m_manager.AddPane( toolBar, wxAuiPaneInfo().Name( wxT( "MainToolBar" ) ).ToolbarPane().Top().Row( 1 ).Dockable( false ).Gripper( false ) );
		}

		void MainFrame::DoInitialiseMainContainers()
		{
			wxSize l_size = GetClientSize();
			// main tabs (contains text editor, test panel...)
			m_projectFrameContainer = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE );
			m_projectFrameContainer->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_projectFrameContainer->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_manager.AddPane( m_projectFrameContainer, wxAuiPaneInfo()
				.CaptionVisible( false )
				.Center()
				.CloseButton( false )
				.Name( _( "Render" ) )
				.MinSize( l_size.x, l_size.y - m_logsHeight )
				.Layer( 0 )
				.Movable( false )
				.PaneBorder( false )
				.Dockable( false ) );
			//logs tabs (contains debug log, compilation log...)
			m_logTabsContainer = new wxAuiNotebook{ this, wxID_ANY };
			m_logTabsContainer->SetArtProvider( new AuiTabArt );
			m_manager.AddPane( m_logTabsContainer, wxAuiPaneInfo()
				.BottomDockable()
				.Bottom()
				.CaptionVisible( false )
				.Dock()
				.MinSize( 0, m_logsHeight )
				.Resizable( true )
				.Show() );
		}

		void MainFrame::DoInitialiseLogs()
		{
			auto && l_creator = [this]( wxString const & p_name, LogCtrl *& p_ctrl, wxLog *& p_log )
			{
				p_ctrl = new LogCtrl( m_logTabsContainer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER );
				p_ctrl->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
				p_ctrl->SetForegroundColour( GuiCommon::PanelForegroundColour );
				p_ctrl->SetFont( wxFont( 10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL ) );
				p_log = new wxLogListBox( p_ctrl );
				m_logTabsContainer->AddPage( p_ctrl, p_name, true );
			};

			if ( m_debug )
			{
				l_creator( TE_LOG_DEBUG, m_debugLog, m_logDebug );
			}

			l_creator( TE_LOG_MESSAGES, m_messageLog, m_logMessage );
			l_creator( TE_LOG_ERRORS, m_errorLog, m_logError );
		}

		void MainFrame::DoInitialise()
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

#if wxCHECK_VERSION( 2, 9, 0 )
			SetMinClientSize( wxSize( 800, 600 ) );
#else
			SetMinSize( wxSize( 800, 600 ) );
#endif

			m_manager.SetArtProvider( new AuiDockArt );
			m_manager.SetManagedWindow( this );

			SetIcon( wxIcon( troll_editor_xpm ) );
			SetBackgroundColour( wxColour( 255, 255, 255 ) );
			CreateStatusBar( 2 );
			SetStatusText( TE_TEXT_WELCOME );
			GetStatusBar()->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			GetStatusBar()->SetForegroundColour( GuiCommon::PanelForegroundColour );

			DoSetMenuBar();
			DoPopulateToolbar();

			DoInitialiseMainContainers();
			DoInitialiseLogs();

			DoCreateTreeWithDefStyle();

			m_manager.Update();
		}

		void MainFrame::DoSetMenuBar()
		{
			m_menuBar = new wxMenuBar( wxMB_DOCKABLE );
			m_menuBar->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_menuBar->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_menuFile = new wxMenu;
			m_menuFile->Append( Menu_NewProject, wxString() << TE_PROJECT_NEW );
			m_menuFile->Append( Menu_OpenProject, wxString() << TE_PROJECT_OPEN );
			m_menuFile->Append( Menu_OpenFile, wxString() << TE_FILE_OPEN );
			m_menuFile->Append( Menu_CloseProject, wxString() << TE_PROJECT_CLOSE );
			m_menuFile->AppendSeparator();
			m_menuFile->Append( Menu_SaveProject, wxString() << TE_PROJECT_SAVE << wxT( "\tCtrl+T" ) );
			m_menuFile->Append( Menu_SaveFile, wxString() << TE_FILE_SAVE << wxT( "\tCtrl+S" ) );
			m_menuFile->Append( Menu_SaveFileAs, wxString() << TE_FILE_SAVE_AS << wxT( "\tCtrl+Shift+S" ) );
			m_menuFile->AppendSeparator();
			m_menuFile->Append( Menu_Quit, wxString() << TE_EXIT << wxT( "\tCtrl+Q" ) );
			m_menuEdition = new wxMenu;
			m_menuEdition->Append( Menu_GoToLine, wxString() << TE_FILE_GOTO_LINE << wxT( "\tCtrl+G" ) );
			m_menuEdition->Append( Menu_SearchNext, wxString() << TE_FILE_FIND_NEXT << wxT( "\tF3" ) );
			m_menuEdition->Append( Menu_SearchPrevious, wxString() << TE_FILE_FIND_PREVIOUS << wxT( "\tShift+F3" ) );
			m_menuEdition->Append( Menu_Replace, wxString() << TE_FILE_REPLACE << wxT( "\tCtrl+H" ) );
			m_menuProject = new wxMenu;
			m_menuProject->Append( Menu_SceneDependencies, wxString() << TE_SCENE_DEPENDENCIES << wxT( "\tF5" ) );
			m_menuProject->Append( Menu_CompileFile, wxString() << TE_PROJECT_COMPILE << wxT( "\tF6" ) );
			m_menuProject->Append( Menu_Musinate, wxString() << TE_PROJECT_MUSINATE << wxT( "\tF7" ) );
			m_menuEditTimeLines = m_menuProject->Append( Menu_TimeLine, wxString() << TE_EDIT_TIMELINE << wxT( "\tF8" ) );
			m_menuEditProject = m_menuProject->Append( Menu_EditScene, wxString() << TE_EDIT_SCENE << wxT( "\tF9" ) );
			m_menuEditOverlays = m_menuProject->Append( Menu_EditOverlays, wxString() << TE_EDIT_OVERLAYS << wxT( "\tF10" ) );
			m_menuTestProject = m_menuProject->Append( Menu_TestProject, wxString() << TE_PROJECT_TEST << wxT( "\tF11" ) );
			m_menuProject->Append( Menu_ProjectProperties, wxString() << TE_PROJECT_PROPERTIES << wxT( "\tCtrl+P" ) );
			m_menuHelp = new wxMenu;
			m_menuHelp->Append( Menu_About, wxString() << TE_ABOUT );
			m_menuBar->Append( m_menuFile, wxString() << TE_MENU_FILE );
			m_menuBar->Append( m_menuEdition, wxString() << TE_MENU_EDIT );
			m_menuBar->Append( m_menuProject, wxString() << TE_MENU_PROJECT );
			m_menuBar->Append( m_menuHelp, wxString() << TE_MENU_HELP );
			SetMenuBar( m_menuBar );
		}

		void MainFrame::DoCreateTreeWithDefStyle()
		{
			long style = wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS;
			wxMenuBar * mbar = GetMenuBar();
		}

		BEGIN_EVENT_TABLE( MainFrame, wxFrame )
			EVT_MENU( Menu_NewProject, MainFrame::OnNewProject )
			EVT_MENU( Menu_OpenProject, MainFrame::OnOpenProject )
			EVT_MENU( Menu_SaveProject, MainFrame::OnSaveProject )
			EVT_MENU( Menu_CloseProject, MainFrame::OnCloseProject )
			EVT_MENU( Menu_Quit, MainFrame::OnQuit )
			EVT_MENU( Menu_About, MainFrame::OnAbout )
			EVT_MENU( Menu_CompileFile, MainFrame::OnCompile )
			EVT_MENU( Menu_Musinate, MainFrame::OnMusinate )
			EVT_MENU( Menu_ProjectProperties, MainFrame::OnProjectProperties )
			EVT_MENU( Menu_TestProject, MainFrame::OnTestProject )
			EVT_MENU( Menu_EditOverlays, MainFrame::OnEditOverlays )
			EVT_MENU( Menu_EditScene, MainFrame::OnEditProject )
			EVT_MENU( Menu_TimeLine, MainFrame::OnEditTimeLine )
			EVT_MENU( Menu_SceneDependencies, MainFrame::OnSceneDependencies )
			EVT_CLOSE( MainFrame::OnClose )
			EVT_SIZE( MainFrame::OnSize )
		END_EVENT_TABLE()

		void MainFrame::OnOpenProject( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( TE_TEXT_SELECT_PROJECT_FILE
				, wxEmptyString
				, wxEmptyString
				, wxEmptyString
				, wxString() << TE_TEXT_PROJECT_FILES << wxT( " (*.teproj)|*.teproj" ) );
			OpenProjectFile( l_fileName );
		}

		void MainFrame::OnSaveProject( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				m_projectFrame->SaveProject();
			}
		}

		void MainFrame::OnNewProject( wxCommandEvent & p_event )
		{
			PropertyDialog l_dialog{ this, TE_TEXT_NEW_PROJECT, new NewProjectProperties };

			l_dialog.ShowModal();
		}

		void MainFrame::OnCloseProject( wxCommandEvent & p_event )
		{
			bool l_continue = true;

			if ( m_projectFrame->GetProject()->IsModified() )
			{
				int l_answer = wxMessageBox( TE_SAVE_CONFIRM, TE_CONFIRM, wxYES_NO | wxCANCEL, this );

				if ( l_answer == wxYES )
				{
					ProjectSaver::SaveProject( *m_projectFrame->GetProject() );
				}
				else if ( l_answer == wxCANCEL )
				{
					bool l_continue = false;
				}
			}

			if ( l_continue )
			{
				CloseProject();
			}
		}

		void MainFrame::OnQuit( wxCommandEvent & p_event )
		{
			delete m_logDebug;
			m_logDebug = nullptr;
			delete m_logMessage;
			m_logMessage = nullptr;
			delete m_logError;
			m_logError = nullptr;
			p_event.Skip();
		}

		void MainFrame::OnClose( wxCloseEvent & p_event )
		{
			if ( m_projectFrame )
			{
				m_projectFrame->Close();
				m_projectFrame->Destroy();
			}

			delete std::cout.rdbuf( m_oldCout );
			delete std::cerr.rdbuf( m_oldCerr );

			if ( m_oldClog )
			{
				delete std::clog.rdbuf( m_oldClog );
			}

			m_manager.UnInit();
			wxLog::SetActiveTarget( nullptr );
			delete m_logDebug;
			m_logDebug = nullptr;
			delete m_logMessage;
			m_logMessage = nullptr;
			delete m_logError;
			m_logError = nullptr;
			m_logTabsContainer = nullptr;
			m_debugLog = nullptr;
			m_messageLog = nullptr;
			m_errorLog = nullptr;

			p_event.Skip();
		}

		void MainFrame::OnCompile( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				LogDebug( TE_TEXT_NO_SELECTED_PROJECT );
				return;
			}

			m_projectFrame->CompileProject();
		}

		void MainFrame::OnAbout( wxCommandEvent & p_event )
		{
			AboutFrame l_frame{ this };
			l_frame.Show();
		}

		void MainFrame::OnMusinate( wxCommandEvent & )
		{
			if ( m_projectFrame )
			{
				LogDebug( TE_TEXT_NO_SELECTED_PROJECT );
			}
			else
			{
				m_projectFrame->MusinateProject();
			}
		}

		void MainFrame::OnSize( wxSizeEvent & p_event )
		{
			if ( m_tbar )
			{
				m_tbar->SetSize( 0, 0, wxDefaultCoord, GetClientSize().y );
			}
			else
			{
				p_event.Skip();
			}
		}

		void MainFrame::OnProjectProperties( wxCommandEvent & p_event )
		{
			if ( !m_projectFrame )
			{
				LogDebug( TE_TEXT_NO_SELECTED_PROJECT );
			}
			else
			{
				m_projectFrame->ShowProjectProperties();
			}
		}

		void MainFrame::OnEditTimeLine( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				if ( m_projectFrame->EditTimeLine() )
				{
					m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
					m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
					m_menuTestProject->SetItemLabel( TE_PROJECT_TEST + wxT( "\tF11" ) );
					m_menuEditTimeLines->SetItemLabel( TE_EDIT_END + wxT( "\tF8" ) );
				}
				else
				{
					m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
				}
			}
		}

		void MainFrame::OnEditProject( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				if ( m_projectFrame->EditProject() )
				{
					m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
					m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
					m_menuTestProject->SetItemLabel( TE_PROJECT_TEST + wxT( "\tF11" ) );
					m_menuEditProject->SetItemLabel( TE_EDIT_END + wxT( "\tF9" ) );
				}
				else
				{
					m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
				}
			}
		}

		void MainFrame::OnEditOverlays( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				if ( m_projectFrame->EditOverlays() )
				{
					m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
					m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
					m_menuTestProject->SetItemLabel( TE_PROJECT_TEST + wxT( "\tF11" ) );
					m_menuEditOverlays->SetItemLabel( TE_EDIT_END + wxT( "\tF10" ) );
				}
				else
				{
					m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
				}
			}
		}

		void MainFrame::OnTestProject( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				if ( m_projectFrame->TestProject() )
				{
					m_menuEditTimeLines->SetItemLabel( TE_EDIT_TIMELINE + wxT( "\tF8" ) );
					m_menuEditProject->SetItemLabel( TE_EDIT_SCENE + wxT( "\tF9" ) );
					m_menuEditOverlays->SetItemLabel( TE_EDIT_OVERLAYS + wxT( "\tF10" ) );
					m_menuTestProject->SetItemLabel( TE_PROJECT_TEST_END + wxT( "\tF11" ) );
				}
				else
				{
					m_menuTestProject->SetItemLabel( TE_PROJECT_TESTING + wxT( "\tF11" ) );
				}
			}
		}

		void MainFrame::OnSceneDependencies( wxCommandEvent & p_event )
		{
			if ( m_projectFrame )
			{
				m_projectFrame->EditSceneDependencies();
			}
		}
	}
}
