/*
See LICENSE file in root folder
*/
#ifndef ___GC_StcContext___
#define ___GC_StcContext___

#include "GUI/TrollEditorGuiPrerequisites.h"

#pragma warning( push )
#pragma warning( disable:4996 )

#define DEFAULT_LANGUAGE	wxT( "<default>" )

#include <wx/stc/stc.h>
#include <memory>
#include <array>
#include <vector>

namespace Troll
{
	namespace GUI
	{
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Lexers enumeration
		\~french
		\brief Enumération des lexers
		*/
		typedef enum eSTC_LEX
		{
			eSTC_LEX_CONTAINER = wxSTC_LEX_CONTAINER,
			eSTC_LEX_NULL = wxSTC_LEX_NULL,
			eSTC_LEX_PYTHON = wxSTC_LEX_PYTHON,
			eSTC_LEX_CPP = wxSTC_LEX_CPP,
			eSTC_LEX_HTML = wxSTC_LEX_HTML,
			eSTC_LEX_XML = wxSTC_LEX_XML,
			eSTC_LEX_PERL = wxSTC_LEX_PERL,
			eSTC_LEX_SQL = wxSTC_LEX_SQL,
			eSTC_LEX_VB = wxSTC_LEX_VB,
			eSTC_LEX_PROPERTIES = wxSTC_LEX_PROPERTIES,
			eSTC_LEX_ERRORLIST = wxSTC_LEX_ERRORLIST,
			eSTC_LEX_MAKEFILE = wxSTC_LEX_MAKEFILE,
			eSTC_LEX_BATCH = wxSTC_LEX_BATCH,
			eSTC_LEX_XCODE = wxSTC_LEX_XCODE,
			eSTC_LEX_LATEX = wxSTC_LEX_LATEX,
			eSTC_LEX_LUA = wxSTC_LEX_LUA,
			eSTC_LEX_DIFF = wxSTC_LEX_DIFF,
			eSTC_LEX_CONF = wxSTC_LEX_CONF,
			eSTC_LEX_PASCAL = wxSTC_LEX_PASCAL,
			eSTC_LEX_AVE = wxSTC_LEX_AVE,
			eSTC_LEX_ADA = wxSTC_LEX_ADA,
			eSTC_LEX_LISP = wxSTC_LEX_LISP,
			eSTC_LEX_RUBY = wxSTC_LEX_RUBY,
			eSTC_LEX_EIFFEL = wxSTC_LEX_EIFFEL,
			eSTC_LEX_TCL = wxSTC_LEX_TCL,
			eSTC_LEX_NNCRONTAB = wxSTC_LEX_NNCRONTAB,
			eSTC_LEX_BULLANT = wxSTC_LEX_BULLANT,
			eSTC_LEX_VBSCRIPT = wxSTC_LEX_VBSCRIPT,
			eSTC_LEX_BAAN = wxSTC_LEX_BAAN,
			eSTC_LEX_MATLAB = wxSTC_LEX_MATLAB,
			eSTC_LEX_SCRIPTOL = wxSTC_LEX_SCRIPTOL,
			eSTC_LEX_ASM = wxSTC_LEX_ASM,
			eSTC_LEX_FORTRAN = wxSTC_LEX_FORTRAN,
			eSTC_LEX_F77 = wxSTC_LEX_F77,
			eSTC_LEX_CSS = wxSTC_LEX_CSS,
			eSTC_LEX_POV = wxSTC_LEX_POV,
			eSTC_LEX_LOUT = wxSTC_LEX_LOUT,
			eSTC_LEX_ESCRIPT = wxSTC_LEX_ESCRIPT,
			eSTC_LEX_PS = wxSTC_LEX_PS,
			eSTC_LEX_NSIS = wxSTC_LEX_NSIS,
			eSTC_LEX_MMIXAL = wxSTC_LEX_MMIXAL,
			eSTC_LEX_CLW = wxSTC_LEX_CLW,
			eSTC_LEX_LOT = wxSTC_LEX_LOT,
			eSTC_LEX_YAML = wxSTC_LEX_YAML,
			eSTC_LEX_TEX = wxSTC_LEX_TEX,
			eSTC_LEX_METAPOST = wxSTC_LEX_METAPOST,
			eSTC_LEX_POWERBASIC = wxSTC_LEX_POWERBASIC,
			eSTC_LEX_FORTH = wxSTC_LEX_FORTH,
			eSTC_LEX_ERLANG = wxSTC_LEX_ERLANG,
			eSTC_LEX_OCTAVE = wxSTC_LEX_OCTAVE,
			eSTC_LEX_MSSQL = wxSTC_LEX_MSSQL,
			eSTC_LEX_VERILOG = wxSTC_LEX_VERILOG,
			eSTC_LEX_KIX = wxSTC_LEX_KIX,
			eSTC_LEX_GUI4CLI = wxSTC_LEX_GUI4CLI,
			eSTC_LEX_SPECMAN = wxSTC_LEX_SPECMAN,
			eSTC_LEX_AU3 = wxSTC_LEX_AU3,
			eSTC_LEX_APDL = wxSTC_LEX_APDL,
			eSTC_LEX_BASH = wxSTC_LEX_BASH,
			eSTC_LEX_ASN1 = wxSTC_LEX_ASN1,
			eSTC_LEX_VHDL = wxSTC_LEX_VHDL,
			eSTC_LEX_CAML = wxSTC_LEX_CAML,
			eSTC_LEX_BLITZBASIC = wxSTC_LEX_BLITZBASIC,
			eSTC_LEX_PUREBASIC = wxSTC_LEX_PUREBASIC,
			eSTC_LEX_HASKELL = wxSTC_LEX_HASKELL,
			eSTC_LEX_PHPSCRIPT = wxSTC_LEX_PHPSCRIPT,
			eSTC_LEX_TADS3 = wxSTC_LEX_TADS3,
			eSTC_LEX_REBOL = wxSTC_LEX_REBOL,
			eSTC_LEX_SMALLTALK = wxSTC_LEX_SMALLTALK,
			eSTC_LEX_FLAGSHIP = wxSTC_LEX_FLAGSHIP,
			eSTC_LEX_CSOUND = wxSTC_LEX_CSOUND,
			eSTC_LEX_FREEBASIC = wxSTC_LEX_FREEBASIC,
			eSTC_LEX_INNOSETUP = wxSTC_LEX_INNOSETUP,
			eSTC_LEX_OPAL = wxSTC_LEX_OPAL,
			eSTC_LEX_SPICE = wxSTC_LEX_SPICE,
#if wxVERSION_NUMBER >= 2900
			eSTC_LEX_D = wxSTC_LEX_D,
			eSTC_LEX_CMAKE = wxSTC_LEX_CMAKE,
			eSTC_LEX_GAP = wxSTC_LEX_GAP,
			eSTC_LEX_PLM = wxSTC_LEX_PLM,
			eSTC_LEX_PROGRESS = wxSTC_LEX_PROGRESS,
			eSTC_LEX_ABAQUS = wxSTC_LEX_ABAQUS,
			eSTC_LEX_ASYMPTOTE = wxSTC_LEX_ASYMPTOTE,
			eSTC_LEX_R = wxSTC_LEX_R,
			eSTC_LEX_MAGIK = wxSTC_LEX_MAGIK,
			eSTC_LEX_POWERSHELL = wxSTC_LEX_POWERSHELL,
			eSTC_LEX_MYSQL = wxSTC_LEX_MYSQL,
			eSTC_LEX_PO = wxSTC_LEX_PO,
			eSTC_LEX_TAL = wxSTC_LEX_TAL,
			eSTC_LEX_COBOL = wxSTC_LEX_COBOL,
			eSTC_LEX_TACL = wxSTC_LEX_TACL,
			eSTC_LEX_SORCUS = wxSTC_LEX_SORCUS,
			eSTC_LEX_POWERPRO = wxSTC_LEX_POWERPRO,
			eSTC_LEX_NIMROD = wxSTC_LEX_NIMROD,
			eSTC_LEX_SML = wxSTC_LEX_SML,
			eSTC_LEX_MARKDOWN = wxSTC_LEX_MARKDOWN,
#endif
			eSTC_LEX_COUNT
		}	eSTC_LEX;

