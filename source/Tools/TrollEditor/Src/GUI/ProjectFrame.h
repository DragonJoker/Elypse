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
#ifndef ___TROLL_GUI_PROJECT_FRAME_H___
#define ___TROLL_GUI_PROJECT_FRAME_H___

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "TrollEditorApplication.h"

#include "GUI/AutoCompletionListBox.h"
#include "GUI/SceneDependenciesFrame.h"
#include "GUI/Popup.h"
#include "GUI/StcContext.h"

#include "GUI/TimeSequence/Module_Time.h"

#include "GUI/Properties/Module_Properties.h"

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

namespace Troll
{
	namespace GUI
	{
		typedef std::map< wxString, TextCtrl * > MapTypeEdit;

		class StcContext;
		class TETestPanel;
		class wxStcTextEditor;

		enum class ElypseState
		{
			None,
			SceneEdition,
			OverlayEdition,
			TimelineEdition,
			ProjectTesting,
		};

		class ProjectFrame
			: public wxPanel
		{
		public:
			ProjectFrame( wxWindow * p_parent, ProjectComponents::ProjectSPtr p_project );
			ProjectFrame( wxWindow * p_parent, wxString const & p_fileName );
			virtual ~ProjectFrame();

			void CompileProject();
			void MusinateProject();
			void ShowProjectProperties();
			void SaveProject();
			bool EditTimeLine();
			bool EditProject();
			bool EditOverlays();
			bool TestProject();
			void GotoFileLine( wxString const & p_file, uint32_t p_line );
			void OpenFile( ProjectComponents::Scene const & p_scene, ProjectComponents::File & p_file );
			void CloseFile( ProjectComponents::File & p_file );
			void CreateScene();
			void RemoveScene();
			void EditSceneDependencies();

			void HideObjectInfos();
			void ShowObjectInfos( ProjectComponents::Object * p_object );
			float GetMuseHeight();
			float GetMuseWidth();
			void CreateNewContinuousEventFrame( ProjectComponents::Temporal::TrollContinuousEvent * p_event );
			void AddPonctualEvent( ProjectComponents::Temporal::TrollSequence * p_sequence, float p_fireTime );
			void AddContinuousEvent( ProjectComponents::Temporal::TrollSequence * p_sequence, float p_fireTime );
			void SaveSceneFiles( ProjectComponents::Scene const & p_scene );
			void CompileFile( ElypseRW::TrollScriptCompiler & p_compiler, ProjectComponents::File const & p_file );
			void CompileNeededScenes( const wxArrayString & p_scenes, ElypseRW::TrollScriptCompiler & p_compiler, wxArrayString & p_compiledScenes );
			void Compile( ProjectComponents::File const & p_file, ProjectComponents::Scene const & p_scene );
			void CloseElypse();
			void CreateFunctionsGroup( wxString const & p_name );
			void CreateFreeFunction( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
			void CreateClassFunction( wxString const & p_funcName, VariableBaseType p_returnValue, std::vector< VariableBaseType > p_params );
			void CreateConstantsGroup( wxString const & p_name );
			void CreateConstant( wxString const & p_constName, VariableBaseType p_type );
			void UpdatePageLabel( wxString const & p_old, wxString const & p_new );
			ProjectComponents::File * GetFile( wxString const & p_name );
			void SelectFileTab( int p_tab );
			void AddOverlay( ProjectComponents::Objects2D::TrollOverlay * p_overlay, EMOverlay * p_parent );
			void IncrementTimePanel( int p_diff );
			void UpdateSequence( int p_width, int p_height );

			inline ProjectComponents::ProjectSPtr GetProject()const
			{
				return m_project;
			}
			inline bool IsEditingScene()const
			{
				return m_elypseState == ElypseState::SceneEdition;
			}
			inline bool IsEditingOverlays()const
			{
				return m_elypseState == ElypseState::OverlayEdition;
			}
			inline bool IsTestingScene()const
			{
				return m_elypseState == ElypseState::ProjectTesting;
			}
			inline bool IsEditingTimeLine()const
			{
				return m_elypseState == ElypseState::TimelineEdition;
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

		private:
			void DoInitialiseMainContainers();
			void DoInitialiseTrees();
			void DoInitialise();
			void DoInitEditLists();
			void DoInitElypse( ProjectComponents::ProjectSPtr p_project, wxString const & p_title, bool p_edit, int p_adjustY = 0 );

			DECLARE_EVENT_TABLE()
			void OnOpenFile( wxCommandEvent & p_event );
			void OnOpenProject( wxCommandEvent & p_event );
			void OnWriteSceneFiles( wxCommandEvent & p_event );
			void OnNew( wxCommandEvent & p_event );
			void OnCloseProject( wxCommandEvent & p_event );
			void OnAbout( wxCommandEvent & p_event );
			void OnRename( wxCommandEvent & p_event );
			void OnSaveFile( wxCommandEvent & p_event );
			void OnSaveAll( wxCommandEvent & p_event );
			void OnSaveFileAs( wxCommandEvent & p_event );
			void OnAddExistingFile( wxCommandEvent & p_event );
			void OnSaveSelectedFile( wxCommandEvent & p_event );
			void OnRemoveFile( wxCommandEvent & p_event );
			void OnDeleteFile( wxCommandEvent & p_event );
			void OnFunctionCopy( wxCommandEvent & p_event );
			void OnFunctionCut( wxCommandEvent & p_event );
			void OnFunctionPaste( wxCommandEvent & p_event );
			void OnCloseFile( wxCommandEvent & p_event );
			void OnListFolder( wxCommandEvent & p_event );
			void OnGoToLine( wxCommandEvent & p_event );
			void OnTreeNewScene( wxCommandEvent & p_event );
			void OnEditSceneDependencies( wxCommandEvent & p_event );
			void OnShow( wxShowEvent & p_event );
			void OnSetFocus( wxFocusEvent & p_event );
			void OnKillFocus( wxFocusEvent & p_event );
			void OnActivate( wxActivateEvent & p_event );
			void OnNotebook( wxNotebookEvent & p_event );
			void OnNotebookMiddleDown( wxMouseEvent & p_event );

			void OnSearch( wxCommandEvent & p_event );
			void OnSearchNext( wxCommandEvent & p_event );
			void OnSearchPrevious( wxCommandEvent & p_event );
			void OnReplace( wxCommandEvent & p_event );

			void OnClose( wxCloseEvent & p_event );

		private:
			int m_treesWidth{ 265 };

			wxMenuItem * m_menuTestProject{ nullptr };
			AutoCompletionListBox * m_listBox{ nullptr };

			// TREES
			wxAuiNotebook * m_treeTabsContainer{ nullptr };
			FilesTree * m_filesList{ nullptr };
			FunctionsTree * m_functionsList{ nullptr };
			wxPanel * m_objectsListContainer{ nullptr };
			ObjectsTree  * m_objectsList{ nullptr };
			Properties::PropertiesHolder * m_objectInfosContainer{ nullptr };
			wxListBox * m_objectInfos{ nullptr };

			// MAIN
			wxAuiNotebook * m_mainTabsContainer{ nullptr };
			wxStcTextEditor * m_editText{ nullptr };
			wxPanel * m_mainPanelContainer{ nullptr };
			wxWindow * m_elypseContainer{ nullptr };
			ElypseCtrl * m_elypseCtrl{ nullptr };
			bool m_isFrame{ false };

			int * m_emscriptEditTextId{ nullptr };

			MapTypeEdit m_mapEdit{};

			wxArrayString m_choices{};

			Time::TimeLineContainer * m_timeLineContainer{ nullptr };
			Time::TimeLinePanel * m_timelineEditor{ nullptr };
			Time::TimePanel * m_timePanel{ nullptr };

			bool m_smallToolbar{ true };
			bool m_horzText{ false };
			bool m_useCustomDisabled{ false };
			bool m_showTooltips{ true };
			bool m_resizing{ false };
			ElypseState m_elypseState{ ElypseState::None };

			TextCtrl * m_textWindow{ nullptr };
			SceneDependenciesFrame m_sceneDependencies;

			int m_rows{ 1 };
			size_t m_nPrint{ 1 };

			wxString m_pathBmp{};
			wxString m_currentPage{};

			ProjectComponents::ProjectSPtr m_project{ nullptr };
			ProjectComponents::Scene * m_selectedScene{ nullptr };
			ProjectComponents::Object * m_viewedObject{ nullptr };

			wxSearchCtrl * m_search{ nullptr };

			StcContext m_stcContext{};
		};
	}
}

#endif

