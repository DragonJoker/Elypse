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
#ifndef ___Troll_MainFrame___
#define ___Troll_MainFrame___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "GUI/Popup.h"
#include "GUI/AutoCompletionListBox.h"
#include "GUI/TimeSequence/Module_Time.h"

#include "Project/Module_Project.h"
#include "Project/Temporal/Module_Temporal.h"

#include "Elypse/TrollScriptCompiler.h"

#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/srchctrl.h>
#include <wx/textctrl.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/frame.h>

#include <map>
#include <string>

#include <Singleton.h>
#include <DataWriter.h>
#include <Module_Script.h>

class wxScrolledWindow;

BEGIN_TROLL_GUI_NAMESPACE
{
	typedef std::map<wxString, TextCtrl *> MapTypeEdit;

	class StcContext;
	class TETestPanel;
	class wxStcTextEditor;

	enum ToolBoxIds
	{
		Tool_new,
		Tool_open,
		Tool_save,
		Tool_copy,
		Tool_cut,
		Tool_paste,
		Tool_Max
	};


	enum ToolBarPositions
	{
		TOOLBAR_LEFT,
		TOOLBAR_TOP,
		TOOLBAR_RIGHT,
		TOOLBAR_BOTTOM
	};

	enum MainFrameIds
	{
		About,
		Add,
		AddFolder,
		BYCLIC,
		BYKEY,
		Combo,
		CompilationLogs,
		IdCompileFile,
		DebugLogs,
		DeleteScene,
		EditSceneDependencies,
		EditTimeLine,
		FilesList,
		FunctionsList,
		HorizSplitter,
		IdAddExistingFile,
		IdAutoWord,
		IdCloseProject,
		IdDelete,
		IdEditerOverlays,
		IdEditerScene,
		IdFermer,
		IdGoToLine,
		IdListBoxSelect,
		IdListFolder,
		IdMusiner,
		IdNew,
		IdOnSaveFile,
		IdOnSaveFileAs,
		IdOnSaveSelectedFile,
		IdOuvrirFichier,
		IdProjectProperties,
		IdRemove,
		IdReplace,
		IdSearch,
		IdSearchNext,
		IdSearchPrevious,
		IdSelectList,
		IdTester,
		IdTimeLinePanel,
		IdTool_copy,
		IdTool_cut,
		IdTool_paste,
		ListBox,
		LogsTabContainer,
		M_TOOLBAR_SHOW_BOTH,
		M_TOOLBAR_TOGGLETOOLTIPS,
		M_TOOLBAR_TOP_ORIENTATION,
		MainTabContainer,
		NewScene,
		ObjectsList,
		OpenFil,
		OpenProject,
		PropertiesList,
		Quit,
		Rename,
		RenameScene,
		SaveAll,
		saveFile,
		saveFileAs,
		saveModifs,
		saveProject,
		Spin,
		Toolbar,
		TreeNewScene,
		TreesTabContainer,
		VerticSplitter
	};

	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame( bool p_debug, const wxString & p_title, int p_x, int p_y, int p_w, int p_h );
		virtual ~MainFrame();
		void LayoutChildren();
		void PopulateToolbar( wxToolBarBase * p_toolBar );
		void AddFonction( wxString text );
		wxString CreateMain( TROLL_PROJECT_NAMESPACE::Project const & p_project, bool p_writeScripts );
		wxString CreateSceneEditorMain( TROLL_PROJECT_NAMESPACE::Project const & p_project );
		wxString CreateOverlayEditorMain( TROLL_PROJECT_NAMESPACE::Project const & p_project );
		void SetProject( const wxString & p_name, const wxString & p_path, const wxString & p_mainSceneName,
							TROLL_PROJECT_NAMESPACE::BackgroundType p_backgroundType, const wxString & m_background, bool p_shadows,
							TROLL_PROJECT_NAMESPACE::AntiAliasing p_aa, const wxSize & p_resolution );
		void OpenSelectedFile();
		void LogDebug( const wxString & p_message );
		void LogMessage( const wxString & p_message );
		void LogError( const wxString & p_message );
		void LogDebug( const String & p_message )
		{
			LogDebug( GuiCommon::make_wxString( p_message ) );
		}
		void LogMessage( const String & p_message )
		{
			LogMessage( GuiCommon::make_wxString( p_message ) );
		}
		void LogError( const String & p_message )
		{
			LogError( GuiCommon::make_wxString( p_message ) );
		}
		void CloseProject( bool p_closingApp );
		void HideObjectInfos();
		void ShowObjectInfos( TROLL_PROJECT_NAMESPACE::Object * p_object );
		float GetMuseHeight();
		float GetMuseWidth();
		void InitialiseTimeLinePanel();
		void OpenProjectFile( const wxString & p_fileName );
		void CreateNewContinuousEventFrame( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event );
		void AddPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence, float p_fireTime );
		void AddContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence, float p_fireTime );
		void SaveSceneFiles( TROLL_PROJECT_NAMESPACE::Scene * p_scene );
		void SaveScene( TROLL_PROJECT_NAMESPACE::Scene * p_scene );
		void SaveProject( TROLL_PROJECT_NAMESPACE::Project & p_project );
		void SaveFileAs( TROLL_PROJECT_NAMESPACE::File * p_file );
		void SaveFile( TROLL_PROJECT_NAMESPACE::File * p_file, const wxString & p_fileName );
		bool ZipFolder( const wxString & p_folderPath, const wxString & p_zipFileName );
		void CompileFile( TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler & p_compiler, TROLL_PROJECT_NAMESPACE::File * p_file );
		void CompileNeededScenes( const wxArrayString & p_scenes, TROLL_ELYPSE_NAMESPACE::TrollScriptCompiler & p_compiler, wxArrayString & p_compiledScenes );
		void Compile( TROLL_PROJECT_NAMESPACE::File * p_file, TROLL_PROJECT_NAMESPACE::Scene * p_scene );
		void ListFolder();
		void CloseElypse();
		void CreateFunctionsGroup( const wxString & p_name );
		void CreateFreeFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
		void CreateClassFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
		void CreateConstantsGroup( const wxString & p_name );
		void CreateConstant( const wxString & p_constName, VariableBaseType p_type );
		void ClosePropertyDialog();
		void UpdatePageLabel( wxString const & p_old, wxString const & p_new );
		TROLL_PROJECT_NAMESPACE::File * GetFile( wxString const & p_name );
		void SelectFileTab( int p_tab );
		void AddOverlay( TROLL_PROJECT_2D_NAMESPACE::TrollOverlay * p_overlay, EMOverlay * p_parent );
		void IncrementTimePanel( int p_diff );
		void UpdateSequence( int p_width, int p_height );

		inline TROLL_PROJECT_NAMESPACE::ProjectSPtr GetCurrentProject()const
		{
			return m_currentProject;
		}
		inline bool IsEditingScene()const
		{
			return m_editingProject;
		}
		inline bool IsEditingOverlays()const
		{
			return m_editingOverlays;
		}
		inline bool IsTestingScene()const
		{
			return m_testingProject;
		}
		inline bool IsEditingTimeLine()const
		{
			return m_editingTimeLines;
		}
		inline wxString GetAppDir()const
		{
			return m_appPath;
		}
		inline wxWindow * GetElypseContainer()const
		{
			return m_elypseContainer;
		}
		inline ElypseCtrl * GetElypseCtrl()const
		{
			return m_elypseCtrl;
		}
		inline ObjectsTree * GetObjectsList()const
		{
			return m_objectsList;
		}
		inline Time::TimeLineContainer * GetTimeLineContainer()const 
		{
			return m_timeLineContainer;
		}
		inline Time::TimeLinePanel * GetTimelineEditor()const
		{
			return m_timelineEditor;
		}
		inline FunctionsTree * GetFunctionsList()const
		{
			return m_functionsList;
		}
		inline wxAuiNotebook * GetMainTabsContainer()const
		{
			return m_mainTabsContainer;
		}
		inline wxStcTextEditor * GetCurrentEdit()
		{
			return m_editText;
		}
		inline FilesTree * GetFilesList()
		{
			return m_filesList;
		}
		static MainFrame * GetInstance()
		{
			return s_mainFrame;
		}

	private:
		void DoInitialiseMainContainers();
		void DoInitialiseTrees();
		void DoInitialiseLogs();
		void DoInitialise();
		void DoSetMenuBar();
		void DoCreateTreeWithDefStyle();
		void DoAddSceneToMuse( TROLL_PROJECT_NAMESPACE::Project const & p_project, bool p_createMain, bool p_withScripts, TROLL_PROJECT_NAMESPACE::Scene * p_scene, wxString const & p_mainFile );
		void DoMusinate( TROLL_PROJECT_NAMESPACE::Project const & p_project, bool p_createMain = true, bool p_withScripts = true );
		void DoMusinateSceneEditor( TROLL_PROJECT_NAMESPACE::Project const & p_project );
		void DoMusinateOverlayEditor( TROLL_PROJECT_NAMESPACE::Project const & p_project );
		void DoInitEditLists();
		void DoInitElypse( TROLL_PROJECT_NAMESPACE::ProjectSPtr p_project, const wxString & p_title, bool p_edit, int p_adjustY = 0 );

		DECLARE_EVENT_TABLE()
		void OnOpenSelectedFile( wxCommandEvent & p_event );
		void OnOpenFile( wxCommandEvent & p_event );
		void OnOpenProject( wxCommandEvent & p_event );
		void OnWriteSceneFiles( wxCommandEvent & p_event );
		void OnNew( wxCommandEvent & p_event );
		void OnCloseProject( wxCommandEvent & p_event );
		void OnQuit( wxCommandEvent & p_event );
		void OnAbout( wxCommandEvent & p_event );
		void OnAdd( wxCommandEvent & p_event );
		void OnRename( wxCommandEvent & p_event );
		void OnSaveFile( wxCommandEvent & p_event );
		void OnSaveAll( wxCommandEvent & p_event );
		void OnSaveFileAs( wxCommandEvent & p_event );
		void OnSaveProject( wxCommandEvent & p_event );
		void OnAddExistingFile( wxCommandEvent & p_event );
		void OnCompile( wxCommandEvent & p_event );
		void OnSaveSelectedFile( wxCommandEvent & p_event );
		void OnAutoWord( wxCommandEvent & p_event );
		void OnMusinate( wxCommandEvent & p_event );
		void OnRemoveFile( wxCommandEvent & p_event );
		void OnDeleteFile( wxCommandEvent & p_event );
		void OnFunctionCopy( wxCommandEvent & p_event );
		void OnFunctionCut( wxCommandEvent & p_event );
		void OnFunctionPaste( wxCommandEvent & p_event );
		void OnNewScene( wxCommandEvent & p_event );
		void OnRenameScene( wxCommandEvent & p_event );
		void OnRemoveScene( wxCommandEvent & p_event );
		void OnEditProject( wxCommandEvent & p_event );
		void OnEditOverlays( wxCommandEvent & p_event );
		void OnTestProject( wxCommandEvent & p_event );
		void OnSelectList( wxCommandEvent & p_event );
		void OnCloseFile( wxCommandEvent & p_event );
		void OnEditTimeLine( wxCommandEvent & p_event );
		void OnListFolder( wxCommandEvent & p_event );
		void OnProjectProperties( wxCommandEvent & p_event );
		void OnGoToLine( wxCommandEvent & p_event );
		void OnTreeNewScene( wxCommandEvent & p_event );
		void OnEditSceneDependencies( wxCommandEvent & p_event );
		void OnSize( wxSizeEvent & p_event );
		void OnShow( wxShowEvent & p_event );
		void OnSetFocus( wxFocusEvent & p_event );
		void OnKillFocus( wxFocusEvent & p_event );
		void OnActivate( wxActivateEvent & p_event );
		void OnNotebook( wxNotebookEvent & p_event );
		void OnNotebookMiddleDown( wxMouseEvent & p_event );
		void OnScroll( wxScrollEvent & p_event );

		void OnSearch( wxCommandEvent & p_event );
		void OnSearchNext( wxCommandEvent & p_event );
		void OnSearchPrevious( wxCommandEvent & p_event );
		void OnReplace( wxCommandEvent & p_event );

		void OnShowKeumar( wxCommandEvent & p_event );
		void OnCloseKeumar( wxCommandEvent & p_event );

		void OnClose( wxCloseEvent & p_event );
		
	private:
		int m_treesWidth;

		wxMenuItem * m_menuTestProject;
		AutoCompletionListBox * m_listBox;

		// TREES
		wxNotebook * m_treeTabsContainer;
		FilesTree * m_filesList;
		FunctionsTree * m_functionsList;
		wxPanel * m_objectsListContainer;
		ObjectsTree  * m_objectsList;
		wxScrolledWindow * m_objectInfosContainer;
		wxListBox * m_objectInfos;

		// MAIN
		wxAuiNotebook * m_mainTabsContainer;
		wxStcTextEditor * m_editText;
		wxPanel * m_mainPanelContainer;
		wxWindow * m_elypseContainer;
		ElypseCtrl * m_elypseCtrl;
		bool m_isFrame;

		// LOGS
		wxNotebook * m_logTabsContainer;
		LogCtrl * m_debugLog;
		LogCtrl * m_messageLog;
		LogCtrl * m_errorLog;
		wxLog * m_logDebug;
		wxLog * m_logMessage;
		wxLog * m_logError;
		std::streambuf * m_oldCout;
		std::streambuf * m_oldClog;
		std::streambuf * m_oldCerr;

		int * m_emscriptEditTextId;

		MapTypeEdit m_mapEdit;

		wxArrayString m_choices;
		wxDialog * m_propertyDialog;

		Time::TimeLineContainer * m_timeLineContainer;
		Time::TimeLinePanel * m_timelineEditor;
		Time::TimePanel * m_timePanel;


		wxAuiManager m_manager;

		bool m_smallToolbar;
		bool m_horzText;
		bool m_useCustomDisabled;
		bool m_showTooltips;
		bool m_editingProject;
		bool m_editingOverlays;
		bool m_testingProject;
		bool m_editingTimeLines;

		wxMenuBar * m_menuBar;
		wxMenu * m_menuFile;
		wxMenu * m_menuEdition;
		wxMenu * m_menuHelp;
		wxMenu * m_menuProject;
		wxMenuItem * m_menuEditProject;
		wxMenuItem * m_menuEditTimeLines;
		wxMenuItem * m_menuEditOverlays;

		TextCtrl * m_textWindow;

		wxToolBar * m_extraToolBar;
		wxToolBar * m_tbar;

		int m_rows;
		size_t m_nPrint;
		size_t m_numSceneFile;
		size_t m_numScriptFile;
		size_t m_numDataFile;

		int m_logsHeight;

		wxString m_pathBmp;
		ToolBarPositions m_toolbarPosition;
		wxString m_currentPage;

		TROLL_PROJECT_NAMESPACE::ProjectSPtr m_currentProject;
		TROLL_PROJECT_NAMESPACE::Scene * m_selectedScene;
		TROLL_PROJECT_NAMESPACE::Object * m_viewedObject;

		wxString m_appPath;
		wxSearchCtrl * m_search;

		SceneDependenciesFrame * m_sceneDependencies;

		bool m_resizing;

		StcContext * m_pStcContext;
		bool m_debug;

		static MainFrame * s_mainFrame;
	};
}
END_TROLL_GUI_NAMESPACE

#endif

