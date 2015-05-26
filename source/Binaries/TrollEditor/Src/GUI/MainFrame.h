//***********************************************************************************************************
#ifndef ___Troll_MainFrame___
#define ___Troll_MainFrame___
//***********************************************************************************************************
#include <wx/srchctrl.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <wx/notebook.h>
#include <Singleton.h>

#include <map>
#include <string>

#include "Popup.h"
#include "NewListbox.h"

#include "wx/splitter.h"

#include <wx/textctrl.h>
#include "../Project/DataWriter.h"
#include "../Project/Temporal/Module_Temporal.h"
#include <Module_Script.h>

#include "../Script/TrollScriptCompiler.h"
//***********************************************************************************************************
class wxScrolledWindow;

namespace Troll
{
	class Project;
	enum BackgroundType;
	enum AntiAliasing;
	namespace GUI
	{
		typedef std::map<wxString, wxString> MapType;
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
			CompileFile,
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


		class MainFrame : public wxFrame
		{
		private:
			bool m_smallToolbar;
			bool m_horzText;
			bool m_useCustomDisabled;
			bool m_showTooltips;
			bool m_editingProject;
			bool m_editingOverlays;
			bool m_testingProject;
			bool m_editingTimeLines;

			wxMenuBar	* m_menuBar;
			wxMenu		* m_menuFile;
			wxMenu		* m_menuEdition;
			wxMenu		* m_menuHelp;
			wxMenu		* m_menuProject;
			wxMenuItem	* m_menuEditProject;
			wxMenuItem	* m_menuEditTimeLines;
			wxMenuItem	* m_menuEditOverlays;

			TextCtrl	* m_textWindow;

			wxSplitterWindow * m_splitterH; //Splitter entre (Text + Trees) & Logs
			wxSplitterWindow * m_splitterV; //Splitter entre Trees & Text

			wxToolBar	* m_extraToolBar;
			wxToolBar	* m_tbar;

			int m_rows;
			size_t m_nPrint;
			size_t m_numSceneFile;
			size_t m_numScriptFile;
			size_t m_numDataFile;

			int m_logsHeight;

			wxString m_pathBmp;
			ToolBarPositions m_toolbarPosition;
			wxString m_currentPage;

			Troll::Project * m_currentProject;
			Troll::TrollScene * m_selectedScene;

			wxLogTextCtrl	* m_logDebug;
			wxLogTextCtrl	* m_logCompilation;

			Troll::TrollObject * m_viewedObject;

			wxString m_appPath;
			wxSearchCtrl * m_search;

			SceneDependenciesFrame * m_sceneDependencies;

			bool m_resizing;

			StcContext * m_pStcContext;

		public:
			int m_treesWidth;

			wxMenuItem		*	 m_menuTestProject;
			AutoCompletionListBox		*	 m_listBox;
			wxPanel			*	 m_mainPanel;
			wxNotebook		*	 m_treeTabsContainer;
			wxPanel			*	 m_filesListContainer;
			FilesTree		*	 m_filesList;
			wxPanel			*	 m_functionsListContainer;
			FunctionsTree		* m_functionsList;
			wxPanel			*	 m_objectsListContainer;
			ObjectsTree		*	 m_objectsList;
			wxScrolledWindow	* m_objectInfosContainer;
			wxListBox		*	 m_objectInfos;

			wxAuiNotebook		* m_mainTabsContainer;
//		TextPanel		* m_editText;
			wxStcTextEditor *	m_editText;
			wxPanel		*	 m_mainPanelContainer;
			wxWindow		* m_elypseContainer;
			ElypseCtrl		* m_elypseCtrl;
			bool			m_isFrame;

			wxNotebook	* m_logTabsContainer;
			wxPanel		* m_debugLogContainer;
			wxTextCtrl	* m_debugLog;
			wxPanel		* m_compilationLogContainer;
			LogCtrl		* m_compilationLog;

			int		*	 m_emscriptEditTextId;

			MapType m_map;
			MapTypeEdit m_mapEdit;

			wxArrayString m_choices;
			wxDialog * m_propertyDialog;

