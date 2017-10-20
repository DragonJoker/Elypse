/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/StcTextEditor.h"

#include "Project/Scene.h"

#include <wx/filename.h>
#include <wx/msgdlg.h>

namespace Troll
{
	namespace GUI
	{
		struct LexerStylesAssoc
		{
			int m_iStyles[eSTC_TYPE_COUNT];

			LexerStylesAssoc( eSTC_LEX p_eLex )
			{
				memset( m_iStyles, 0, sizeof( m_iStyles ) );

				if ( p_eLex == eSTC_LEX_CPP )
				{
					int iStyles[eSTC_TYPE_COUNT] =
					{
						wxSTC_C_DEFAULT, //eSTC_TYPE_DEFAULT
						wxSTC_C_WORD, //eSTC_TYPE_WORD1
						wxSTC_C_WORD2, //eSTC_TYPE_WORD2
						wxSTC_C_DEFAULT, //eSTC_TYPE_WORD3
						wxSTC_C_GLOBALCLASS, //eSTC_TYPE_WORD4
						wxSTC_C_DEFAULT, //eSTC_TYPE_WORD5
						wxSTC_C_DEFAULT, //eSTC_TYPE_WORD6
						wxSTC_C_COMMENT, //eSTC_TYPE_COMMENT
						wxSTC_C_COMMENTDOC, //eSTC_TYPE_COMMENT_DOC
						wxSTC_C_COMMENTLINE, //eSTC_TYPE_COMMENT_LINE
						wxSTC_C_DEFAULT, //eSTC_TYPE_COMMENT_SPECIAL
						wxSTC_C_CHARACTER, //eSTC_TYPE_CHARACTER
						wxSTC_C_DEFAULT, //eSTC_TYPE_CHARACTER_EOL
						wxSTC_C_STRING, //eSTC_TYPE_STRING
						wxSTC_C_STRINGEOL, //eSTC_TYPE_STRING_EOL
						wxSTC_C_DEFAULT, //eSTC_TYPE_DELIMITER
						wxSTC_C_DEFAULT, //eSTC_TYPE_PUNCTUATION
						wxSTC_C_OPERATOR, //eSTC_TYPE_OPERATOR
						wxSTC_C_DEFAULT, //eSTC_TYPE_BRACE
						wxSTC_C_DEFAULT, //eSTC_TYPE_COMMAND
						wxSTC_C_IDENTIFIER, //eSTC_TYPE_IDENTIFIER
						wxSTC_C_DEFAULT, //eSTC_TYPE_LABEL
						wxSTC_C_NUMBER, //eSTC_TYPE_NUMBER
						wxSTC_C_DEFAULT, //eSTC_TYPE_PARAMETER
						wxSTC_C_REGEX, //eSTC_TYPE_REGEX
						wxSTC_C_UUID, //eSTC_TYPE_UUID
						wxSTC_C_DEFAULT, //eSTC_TYPE_VALUE
						wxSTC_C_PREPROCESSOR, //eSTC_TYPE_PREPROCESSOR
						wxSTC_C_DEFAULT, //eSTC_TYPE_SCRIPT
						wxSTC_C_VERBATIM, //eSTC_TYPE_VERBATIM
						wxSTC_C_GLOBALCLASS, //eSTC_TYPE_GLOBAL_CLASS
						wxSTC_C_DEFAULT, //eSTC_TYPE_ERROR
						wxSTC_C_DEFAULT, //eSTC_TYPE_UNDEFINED
					};
					memcpy( m_iStyles, iStyles, sizeof( iStyles ) );
				}
			}

			inline int operator []( std::size_t p_uiIndex )
			{
				return m_iStyles[p_uiIndex];
			}
		};

		//*************************************************************************************************