		class StcContext;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Word and symbol type enumeration
		\~french
		\brief Enumération des différents types de mots et symboles
		*/
		typedef enum eSTC_TYPE
		{
			eSTC_TYPE_DEFAULT,
			eSTC_TYPE_WORD1,
			eSTC_TYPE_WORD2,
			eSTC_TYPE_WORD3,
			eSTC_TYPE_WORD4,
			eSTC_TYPE_WORD5,
			eSTC_TYPE_WORD6,
			eSTC_TYPE_COMMENT,
			eSTC_TYPE_COMMENT_DOC,
			eSTC_TYPE_COMMENT_LINE,
			eSTC_TYPE_COMMENT_SPECIAL,
			eSTC_TYPE_CHARACTER,
			eSTC_TYPE_CHARACTER_EOL,
			eSTC_TYPE_STRING,
			eSTC_TYPE_STRING_EOL,
			eSTC_TYPE_DELIMITER,
			eSTC_TYPE_PUNCTUATION,
			eSTC_TYPE_OPERATOR,
			eSTC_TYPE_BRACE,
			eSTC_TYPE_COMMAND,
			eSTC_TYPE_IDENTIFIER,
			eSTC_TYPE_LABEL,
			eSTC_TYPE_NUMBER,
			eSTC_TYPE_PARAMETER,
			eSTC_TYPE_REGEX,
			eSTC_TYPE_UUID,
			eSTC_TYPE_VALUE,
			eSTC_TYPE_PREPROCESSOR,
			eSTC_TYPE_SCRIPT,
			eSTC_TYPE_VERBATIM,
			eSTC_TYPE_GLOBAL_CLASS,
			eSTC_TYPE_ERROR,
			eSTC_TYPE_UNDEFINED,
			eSTC_TYPE_COUNT,
		}	eSTC_TYPE;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Text style enumeration
		\~french
		\brief Enumération des styles de texte
		*/
		typedef enum eSTC_STYLE
		{
			eSTC_STYLE_BOLD = 0x01,
			eSTC_STYLE_ITALIC = 0x02,
			eSTC_STYLE_UNDERL = 0x04,
			eSTC_STYLE_HIDDEN = 0x08,
		}	eSTC_STYLE;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english Folded types enumeration
		\brief
		\~french
		\brief Enumération des types de dblocks pouvant être réduits
		*/
		typedef enum eSTC_FOLD
		{
			eSTC_FOLD_COMMENT = 0x01,
			eSTC_FOLD_COMPACT = 0x02,
			eSTC_FOLD_PREPROC = 0x04,
			eSTC_FOLD_HTML = 0x10,
			eSTC_FOLD_HTMLPREP = 0x20,
			eSTC_FOLD_COMMENTPY = 0x40,
			eSTC_FOLD_QUOTESPY = 0x80,
		}	eSTC_FOLD;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Scintilla flag enumeration
		\~french
		\brief Enumération des flags scintilla
		*/
		typedef enum eSTC_FLAG
		{
			eSTC_FLAG_WRAPMODE = 0x10
		}	eSTC_FLAG;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Defines style informations for a given type of words
		\~french
		\brief Définit les informations de style pour un type de mots
		*/
		class StyleInfo
		{
		public:
			StyleInfo( wxString const & p_strName, wxString const & p_strForeground, wxString const & p_strBackground, wxString const & p_strFontName, int p_iFontsize, int p_iFontstyle, int p_iLetterCase );
			StyleInfo( StyleInfo const & p_infos );
			StyleInfo & operator = ( StyleInfo const & p_infos );
			virtual ~StyleInfo();

