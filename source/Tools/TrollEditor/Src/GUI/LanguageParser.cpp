#include "PrecompiledHeader.h"

#include "LanguageParser.hpp"

using namespace Troll::GUI;

#if defined( _MSC_VER )
#	pragma warning( push )
#	pragma warning( disable:4100 )
#endif

//*************************************************************************************************

LanguageFileContext::LanguageFileContext( FILE * p_pFile )
	:	FileParserContext( p_pFile )
{
	mapTypes[wxT( "default"	)] = eSTC_TYPE_DEFAULT;
	mapTypes[wxT( "word1"	)] = eSTC_TYPE_WORD1;
	mapTypes[wxT( "word2"	)] = eSTC_TYPE_WORD2;
	mapTypes[wxT( "word3"	)] = eSTC_TYPE_WORD3;
	mapTypes[wxT( "word4"	)] = eSTC_TYPE_WORD4;
	mapTypes[wxT( "word5"	)] = eSTC_TYPE_WORD5;
	mapTypes[wxT( "word6"	)] = eSTC_TYPE_WORD6;
	mapTypes[wxT( "comment"	)] = eSTC_TYPE_COMMENT;
	mapTypes[wxT( "comment_doc"	)] = eSTC_TYPE_COMMENT_DOC;
	mapTypes[wxT( "comment_line"	)] = eSTC_TYPE_COMMENT_LINE;
	mapTypes[wxT( "comment_special"	)] = eSTC_TYPE_COMMENT_SPECIAL;
	mapTypes[wxT( "char"	)] = eSTC_TYPE_CHARACTER;
	mapTypes[wxT( "char_eol"	)] = eSTC_TYPE_CHARACTER_EOL;
	mapTypes[wxT( "string"	)] = eSTC_TYPE_STRING;
	mapTypes[wxT( "string_eol"	)] = eSTC_TYPE_STRING_EOL;
	mapTypes[wxT( "delimiter"	)] = eSTC_TYPE_DELIMITER;
	mapTypes[wxT( "punctuation"	)] = eSTC_TYPE_PUNCTUATION;
	mapTypes[wxT( "operator"	)] = eSTC_TYPE_OPERATOR;
	mapTypes[wxT( "brace"	)] = eSTC_TYPE_BRACE;
	mapTypes[wxT( "command"	)] = eSTC_TYPE_COMMAND;
	mapTypes[wxT( "identifier"	)] = eSTC_TYPE_IDENTIFIER;
	mapTypes[wxT( "label"	)] = eSTC_TYPE_LABEL;
	mapTypes[wxT( "number"	)] = eSTC_TYPE_NUMBER;
	mapTypes[wxT( "parameter"	)] = eSTC_TYPE_PARAMETER;
	mapTypes[wxT( "regex"	)] = eSTC_TYPE_REGEX;
	mapTypes[wxT( "uuid"	)] = eSTC_TYPE_UUID;
	mapTypes[wxT( "value"	)] = eSTC_TYPE_VALUE;
	mapTypes[wxT( "preprocessor"	)] = eSTC_TYPE_PREPROCESSOR;
	mapTypes[wxT( "script"	)] = eSTC_TYPE_SCRIPT;
	mapTypes[wxT( "verbatim"	)] = eSTC_TYPE_VERBATIM;
	mapTypes[wxT( "global_class"	)] = eSTC_TYPE_GLOBAL_CLASS;
	mapTypes[wxT( "error"	)] = eSTC_TYPE_ERROR;
	mapTypes[wxT( "undefined"	)] = eSTC_TYPE_UNDEFINED;
	mapLexers[wxT( "Container"	)] =	eSTC_LEX_CONTAINER	;
	mapLexers[wxT( "Null"	)] =	eSTC_LEX_NULL		;
	mapLexers[wxT( "Python"	)] =	eSTC_LEX_PYTHON		;
	mapLexers[wxT( "C/C++"	)] =	eSTC_LEX_CPP		;
	mapLexers[wxT( "HTML"	)] =	eSTC_LEX_HTML		;
	mapLexers[wxT( "XML"	)] =	eSTC_LEX_XML		;
	mapLexers[wxT( "Perl"	)] =	eSTC_LEX_PERL		;
	mapLexers[wxT( "SQL"	)] =	eSTC_LEX_SQL		;
	mapLexers[wxT( "VB"	)] =	eSTC_LEX_VB			;
	mapLexers[wxT( "Properties"	)] =	eSTC_LEX_PROPERTIES	;
	mapLexers[wxT( "ErrorList"	)] =	eSTC_LEX_ERRORLIST	;
	mapLexers[wxT( "Makefile"	)] =	eSTC_LEX_MAKEFILE	;
	mapLexers[wxT( "Batch"	)] =	eSTC_LEX_BATCH		;
	mapLexers[wxT( "XCode"	)] =	eSTC_LEX_XCODE		;
	mapLexers[wxT( "Latex"	)] =	eSTC_LEX_LATEX		;
	mapLexers[wxT( "LUA"	)] =	eSTC_LEX_LUA		;
	mapLexers[wxT( "Diff"	)] =	eSTC_LEX_DIFF		;
	mapLexers[wxT( "Conf"	)] =	eSTC_LEX_CONF		;
	mapLexers[wxT( "Pascal"	)] =	eSTC_LEX_PASCAL		;
	mapLexers[wxT( "Ave"	)] =	eSTC_LEX_AVE		;
	mapLexers[wxT( "Ada"	)] =	eSTC_LEX_ADA		;
	mapLexers[wxT( "Lisp"	)] =	eSTC_LEX_LISP		;
	mapLexers[wxT( "Ruby"	)] =	eSTC_LEX_RUBY		;
	mapLexers[wxT( "Eiffel"	)] =	eSTC_LEX_EIFFEL		;
	mapLexers[wxT( "TCL"	)] =	eSTC_LEX_TCL		;
	mapLexers[wxT( "NnCrontTab"	)] =	eSTC_LEX_NNCRONTAB	;
	mapLexers[wxT( "Bullant"	)] =	eSTC_LEX_BULLANT	;
	mapLexers[wxT( "VBScript"	)] =	eSTC_LEX_VBSCRIPT	;
	mapLexers[wxT( "BAAN"	)] =	eSTC_LEX_BAAN		;
	mapLexers[wxT( "Matlab"	)] =	eSTC_LEX_MATLAB		;
	mapLexers[wxT( "Scriptol"	)] =	eSTC_LEX_SCRIPTOL	;
	mapLexers[wxT( "Asm"	)] =	eSTC_LEX_ASM		;
	mapLexers[wxT( "Fortran"	)] =	eSTC_LEX_FORTRAN	;
	mapLexers[wxT( "F77"	)] =	eSTC_LEX_F77		;
	mapLexers[wxT( "CSS"	)] =	eSTC_LEX_CSS		;
	mapLexers[wxT( "POV"	)] =	eSTC_LEX_POV		;
	mapLexers[wxT( "Lout"	)] =	eSTC_LEX_LOUT		;
	mapLexers[wxT( "EScript"	)] =	eSTC_LEX_ESCRIPT	;
	mapLexers[wxT( "PS"	)] =	eSTC_LEX_PS			;
	mapLexers[wxT( "NSIS"	)] =	eSTC_LEX_NSIS		;
	mapLexers[wxT( "MMixal"	)] =	eSTC_LEX_MMIXAL		;
	mapLexers[wxT( "CLW"	)] =	eSTC_LEX_CLW		;
	mapLexers[wxT( "LOT"	)] =	eSTC_LEX_LOT		;
	mapLexers[wxT( "YAML"	)] =	eSTC_LEX_YAML		;
	mapLexers[wxT( "Tex"	)] =	eSTC_LEX_TEX		;
	mapLexers[wxT( "Metapost"	)] =	eSTC_LEX_METAPOST	;
	mapLexers[wxT( "PowerBasic"	)] =	eSTC_LEX_POWERBASIC	;
	mapLexers[wxT( "Forth"	)] =	eSTC_LEX_FORTH		;
	mapLexers[wxT( "Erlang"	)] =	eSTC_LEX_ERLANG		;
	mapLexers[wxT( "Octave"	)] =	eSTC_LEX_OCTAVE		;
	mapLexers[wxT( "MSSQL"	)] =	eSTC_LEX_MSSQL		;
	mapLexers[wxT( "Verilog"	)] =	eSTC_LEX_VERILOG	;
	mapLexers[wxT( "Kix"	)] =	eSTC_LEX_KIX		;
	mapLexers[wxT( "Gui4Cli"	)] =	eSTC_LEX_GUI4CLI	;
	mapLexers[wxT( "Specman"	)] =	eSTC_LEX_SPECMAN	;
	mapLexers[wxT( "AU3"	)] =	eSTC_LEX_AU3		;
	mapLexers[wxT( "APDL"	)] =	eSTC_LEX_APDL		;
	mapLexers[wxT( "Bash"	)] =	eSTC_LEX_BASH		;
	mapLexers[wxT( "ASN1"	)] =	eSTC_LEX_ASN1		;
	mapLexers[wxT( "VHDL"	)] =	eSTC_LEX_VHDL		;
	mapLexers[wxT( "XAML"	)] =	eSTC_LEX_CAML		;
	mapLexers[wxT( "BlitzBasic"	)] =	eSTC_LEX_BLITZBASIC	;
	mapLexers[wxT( "PureBasic"	)] =	eSTC_LEX_PUREBASIC	;
	mapLexers[wxT( "Haskell"	)] =	eSTC_LEX_HASKELL	;
	mapLexers[wxT( "PhpScript"	)] =	eSTC_LEX_PHPSCRIPT	;
	mapLexers[wxT( "TADS3"	)] =	eSTC_LEX_TADS3		;
	mapLexers[wxT( "Rebol"	)] =	eSTC_LEX_REBOL		;
	mapLexers[wxT( "SmallTalk"	)] =	eSTC_LEX_SMALLTALK	;
	mapLexers[wxT( "Flagship"	)] =	eSTC_LEX_FLAGSHIP	;
	mapLexers[wxT( "CSound"	)] =	eSTC_LEX_CSOUND		;
	mapLexers[wxT( "FreeBasic"	)] =	eSTC_LEX_FREEBASIC	;
	mapLexers[wxT( "InnoSetup"	)] =	eSTC_LEX_INNOSETUP	;
	mapLexers[wxT( "Opal"	)] =	eSTC_LEX_OPAL		;
	mapLexers[wxT( "Spice"	)] =	eSTC_LEX_SPICE		;
#if wxVERSION_NUMBER >= 2900
	mapLexers[wxT( "D"	)] =	eSTC_LEX_D			;
	mapLexers[wxT( "CMake"	)] =	eSTC_LEX_CMAKE		;
	mapLexers[wxT( "Gap"	)] =	eSTC_LEX_GAP		;
	mapLexers[wxT( "PLM"	)] =	eSTC_LEX_PLM		;
	mapLexers[wxT( "Progress"	)] =	eSTC_LEX_PROGRESS	;
	mapLexers[wxT( "Abaqs"	)] =	eSTC_LEX_ABAQUS		;
	mapLexers[wxT( "Asymptote"	)] =	eSTC_LEX_ASYMPTOTE	;
	mapLexers[wxT( "R"	)] =	eSTC_LEX_R			;
	mapLexers[wxT( "Magik"	)] =	eSTC_LEX_MAGIK		;
	mapLexers[wxT( "PowerShell"	)] =	eSTC_LEX_POWERSHELL	;
	mapLexers[wxT( "MySQL"	)] =	eSTC_LEX_MYSQL		;
	mapLexers[wxT( "PO"	)] =	eSTC_LEX_PO			;
	mapLexers[wxT( "TAL"	)] =	eSTC_LEX_TAL		;
	mapLexers[wxT( "Cobol"	)] =	eSTC_LEX_COBOL		;
	mapLexers[wxT( "TACL"	)] =	eSTC_LEX_TACL		;
	mapLexers[wxT( "Sorcus"	)] =	eSTC_LEX_SORCUS		;
	mapLexers[wxT( "PowerPro"	)] =	eSTC_LEX_POWERPRO	;
	mapLexers[wxT( "Nimrod"	)] =	eSTC_LEX_NIMROD		;
	mapLexers[wxT( "SML"	)] =	eSTC_LEX_SML		;
	mapLexers[wxT( "Markdown"	)] =	eSTC_LEX_MARKDOWN	;
#endif
	mapFoldFlags[wxT( "comment"	)] = eSTC_FOLD_COMMENT;
	mapFoldFlags[wxT( "compact"	)] = eSTC_FOLD_COMPACT;
	mapFoldFlags[wxT( "preprocessor" )] = eSTC_FOLD_PREPROC;
}

