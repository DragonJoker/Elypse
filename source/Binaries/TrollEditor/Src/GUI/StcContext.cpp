#include "PrecompiledHeader.h"

#include "GUI/LanguageParser.hpp"

using namespace Troll::GUI;

//*************************************************************************************************

StyleInfo::StyleInfo( wxString const & p_strName, wxString const & p_strForeground, wxString const & p_strBackground, wxString const & p_strFontName, int p_iFontSize, int p_iFontStyle, int p_iLetterCase )
	:	m_strName( p_strName	)
	,	m_strForeground( p_strForeground	)
	,	m_strBackground( p_strBackground	)
	,	m_strFontName( p_strFontName	)
	,	m_iFontSize( p_iFontSize	)
	,	m_iFontStyle( p_iFontStyle	)
	,	m_iLetterCase( p_iLetterCase	)
{
}

StyleInfo::StyleInfo( StyleInfo const & p_infos )
	:	m_strName( p_infos.m_strName	)
	,	m_strForeground( p_infos.m_strForeground	)
	,	m_strBackground( p_infos.m_strBackground	)
	,	m_strFontName( p_infos.m_strFontName	)
	,	m_iFontSize( p_infos.m_iFontSize	)
	,	m_iFontStyle( p_infos.m_iFontStyle	)
	,	m_iLetterCase( p_infos.m_iLetterCase	)
{
}

StyleInfo & StyleInfo::operator =( StyleInfo const & p_infos )
{
	m_strName		= p_infos.m_strName			;
	m_strForeground	= p_infos.m_strForeground	;
	m_strBackground	= p_infos.m_strBackground	;
	m_strFontName	= p_infos.m_strFontName		;
	m_iFontSize		= p_infos.m_iFontSize		;
	m_iFontStyle	= p_infos.m_iFontStyle		;
	m_iLetterCase	= p_infos.m_iLetterCase		;
	return * this;
}

StyleInfo::~StyleInfo()
{
}

//*************************************************************************************************

LanguageInfo::LanguageInfo()
	:	m_strName( DEFAULT_LANGUAGE	)
	,	m_strFilePattern( wxEmptyString	)
	,	m_eLexerID( eSTC_LEX_COUNT	)
	,	m_iFoldFlags( 0	)
{
	m_arrayStyles[eSTC_TYPE_DEFAULT				] = std::make_shared< StyleInfo >(	wxT( "Default"	),	wxT( "BLACK"	),	wxT( "WHITE" ),	wxT( "" ),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_WORD1				] = std::make_shared< StyleInfo >(	wxT("Keyword1"				),	wxT("BLUE"				),	wxT("WHITE"),	wxT(""),	10,		eSTC_STYLE_BOLD,	0 );
	//m_arrayStyles[eSTC_TYPE_WORD2				] = std::make_shared< StyleInfo >(	wxT("Keyword2"				),	wxT("MIDNIGHT BLUE"		),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_WORD3				] = std::make_shared< StyleInfo >(	wxT("Keyword3"				),	wxT("CORNFLOWER BLUE"	),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_WORD4				] = std::make_shared< StyleInfo >(	wxT("Keyword4"				),	wxT("CYAN"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_WORD5				] = std::make_shared< StyleInfo >(	wxT("Keyword5"				),	wxT("DARK GREY"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_WORD6				] = std::make_shared< StyleInfo >(	wxT("Keyword6"				),	wxT("GREY"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_COMMENT				] = std::make_shared< StyleInfo >(	wxT("Comment"				),	wxT("FOREST GREEN"		),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_COMMENT_DOC			] = std::make_shared< StyleInfo >(	wxT("Comment (Doc)"			),	wxT("FOREST GREEN"		),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_COMMENT_LINE		] = std::make_shared< StyleInfo >(	wxT("Comment line"			),	wxT("FOREST GREEN"		),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_COMMENT_SPECIAL		] = std::make_shared< StyleInfo >(	wxT("Special comment"		),	wxT("FOREST GREEN"		),	wxT("WHITE"),	wxT(""),	10,		eSTC_STYLE_ITALIC,	0 );
	//m_arrayStyles[eSTC_TYPE_CHARACTER			] = std::make_shared< StyleInfo >(	wxT("Character"				),	wxT("KHAKI"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_CHARACTER_EOL		] = std::make_shared< StyleInfo >(	wxT("Character (EOL)"		),	wxT("KHAKI"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_STRING				] = std::make_shared< StyleInfo >(	wxT("String"				),	wxT("BROWN"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_STRING_EOL			] = std::make_shared< StyleInfo >(	wxT("String (EOL)"			),	wxT("BROWN"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_DELIMITER			] = std::make_shared< StyleInfo >(	wxT("Delimiter"				),	wxT("ORANGE"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_PUNCTUATION			] = std::make_shared< StyleInfo >(	wxT("Punctuation"			),	wxT("ORANGE"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_OPERATOR			] = std::make_shared< StyleInfo >(	wxT("Operator"				),	wxT("BLACK"				),	wxT("WHITE"),	wxT(""),	10,		eSTC_STYLE_BOLD,	0 );
	//m_arrayStyles[eSTC_TYPE_BRACE				] = std::make_shared< StyleInfo >(	wxT("Label"					),	wxT("VIOLET"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_COMMAND				] = std::make_shared< StyleInfo >(	wxT("Command"				),	wxT("BLUE"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_IDENTIFIER			] = std::make_shared< StyleInfo >(	wxT("Identifier"			),	wxT("BLACK"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_LABEL				] = std::make_shared< StyleInfo >(	wxT("Label"					),	wxT("VIOLET"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_NUMBER				] = std::make_shared< StyleInfo >(	wxT("Number"				),	wxT("SIENNA"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_PARAMETER			] = std::make_shared< StyleInfo >(	wxT("Parameter"				),	wxT("VIOLET"			),	wxT("WHITE"),	wxT(""),	10,		eSTC_STYLE_ITALIC,	0 );
	//m_arrayStyles[eSTC_TYPE_REGEX				] = std::make_shared< StyleInfo >(	wxT("Regular expression"	),	wxT("ORCHID"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_UUID				] = std::make_shared< StyleInfo >(	wxT("UUID"					),	wxT("ORCHID"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_VALUE				] = std::make_shared< StyleInfo >(	wxT("Value"					),	wxT("ORCHID"			),	wxT("WHITE"),	wxT(""),	10,		eSTC_STYLE_ITALIC,	0 );
	//m_arrayStyles[eSTC_TYPE_PREPROCESSOR		] = std::make_shared< StyleInfo >(	wxT("Preprocessor"			),	wxT("GREY"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_SCRIPT				] = std::make_shared< StyleInfo >(	wxT("Script"				),	wxT("DARK GREY"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_ERROR				] = std::make_shared< StyleInfo >(	wxT("Error"					),	wxT("RED"				),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
	//m_arrayStyles[eSTC_TYPE_UNDEFINED			] = std::make_shared< StyleInfo >(	wxT("Undefined"				),	wxT("ORANGE"			),	wxT("WHITE"),	wxT(""),	10,		0,					0 );
}