			inline int GetLetterCase()const
			{
				return m_iLetterCase;
			}
			inline int GetFontStyle()const
			{
				return m_iFontStyle;
			}
			inline int GetFontSize()const
			{
				return m_iFontSize;
			}
			inline wxString	const & GetFontName()const
			{
				return m_strFontName;
			}
			inline wxString	const & GetBackground()const
			{
				return m_strBackground;
			}
			inline wxString	const & GetForeground()const
			{
				return m_strForeground;
			}
			inline wxString	const & GetName()const
			{
				return m_strName;
			}
			inline void SetName( wxString const & val )
			{
				m_strName = val;
			}
			inline void SetForeground( wxString const & val )
			{
				m_strForeground = val;
			}
			inline void SetBackground( wxString const & val )
			{
				m_strBackground = val;
			}
			inline void SetFontName( wxString const & val )
			{
				m_strFontName = val;
			}
			inline void SetFontSize( int val )
			{
				m_iFontSize = val;
			}
			inline void SetFontStyle( int val )
			{
				m_iFontStyle = val;
			}
			inline void SetLetterCase( int val )
			{
				m_iLetterCase = val;
			}

		private:
			wxString m_strName;
			wxString m_strForeground;
			wxString m_strBackground;
			wxString m_strFontName;
			int m_iFontSize;
			int m_iFontStyle;
			int m_iLetterCase;
		};
		//!\~english Typedef over a pointer to a StyleInfo	\~french Typedef d'un pointeur sur un StyleInfo
		typedef std::shared_ptr< StyleInfo > StyleInfoPtr;
		//!\~english Typedef over a static array of StyleInfoPtr	\~french Typedef d'un tableau statique de StyleInfoPtr
		typedef std::array< StyleInfoPtr, eSTC_TYPE_COUNT > StyleInfoPtrArray;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Defines language informations (name, file patterns, Scintilla lexer, ...)
		\~french
		\brief Définit les informations d'un langage (nom, extension de fichier, lexer Scintilla, ...)
		*/
		class LanguageInfo
		{
		private:
			typedef std::array< wxString, eSTC_TYPE_COUNT > WordArray;