//*************************************************************************************************

LanguageFileParser::LanguageFileParser( StcContext * p_pStcContext )
	:	FileParser( eSECTION_ROOT, eSECTION_COUNT )
	,	m_pStcContext( p_pStcContext )
{
}

LanguageFileParser::~LanguageFileParser()
{
}

void LanguageFileParser::DoInitialiseParser( FILE * p_file )
{
	AddParser( eSECTION_ROOT,		wxT( "language"	),	Root_Language	);
	AddParser( eSECTION_LANGUAGE,	wxT( "pattern"	),	Language_Pattern	);
	AddParser( eSECTION_LANGUAGE,	wxT( "lexer"	),	Language_Lexer	);
	AddParser( eSECTION_LANGUAGE,	wxT( "fold_flags"	),	Language_FoldFlags	);
	AddParser( eSECTION_LANGUAGE,	wxT( "section"	),	Language_Section	);
	AddParser( eSECTION_LANGUAGE,	wxT( "style"	),	Language_Style	);
	AddParser( eSECTION_LANGUAGE,	wxT( "}"	),	Language_End	);
	AddParser( eSECTION_STYLE,		wxT( "type"	),	Style_Type	);
	AddParser( eSECTION_STYLE,		wxT( "fg_colour"	),	Style_FgColour	);
	AddParser( eSECTION_STYLE,		wxT( "bg_colour"	),	Style_BgColour	);
	AddParser( eSECTION_STYLE,		wxT( "font_name"	),	Style_FontName	);
	AddParser( eSECTION_STYLE,		wxT( "font_style"	),	Style_FontStyle	);
	AddParser( eSECTION_STYLE,		wxT( "font_size"	),	Style_FontSize	);
	AddParser( eSECTION_STYLE,		wxT( "}"	),	Style_End	);
	AddParser( eSECTION_SECTION,	wxT( "type"	),	Section_Type	);
	AddParser( eSECTION_SECTION,	wxT( "list"	),	Section_List	);
	AddParser( eSECTION_SECTION,	wxT( "style"	),	Section_Style	);
	AddParser( eSECTION_SECTION,	wxT( "}"	),	Section_End	);
	AddParser( eSECTION_LIST,		wxT( "}"	),	List_End	);
	LanguageFileContextPtr l_pContext( new LanguageFileContext( p_file ) );
	m_pParsingContext = l_pContext;
	l_pContext->pCurrentLanguage = std::make_shared< LanguageInfo >();
}

