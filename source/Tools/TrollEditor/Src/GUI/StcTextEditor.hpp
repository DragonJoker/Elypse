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
#ifndef ___GC_StcTextEditor___
#define ___GC_StcTextEditor___

#include "GUI/TrollEditorGuiPrerequisites.h"

#pragma warning( push )
#pragma warning( disable:4996 )

#include <wx/stc/stc.h>

#include "GUI/StcContext.hpp"

#include "Project/Module_Project.h"

BEGIN_TROLL_GUI_NAMESPACE
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
		wxStcTextEditor( TROLL_PROJECT_NAMESPACE::File * p_file, StcContext const & p_context, wxWindow * p_pParent, wxWindowID p_id = wxID_ANY, wxPoint const & p_ptPos = wxDefaultPosition, wxSize const & p_size = wxDefaultSize, long p_lStyle = wxVSCROLL );
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
		bool DoUserSettings( wxString const & p_strFilename );

	private:
		DECLARE_EVENT_TABLE()
		void OnClose( wxCloseEvent & p_event );
		void OnSize( wxSizeEvent & p_event );
		void OnEditRedo( wxCommandEvent & p_event );
		void OnEditUndo( wxCommandEvent & p_event );
		void OnEditClear( wxCommandEvent & p_event );
		void OnEditCut( wxCommandEvent & p_event );
		void OnEditCopy( wxCommandEvent & p_event );
		void OnEditPaste( wxCommandEvent & p_event );
		void OnFind( wxCommandEvent & p_event );
		void OnFindNext( wxCommandEvent & p_event );
		void OnReplace( wxCommandEvent & p_event );
		void OnReplaceNext( wxCommandEvent & p_event );
		void OnBraceMatch( wxCommandEvent & p_event );
		void OnGoto( wxCommandEvent & p_event );
		void OnEditIndentInc( wxCommandEvent & p_event );
		void OnEditIndentRed( wxCommandEvent & p_event );
		void OnEditSelectAll( wxCommandEvent & p_event );
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
		void OnCharAdded( wxStyledTextEvent & p_event );
		void OnKey( wxStyledTextEvent & p_event );

	private:
		LanguageInfoPtr m_pLanguage;
		int m_iLineNrID;
		int m_iLineNrMargin;
		int m_iFoldingID;
		int m_iFoldingMargin;
		int m_iDividerID;
		StcContext m_context;
		TROLL_PROJECT_NAMESPACE::File * m_file;
	};
}
END_TROLL_GUI_NAMESPACE

#pragma warning( pop )

#endif