LanguageInfo::~LanguageInfo()
{
}

void LanguageInfo::SetWords( eSTC_TYPE p_eType, wxArrayString const & p_arrayWords )
{
	wxString l_strWords;

	for ( std::size_t i = 0 ; i < p_arrayWords.size() ; ++i )
	{
		if ( ! l_strWords.empty() )
		{
			l_strWords += wxT( " " );
		}

		l_strWords += p_arrayWords[i];
	}

	m_arrayWords[p_eType] = l_strWords;
}

void LanguageInfo::AddWord( eSTC_TYPE p_eType, wxString const & p_strWord )
{
	m_arrayWords[p_eType] += p_strWord + wxT( " " );
}

StyleInfoPtr LanguageInfo::CreateStyle( wxString const & p_strType, eSTC_TYPE p_eType )
{
	StyleInfoPtr l_pDefault = GetStyle( eSTC_TYPE_DEFAULT );
	m_arrayStyles[p_eType] = std::make_shared< StyleInfo >( p_strType,	l_pDefault->GetForeground(), l_pDefault->GetBackground(), l_pDefault->GetFontName(), l_pDefault->GetFontSize(), l_pDefault->GetFontStyle(), l_pDefault->GetLetterCase() );
	return m_arrayStyles[p_eType];
}

//*************************************************************************************************

StcContext::StcContext()
	:	m_bSyntaxEnable( true	)
	,	m_bFoldEnable( true	)
	,	m_bIndentEnable( true	)
	,	m_bReadOnlyInitial( false	)
	,	m_bOverTypeInitial( false	)
	,	m_bWrapModeInitial( false	)
	,	m_bDisplayEOLEnable( false	)
	,	m_bIndentGuideEnable( false	)
	,	m_bLineNumberEnable( true	)
	,	m_bLongLineOnEnable( true	)
	,	m_bWhiteSpaceEnable( false	)
{
	m_arrayLanguages.push_back( std::make_shared< LanguageInfo >() );
}

StcContext::~StcContext()
{
	m_arrayLanguages.clear();
}

void StcContext::ParseFile( wxString const & p_strFileName )
{
	LanguageFileParser l_parser( this );
	l_parser.ParseFile( ( wxChar const * )p_strFileName.c_str() );
}

LanguagesConstIterator StcContext::Find( wxString const & p_strName )const
{
	LanguagesConstIterator l_itReturn = End();

	for ( LanguagesConstIterator l_it = Begin() ; l_it != End() && l_itReturn == End() ; ++l_it )
	{
		if ( ( *l_it )->GetName() == p_strName )
		{
			l_itReturn = l_it;
		}
	}

	return l_itReturn;
}

LanguagesIterator StcContext::Find( wxString const & p_strName )
{
	LanguagesIterator l_itReturn = End();

	for ( LanguagesIterator l_it = Begin() ; l_it != End() && l_itReturn == End() ; ++l_it )
	{
		if ( ( *l_it )->GetName() == p_strName )
		{
			l_itReturn = l_it;
		}
	}

	return l_itReturn;
}

//*************************************************************************************************