		wxStcTextEditor::wxStcTextEditor( ProjectComponents::File * p_file, StcContext const & p_context, wxWindow * p_pParent, wxWindowID p_id, wxPoint const & p_ptPos, wxSize const & p_size, long p_lStyle )
			: wxStyledTextCtrl( p_pParent, p_id, p_ptPos, p_size, p_lStyle )
			, m_file( p_file )
			, m_context( p_context )
			, m_iLineNrID( 0 )
			, m_iDividerID( 1 )
			, m_iFoldingID( 2 )
			, m_pLanguage()
		{
			wxFont font( 10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );
			SetTabWidth( m_iTabSpaces );
			SetSelAlpha( 127 );
			SetSelBackground( true, wxColour( 51, 153, 255, 127 ) );
			SetViewEOL( m_context.GetDisplayEOLEnable() );
			SetIndentationGuides( m_context.GetIndentGuideEnable() );
			SetEdgeMode( m_context.GetLongLineOnEnable() ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE );
			SetViewWhiteSpace( m_context.GetWhiteSpaceEnable() ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE );
			SetOvertype( m_context.GetOverTypeInitial() );
			SetReadOnly( m_context.GetReadOnlyInitial() );
			SetWrapMode( m_context.GetWrapModeInitial() ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE );
			StyleSetFont( wxSTC_STYLE_DEFAULT, font );
			InitializePrefs( DEFAULT_LANGUAGE );
			// set visibility
			SetVisiblePolicy( wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1 );
			SetXCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );
			SetYCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );
			// markers
			MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_DOTDOTDOT, wxT( "BLACK" ), wxT( "BLACK" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, wxT( "BLACK" ), wxT( "BLACK" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, wxT( "BLACK" ), wxT( "BLACK" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_DOTDOTDOT, wxT( "BLACK" ), wxT( "WHITE" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, wxT( "BLACK" ), wxT( "WHITE" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, wxT( "BLACK" ), wxT( "BLACK" ) );
			MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, wxT( "BLACK" ), wxT( "BLACK" ) );
			// miscellaneous
			m_iLineNrMargin = TextWidth( wxSTC_STYLE_LINENUMBER, wxT( "_999999" ) );
			m_iFoldingMargin = 16;
			CmdKeyClear( wxSTC_KEY_TAB, 0 ); // this is done by the menu accelerator key
			SetLayoutCache( wxSTC_CACHE_PAGE );
		}

		wxStcTextEditor::~wxStcTextEditor()
		{
			if ( m_file )
			{
				m_file->Open = false;
			}
		}

		bool wxStcTextEditor::LoadFile( wxString const & p_projectPath )
		{
			return DoLoadFile( p_projectPath + m_file->m_scene->GetName() + wxFileName::GetPathSeparator() + m_file->FileName );
		}

		bool wxStcTextEditor::SaveFile( wxString const & p_projectPath )
		{
			bool l_bReturn = true;

			if ( IsModified() )
			{
				l_bReturn = wxStyledTextCtrl::SaveFile( p_projectPath + m_file->m_scene->GetName() + wxFileName::GetPathSeparator() + m_file->FileName );
			}

			return l_bReturn;
		}

		void wxStcTextEditor::SetText( wxString const & p_strSource )
		{
			wxStyledTextCtrl::ClearAll();
			wxStyledTextCtrl::SetEOLMode( wxSTC_EOL_LF );
			wxStyledTextCtrl::SetText( p_strSource );
			wxStyledTextCtrl::ConvertEOLs( wxSTC_EOL_LF );
			wxStyledTextCtrl::EmptyUndoBuffer();
		}

		bool wxStcTextEditor::IsModified()
		{
			return ( GetModify() && !GetReadOnly() );
		}

		wxString wxStcTextEditor::DeterminePrefs( wxString const & p_strFilename )
		{
			LanguageInfoPtr l_pCurInfo;
			wxString l_strReturn;
			wxString l_strFilepattern;
			wxString l_strCur;

			for ( LanguagesConstIterator l_it = m_context.Begin(); l_it != m_context.End() && l_strReturn.empty(); ++l_it )
			{
				l_pCurInfo = *l_it;
				l_strFilepattern = l_pCurInfo->GetFilePattern();
				l_strFilepattern.Lower();

				while ( !l_strFilepattern.empty() && l_strReturn.empty() )
				{
					l_strCur = l_strFilepattern.BeforeFirst( ';' );

					if ( ( l_strCur == p_strFilename ) || ( l_strCur == ( p_strFilename.BeforeLast( '.' ) + wxT( ".*" ) ) ) || ( l_strCur == ( wxT( "*." ) + p_strFilename.AfterLast( '.' ) ) ) )
					{
						l_strReturn = l_pCurInfo->GetName();
					}
					else
					{
						l_strFilepattern = l_strFilepattern.AfterFirst( ';' );
					}
				}
			}

			return l_strReturn;
		}

		bool wxStcTextEditor::InitializePrefs( wxString const & p_strName )
		{
			StyleClearAll();
			LanguageInfoPtr l_pCurInfo;
			bool l_bFound = false;

			for ( LanguagesConstIterator l_it = m_context.Begin(); l_it != m_context.End() && !l_bFound; ++l_it )
			{
				l_pCurInfo = *l_it;

				if ( l_pCurInfo->GetName().c_str() == p_strName )
				{
					l_bFound = true;
				}
			}

			if ( l_bFound )
			{
				// set lexer and language
				SetLexer( l_pCurInfo->GetLexerID() );
				m_pLanguage = l_pCurInfo;
				StyleInfoPtr const & l_stDefaultStyle = l_pCurInfo->GetStyle( eSTC_TYPE_DEFAULT );
				wxFont l_font( l_stDefaultStyle->GetFontSize(), wxMODERN, wxNORMAL, wxNORMAL, false, l_stDefaultStyle->GetFontName() );

				// default fonts for all styles!
				for ( int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; i++ )
				{
					StyleSetFont( i, l_font );
					StyleSetForeground( i, l_stDefaultStyle->GetForeground() );
					StyleSetBackground( i, l_stDefaultStyle->GetBackground() );
					StyleSetBold( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_BOLD ) > 0 );
					StyleSetItalic( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_ITALIC ) > 0 );
					StyleSetUnderline( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_UNDERL ) > 0 );
					StyleSetVisible( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_HIDDEN ) == 0 );
				}

