/*
See LICENSE file in root folder
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

#include "ProjectFrame.h"

#include "GUI/MainFrame.h"
#include "GUI/StcTextEditor.h"
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
#include "GUI/PropertyDialog.h"
#include "GUI/LogCtrl.h"
#include "GUI/FolderList.h"
#include "GUI/AboutFrame.h"
#include "GUI/ReplaceDialog.h"
#include "GUI/Properties/Project/ProjectProperties.h"
#include "GUI/Properties/Common/PropertiesHolder.h"
#include "GUI/TimeSequence/TimeLinePanel.h"
#include "GUI/TimeSequence/TimePanel.h"
#include "GUI/TimeSequence/TimeLineContainer.h"
#include "GUI/TimeSequence/LinePanel.h"
#include "GUI/TimeSequence/SequencePanel.h"
#include "GUI/LanguageParser.h"
#include "GUI/StcContext.h"
#include "GUI/StcContext.h"
#include "GUI/Aui/AuiTabArt.h"

#include "TrollEditorApplication.h"
#include "TrollLogs.h"
#include "MuseFileWriter.h"
#include "ProjectSaver.h"

#include "Project/Scene.h"
#include "Project/Project.h"
#include "Project/ProjectFileParser.h"
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

namespace Troll
{
	using namespace ProjectComponents;
	using namespace ProjectComponents::Objects2D;

	namespace GUI
	{
		namespace
		{
			wxString const TE_TIMELINE_EDITION = _( "Timeline Edition" );
			wxString const TE_OVERLAYS_EDITION = _( "Overlays Edition" );
			wxString const TE_SCENE_EDITION = _( "Scene Edition" );
			wxString const TE_PROJECT_TESTING = _( "Project Testing" );
			wxString const TE_NO_SELECTED_FILE = _( "No selected file" );
			wxString const TE_NOT_COMPILABLE_TYPE = _( "Can't compile this file type" );
			wxString const TE_FATAL_COMPILATION_ERROR = _( "Fatal error encountered while compiling this file" );
			wxString const TE_PROJECT = _( "Project" );

			wxString const TE_EDITOR = _( "Editor" );
			wxString const TE_OBJECTS = _( "Objects" );
			wxString const TE_COMPILING = _( "Compiling" );
			wxString const TE_COMPILED = _( "compiled" );
			wxString const TE_WARNINGS = _( "warnings" );
			wxString const TE_ERRORS = _( "errors" );
			wxString const TE_COMPILING_DURING_TEST = _( "Can't compile during a test or a scene edition" );
			wxString const TE_COMPILING_UNSAVED = _( "Please save the file before compiling it" );
			wxString const TE_PROJECT_FILES = _( "Project Files" );
			wxString const TE_FUNCTIONS = _( "Functions" );
			wxString const TE_PROJECT_LOAD_ERROR = _( "Problems during project loading:" );
			wxString const TE_OPEN_FILE = _( "Choose a file to open" );
			wxString const TE_ELYPSE_SCENE_FILE = _( "Elypse Scene files" );
			wxString const TE_ELYPSE_SCRIPT_FILE = _( "Elypse Srcipt files" );
			wxString const TE_NEW_PROJECT = _( "New Project" );
			wxString const TE_FILE_SAVE_ERROR = _( "File Save error:" );
			wxString const TE_CHOOSE_LINE = _( "Choose the line" );
			wxString const TE_GO_TO = _( "Go to..." );
		}

		ProjectFrame::ProjectFrame( wxWindow * p_parent, ProjectComponents::ProjectSPtr p_project )
			: wxPanel{ p_parent, wxID_ANY, wxDefaultPosition, p_parent->GetClientSize() }
			, m_project{ p_project }
			, m_sceneDependencies{ this }
		{
			DoInitialise();
		}

		ProjectFrame::ProjectFrame( wxWindow * p_parent, wxString const & p_fileName )
			: wxPanel{ p_parent, wxID_ANY, wxDefaultPosition, p_parent->GetClientSize() }
			, m_project{ std::make_shared< Project >() }
			, m_sceneDependencies{ this }
		{
			m_project->Load( p_fileName );
			DoInitialise();
		}

		ProjectFrame::~ProjectFrame()
		{
			wxGetApp().m_projectFrame = nullptr;
		}

		void ProjectFrame::CompileProject()
		{
			ScenePtr l_scene{ nullptr };
			File * l_file{ nullptr };

			if ( !GetProject()->FindFileInScenes( m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ), l_file, l_scene ) )
			{
				LogDebug( TE_NO_SELECTED_FILE );
			}
			else if ( !l_file->FileName.Contains( wxT( ".emscript" ) ) )
			{
				LogDebug( TE_NOT_COMPILABLE_TYPE );
			}
			else
			{
				std::clog << m_mainTabsContainer->GetPageText( m_mainTabsContainer->GetSelection() ) << "\n";

				try
				{
					Compile( *l_file, *l_scene );
				}
				catch ( ... )
				{
					LogDebug( TE_FATAL_COMPILATION_ERROR );
				}
			}
		}

		void ProjectFrame::MusinateProject()
		{
			MuseFileWriter::Musinate( *GetProject() );
		}

		void ProjectFrame::ShowProjectProperties()
		{
			auto project = GetProject();

			if ( project )
			{
				PropertyDialog dialog{ this
					, TE_PROJECT
					, new Properties::ProjectProperties{ *project }
					, wxDefaultPosition
					, wxSize{ 300, 300 } };
				dialog.ShowModal();
			}
		}

		void ProjectFrame::SaveProject()
		{
			PropertyDialog l_dialog{ this
				, TE_PROJECT
				, new Properties::ProjectProperties{ *GetProject() }
				, wxDefaultPosition
				, wxSize{ 300, 300 } };
			l_dialog.ShowModal();
		}

		bool ProjectFrame::EditTimeLine()
		{
			CloseElypse();
			m_elypseState = m_elypseState == ElypseState::TimelineEdition ? ElypseState::None : ElypseState::TimelineEdition;
			bool l_return = m_elypseState == ElypseState::TimelineEdition;

			if ( l_return )
			{
				DoInitEditLists();
				MuseFileWriter::Musinate( *GetProject() );
				DoInitElypse( GetProject(), TE_TIMELINE_EDITION, false, -120 );
				m_timePanel = new Time::TimePanel( m_elypseContainer, this, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 120 ), wxSize( m_elypseContainer->GetClientSize().x - 20, 20 ) );
				m_timeLineContainer = new Time::TimeLineContainer( m_elypseContainer, this, wxID_ANY, wxPoint( 0, m_elypseContainer->GetClientSize().y - 100 ), wxSize( m_elypseContainer->GetClientSize().x, 100 ) );
				m_timeLineContainer->Show();
				m_timeLineContainer->SetScrollbars( 20, 20, 0, 0, 0, 0 );
				m_timelineEditor = new Time::TimeLinePanel( m_timeLineContainer, this, wxID_ANY, wxPoint( 0, 0 ), m_timeLineContainer->GetClientSize() );
				m_timelineEditor->Show();
				//m_timeLineInfos = new wxPanel(m_elypseContainer, wxID_ANY, wxPoint( l_size.x, 0), wxSize( m_elypseContainer->GetClientSize().x - l_size.x, l_size.y), wxBORDER_SIMPLE);
				m_timeLineContainer->SetScrollbars( 20, 20, m_timelineEditor->GetSize().x / 20, m_timelineEditor->GetSize().y / 20, 0, 0 );
			}
			else
			{
				m_project->GetMainScene()->FlushObjects();
			}

			return l_return;
		}

		bool ProjectFrame::EditProject()
		{
			CloseElypse();
			m_elypseState = m_elypseState == ElypseState::SceneEdition ? ElypseState::None : ElypseState::SceneEdition;
			bool l_return = m_elypseState == ElypseState::SceneEdition;

			if ( l_return )
			{
				DoInitEditLists();
				wxString l_fileName = wxGetApp().GetMainFrame()->GetAppDir() + wxT( "/SceneEditor/Editeur.teproj" );
				ProjectSPtr l_project = std::make_shared< Project >();
				l_project->Load( l_fileName );
				l_project->SetResolution( m_project->GetResolution() );
				l_project->SetBackgroundImage( m_project->GetBackgroundImage() );
				l_project->SetBackgroundColour( m_project->GetBackgroundColour() );
				l_project->SetProjectFileName( l_fileName );
				l_project->SetSaved( true );
				MuseFileWriter::Musinate( *GetProject(), false, false );
				MuseFileWriter::MusinateSceneEditor( *GetProject(), *l_project );
				DoInitElypse( l_project, TE_SCENE_EDITION, true );
			}

			return l_return;
		}

		bool ProjectFrame::EditOverlays()
		{
			CloseElypse();
			m_elypseState = m_elypseState == ElypseState::OverlayEdition ? ElypseState::None : ElypseState::OverlayEdition;
			bool l_return = m_elypseState == ElypseState::OverlayEdition;

			if ( l_return )
			{
				DoInitEditLists();
				wxString l_fileName = wxGetApp().GetMainFrame()->GetAppDir() + wxT( "/OverlayEditor/Editeur.teproj" );
				ProjectSPtr l_project = std::make_shared< Project >();
				l_project->Load( l_fileName );
				l_project->SetResolution( m_project->GetResolution() );
				l_project->SetBackgroundImage( m_project->GetBackgroundImage() );
				l_project->SetBackgroundColour( m_project->GetBackgroundColour() );
				l_project->SetProjectFileName( l_fileName );
				l_project->SetSaved( true );
				MuseFileWriter::Musinate( *GetProject(), false, false );
				MuseFileWriter::MusinateOverlayEditor( *GetProject(), *l_project );
				DoInitElypse( l_project, TE_OVERLAYS_EDITION, false );
			}
			else
			{
				m_project->GetMainScene()->FlushObjects();
			}

			return l_return;
		}

		bool ProjectFrame::TestProject()
		{
			CloseElypse();
			m_elypseState = m_elypseState == ElypseState::ProjectTesting ? ElypseState::None : ElypseState::ProjectTesting;
			bool l_return = m_elypseState == ElypseState::ProjectTesting;

			if ( l_return )
			{
				MuseFileWriter().Musinate( *GetProject() );
				DoInitElypse( GetProject(), TE_PROJECT_TESTING + wxT( " " ) + m_project->GetName(), false );
			}

			return l_return;
		}

		void ProjectFrame::GotoFileLine( wxString const & p_file, uint32_t p_line )
		{
			auto l_file = GetFile( p_file );

			if ( l_file )
			{
				SelectFileTab( l_file->IdOnglet );
				std::clog << "LogCtrl::OnDoubleClick - " << l_file->IdOnglet << "\n";
				wxStcTextEditor * l_edit = l_file->EditPanel;
				l_edit->SetFocus();
				l_edit->GotoLine( p_line );
			}
		}

		void ProjectFrame::OpenFile( Scene const & p_scene, File & p_file )
		{
			if ( p_file.Saved && !p_file.Open )
			{
				wxPanel * l_editTextContainer = new wxPanel{ m_mainTabsContainer, wxID_ANY, wxPoint( 0, 0 ) };
				m_mainTabsContainer->AddPage( l_editTextContainer, p_file.FileName, true );
				l_editTextContainer->SetSize( 0, 22, m_mainTabsContainer->GetClientSize().x, m_mainTabsContainer->GetClientSize().y - 22 );
				m_editText = new wxStcTextEditor{ &p_file, m_stcContext, l_editTextContainer, wxID_ANY, wxPoint( 0, 0 ), l_editTextContainer->GetClientSize() };
				wxString l_path = GetProject()->GetPath() + p_file.m_scene->GetName() + wxFileName::GetPathSeparator();
				m_editText->LoadFile( GetProject()->GetPath() );
				p_file.Open = true;
				p_file.EditPanel = m_editText;

				for ( size_t i = 0; i < m_mainTabsContainer->GetPageCount(); i++ )
				{
					if ( m_mainTabsContainer->GetPageText( i ) == TE_EDITOR )
					{
						m_mainTabsContainer->RemovePage( i );
						return;
					}
				}

				if ( p_file.Filetype == loadScriptFile || p_file.Filetype == unloadScriptFile )
				{
					Compile( p_file, p_scene );
				}
			}
		}

		void ProjectFrame::CloseFile( File & p_file )
		{
			size_t i = 0;
			auto l_found = false;

			while ( i <= m_mainTabsContainer->GetPageCount() && !l_found )
			{
				if ( m_mainTabsContainer->GetPageText( i ) == p_file.FileName )
				{
					p_file.Open = false;
					p_file.EditPanel = nullptr;
					m_mainTabsContainer->DeletePage( i );
					l_found = true;
				}

				i++;
			}
		}

		void ProjectFrame::EditSceneDependencies()
		{
			if ( m_project )
			{
				m_sceneDependencies.SetScene( m_filesList->GetSelectedScene() );
				m_sceneDependencies.Show();
			}
		}

		void ProjectFrame::DoInitEditLists()
		{
			m_objectsListContainer = new wxPanel( m_treeTabsContainer, wxID_ANY, wxPoint( 0, 0 ), m_treeTabsContainer->GetClientSize() );
			m_objectsListContainer->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_objectsListContainer->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_treeTabsContainer->InsertPage( 2, m_objectsListContainer, TE_OBJECTS, false );

			wxSize l_size = m_objectsListContainer->GetClientSize();
			l_size.y /= 2;
			m_objectsList = new ObjectsTree( m_objectsListContainer, this, wxDefaultPosition, l_size, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_objectsList->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_objectsList->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_objectsList->InitObjectList();

			m_objectInfosContainer = new Properties::PropertiesHolder( m_objectsListContainer, wxPoint( 5, l_size.y + 5 ), wxSize( l_size.x - 10, l_size.y - 10 ) );
			m_objectInfosContainer->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_objectInfosContainer->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_objectInfosContainer->SetCaptionBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_objectInfosContainer->SetCaptionTextColour( GuiCommon::PanelForegroundColour );
			m_objectInfosContainer->SetSelectionBackgroundColour( GuiCommon::ActiveTabColour );
			m_objectInfosContainer->SetSelectionTextColour( GuiCommon::ActiveTextColour );
			m_objectInfosContainer->SetCellBackgroundColour( GuiCommon::InactiveTabColour );
			m_objectInfosContainer->SetCellTextColour( GuiCommon::InactiveTextColour );
			m_objectInfosContainer->SetLineColour( GuiCommon::BorderColour );
			m_objectInfosContainer->SetMarginColour( GuiCommon::BorderColour );
		}

		void ProjectFrame::DoInitElypse( ProjectSPtr p_project, wxString const & p_title, bool p_edit, int p_adjustY )
		{
			wxSize l_projectSize = p_project->GetResolution();
			wxSize l_size = m_mainTabsContainer->GetClientSize();

			if ( l_size.x < l_projectSize.x || l_size.y < l_projectSize.y && m_elypseState == ElypseState::ProjectTesting )
			{
				l_size = l_projectSize;
				m_elypseContainer = new ElypseFrame( this, wxPoint( 0, 0 ), l_size );
				m_elypseContainer->SetBackgroundColour( wxColour( 212, 208, 200 ) );
				m_elypseContainer->Show();
				m_isFrame = true;
			}
			else
			{
				if ( m_elypseState == ElypseState::ProjectTesting )
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

			m_elypseCtrl = new ElypseCtrl{ m_elypseContainer, p_project, this, p_edit };
			m_elypseCtrl->Init();
		}

		float ProjectFrame::GetMuseHeight()
		{
			if ( m_elypseCtrl != nullptr )
			{
				return m_elypseCtrl->GetMuseHeight();
			}

			return 0;
		}

		float ProjectFrame::GetMuseWidth()
		{
			if ( m_elypseCtrl != nullptr )
			{
				return m_elypseCtrl->GetMuseWidth();
			}

			return 0;
		}

		void ProjectFrame::CreateNewContinuousEventFrame( ProjectComponents::Temporal::TrollContinuousEvent * p_event )
		{
		}

		void ProjectFrame::AddPonctualEvent( ProjectComponents::Temporal::TrollSequence * p_sequence, float p_fireTime )
		{
			StringArray l_tmp;
			auto * l_museEvent = ScriptEngine::GetContext()->sequenceManager->CreatePonctualEvent( "Overlay_Hide", l_tmp );
			auto * l_event = new ProjectComponents::Temporal::TrollPonctualEvent( p_sequence
				, l_museEvent
				, wxT( "overlay" )
				, wxEmptyString
				, p_fireTime
				, wxEmptyString
				, wxT( "Overlay_Hide" )
				, wxEmptyString );
			p_sequence->AddPonctualEvent( l_event );
			Time::SequencePanel * l_panel = reinterpret_cast< GUI::Time::SequencePanel * >( m_timelineEditor->GetSequencePanel( p_sequence->GetObjectName() )->GetParent() );
			l_panel->AddPonctualEvent( l_event, p_fireTime );
		}

		void ProjectFrame::AddContinuousEvent( ProjectComponents::Temporal::TrollSequence * p_sequence, float p_fireTime )
		{
			StringArray l_tmp;
			auto * l_museEvent = reinterpret_cast <Overlay_Translate *>( ScriptEngine::GetContext()->sequenceManager->CreateContinuousEvent( "Overlay_Translate" ) );
			l_museEvent->SetInterpolator( ScriptEngine::GetContext()->sequenceManager->GetInterpolator_V3( "linear" ) );
			l_museEvent->AddFrame( p_fireTime, Vector3( 0.0, 0.0, 0.0 ) );
			auto * l_event = new ProjectComponents::Temporal::TrollContinuousEvent( p_sequence
				, l_museEvent
				, wxT( "Overlay_Translate" )
				, wxEmptyString );
			p_sequence->AddContinuousEvent( l_event );
			l_event->AddKeyFrame( p_fireTime, wxT( "0.0 0.0 0.0" ) );
			l_event->SetTargetType( wxT( "overlay" ) );
			l_event->SetInterpolation( wxT( "linear" ) );
			l_event->SetActionName( wxT( "Overlay_Translate" ) );
			Time::SequencePanel * l_panel = reinterpret_cast< GUI::Time::SequencePanel * >( m_timelineEditor->GetSequencePanel( p_sequence->GetObjectName() )->GetParent() );
			l_panel->AddContinuousEvent( l_event );
		}

		void ProjectFrame::SaveSceneFiles( Scene const & p_scene )
		{
			auto l_func = [this]( File const & p_file )
			{
				if ( p_file.EditPanel && p_file.EditPanel->IsModified() )
				{
					p_file.EditPanel->SaveFile( m_project->GetPath() + p_file.FileName );
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

		void ProjectFrame::CompileFile( ElypseRW::TrollScriptCompiler & p_compiler, File const & p_file )
		{
			wxString l_fileName = p_file.m_scene->GetName() + wxFileName::GetPathSeparator() + p_file.FileName;
			wxString l_path = GetProject()->GetPath() + l_fileName;
			LogMessage( wxString( TE_COMPILING ) << l_fileName );
			ScriptNode * l_scriptNode = p_compiler.CompileScriptFile( p_file.CfgFile.get() );
			LogMessage( l_fileName << wxT( " " ) << TE_COMPILED
				<< wxT( " - " ) << p_compiler.GetNumWarnings() << _( " " ) << TE_WARNINGS
				<< wxT( " - " ) << p_compiler.GetNumErrors() << _( " " ) << TE_ERRORS );

			if ( l_scriptNode )
			{
				l_scriptNode->Use();
				l_scriptNode->Delete();
			}
		}

		void ProjectFrame::CompileNeededScenes( const wxArrayString & p_scenes, ElypseRW::TrollScriptCompiler & p_compiler, wxArrayString & p_compiledScenes )
		{
			for ( auto const & l_name : p_scenes )
			{
				try
				{
					auto const & l_scene = m_project->GetScene( l_name );
					wxArrayString l_neededScenes = l_scene->GetNeededScenes();
					CompileNeededScenes( l_neededScenes, p_compiler, p_compiledScenes );

					if ( std::find( p_compiledScenes.begin(), p_compiledScenes.end(), l_name ) == p_compiledScenes.end() )
					{
						for ( auto const & l_itFiles : l_scene->GetLoadScriptFiles() )
						{
							CompileFile( p_compiler, l_itFiles );
						}

						p_compiledScenes.push_back( l_name );
					}
				}
				catch ( std::exception & )
				{
					std::cerr << "Scene not found in project: " << l_name << "\n";
				}
			}
		}

		void ProjectFrame::Compile( File const & p_file, Scene const & p_scene )
		{
			if ( m_elypseContainer )
			{
				LogMessage( TE_COMPILING_DURING_TEST );
			}
			else
			{
				p_file.EditPanel->SaveFile( m_project->GetPath() );

				if ( !p_file.Saved || ( p_file.EditPanel && p_file.EditPanel->IsModified() ) )
				{
					LogMessage( TE_COMPILING_UNSAVED );
				}
				else
				{
					String l_path = make_string( m_project->GetPath() );

					if ( p_file.EditPanel )
					{
						p_file.EditPanel->ReinitContext( m_stcContext );
					}

					{
						ElypseRW::TrollScriptCompiler l_compiler{ &m_stcContext, l_path, false };
						l_compiler.Initialise( p_file.EditPanel );
						l_compiler.CompileScript( make_string( m_project->GetStartupScript() ) );
						wxArrayString l_compiledScenes;
						CompileNeededScenes( p_scene.GetNeededScenes(), l_compiler, l_compiledScenes );
						auto l_it = std::find_if( p_scene.GetLoadScriptFiles().begin(), p_scene.GetLoadScriptFiles().end(), [&p_file]( File const & l_file )
						{
							return l_file.FileName == p_file.FileName;
						} );
						bool l_found = l_it != p_scene.GetLoadScriptFiles().end();

						if ( l_found )
						{
							CompileFile( l_compiler, *l_it );
						}

						if ( !l_found )
						{
							for ( auto const & l_file : p_scene.GetUnloadScriptFiles() )
							{
								CompileFile( l_compiler, l_file );
							}
						}
					}

					for ( auto & l_scene : m_project->GetScenes() )
					{
						for ( auto & l_file : l_scene->GetLoadScriptFiles() )
						{
							l_file.m_compiled = false;
						}
					}
				}
			}
		}

		void ProjectFrame::CloseElypse()
		{
			if ( m_elypseCtrl != nullptr )
			{
				m_elypseCtrl->Close();
				m_elypseContainer->Destroy();

				if ( !m_isFrame )
				{
					m_mainTabsContainer->DeletePage( 0 );
				}

				//m_elypseCtrl->Destroy();
				m_elypseCtrl = nullptr;
				m_elypseContainer = nullptr;
				m_viewedObject = nullptr;

				//m_project->GetMainScene()->FlushObjects();

				if ( m_objectsListContainer != nullptr )
				{
					m_treeTabsContainer->DeletePage( 2 );
					m_objectsListContainer = nullptr;
					m_objectsList = nullptr;
					m_objectInfosContainer = nullptr;
				}
			}

#if defined( _WIN32 )

			if ( m_project && m_project->GetShowDebug() )
			{
				FreeConsole();
			}

#endif
		}

		void ProjectFrame::CreateFunctionsGroup( wxString const & p_name )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateTitreFunction( p_name );
			}
		}

		void ProjectFrame::CreateFreeFunction( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreaFunc( p_funcName, p_returnValue, p_params );
			}
		}

		void ProjectFrame::CreateClassFunction( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params )
		{
			if ( m_functionsList )
			{
				m_functionsList->ClassFunc( p_funcName, p_returnValue, p_params );
			}
		}

		void ProjectFrame::CreateConstantsGroup( wxString const & p_name )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateTitreConstant( p_name );
			}
		}

		void ProjectFrame::CreateConstant( wxString const & p_constName, VariableBaseType p_type )
		{
			if ( m_functionsList )
			{
				m_functionsList->CreateConstant( p_constName, p_type );
			}
		}

		void ProjectFrame::UpdatePageLabel( wxString const & p_old, wxString const & p_new )
		{
			for ( size_t i = 0; i <= m_mainTabsContainer->GetPageCount(); i++ )
			{
				if ( m_mainTabsContainer->GetPageText( i ) == p_old )
				{
					m_mainTabsContainer->SetPageText( i, p_new );
				}
			}
		}

		File * ProjectFrame::GetFile( wxString const & p_name )
		{
			File * l_file{ nullptr };

			try
			{
				auto l_itemId = m_filesList->GetItemByName( p_name );
				l_file = &m_filesList->GetSelectedScene()->FindFile( [l_itemId]( File & p_file )
				{
					return p_file.ItemId == l_itemId;
				} );
			}
			catch ( std::exception const & )
			{
			}

			return l_file;
		}

		void ProjectFrame::SelectFileTab( int p_tab )
		{
			m_mainTabsContainer->SetFocus();
			m_mainTabsContainer->SetSelection( p_tab );
		}

		void ProjectFrame::AddOverlay( TrollOverlay * p_overlay, EMOverlay * p_parent )
		{
			GetProject()->GetMainScene()->AddOverlay( p_overlay );
			m_objectsList->AddOverlay( p_overlay, ( p_parent ? make_wxString( p_parent->GetName() ) : wxT( "" ) ) );
		}

		void ProjectFrame::IncrementTimePanel( int p_diff )
		{
			m_timePanel->IncrementFirst( 0.2f * p_diff );
			m_timePanel->IncrementTickPosition( ( 0 - p_diff ) * 20 );
		}

		void ProjectFrame::UpdateSequence( int p_width, int p_height )
		{
			float l_first = m_timePanel->GetFirst();
			m_timeLineContainer->Scroll( 0, 0 );
			SetSize( p_width + 20, p_height );
			int l_verticalScroll = m_timeLineContainer->GetScrollPos( wxVERTICAL );
			//std::cout << "TimeLinePanel::UpdateSequence - " << l_verticalScroll << "\n";
			m_timeLineContainer->SetScrollbars( 20, 20, ( p_width + 20 ) / 20, p_height / 20, 0, 0 );
			m_timeLineContainer->Scroll( int( l_first >= 0.1 ? l_first * 5 + 1 : l_first * 5 ), l_verticalScroll );
		}

		void ProjectFrame::DoInitialiseMainContainers()
		{
			wxSize l_size = GetClientSize();
			//trees tabs (contains files list, objects list...)
			m_treeTabsContainer = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxSize{ m_treesWidth, l_size.y } );
			m_treeTabsContainer->SetArtProvider( new AuiTabArt );
			m_treeTabsContainer->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_treeTabsContainer->SetForegroundColour( GuiCommon::PanelForegroundColour );
			// main tabs (contains text editor, test panel...)
			m_mainTabsContainer = new wxAuiNotebook( this, wxID_ANY, wxPoint{ m_treesWidth, 0 }, wxSize{ l_size.x - m_treesWidth, l_size.y }, wxBORDER_NONE | wxAUI_NB_MIDDLE_CLICK_CLOSE | wxAUI_NB_CLOSE_ON_ALL_TABS );
			m_mainTabsContainer->SetArtProvider( new AuiTabArt );
			m_mainTabsContainer->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_mainTabsContainer->SetForegroundColour( GuiCommon::PanelForegroundColour );

			auto l_sizer = new wxBoxSizer{ wxHORIZONTAL };
			l_sizer->Add( m_treeTabsContainer, wxSizerFlags().Left() );
			l_sizer->Add( m_mainTabsContainer, wxSizerFlags( 1 ).Expand() );
			l_sizer->SetSizeHints( this );
			SetSizer( l_sizer );
		}

		void ProjectFrame::DoInitialiseTrees()
		{
			wxSize l_size = m_mainTabsContainer->GetClientSize();
			//files list
			m_filesList = new FilesTree( m_treeTabsContainer, this, wxDefaultPosition, wxSize{ l_size.x - m_treesWidth, l_size.y }, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_filesList->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_filesList->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_treeTabsContainer->AddPage( m_filesList, TE_PROJECT_FILES, true );
			//functions list
			m_functionsList = new FunctionsTree( m_treeTabsContainer, this, wxDefaultPosition, wxSize{ l_size.x - m_treesWidth, l_size.y }, wxTR_DEFAULT_STYLE | wxTR_EDIT_LABELS | wxSUNKEN_BORDER );
			m_functionsList->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_functionsList->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_treeTabsContainer->AddPage( m_functionsList, TE_FUNCTIONS, false );
			m_functionsList->AddFunctionsToTree();

			ElypseRW::TrollScriptCompiler l_compiler( &m_stcContext, "", true );
			l_compiler.Initialise( nullptr );
		}

		void ProjectFrame::DoInitialise()
		{
			wxGetApp().m_projectFrame = this;

			LanguageFileParser l_parser( &m_stcContext );
			l_parser.ParseFile( wxGetApp().GetMainFrame()->GetAppDir() + wxFileName::GetPathSeparator() + wxT( "EMSCRIPT.lang" ) );
			l_parser.ParseFile( wxGetApp().GetMainFrame()->GetAppDir() + wxFileName::GetPathSeparator() + wxT( "EMSCENE.lang" ) );

#if wxCHECK_VERSION( 2, 9, 0 )
			SetMinClientSize( wxSize( 800, 600 ) );
#else
			SetMinSize( wxSize( 800, 600 ) );
#endif

			SetBackgroundColour( wxColour( 255, 255, 255 ) );

			DoInitialiseMainContainers();
			DoInitialiseTrees();

			// listbox for code completion
			m_listBox = new AutoCompletionListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choices );
			m_listBox->Hide();

			m_project->SetSaved( false );

			try
			{
				m_filesList->LoadProject( *m_project );
			}
			catch ( std::exception const & p_exc )
			{
				wxMessageBox( TE_PROJECT_LOAD_ERROR + wxT( "\n" ) + make_wxString( p_exc.what() ), TROLL_EDITOR_ERROR, wxOK | wxICON_ERROR, this );
			}

			if ( m_editText )
			{
				m_editText->GetText().Clear();
			}
		}

		BEGIN_EVENT_TABLE( ProjectFrame, wxPanel )
			EVT_CLOSE( ProjectFrame::OnClose )
			EVT_SHOW( ProjectFrame::OnShow )
			EVT_SET_FOCUS( ProjectFrame::OnSetFocus )
			EVT_KILL_FOCUS( ProjectFrame::OnKillFocus )
			EVT_ACTIVATE( ProjectFrame::OnActivate )
			EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, ProjectFrame::OnNotebook )
			EVT_AUINOTEBOOK_PAGE_CLOSE( wxID_ANY, ProjectFrame::OnNotebookPageClosed )
		END_EVENT_TABLE()

		void ProjectFrame::OnOpenFile( wxCommandEvent & p_event )
		{
			wxString l_fileName = wxFileSelector( TE_OPEN_FILE
				, wxEmptyString
				, wxEmptyString
				, wxEmptyString
				, wxString()
					<< TE_ELYPSE_SCENE_FILE << wxT( "(*.emscene)|*.emscene|" )
					<< TE_ELYPSE_SCRIPT_FILE << wxT( " (*.emscript)|*.emscript" ) );

			if ( l_fileName.empty() )
			{
				return;
			}
		}

		void ProjectFrame::OnNew( wxCommandEvent & p_event )
		{
			PropertyDialog l_dialog{ this, TE_NEW_PROJECT, new NewProjectProperties };
			l_dialog.ShowModal();
		}

		void ProjectFrame::OnSaveAll( wxCommandEvent & p_event )
		{
			auto l_mainScene = GetProject()->GetMainScene();
			SaveSceneFiles( *l_mainScene );

			for ( auto const & l_scene : GetProject()->GetScenes() )
			{
				if ( l_mainScene != l_scene )
				{
					SaveSceneFiles( *l_scene );
				}
			}
		}

		void ProjectFrame::OnClose( wxCloseEvent & p_event )
		{
			CloseElypse();
			p_event.Skip();
		}

		void ProjectFrame::OnSaveFile( wxCommandEvent & p_event )
		{
			if ( m_editText )
			{
				int l_selectedPage = m_mainTabsContainer->GetSelection();
				wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );

				if ( l_fileName != TE_TIMELINE_EDITION && l_fileName != TE_SCENE_EDITION && l_fileName != TE_OVERLAYS_EDITION && l_fileName != TE_PROJECT_TESTING )
				{
					m_editText->SaveFile( GetProject()->GetPath() );
				}
			}
		}

		void ProjectFrame::OnSaveFileAs( wxCommandEvent & p_event )
		{
			//std::cout << "ProjectFrame::OnSaveFileAs\n";
			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxString l_fileName = m_mainTabsContainer->GetPageText( l_selectedPage );
			auto l_scene = m_filesList->GetSelectedScene();

			try
			{
				auto & l_file = l_scene->GetFile( l_fileName );
				auto l_name = ProjectSaver::SaveFileAs( *m_project, l_file );

				if ( !l_name.empty() )
				{
					m_filesList->SetItemText( l_file.ItemId, l_name );
				}
			}
			catch ( General::Utils::GenException & p_exc )
			{
				wxMessageBox( wxString() << TE_FILE_SAVE_ERROR << "\n" << p_exc.what() );
			}
		}

		void ProjectFrame::OnFunctionCopy( wxCommandEvent & )
		{
		}

		void ProjectFrame::OnFunctionCut( wxCommandEvent & )
		{
		}

		void ProjectFrame::OnFunctionPaste( wxCommandEvent & )
		{
		}

		void ProjectFrame::OnShow( wxShowEvent & p_event )
		{
			if ( m_elypseCtrl != nullptr )
			{
				m_elypseCtrl->OnShow( p_event );
			}
		}

		void ProjectFrame::OnSetFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != nullptr )
			{
				m_elypseCtrl->OnSetFocus( p_event );
			}
		}

		void ProjectFrame::OnKillFocus( wxFocusEvent & p_event )
		{
			if ( m_elypseCtrl != nullptr )
			{
				m_elypseCtrl->OnKillFocus( p_event );
			}
		}

		void ProjectFrame::OnActivate( wxActivateEvent & p_event )
		{
			if ( m_elypseCtrl != nullptr )
			{
				m_elypseCtrl->OnActivate( p_event );
			}
		}

		void ProjectFrame::OnNotebook( wxNotebookEvent & p_event )
		{
			int l_idx = p_event.GetSelection();
			wxBookCtrlBase  * l_book = wx_static_cast( wxBookCtrlBase *, p_event.GetEventObject() );
			m_currentPage = l_book->GetPageText( l_idx );

			if ( l_book->GetName().find( TE_PROJECT_TESTING ) == 0 )
			{
				if ( m_elypseCtrl != nullptr )
				{
					m_elypseCtrl->SetCtrlFocus();
				}
			}
			else
			{
				if ( m_elypseCtrl != nullptr )
				{
					m_elypseCtrl->KillCtrlFocus();
				}

				if ( l_book->GetName() == TE_EDITOR )
				{
					if ( m_filesList->GetSelectedScene() != nullptr )
					{
						try
						{
							File const & l_file = m_filesList->GetSelectedScene()->GetFile( m_currentPage );
							m_editText = l_file.EditPanel;
						}
						catch ( std::exception const & p_exc )
						{
							std::cerr << "ProjectFrame::OnNotebook - " << p_exc.what() << "\n";
						}
					}
				}
			}
		}

		void ProjectFrame::OnNotebookPageClosed( wxAuiNotebookEvent & p_event )
		{
			wxBookCtrlBase * l_book = wx_static_cast( wxBookCtrlBase *, p_event.GetEventObject() );

			if ( l_book->GetName().find( TE_PROJECT_TESTING ) == 0 )
			{
				CloseElypse();
			}
		}

		void ProjectFrame::OnNotebookMiddleDown( wxMouseEvent & p_event )
		{
			std::cout << "ProjectFrame::OnNotebookMiddleDown\n";
		}

		void ProjectFrame::OnGoToLine( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer->GetPageCount() <= 0 )
			{
				return;
			}

			int l_page = m_mainTabsContainer->GetSelection();
			wxStcTextEditor * l_panel = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_page )->GetChildren().GetFirst()->GetData() );

			if ( l_panel == nullptr )
			{
				return;
			}

			int l_maxLines = l_panel->GetLineCount();
			wxString l_text;
			l_text << TE_CHOOSE_LINE << wxT( " (1 - " ) << l_maxLines << wxT( ")" );
			wxTextEntryDialog * l_dialog = new wxTextEntryDialog( this, l_text, TE_GO_TO );

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

		void ProjectFrame::OnSearch( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == nullptr || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchNext( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
		}

		void ProjectFrame::OnSearchNext( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == nullptr || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchNext( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
		}

		void ProjectFrame::OnSearchPrevious( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == nullptr || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			l_editCtrl->SearchPrev( wxSTC_FIND_MATCHCASE, m_search->GetValue() );
		}

		void ProjectFrame::OnReplace( wxCommandEvent & p_event )
		{
			if ( m_mainTabsContainer == nullptr || m_mainTabsContainer->GetPageCount() == 0 )
			{
				return;
			}

			int l_selectedPage = m_mainTabsContainer->GetSelection();
			wxStcTextEditor * l_editCtrl = static_cast< wxStcTextEditor * >( m_mainTabsContainer->GetPage( l_selectedPage )->GetChildren().GetFirst()->GetData() );
			ReplaceDialog * l_dialog = new ReplaceDialog( this, l_editCtrl );
			l_dialog->Show();
		}

		void ProjectFrame::OnTreeNewScene( wxCommandEvent & p_event )
		{
			if ( m_project )
			{
				NewSceneFrame * l_frame = new NewSceneFrame( this );
				l_frame->Show();
			}
		}

		void ProjectFrame::HideObjectInfos()
		{
			if ( m_viewedObject )
			{
				m_viewedObject->HideProperties();
				m_viewedObject = nullptr;
			}
		}

		void ProjectFrame::ShowObjectInfos( Object * p_object )
		{
			//std::cout << "ProjectFrame::ShowObjectInfos\n";
			m_objectInfosContainer->Clear();
			auto l_properties = p_object->CreateProperties( m_objectInfosContainer );
			m_viewedObject = p_object;
		}
	}
}
