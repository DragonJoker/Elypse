/*
See LICENSE file in root folder
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

namespace Troll
{
	namespace GUI
	{
		enum MainFrameIds
		{
			Menu_About,
			Menu_CompileFile,
			Menu_SceneDependencies,
			Menu_TimeLine,
			Menu_NewProject,
			Menu_OpenProject,
			Menu_SaveProject,
			Menu_CloseProject,
			Menu_TestProject,
			Menu_EditOverlays,
			Menu_EditScene,
			Menu_GoToLine,
			Menu_Musinate,
			Menu_SaveFile,
			Menu_SaveFileAs,
			Menu_ProjectProperties,
			Menu_Replace,
			Menu_SearchNext,
			Menu_SearchPrevious,
			Menu_OpenFile,
			Menu_Quit,
		};

		typedef std::map< wxString, TextCtrl * > MapTypeEdit;

		class StcContext;
		class TETestPanel;
		class wxStcTextEditor;

		class MainFrame
			: public wxFrame
		{
		public:
			MainFrame( bool p_debug, wxString const & p_title, int p_x, int p_y, int p_w, int p_h );
			virtual ~MainFrame();

			void SetProject( ProjectComponents::ProjectSPtr p_project );
			void LogDebug( wxString const & p_message );
			void LogMessage( wxString const & p_message );
			void LogError( wxString const & p_message );
			void CloseProject();
			void OpenProjectFile( wxString const & p_fileName );

			ProjectComponents::ProjectSPtr GetProject()const;
			bool IsEditingScene()const;
			bool IsEditingOverlays()const;
			bool IsTestingScene()const;
			bool IsEditingTimeLine()const;
			ObjectsTree * GetObjectsList()const;
			Time::TimeLinePanel * GetTimelineEditor()const;
			Time::TimeLineContainer * GetTimeLineContainer()const;
			void GotoFileLine( wxString const & p_file, uint32_t p_line );

			inline wxString GetAppDir()const
			{
				return m_appPath;
			}
			inline ProjectFrame * GetProjectFrame()const
			{
				return m_projectFrame;
			}
			inline void LogDebug( String const & p_message )
			{
				LogDebug( GuiCommon::make_wxString( p_message ) );
			}
			inline void LogMessage( String const & p_message )
			{
				LogMessage( GuiCommon::make_wxString( p_message ) );
			}
			inline void LogError( String const & p_message )
			{
				LogError( GuiCommon::make_wxString( p_message ) );
			}

		private:
			void DoPopulateToolbar();
			void DoInitialiseMainContainers();
			void DoInitialiseLogs();
			void DoInitialise();
			void DoSetMenuBar();
			void DoCreateTreeWithDefStyle();

			DECLARE_EVENT_TABLE()
			void OnOpenProject( wxCommandEvent & p_event );
			void OnNewProject( wxCommandEvent & p_event );
			void OnCloseProject( wxCommandEvent & p_event );
			void OnSaveProject( wxCommandEvent & p_event );
			void OnAbout( wxCommandEvent & p_event );
			void OnCompile( wxCommandEvent & p_event );
			void OnMusinate( wxCommandEvent & p_event );
			void OnProjectProperties( wxCommandEvent & p_event );
			void OnQuit( wxCommandEvent & p_event );
			void OnSize( wxSizeEvent & p_event );
			void OnClose( wxCloseEvent & p_event );
			void OnEditTimeLine( wxCommandEvent & p_event );
			void OnEditProject( wxCommandEvent & p_event );
			void OnEditOverlays( wxCommandEvent & p_event );
			void OnTestProject( wxCommandEvent & p_event );
			void OnSceneDependencies( wxCommandEvent & p_event );

		private:
			int m_logsHeight{ 200 };

			// MAIN
			ProjectFrame * m_projectFrame{ nullptr };
			wxPanel * m_projectFrameContainer{ nullptr };

			// LOGS
			wxAuiNotebook * m_logTabsContainer{ nullptr };
			LogCtrl * m_debugLog{ nullptr };
			LogCtrl * m_messageLog{ nullptr };
			LogCtrl * m_errorLog{ nullptr };
			wxLog * m_logDebug{ nullptr };
			wxLog * m_logMessage{ nullptr };
			wxLog * m_logError{ nullptr };
			std::streambuf * m_oldCout{ nullptr };
			std::streambuf * m_oldClog{ nullptr };
			std::streambuf * m_oldCerr{ nullptr };

			wxAuiManager m_manager;

			bool m_smallToolbar{ true };
			bool m_horzText{ false };
			bool m_useCustomDisabled{ false };
			bool m_showTooltips{ true };
			bool m_resizing{ false };

			wxMenuBar * m_menuBar{ nullptr };
			wxMenu * m_menuFile{ nullptr };
			wxMenu * m_menuEdition{ nullptr };
			wxMenu * m_menuHelp{ nullptr };
			wxMenu * m_menuProject{ nullptr };
			wxMenuItem * m_menuEditProject{ nullptr };
			wxMenuItem * m_menuEditTimeLines{ nullptr };
			wxMenuItem * m_menuEditOverlays{ nullptr };
			wxMenuItem * m_menuTestProject{ nullptr };

			wxToolBar * m_extraToolBar{ nullptr };
			wxToolBar * m_tbar{ nullptr };

			int m_rows{ 1 };
			size_t m_nPrint{ 1 };

			wxString m_pathBmp;
			uint32_t m_toolbarPosition{ wxTOP };

			wxString m_appPath;
			bool m_debug;
		};
	}
}

#endif