				SetBackgroundColour( l_stDefaultStyle->GetBackground() );
				SetForegroundColour( l_stDefaultStyle->GetForeground() );
				// Lines numbers
				StyleSetForeground( wxSTC_STYLE_LINENUMBER, l_stDefaultStyle->GetForeground() );
				StyleSetBackground( wxSTC_STYLE_LINENUMBER, l_stDefaultStyle->GetBackground() );
				m_iLineNrMargin = 50;
				SetMarginType( 0, wxSTC_MARGIN_NUMBER );
				SetMarginWidth( 0, m_iLineNrMargin );
				// Symbols
				m_iFoldingMargin = 16;
				SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
				SetMarginWidth( 1, m_iFoldingMargin );
				// Folding Markers
				SetMarginType( 2, wxSTC_MARGIN_SYMBOL );
				SetMarginMask( 2, wxSTC_MASK_FOLDERS );
				SetMarginSensitive( 2, true );
				SetMarginWidth( 2, m_iFoldingMargin );
				// Markers
				MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, l_stDefaultStyle->GetForeground(), l_stDefaultStyle->GetBackground() );
				// set common styles
				StyleSetForeground( wxSTC_STYLE_CONTROLCHAR, wxColour( wxT( "DARK GREY" ) ) );
				StyleSetForeground( wxSTC_STYLE_BRACELIGHT, wxColour( wxT( "DARK GREEN" ) ) );
				StyleSetForeground( wxSTC_STYLE_BRACEBAD, wxColour( wxT( "RED" ) ) );
				SetWhitespaceBackground( true, l_stDefaultStyle->GetBackground() );
				SetWhitespaceForeground( true, l_stDefaultStyle->GetForeground() );
				SetCaretForeground( l_stDefaultStyle->GetForeground() );

				// initialize settings
				if ( m_context.GetSyntaxEnable() )
				{
					int l_iNbKeywords = 0;
					int l_iStyle;
					eSTC_TYPE l_eType;
					LexerStylesAssoc l_lexerAssoc( l_pCurInfo->GetLexerID() );

					for ( int j = eSTC_TYPE_DEFAULT; j < eSTC_TYPE_COUNT; ++j )
					{
						l_eType = eSTC_TYPE( j );
						l_iStyle = l_lexerAssoc[l_eType];
						StyleInfoPtr l_stCurType = l_pCurInfo->GetStyle( l_eType );

						if ( !l_stCurType )
						{
							l_stCurType = l_stDefaultStyle;
						}

						wxFont l_font( l_stCurType->GetFontSize(), wxMODERN, wxNORMAL, wxNORMAL, false, l_stCurType->GetFontName() );
						StyleSetFont( l_iStyle, l_font );

						if ( !l_stCurType->GetForeground().empty() )
						{
							StyleSetForeground( l_iStyle, wxColour( l_stCurType->GetForeground() ) );
						}

						if ( !l_stCurType->GetBackground().empty() )
						{
							StyleSetBackground( l_iStyle, wxColour( l_stCurType->GetBackground() ) );
						}

						StyleSetBold( l_iStyle, ( l_stCurType->GetFontStyle() & eSTC_STYLE_BOLD ) > 0 );
						StyleSetItalic( l_iStyle, ( l_stCurType->GetFontStyle() & eSTC_STYLE_ITALIC ) > 0 );
						StyleSetUnderline( l_iStyle, ( l_stCurType->GetFontStyle() & eSTC_STYLE_UNDERL ) > 0 );
						StyleSetVisible( l_iStyle, ( l_stCurType->GetFontStyle() & eSTC_STYLE_HIDDEN ) == 0 );
						StyleSetCase( l_iStyle, ( l_stCurType->GetLetterCase() ) );

						if ( !l_pCurInfo->GetWords( l_eType ).empty() )
						{
							switch ( l_eType )
							{
							case eSTC_TYPE_WORD1:
								SetKeyWords( 0, l_pCurInfo->GetWords( l_eType ).c_str() );
								break;

							case eSTC_TYPE_WORD2:
								SetKeyWords( 1, l_pCurInfo->GetWords( l_eType ).c_str() );
								break;

							case eSTC_TYPE_WORD3:
								SetKeyWords( 3, l_pCurInfo->GetWords( l_eType ).c_str() );
								break;
							}

							++l_iNbKeywords;
						}
					}
				}

				if ( m_context.GetFoldEnable() )
				{
					SetMarginWidth( m_iFoldingID, ( ( l_pCurInfo->GetFoldFlags() != 0 ) ? m_iFoldingMargin : 0 ) );
					SetMarginSensitive( m_iFoldingID, ( ( l_pCurInfo->GetFoldFlags() != 0 ) ) );
					SetProperty( wxT( "fold" ), ( ( l_pCurInfo->GetFoldFlags() != 0 ) ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.comment" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_COMMENT ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.compact" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_COMPACT ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.preprocessor" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_PREPROC ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.html" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_HTML ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.html.preprocessor" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_HTMLPREP ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.comment.python" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_COMMENTPY ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.quotes.python" ), ( ( l_pCurInfo->GetFoldFlags() & eSTC_FOLD_QUOTESPY ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
				}

				SetFoldFlags( wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED );
				// set spaces and indention
				SetTabWidth( 4 );
				SetUseTabs( true );
				SetTabIndents( true );
				SetBackSpaceUnIndents( true );
				SetIndent( m_context.GetIndentEnable() ? 4 : 0 );
				// others
				SetViewEOL( m_context.GetDisplayEOLEnable() );
				SetIndentationGuides( m_context.GetIndentGuideEnable() );
				SetEdgeColumn( 80 );
				SetEdgeMode( m_context.GetLongLineOnEnable() ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE );
				SetViewWhiteSpace( m_context.GetWhiteSpaceEnable() ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE );
				SetOvertype( m_context.GetOverTypeInitial() );
				SetReadOnly( m_context.GetReadOnlyInitial() );
				SetWrapMode( m_context.GetWrapModeInitial() ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE );
			}

			return l_bFound;
		}

		void wxStcTextEditor::UpdatePrefs()
		{
			wxFileName l_fileName( make_wxString( m_file->FileName ) );
			InitializePrefs( DeterminePrefs( l_fileName.GetFullName() ) );
		}

		void wxStcTextEditor::ReinitContext( StcContext const & p_context )
		{
			m_context = StcContext( p_context );
		}

		wxString wxStcTextEditor::GetFileName()
		{
			return m_file ? m_file->m_scene->GetName() + m_file->FileName : wxT( "NULL" );
		}

		bool wxStcTextEditor::DoLoadFile( wxString const & p_strFilename )
		{
			wxStyledTextCtrl::ClearAll();
			wxStyledTextCtrl::SetEOLMode( wxSTC_EOL_LF );
			wxFileName l_fileName( p_strFilename );
			InitializePrefs( DeterminePrefs( l_fileName.GetFullName() ) );
			wxStyledTextCtrl::LoadFile( p_strFilename );
			wxStyledTextCtrl::ConvertEOLs( wxSTC_EOL_LF );
			wxStyledTextCtrl::EmptyUndoBuffer();
			return true;
		}

		BEGIN_EVENT_TABLE( wxStcTextEditor, wxStyledTextCtrl )
			EVT_CLOSE( wxStcTextEditor::OnClose )
			EVT_SIZE( wxStcTextEditor::OnSize )
			EVT_MENU( wxID_CLEAR, wxStcTextEditor::OnEditClear )
			EVT_MENU( wxID_CUT, wxStcTextEditor::OnEditCut )
			EVT_MENU( wxID_COPY, wxStcTextEditor::OnEditCopy )
			EVT_MENU( wxID_PASTE, wxStcTextEditor::OnEditPaste )
			EVT_MENU( gcID_INDENTINC, wxStcTextEditor::OnEditIndentInc )
			EVT_MENU( gcID_INDENTRED, wxStcTextEditor::OnEditIndentRed )
			EVT_MENU( wxID_SELECTALL, wxStcTextEditor::OnEditSelectAll )
			EVT_MENU( gcID_SELECTLINE, wxStcTextEditor::OnEditSelectLine )
			EVT_MENU( wxID_REDO, wxStcTextEditor::OnEditRedo )
			EVT_MENU( wxID_UNDO, wxStcTextEditor::OnEditUndo )
			EVT_MENU( wxID_FIND, wxStcTextEditor::OnFind )
			EVT_MENU( gcID_FINDNEXT, wxStcTextEditor::OnFindNext )
			EVT_MENU( gcID_REPLACE, wxStcTextEditor::OnReplace )
			EVT_MENU( gcID_REPLACENEXT, wxStcTextEditor::OnReplaceNext )
			EVT_MENU( gcID_BRACEMATCH, wxStcTextEditor::OnBraceMatch )
			EVT_MENU( gcID_GOTO, wxStcTextEditor::OnGoto )
			EVT_MENU_RANGE( gcID_HILIGHTFIRST, gcID_HILIGHTLAST, wxStcTextEditor::OnHilightLang )
			EVT_MENU( gcID_DISPLAYEOL, wxStcTextEditor::OnDisplayEOL )
			EVT_MENU( gcID_INDENTGUIDE, wxStcTextEditor::OnIndentGuide )
			EVT_MENU( gcID_LINENUMBER, wxStcTextEditor::OnLineNumber )
			EVT_MENU( gcID_LONGLINEON, wxStcTextEditor::OnLongLineOn )
			EVT_MENU( gcID_WHITESPACE, wxStcTextEditor::OnWhiteSpace )
			EVT_MENU( gcID_FOLDTOGGLE, wxStcTextEditor::OnFoldToggle )
			EVT_MENU( gcID_OVERTYPE, wxStcTextEditor::OnSetOverType )
			EVT_MENU( gcID_READONLY, wxStcTextEditor::OnSetReadOnly )
			EVT_MENU( gcID_WRAPMODEON, wxStcTextEditor::OnWrapmodeOn )
			EVT_MENU( gcID_CHARSETANSI, wxStcTextEditor::OnUseCharset )
			EVT_MENU( gcID_CHARSETMAC, wxStcTextEditor::OnUseCharset )
			EVT_MENU( gcID_CHANGELOWER, wxStcTextEditor::OnChangeCase )
			EVT_MENU( gcID_CHANGEUPPER, wxStcTextEditor::OnChangeCase )
			EVT_MENU( gcID_CONVERTCR, wxStcTextEditor::OnConvertEOL )
			EVT_MENU( gcID_CONVERTCRLF, wxStcTextEditor::OnConvertEOL )
			EVT_MENU( gcID_CONVERTLF, wxStcTextEditor::OnConvertEOL )
			EVT_STC_MARGINCLICK( wxID_ANY, wxStcTextEditor::OnMarginClick )
			EVT_STC_CHARADDED( wxID_ANY, wxStcTextEditor::OnCharAdded )
			EVT_STC_KEY( wxID_ANY, wxStcTextEditor::OnKey )
		END_EVENT_TABLE()

		void wxStcTextEditor::OnClose( wxCloseEvent & p_event )
		{
			if ( m_file )
			{
				m_file->Open = false;
			}

			p_event.Skip();
		}

		void wxStcTextEditor::OnSize( wxSizeEvent & p_event )
		{
			int x = GetClientSize().x + ( m_context.GetLineNumberEnable() ? m_iLineNrMargin : 0 ) + ( m_context.GetFoldEnable() ? m_iFoldingMargin : 0 );

			if ( x > 0 )
			{
				SetScrollWidth( x );
			}

			p_event.Skip();
		}

		void wxStcTextEditor::OnEditRedo( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( CanRedo() )
			{
				Redo();
			}
		}

		void wxStcTextEditor::OnEditUndo( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( CanUndo() )
			{
				Undo();
			}
		}

		void wxStcTextEditor::OnEditClear( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( !GetReadOnly() )
			{
				Clear();
			}
		}

		void wxStcTextEditor::OnKey( wxStyledTextEvent & WXUNUSED( p_event ) )
		{
			wxMessageBox( wxT( "OnKey" ) );
		}

		void wxStcTextEditor::OnEditCut( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( !GetReadOnly() && ( GetSelectionEnd() - GetSelectionStart() ) > 0 )
			{
				Cut();
			}
		}

		void wxStcTextEditor::OnEditCopy( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( GetSelectionEnd() - GetSelectionStart() > 0 )
			{
				Copy();
			}
		}

		void wxStcTextEditor::OnEditPaste( wxCommandEvent & WXUNUSED( p_event ) )
		{
			if ( CanPaste() )
			{
				Paste();
			}
		}

		void wxStcTextEditor::OnFind( wxCommandEvent & WXUNUSED( p_event ) )
		{
		}

		void wxStcTextEditor::OnFindNext( wxCommandEvent & WXUNUSED( p_event ) )
		{
		}

		void wxStcTextEditor::OnReplace( wxCommandEvent & WXUNUSED( p_event ) )
		{
		}

		void wxStcTextEditor::OnReplaceNext( wxCommandEvent & WXUNUSED( p_event ) )
		{
		}

		void wxStcTextEditor::OnBraceMatch( wxCommandEvent & WXUNUSED( p_event ) )
		{
			int l_iMin = GetCurrentPos();
			int l_iMax = BraceMatch( l_iMin );

			if ( l_iMax > ( l_iMin + 1 ) )
			{
				BraceHighlight( l_iMin + 1, l_iMax );
				SetSelection( l_iMin + 1, l_iMax );
			}
			else
			{
				BraceBadLight( l_iMin );
			}
		}

		void wxStcTextEditor::OnGoto( wxCommandEvent & WXUNUSED( p_event ) )
		{
		}

		void wxStcTextEditor::OnEditIndentInc( wxCommandEvent & WXUNUSED( p_event ) )
		{
			CmdKeyExecute( wxSTC_CMD_TAB );
		}

		void wxStcTextEditor::OnEditIndentRed( wxCommandEvent & WXUNUSED( p_event ) )
		{
			CmdKeyExecute( wxSTC_CMD_DELETEBACK );
		}

		void wxStcTextEditor::OnEditSelectAll( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetSelection( 0, GetTextLength() );
		}

		void wxStcTextEditor::OnEditSelectLine( wxCommandEvent & WXUNUSED( p_event ) )
		{
			int l_iLineStart = PositionFromLine( GetCurrentLine() );
			int l_iLineEnd = PositionFromLine( GetCurrentLine() + 1 );
			SetSelection( l_iLineStart, l_iLineEnd );
		}

		void wxStcTextEditor::OnHilightLang( wxCommandEvent & p_event )
		{
			InitializePrefs( ( *( m_context.Begin() + ( p_event.GetId() - gcID_HILIGHTFIRST ) ) )->GetName() );
		}

		void wxStcTextEditor::OnDisplayEOL( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetViewEOL( !GetViewEOL() );
		}

		void wxStcTextEditor::OnIndentGuide( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetIndentationGuides( !GetIndentationGuides() );
		}

		void wxStcTextEditor::OnLineNumber( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetMarginWidth( m_iLineNrID, GetMarginWidth( m_iLineNrID ) == 0 ? m_iLineNrMargin : 0 );
		}

		void wxStcTextEditor::OnLongLineOn( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetEdgeMode( GetEdgeMode() == 0 ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE );
		}

		void wxStcTextEditor::OnWhiteSpace( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetViewWhiteSpace( GetViewWhiteSpace() == 0 ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE );
		}

		void wxStcTextEditor::OnFoldToggle( wxCommandEvent & WXUNUSED( p_event ) )
		{
			ToggleFold( GetFoldParent( GetCurrentLine() ) );
		}

		void wxStcTextEditor::OnSetOverType( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetOvertype( !GetOvertype() );
		}

		void wxStcTextEditor::OnSetReadOnly( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetReadOnly( !GetReadOnly() );
		}

		void wxStcTextEditor::OnWrapmodeOn( wxCommandEvent & WXUNUSED( p_event ) )
		{
			SetWrapMode( GetWrapMode() == 0 ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE );
		}

		void wxStcTextEditor::OnUseCharset( wxCommandEvent & p_event )
		{
			int l_iCharset = GetCodePage();

			switch ( p_event.GetId() )
			{
			case gcID_CHARSETANSI:
				l_iCharset = wxSTC_CHARSET_ANSI;
				break;

			case gcID_CHARSETMAC:
				l_iCharset = wxSTC_CHARSET_ANSI;
				break;
			}

			for ( int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; ++i )
			{
				StyleSetCharacterSet( i, l_iCharset );
			}

			SetCodePage( l_iCharset );
		}

		void wxStcTextEditor::OnChangeCase( wxCommandEvent & p_event )
		{
			switch ( p_event.GetId() )
			{
			case gcID_CHANGELOWER:
				CmdKeyExecute( wxSTC_CMD_LOWERCASE );
				break;

			case gcID_CHANGEUPPER:
				CmdKeyExecute( wxSTC_CMD_UPPERCASE );
				break;
			}
		}

		void wxStcTextEditor::OnConvertEOL( wxCommandEvent & p_event )
		{
			int l_iEolMode = GetEOLMode();

			switch ( p_event.GetId() )
			{
			case gcID_CONVERTCR:
				l_iEolMode = wxSTC_EOL_CR;
				break;

			case gcID_CONVERTCRLF:
				l_iEolMode = wxSTC_EOL_CRLF;
				break;

			case gcID_CONVERTLF:
				l_iEolMode = wxSTC_EOL_LF;
				break;
			}

			ConvertEOLs( l_iEolMode );
			SetEOLMode( l_iEolMode );
		}

		void wxStcTextEditor::OnMarginClick( wxStyledTextEvent & p_event )
		{
			if ( p_event.GetMargin() == 2 )
			{
				int l_iLineClick = LineFromPosition( p_event.GetPosition() );
				int l_iLevelClick = GetFoldLevel( l_iLineClick );

				if ( ( l_iLevelClick & wxSTC_FOLDLEVELHEADERFLAG ) > 0 )
				{
					ToggleFold( l_iLineClick );
				}
			}
		}

		void wxStcTextEditor::OnCharAdded( wxStyledTextEvent & p_event )
		{
			char l_chr = ( char )p_event.GetKey();
			int l_iCurrentLine = GetCurrentLine();

			if ( l_chr == '\n' )
			{
				int l_iLineInd = 0;

				if ( l_iCurrentLine > 0 )
				{
					l_iLineInd = GetLineIndentation( l_iCurrentLine - 1 );
				}

				if ( l_iLineInd != 0 )
				{
					SetLineIndentation( l_iCurrentLine, l_iLineInd );
					GotoPos( PositionFromLine( l_iCurrentLine ) + l_iLineInd );
				}
			}
		}
	}
}