void LanguageFileParser::DoCleanupParser()
{
	std::static_pointer_cast< LanguageFileContext >( m_pParsingContext )->pCurrentLanguage.reset();
}

void LanguageFileParser::DoDiscardParser( wxString const & p_strLine )
{
	if ( m_pParsingContext->stackSections.top() == eSECTION_LIST )
	{
		wxString l_strWords( p_strLine );
		l_strWords.Replace( wxT( "\\" ), wxT( "" ) );
		wxArrayString l_arrayWords = split( trim( l_strWords ), wxT( "\t " ), 1000, false );
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( m_pParsingContext );
		l_pContext->arrayWords.insert( l_pContext->arrayWords.end(), l_arrayWords.begin(), l_arrayWords.end() );
	}
	else
	{
//		Logger::LogWarning( wxT( "Parser not found @ line #d : %S" ), m_pParsingContext->ui64Line, p_strLine.c_str() );
	}
}

void LanguageFileParser::DoValidate()
{
	m_pStcContext->AddLanguage( std::static_pointer_cast< LanguageFileContext >( m_pParsingContext )->pCurrentLanguage );
}

//*************************************************************************************************

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Root_Language )
{
	if ( ! p_strParams.empty() )
	{
		std::static_pointer_cast< LanguageFileContext >( p_pContext )->pCurrentLanguage->SetName( p_strParams );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <language> must be followed by a name : language <name>" ) );
	}

	ATTRIBUTE_END_PUSH( eSECTION_LANGUAGE );
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_Pattern )
{
	if ( ! p_strParams.empty() )
	{
		wxArrayString l_array = split( p_strParams, wxT( "\t ,;" ), 100, false );
		wxString l_strPatterns;

		for ( std::size_t i = 0 ; i < l_array.size() ; i++ )
		{
			if ( ! l_strPatterns.empty() )
			{
				l_strPatterns += wxT( ";" );
			}

			l_strPatterns += l_array[i];
		}

		std::static_pointer_cast< LanguageFileContext >( p_pContext )->pCurrentLanguage->SetFilePattern( l_strPatterns );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <pattern> must be followed by a list of file pattern : pattern *.glsl [*.frag ...]" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_Lexer )
{
	if ( ! p_strParams.empty() )
	{
		wxString l_strParams( p_strParams );
		std::static_pointer_cast< LanguageFileContext >( p_pContext )->pCurrentLanguage->SetLexer( std::static_pointer_cast< LanguageFileContext >( p_pContext )->mapLexers[l_strParams.Trim()] );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <lexer> must be followed by a name : lexer C++" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_FoldFlags )
{
	if ( ! p_strParams.empty() )
	{
		wxArrayString l_array = split( p_strParams, wxT( "\t ,;" ), 100, false );
		unsigned long l_ulFoldFlags = 0;

		for ( std::size_t i = 0 ; i < l_array.size() ; i++ )
		{
			l_ulFoldFlags |= std::static_pointer_cast< LanguageFileContext >( p_pContext )->mapFoldFlags[l_array[i]];
		}

		std::static_pointer_cast< LanguageFileContext >( p_pContext )->pCurrentLanguage->SetFoldFlags( l_ulFoldFlags );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <fold_flags> must be followed by a list of parameters : fold_flags <param1> <param2> ..." ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_Section )
{
	LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
	l_pContext->arrayWords.clear();
	l_pContext->eStyle = eSTC_TYPE_COUNT;
	l_pContext->eType = eSTC_TYPE_COUNT;
	ATTRIBUTE_END_PUSH( eSECTION_SECTION );
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_Style )
{
	LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
	l_pContext->arrayWords.clear();
	l_pContext->eStyle = eSTC_TYPE_COUNT;
	l_pContext->eType = eSTC_TYPE_COUNT;
	ATTRIBUTE_END_PUSH( eSECTION_STYLE );
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Language_End )
{
	ATTRIBUTE_END_POP();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_Type )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		wxString l_strType = trim( p_strParams ).Lower();
		l_pContext->pCurrentStyle = l_pContext->pCurrentLanguage->GetStyle( l_pContext->mapTypes[l_strType] );

		if ( !l_pContext->pCurrentStyle )
		{
			l_pContext->pCurrentStyle = l_pContext->pCurrentLanguage->CreateStyle( l_strType, l_pContext->mapTypes[l_strType] );
		}
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::type> must be followed by a parameter : type <param>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_FgColour )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		l_pContext->pCurrentStyle->SetForeground( trim( p_strParams ) );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::fg_colour> must be followed by a parameter : fg_colour <param>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_BgColour )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		l_pContext->pCurrentStyle->SetBackground( trim( p_strParams ) );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::bg_colour> must be followed by a parameter : bg_colour <param>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_FontName )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		l_pContext->pCurrentStyle->SetFontName( trim( p_strParams ) );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::font_name> must be followed by a parameter : font_name <param>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_FontStyle )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		wxArrayString l_arrayStyles = split( trim( p_strParams ).Lower(), wxT( "\t " ), 10, false );
		int l_iStyle = 0;

		for ( std::size_t i = 0 ; i < l_arrayStyles.size() ; i++ )
		{
			if ( l_arrayStyles[i] == wxT( "bold" ) )
			{
				l_iStyle |= eSTC_STYLE_BOLD;
			}
			else if ( l_arrayStyles[i] == wxT( "italic" ) )
			{
				l_iStyle |= eSTC_STYLE_ITALIC;
			}
			else if ( l_arrayStyles[i] == wxT( "underlined" ) )
			{
				l_iStyle |= eSTC_STYLE_UNDERL;
			}
			else if ( l_arrayStyles[i] == wxT( "hidden" ) )
			{
				l_iStyle |= eSTC_STYLE_HIDDEN;
			}
		}

		l_pContext->pCurrentStyle->SetFontStyle( l_iStyle );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::font_style> must be followed by a list of parameters : font_style <param1> <param2> ..." ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_FontSize )
{
	if ( !p_strParams.empty() )
	{
		LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
		long l_lFontSize;
		p_strParams.ToLong( &l_lFontSize );
		l_pContext->pCurrentStyle->SetFontSize( l_lFontSize );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <style::font_size> must be followed by a parameter : font_size <param>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Style_End )
{
	ATTRIBUTE_END_POP();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Section_Type )
{
	LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );

	if ( ! p_strParams.empty() )
	{
		wxString l_strParams( p_strParams );
		l_pContext->strName = trim( p_strParams ).Lower();
		l_pContext->eType = std::static_pointer_cast< LanguageFileContext >( p_pContext )->mapTypes[l_pContext->strName];
	}
	else
	{
		PARSE_ERROR( wxT( "directive <type> must be followed by a name : type <name>" ) );
	}

	ATTRIBUTE_END();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Section_End )
{
	LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );

	if ( l_pContext->eType != eSTC_TYPE_COUNT )
	{
		l_pContext->pCurrentLanguage->SetWords( l_pContext->eType, l_pContext->arrayWords );
	}
	else
	{
		PARSE_ERROR( wxT( "directive <section> must contain a directive <type>" ) );
	}

	ATTRIBUTE_END_POP();
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Section_List )
{
	ATTRIBUTE_END_PUSH( eSECTION_LIST );
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, Section_Style )
{
	LanguageFileContextPtr l_pContext = std::static_pointer_cast< LanguageFileContext >( p_pContext );
	l_pContext->pCurrentStyle = l_pContext->pCurrentLanguage->GetStyle( l_pContext->eType );

	if ( !l_pContext->pCurrentStyle )
	{
		l_pContext->pCurrentStyle = l_pContext->pCurrentLanguage->CreateStyle( l_pContext->strName, l_pContext->eType );
	}

	ATTRIBUTE_END_PUSH( eSECTION_STYLE );
}

IMPLEMENT_ATTRIBUTE_PARSER( Troll::GUI, List_End )
{
	ATTRIBUTE_END_POP();
}

#if defined( _MSC_VER )
#	pragma warning( pop )
#endif

//*************************************************************************************************
