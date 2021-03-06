/*
See LICENSE file in root folder
*/
#ifndef ___GC_StcTextEditor___
#define ___GC_StcTextEditor___

#include "GUI/TrollEditorGuiPrerequisites.h"

#pragma warning( push )
#pragma warning( disable:4996 )

#include <wx/stc/stc.h>

#include "GUI/StcContext.h"

#include "Project/Module_Project.h"

namespace Troll
{
	namespace GUI
	{
		class StcContext;
		class LanguageInfo;

		class wxStcTextEditor
			: public wxStyledTextCtrl
		{
		private:
			typedef enum gcID
			{
				gcID_PROPERTIES = wxID_HIGHEST
				, gcID_INDENTINC
				, gcID_INDENTRED
				, gcID_FINDNEXT
				, gcID_REPLACE
				, gcID_REPLACENEXT
				, gcID_BRACEMATCH
				, gcID_GOTO
				, gcID_PAGEACTIVE
				, gcID_DISPLAYEOL
				, gcID_INDENTGUIDE
				, gcID_LINENUMBER
				, gcID_LONGLINEON
				, gcID_WHITESPACE
				, gcID_FOLDTOGGLE
				, gcID_OVERTYPE
				, gcID_READONLY
				, gcID_WRAPMODEON
				, gcID_CHANGECASE
				, gcID_CHANGELOWER
				, gcID_CHANGEUPPER
				, gcID_HILIGHTLANG
				, gcID_HILIGHTFIRST
				, gcID_HILIGHTLAST = gcID_HILIGHTFIRST + 99
				, gcID_CONVERTEOL
				, gcID_CONVERTCR
				, gcID_CONVERTCRLF
				, gcID_CONVERTLF
				, gcID_USECHARSET
				, gcID_CHARSETANSI
				, gcID_CHARSETMAC
				, gcID_PAGEPREV
				, gcID_PAGENEXT
				, gcID_SELECTLINE
				, gcID_WINDOW_MINIMAL
				, gcID_STATUSBAR
				, gcID_TITLEBAR
				, gcID_ABOUTTIMER
				, gcID_UPDATETIMER
				, gcID_DLG_FIND_TEXT
				, gcID_PREFS_LANGUAGE
				, gcID_PREFS_STYLETYPE
				, gcID_PREFS_KEYWORDS
			}	gcID;

		public:
			wxStcTextEditor( ProjectComponents::File * p_file, StcContext const & p_context, wxWindow * p_pParent, wxWindowID p_id = wxID_ANY, wxPoint const & p_ptPos = wxDefaultPosition, wxSize const & p_size = wxDefaultSize, long p_lStyle = wxVSCROLL );
			virtual ~wxStcTextEditor();

			bool LoadFile( wxString const & p_projectPath );
			bool SaveFile( wxString const & p_projectPath );
			void SetText( wxString const & p_strSource );
			bool IsModified();
			wxString DeterminePrefs( wxString const & p_strFilename );
			bool InitializePrefs( wxString const & p_strFilename );
			void UpdatePrefs();
			void ReinitContext( StcContext const & p_pContext );
			wxString GetFileName();

			inline LanguageInfoPtr GetLanguageInfo()
			{
				return m_pLanguage;
			}

			inline StcContext * GetContext()
			{
				return &m_context;
			}

		private:
			bool DoLoadFile( wxString const & p_strFilename );
			void DoCloseFile();

		private:
			DECLARE_EVENT_TABLE()
			void OnClose( wxCloseEvent & p_event );
			void OnSize( wxSizeEvent & p_event );
			void OnBraceMatch( wxCommandEvent & p_event );
			void OnEditIndentInc( wxCommandEvent & p_event );
			void OnEditIndentRed( wxCommandEvent & p_event );
			void OnEditSelectLine( wxCommandEvent & p_event );
			void OnHilightLang( wxCommandEvent & p_event );
			void OnDisplayEOL( wxCommandEvent & p_event );
			void OnIndentGuide( wxCommandEvent & p_event );
			void OnLineNumber( wxCommandEvent & p_event );
			void OnLongLineOn( wxCommandEvent & p_event );
			void OnWhiteSpace( wxCommandEvent & p_event );
			void OnFoldToggle( wxCommandEvent & p_event );
			void OnSetOverType( wxCommandEvent & p_event );
			void OnSetReadOnly( wxCommandEvent & p_event );
			void OnWrapmodeOn( wxCommandEvent & p_event );
			void OnUseCharset( wxCommandEvent & p_event );
			void OnChangeCase( wxCommandEvent & p_event );
			void OnConvertEOL( wxCommandEvent & p_event );
			void OnMarginClick( wxStyledTextEvent & p_event );

		private:
			LanguageInfoPtr m_pLanguage;
			int m_iLineNrID;
			int m_iLineNrMargin;
			int m_iFoldingID;
			int m_iFoldingMargin;
			int m_iDividerID;
			int m_iTabSpaces{ 4 };
			StcContext m_context;
			ProjectComponents::File * m_file;
			wxString m_fileName;
		};
	}
}

#pragma warning( pop )

#endif