			TimeLineContainer * m_timeLineContainer;
			TimeLinePanel * m_timelineEditor;
			//wxPanel * m_timeLineInfos;
			TimePanel * m_timePanel;

		public:
			MainFrame( const wxString & p_title, int p_x, int p_y, int p_w, int p_h );
			void LayoutChildren();
			void PopulateToolbar( wxToolBarBase * p_toolBar );
			void AddFonction( wxString text );
			wxString CreateMain( Project * p_project, bool p_writeScripts );
			wxString CreateSceneEditorMain( Project * p_project );
			wxString CreateOverlayEditorMain( Project * p_project );
			void SetProject( const wxString & p_name, const wxString & p_path, const wxString & p_mainSceneName,
							 BackgroundType p_backgroundType, const wxString & m_background, bool p_shadows,
							 AntiAliasing p_aa, const wxSize & p_resolution );
			void OpenSelectedFile();
			void LogDebugMessage( const wxString & p_message );
			void LogOutMessage( const wxString & p_message );
			void LogDebugMessage( const String & p_message )
			{
				LogDebugMessage( make_wxString( p_message ) );
			}
			void LogOutMessage( const String & p_message )
			{
				LogOutMessage( make_wxString( p_message ) );
			}
			void CloseProject();
			void HideObjectInfos();
			void ShowObjectInfos( TrollObject * p_object );
			float GetMuseHeight();
			float GetMuseWidth();
			void InitialiseTimeLinePanel();
			void OpenProjectFile( const wxString & p_fileName );
			void CreateNewContinuousEventFrame( Temporal::TrollContinuousEvent * p_event );
			void AddPonctualEvent( Temporal::TrollSequence * p_sequence, float p_fireTime );
			void AddContinuousEvent( Temporal::TrollSequence * p_sequence, float p_fireTime );
			void SaveSceneFiles( TrollScene * p_scene );
			void SaveScene( TrollScene * p_scene );
			void SaveProject( Project * p_project );
			void SaveFileAs( TrollFile * p_file );
			void SaveFile( TrollFile * p_file, const wxString & p_fileName );
			bool ZipFolder( const wxString & p_folderPath, const wxString & p_zipFileName );
			void CompileNeededScenes( const wxArrayString & p_scenes, TrollScriptCompiler * p_compiler, wxArrayString & p_compiledScenes );
			void Compile( TrollFile * p_file, TrollScene * p_scene );
			void ListFolder();
			void CloseElypse();
			void CreateFunctionsGroup( const wxString & p_name );
			void CreateFreeFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
			void CreateClassFunction( const wxString & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
			void CreateConstantsGroup( const wxString & p_name );
			void CreateConstant( const wxString & p_constName, VariableBaseType p_type );

		private:
			void _initialise();
			void _setMenuBar();
			void _resize();
			void _resizeTrees();
			void _resizePanels();
			void _resizeLogs();
			void _createTreeWithDefStyle();
			void _musinate( Project * p_project, bool p_createMain = true, bool p_withScripts = true );
			void _musinateSceneEditor( Project * p_project );
			void _musinateOverlayEditor( Project * p_project );
			void _initEditLists();
			void _initElypse( Project * p_project, const wxString & p_title, bool p_edit, int p_adjustY = 0 );

			void _onOpenSelectedFile( wxCommandEvent & p_event );
			void _onOpenFile( wxCommandEvent & p_event );
			void _onOpenProject( wxCommandEvent & p_event );
			void _onWriteSceneFiles( wxCommandEvent & p_event );
			void _onNew( wxCommandEvent & p_event );
			void _onCloseProject( wxCommandEvent & p_event );
			void _onQuit( wxCommandEvent & p_event );
			void _onAbout( wxCommandEvent & p_event );
			void _onAdd( wxCommandEvent & p_event );
			void _onRename( wxCommandEvent & p_event );
			void _onSaveFile( wxCommandEvent & p_event );
			void _onSaveAll( wxCommandEvent & p_event );
			void _onSaveFileAs( wxCommandEvent & p_event );
			void _onSaveProject( wxCommandEvent & p_event );
			void _onAddExistingFile( wxCommandEvent & p_event );
			void _onCompile( wxCommandEvent & p_event );
			void _onSaveSelectedFile( wxCommandEvent & p_event );
			void _onAutoWord( wxCommandEvent & p_event );
			void _onMusinate( wxCommandEvent & p_event );
			void _onRemoveFile( wxCommandEvent & p_event );
			void _onDeleteFile( wxCommandEvent & p_event );
			void _onFunctionCopy( wxCommandEvent & p_event );
			void _onFunctionCut( wxCommandEvent & p_event );
			void _onFunctionPaste( wxCommandEvent & p_event );
			void _onNewScene( wxCommandEvent & p_event );
			void _onRenameScene( wxCommandEvent & p_event );
			void _onRemoveScene( wxCommandEvent & p_event );
			void _onEditProject( wxCommandEvent & p_event );
			void _onEditOverlays( wxCommandEvent & p_event );
			void _onTestProject( wxCommandEvent & p_event );
			void _onSelectList( wxCommandEvent & p_event );
			void _onCloseFile( wxCommandEvent & p_event );
			void _onEditTimeLine( wxCommandEvent & p_event );
			void _onListFolder( wxCommandEvent & p_event );
			void _onProjectProperties( wxCommandEvent & p_event );
			void _onGoToLine( wxCommandEvent & p_event );
			void _onTreeNewScene( wxCommandEvent & p_event );
			void _onEditSceneDependencies( wxCommandEvent & p_event );
			void _onSize( wxSizeEvent & p_event );
			void _onShow( wxShowEvent & p_event );
			void _onSetFocus( wxFocusEvent & p_event );
			void _onKillFocus( wxFocusEvent & p_event );
			void _onActivate( wxActivateEvent & p_event );
			void _onNotebook( wxNotebookEvent & p_event );
			void _onNotebookMiddleDown( wxMouseEvent & p_event );
			void _onVerticalSplitterPositionChanged( wxSplitterEvent & p_event );
			void _onVerticalSplitterPositionChanging( wxSplitterEvent & p_event );
			void _onVerticalSplitterDClick( wxSplitterEvent & p_event );
			void _onVerticalSplitterUnsplitEvent( wxSplitterEvent & p_event );
			void _onHorizontalSplitterPositionChanged( wxSplitterEvent & p_event );
			void _onHorizontalSplitterPositionChanging( wxSplitterEvent & p_event );
			void _onHorizontalSplitterDClick( wxSplitterEvent & p_event );
			void _onHorizontalSplitterUnsplitEvent( wxSplitterEvent & p_event );
			void _onScroll( wxScrollEvent & p_event );

			void _onSearch( wxCommandEvent & p_event );
			void _onSearchNext( wxCommandEvent & p_event );
			void _onSearchPrevious( wxCommandEvent & p_event );
			void _onReplace( wxCommandEvent & p_event );

			void _onShowKeumar( wxCommandEvent & p_event );
			void _onCloseKeumar( wxCommandEvent & p_event );

			void _onClose( wxCloseEvent & p_event );

		public:
			inline Project 	*		GetCurrentProject()const
			{
				return m_currentProject;
			}
			inline bool					IsEditingScene()const
			{
				return m_editingProject;
			}
			inline bool					IsEditingOverlays()const
			{
				return m_editingOverlays;
			}
			inline bool					IsTestingScene()const
			{
				return m_testingProject;
			}
			inline bool					IsEditingTimeLine()const
			{
				return m_editingTimeLines;
			}
			inline wxString				GetAppDir()const
			{
				return m_appPath;
			}
			inline wxWindow 	*		GetElypseContainer()const
			{
				return m_elypseContainer;
			}
			inline ElypseCtrl 	*		GetElypseCtrl()const
			{
				return m_elypseCtrl;
			}

		public:
			DECLARE_EVENT_TABLE()
		};
	}
}
//***********************************************************************************************************
#endif
//***********************************************************************************************************
