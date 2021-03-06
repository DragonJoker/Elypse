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
			SetViewEOL( m_context.GetDisplayEOLEnable() );
			SetIndentationGuides( m_context.GetIndentGuideEnable() );
			SetEdgeMode( m_context.GetLongLineOnEnable() ? wxSTC_EDGE_LINE : wxSTC_EDGE_NONE );
			SetWhitespaceForeground( m_context.GetWhiteSpaceEnable(), GuiCommon::ActiveTabColour );
			SetViewWhiteSpace( m_context.GetWhiteSpaceEnable() ? wxSTC_WS_VISIBLEALWAYS : wxSTC_WS_INVISIBLE );
			SetOvertype( m_context.GetOverTypeInitial() );
			SetReadOnly( m_context.GetReadOnlyInitial() );
			SetWrapMode( m_context.GetWrapModeInitial() ? wxSTC_WRAP_WORD : wxSTC_WRAP_NONE );
			// set visibility
			SetVisiblePolicy( wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1 );
			SetXCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );
			SetYCaretPolicy( wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1 );
			// miscellaneous
			m_iLineNrMargin = TextWidth( wxSTC_STYLE_LINENUMBER, wxT( "_999999" ) );
			m_iFoldingMargin = 16;
			CmdKeyClear( wxSTC_KEY_TAB, 0 ); // this is done by the menu accelerator key
			SetLayoutCache( wxSTC_CACHE_PAGE );

			InitializePrefs( DEFAULT_LANGUAGE );
		}

		wxStcTextEditor::~wxStcTextEditor()
		{
			DoCloseFile();
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

		wxString wxStcTextEditor::DeterminePrefs( wxString const & filename )
		{
			wxString result;
			( void )std::find_if( m_context.begin()
				, m_context.end()
				, [&filename, &result]( LanguageInfoPtr const & lookup )
				{
					auto filepattern = lookup->GetFilePattern();
					filepattern.Lower();

					while ( !filepattern.empty() && result.empty() )
					{
						auto cur = filepattern.BeforeFirst( ';' );

						if ( ( cur == filename )
							|| ( cur == ( filename.BeforeLast( '.' ) + wxT( ".*" ) ) )
							|| ( cur == ( wxT( "*." ) + filename.AfterLast( '.' ) ) ) )
						{
							result = lookup->GetName();
						}
						else
						{
							filepattern = filepattern.AfterFirst( ';' );
						}
					}

					return !result.empty();
				} );
			return result;
		}

		bool wxStcTextEditor::InitializePrefs( wxString const & languageName )
		{
			StyleClearAll();
			auto it = std::find_if( m_context.begin()
				, m_context.end()
				, [&languageName]( LanguageInfoPtr const & lookup )
				{
					return lookup->GetName().c_str() == languageName;
				} );

			if ( it != m_context.end() )
			{
				// set lexer and language
				LanguageInfoPtr curInfo = *it;
				SetLexer( curInfo->GetLexerID() );
				m_pLanguage = curInfo;
				StyleInfoPtr const & l_stDefaultStyle = curInfo->GetStyle( eSTC_TYPE_DEFAULT );
				auto const & bgColour = l_stDefaultStyle->GetBackground();
				auto const & fgColour = l_stDefaultStyle->GetForeground();
				wxFont l_font( l_stDefaultStyle->GetFontSize(), wxMODERN, wxNORMAL, wxNORMAL, false, l_stDefaultStyle->GetFontName() );

				// default fonts for all styles!
				for ( int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; i++ )
				{
					StyleSetFont( i, l_font );
					StyleSetForeground( i, fgColour );
					StyleSetBackground( i, bgColour );
					StyleSetBold( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_BOLD ) > 0 );
					StyleSetItalic( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_ITALIC ) > 0 );
					StyleSetUnderline( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_UNDERL ) > 0 );
					StyleSetVisible( i, ( l_stDefaultStyle->GetFontStyle() & eSTC_STYLE_HIDDEN ) == 0 );
				}

				SetBackgroundColour( bgColour );
				SetForegroundColour( fgColour );
				SetSelAlpha( 127 );
				SetSelBackground( true, wxColour( 51, 153, 255, 127 ) );
				SetFoldMarginColour( true, bgColour );
				SetFoldMarginHiColour( true, bgColour );
				// Lines numbers
				StyleSetForeground( wxSTC_STYLE_LINENUMBER, fgColour );
				StyleSetBackground( wxSTC_STYLE_LINENUMBER, bgColour );
				SetMarginType( 0, wxSTC_MARGIN_NUMBER );
				SetMarginWidth( 0, 50 );
				// Symbols
				SetMarginType( 1, wxSTC_MARGIN_SYMBOL );
				SetMarginWidth( 1, 16 );
				// Folding Markers
				SetMarginType( 2, wxSTC_MARGIN_SYMBOL );
				SetMarginMask( 2, wxSTC_MASK_FOLDERS );
				SetMarginSensitive( 2, true );
				SetMarginWidth( 2, 16 );
				// Markers
				MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_VLINE, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUSCONNECTED, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUSCONNECTED, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_TCORNER, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_LCORNER, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDER, wxSTC_MARK_DOTDOTDOT, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_ARROWDOWN, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_DOTDOTDOT, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_ARROWDOWN, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY, fgColour, bgColour );
				MarkerDefine( wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY, fgColour, bgColour );

				MarkerSetBackground( wxSTC_MARKNUM_FOLDER, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDER, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPEN, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPEN, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDERSUB, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDERSUB, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDEREND, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDEREND, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDEROPENMID, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDEROPENMID, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDERMIDTAIL, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDERMIDTAIL, fgColour );
				MarkerSetBackground( wxSTC_MARKNUM_FOLDERTAIL, bgColour );
				MarkerSetForeground( wxSTC_MARKNUM_FOLDERTAIL, fgColour );

				// set common styles
				StyleSetBackground( wxSTC_STYLE_LINENUMBER, bgColour );
				StyleSetForeground( wxSTC_STYLE_LINENUMBER, fgColour );
				StyleSetBackground( wxSTC_STYLE_CONTROLCHAR, bgColour );
				StyleSetForeground( wxSTC_STYLE_CONTROLCHAR, fgColour );
				StyleSetBackground( wxSTC_STYLE_BRACELIGHT, bgColour );
				StyleSetForeground( wxSTC_STYLE_BRACELIGHT, wxColour( wxT( "DARK GREEN" ) ) );
				StyleSetBackground( wxSTC_STYLE_BRACEBAD, bgColour );
				StyleSetForeground( wxSTC_STYLE_BRACEBAD, wxColour( wxT( "DARK RED" ) ) );
				StyleSetBackground( wxSTC_STYLE_INDENTGUIDE, bgColour );
				StyleSetBackground( wxSTC_STYLE_CALLTIP, bgColour );
				StyleSetBackground( wxSTC_STYLE_LASTPREDEFINED, bgColour );

				StyleSetBackground( wxSTC_C_DEFAULT, bgColour );
				StyleSetForeground( wxSTC_C_DEFAULT, fgColour );
				StyleSetBackground( wxSTC_C_COMMENT, bgColour );
				StyleSetBackground( wxSTC_C_COMMENTLINE, bgColour );
				StyleSetBackground( wxSTC_C_COMMENTDOC, bgColour );
				StyleSetBackground( wxSTC_C_COMMENTLINEDOC, bgColour );
				StyleSetBackground( wxSTC_C_COMMENTDOCKEYWORD, bgColour );
				StyleSetBackground( wxSTC_C_COMMENTDOCKEYWORDERROR, bgColour );
				StyleSetBackground( wxSTC_C_PREPROCESSORCOMMENT, bgColour );