		public:
			LanguageInfo();
			virtual ~LanguageInfo();

			void SetWords( eSTC_TYPE p_eType, wxArrayString const & p_arrayWords );
			void AddWord( eSTC_TYPE p_eType, wxString const & p_strWord );
			StyleInfoPtr CreateStyle( wxString const & p_strType, eSTC_TYPE p_eType );

			inline int GetFoldFlags()const
			{
				return m_iFoldFlags;
			}
			inline wxString const & GetWords( eSTC_TYPE p_eType )const
			{
				return m_arrayWords[p_eType];
			}
			inline wxString const & GetFilePattern()const
			{
				return m_strFilePattern;
			}
			inline wxString const & GetName()const
			{
				return m_strName;
			}
			inline eSTC_LEX GetLexerID()const
			{
				return m_eLexerID;
			}
			inline StyleInfoPtr & GetStyle( eSTC_TYPE p_eType )
			{
				return m_arrayStyles[p_eType];
			}
			inline StyleInfoPtr const & GetStyle( eSTC_TYPE p_eType )const
			{
				return m_arrayStyles[p_eType];
			}

			inline void SetLexer( eSTC_LEX p_eLexer )
			{
				m_eLexerID = p_eLexer;
			}
			inline void SetName( wxString const & p_strVal )
			{
				m_strName = p_strVal;
			}
			inline void SetFilePattern( wxString const & p_strVal )
			{
				m_strFilePattern = p_strVal;
			}
			inline void SetFoldFlags( int p_iFlags )
			{
				m_iFoldFlags = p_iFlags;
			}

		private:
			wxString m_strName;
			wxString m_strFilePattern;
			WordArray m_arrayWords;
			eSTC_LEX m_eLexerID;
			int m_iFoldFlags;
			StyleInfoPtrArray m_arrayStyles;
		};
		typedef std::shared_ptr< LanguageInfo > LanguageInfoPtr;
		typedef std::vector< LanguageInfoPtr > LanguageInfoPtrArray;
		typedef LanguageInfoPtrArray::iterator LanguagesIterator;
		typedef LanguageInfoPtrArray::const_iterator LanguagesConstIterator;
		/*!
		\author Sylvain DOREMUS
		\version 0.6.1.0
		\date 19/10/2011
		\~english
		\brief Holds the general context used by Scintilla
		\~french
		\brief Contient le contexte général utilisé par Scintilla
		*/
		class StcContext
		{
		public:
			StcContext();
			virtual ~StcContext();

			void ParseFile( wxString const & fileName );
			LanguagesConstIterator find( wxString const & name )const;
			LanguagesIterator find( wxString const & name );

			inline std::size_t GetNbLanguages()const
			{
				return m_arrayLanguages.size();
			}
			inline bool GetWhiteSpaceEnable()const
			{
				return m_bWhiteSpaceEnable;
			}
			inline bool GetLongLineOnEnable()const
			{
				return m_bLongLineOnEnable;
			}
			inline bool GetLineNumberEnable()const
			{
				return m_bLineNumberEnable;
			}
			inline bool GetIndentGuideEnable()const
			{
				return m_bIndentGuideEnable;
			}
			inline bool GetDisplayEOLEnable()const
			{
				return m_bDisplayEOLEnable;
			}
			inline bool GetWrapModeInitial()const
			{
				return m_bWrapModeInitial;
			}
			inline bool GetOverTypeInitial()const
			{
				return m_bOverTypeInitial;
			}
			inline bool GetReadOnlyInitial()const
			{
				return m_bReadOnlyInitial;
			}
			inline bool GetIndentEnable()const
			{
				return m_bIndentEnable;
			}
			inline bool GetFoldEnable()const
			{
				return m_bFoldEnable;
			}
			inline bool GetSyntaxEnable()const
			{
				return m_bSyntaxEnable;
			}
			inline void AddLanguage( LanguageInfoPtr p_pLanguage )
			{
				m_arrayLanguages.push_back( p_pLanguage );
			}
			inline void SetSyntaxEnable( bool val )
			{
				m_bSyntaxEnable = val;
			}
			inline void SetFoldEnable( bool val )
			{
				m_bFoldEnable = val;
			}
			inline void SetIndentEnable( bool val )
			{
				m_bIndentEnable = val;
			}
			inline void SetReadOnlyInitial( bool val )
			{
				m_bReadOnlyInitial = val;
			}
			inline void SetOverTypeInitial( bool val )
			{
				m_bOverTypeInitial = val;
			}
			inline void SetWrapModeInitial( bool val )
			{
				m_bWrapModeInitial = val;
			}
			inline void SetDisplayEOLEnable( bool val )
			{
				m_bDisplayEOLEnable = val;
			}
			inline void SetIndentGuideEnable( bool val )
			{
				m_bIndentGuideEnable = val;
			}
			inline void SetLineNumberEnable( bool val )
			{
				m_bLineNumberEnable = val;
			}
			inline void SetLongLineOnEnable( bool val )
			{
				m_bLongLineOnEnable = val;
			}
			inline void SetWhiteSpaceEnable( bool val )
			{
				m_bWhiteSpaceEnable = val;
			}
			inline std::size_t size()const
			{
				return m_arrayLanguages.size();
			}
			inline LanguagesIterator begin()
			{
				return m_arrayLanguages.begin();
			}
			inline LanguagesConstIterator begin()const
			{
				return m_arrayLanguages.begin();
			}
			inline LanguagesConstIterator end()const
			{
				return m_arrayLanguages.end();
			}
			inline LanguagesIterator end()
			{
				return m_arrayLanguages.end();
			}

		private:
			bool m_bSyntaxEnable;
			bool m_bFoldEnable;
			bool m_bIndentEnable;
			bool m_bReadOnlyInitial;
			bool m_bOverTypeInitial;
			bool m_bWrapModeInitial;
			bool m_bDisplayEOLEnable;
			bool m_bIndentGuideEnable;
			bool m_bLineNumberEnable;
			bool m_bLongLineOnEnable;
			bool m_bWhiteSpaceEnable;
			LanguageInfoPtrArray m_arrayLanguages;
		};
	}
}

#endif