#if wxCHECK_VERSION( 3, 1, 0 )
				StyleSetBackground( wxSTC_C_PREPROCESSORCOMMENTDOC, bgColour );
#endif
				// set common styles
				StyleSetForeground( wxSTC_STYLE_CONTROLCHAR, wxColour( wxT( "DARK GREY" ) ) );
				StyleSetForeground( wxSTC_STYLE_BRACELIGHT, wxColour( wxT( "DARK GREEN" ) ) );
				StyleSetForeground( wxSTC_STYLE_BRACEBAD, wxColour( wxT( "RED" ) ) );
				SetWhitespaceBackground( true, bgColour );
				SetWhitespaceForeground( true, fgColour );
				SetCaretForeground( fgColour );

				// initialize settings
				if ( m_context.GetSyntaxEnable() )
				{
					int l_iNbKeywords = 0;
					int l_iStyle;
					eSTC_TYPE l_eType;
					LexerStylesAssoc l_lexerAssoc( curInfo->GetLexerID() );

					for ( int j = eSTC_TYPE_DEFAULT; j < eSTC_TYPE_COUNT; ++j )
					{
						l_eType = eSTC_TYPE( j );
						l_iStyle = l_lexerAssoc[l_eType];
						StyleInfoPtr l_stCurType = curInfo->GetStyle( l_eType );

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

						if ( !curInfo->GetWords( l_eType ).empty() )
						{
							switch ( l_eType )
							{
							case eSTC_TYPE_WORD1:
								SetKeyWords( 0, curInfo->GetWords( l_eType ).c_str() );
								break;

							case eSTC_TYPE_WORD2:
								SetKeyWords( 1, curInfo->GetWords( l_eType ).c_str() );
								break;

							case eSTC_TYPE_WORD3:
								SetKeyWords( 3, curInfo->GetWords( l_eType ).c_str() );
								break;
							}

							++l_iNbKeywords;
						}
					}
				}

				if ( m_context.GetFoldEnable() )
				{
					SetMarginWidth( m_iFoldingID, ( ( curInfo->GetFoldFlags() != 0 ) ? m_iFoldingMargin : 0 ) );
					SetMarginSensitive( m_iFoldingID, ( ( curInfo->GetFoldFlags() != 0 ) ) );
					SetProperty( wxT( "fold" ), ( ( curInfo->GetFoldFlags() != 0 ) ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.comment" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_COMMENT ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.compact" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_COMPACT ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.preprocessor" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_PREPROC ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.html" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_HTML ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.html.preprocessor" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_HTMLPREP ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.comment.python" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_COMMENTPY ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
					SetProperty( wxT( "fold.quotes.python" ), ( ( curInfo->GetFoldFlags() & eSTC_FOLD_QUOTESPY ) > 0 ? wxT( "1" ) : wxT( "0" ) ) );
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

			return it != m_context.end();
		}

		void wxStcTextEditor::UpdatePrefs()
		{
			wxString file;

			if ( m_file )
			{
				wxFileName fileName( make_wxString( m_file->FileName ) );
				file = fileName.GetFullName();
			}
			else
			{
				file = wxT( "coin.emscript" );
			}

			InitializePrefs( DeterminePrefs( file ) );
		}

		void wxStcTextEditor::ReinitContext( StcContext const & p_context )
		{
			m_context = StcContext( p_context );
		}

		wxString wxStcTextEditor::GetFileName()
		{
			return m_file ? m_file->m_scene->GetName() + m_file->FileName : wxT( "NULL" );
		}

		bool wxStcTextEditor::DoLoadFile( wxString const & filename )
		{
			wxStyledTextCtrl::ClearAll();
			wxStyledTextCtrl::SetEOLMode( wxSTC_EOL_LF );
			m_fileName = filename;
			wxStyledTextCtrl::LoadFile( filename );
			wxStyledTextCtrl::ConvertEOLs( wxSTC_EOL_LF );
			wxStyledTextCtrl::EmptyUndoBuffer();
			InitializePrefs( DeterminePrefs( wxFileName{ filename }.GetFullName() ) );
			return true;
		}

		void wxStcTextEditor::DoCloseFile()
		{
			if ( m_file )
			{
				if ( IsModified() )
				{
					wxStyledTextCtrl::SaveFile( m_fileName );
				}

				m_file->EditPanel = nullptr;
				m_file->Open = false;
				m_file = nullptr;
			}
		}

		BEGIN_EVENT_TABLE( wxStcTextEditor, wxStyledTextCtrl )
			EVT_CLOSE( wxStcTextEditor::OnClose )
			EVT_SIZE( wxStcTextEditor::OnSize )
			EVT_MENU( gcID_INDENTINC, wxStcTextEditor::OnEditIndentInc )
			EVT_MENU( gcID_INDENTRED, wxStcTextEditor::OnEditIndentRed )
			EVT_MENU( gcID_SELECTLINE, wxStcTextEditor::OnEditSelectLine )
			EVT_MENU( gcID_BRACEMATCH, wxStcTextEditor::OnBraceMatch )
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
		END_EVENT_TABLE()

		void wxStcTextEditor::OnClose( wxCloseEvent & p_event )
		{
			DoCloseFile();
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

		void wxStcTextEditor::OnEditIndentInc( wxCommandEvent & WXUNUSED( p_event ) )
		{
			CmdKeyExecute( wxSTC_CMD_TAB );
		}

		void wxStcTextEditor::OnEditIndentRed( wxCommandEvent & WXUNUSED( p_event ) )
		{
			CmdKeyExecute( wxSTC_CMD_DELETEBACK );
		}

		void wxStcTextEditor::OnEditSelectLine( wxCommandEvent & WXUNUSED( p_event ) )
		{
			int l_iLineStart = PositionFromLine( GetCurrentLine() );
			int l_iLineEnd = PositionFromLine( GetCurrentLine() + 1 );
			SetSelection( l_iLineStart, l_iLineEnd );
		}

		void wxStcTextEditor::OnHilightLang( wxCommandEvent & p_event )
		{
			InitializePrefs( ( *( m_context.begin() + ( p_event.GetId() - gcID_HILIGHTFIRST ) ) )->GetName() );
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
	}
}
