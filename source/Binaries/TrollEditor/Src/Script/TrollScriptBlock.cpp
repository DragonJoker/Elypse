#include "PrecompiledHeader.h"

#include "Script/TrollScriptCompiler.h"
#include "Script/TrollScriptBlock.h"
#include "GUI/MainFrame.h"
/*
#include <ScriptNode.h>
#include <ScriptFunctions.h>
#include <EmuseLogs.h>
#include <General/Exception.h>
*/
#include <VariableType.h>
using namespace Troll::GUI;

extern MainFrame * g_mainFrame;

#define COMPILE_ERROR( p_desc )	m_trollCompiler->_error();															\
								g_mainFrame->LogOutMessage( "Compiler Error : ["									\
										+ String(m_trollCompiler->_getScriptFileName())								\
										+ " @ L# " + StringConverter::toString( m_lineNumBegin)						\
										+ " ] -> " + p_desc );

#define COMPILE_WARNING( p_desc )	m_trollCompiler->_warning();													\
									g_mainFrame->LogOutMessage(	"Compiler Warning : "								\
									+ m_trollCompiler->_getScriptFileName() + " @ L# "								\
									+ wxString(StringConverter::toString( m_lineNumBegin ))							\
									+ " ] -> " + p_desc );

#define COMPILE_ERROR_IN_BLOCK( p_desc , p_block ) m_trollCompiler->_error();										\
												   g_mainFrame->LogOutMessage( "Compiler Error : ["					\
												   + String(m_trollCompiler->_getScriptFileName())					\
												   + " @ L# "+ StringConverter::toString( p_block->m_lineNumBegin )	\
												   + " ] -> " + p_desc );

#define COMPILE_WARNING_IN_BLOCK( p_desc , p_block ) m_trollCompiler->_warning();									\
													 g_mainFrame->LogOutMessage( "Compiler Warning [ "				\
													 + m_trollCompiler->_getScriptFileName()						\
													 + " @ L# "+ StringConverter::toString( p_block->m_lineNumBegin)\
													 + " ] -> " + p_desc );

TrollScriptBlock :: TrollScriptBlock()
	:	ScriptBlock(),
		m_trollCompiler( NULL )
{
}

TrollScriptBlock :: ~TrollScriptBlock()
{
}

ScriptBlock * TrollScriptBlock :: _initialise( ScriptCompiler * p_compiler, BlockType p_type,
		unsigned int p_lineNum , unsigned int p_depth,
		ScriptBlock * p_parent )
{
	m_trollCompiler = static_cast<TrollScriptCompiler *>( p_compiler );
	m_compiler = p_compiler;
	return ScriptBlock :: _initialise( p_compiler, p_type, p_lineNum, p_depth, p_parent );
}

bool TrollScriptBlock :: _parseString()
{
	unsigned int l_begin = m_trollCompiler->GetCurrentCharIndex();

	if ( ScriptBlock::_parseString() && m_trollCompiler->IsHighlighting() )
	{
		unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
		//if (m_trollCompiler->GetTextCtrl() != NULL)
		//{
		//	if (m_type == BT_VARIABLE_TYPE)
		//	{
		//		m_trollCompiler->GetTextCtrl()->SetStyle(l_begin, l_end, wxTextAttr(*wxBLUE));
		//	}
		//	else if (m_type == BT_IF_ELSE || m_contents == "for" || m_contents == "while")
		//	{
		//		m_trollCompiler->GetTextCtrl()->SetStyle(l_begin, l_end, wxTextAttr(*wxBLUE));
		//	}
		//	else
		//	{
		//		m_trollCompiler->GetTextCtrl()->SetStyle(l_begin, l_end, wxTextAttr(*wxBLACK));
		//	}
		//}
		return true;
	}

	return false;
}

bool TrollScriptBlock :: _parseNumeral()
{
	VERBOSE_COMPILATOR( "TrollScriptBlock _parseNumeral" );
	unsigned int l_begin = m_trollCompiler->GetCurrentCharIndex();

	if ( ScriptBlock::_parseNumeral() && m_trollCompiler->IsHighlighting() )
	{
		unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
		//if (m_trollCompiler->GetTextCtrl() != NULL)
		//{
		//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin, l_end, wxTextAttr(wxColour(127, 127, 127)));
		//}
	}

	return true;
}

bool TrollScriptBlock :: _parseOperator()
{
	VERBOSE_COMPILATOR( "_parseOperator_TROLL" );
	bool l_commented = false;
	bool l_cStyleComment = false;
	unsigned int l_begin;
	char l_previousChar = '\0';

	while ( ! m_trollCompiler->_eof() )
	{
		char l_currentChar = m_trollCompiler->_getNextChar();

		if ( ! l_commented )
		{
			if ( _checkOperator( l_currentChar ) )
			{
				if ( l_currentChar == '*' && l_previousChar == '/' )
				{
					l_begin = m_trollCompiler->GetCurrentCharIndex();
					l_commented = true;
					l_cStyleComment = false;
					m_contents.erase( m_contents.end() - 1 );
				}
				else if ( l_currentChar == '/' && l_previousChar == '/' )
				{
					l_begin = m_trollCompiler->GetCurrentCharIndex();
					l_commented = true;
					l_cStyleComment = true;
					m_contents.erase( m_contents.end() - 1 );
				}
				else
				{
					m_contents.push_back( l_currentChar );
				}
			}
			else
			{
				m_trollCompiler->_putBack( l_currentChar );

				if ( m_contents.empty() )
				{
					return false;
				}
				else
				{
					_compileOperatorBlock();
					return true;
				}
			}
		}
		else
		{
			if ( l_currentChar == '\n' )
			{
				m_trollCompiler->_newLine();

				if ( l_cStyleComment )
				{
					if ( m_contents.empty() )
					{
						if ( m_trollCompiler->IsHighlighting() )
						{
							unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
							//if (m_trollCompiler->GetTextCtrl() != NULL)
							//{
							//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin-2, l_end, wxTextAttr(wxColour(0, 128, 0)));
							//}
						}

						return false;
					}
					else
					{
						if ( m_trollCompiler->IsHighlighting() )
						{
							unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
							//if (m_trollCompiler->GetTextCtrl() != NULL)
							//{
							//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin-2, l_end, wxTextAttr(wxColour(0, 128, 0)));
							//}
						}

						_compileOperatorBlock();
						return false;
					}
				}
			}
			else if ( !l_cStyleComment && l_currentChar == '/' && l_previousChar == '*' )
			{
				if ( m_trollCompiler->IsHighlighting() )
				{
					unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
					//if (m_trollCompiler->GetTextCtrl() != NULL)
					//{
					//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin-2, l_end, wxTextAttr(wxColour(0, 128, 0)));
					//}
				}

				l_commented = false;
			}
		}

		l_previousChar = l_currentChar;
	}

	COMPILE_ERROR( "Unexpected end of file while parsing an operator block" );
	return false;
}

bool TrollScriptBlock :: _parseSimpleQuote()
{
	VERBOSE_COMPILATOR( "TrollScriptBlock _parseSimpleQuote" );
	unsigned int l_begin = m_trollCompiler->GetCurrentCharIndex();

	while ( ! m_trollCompiler->_eof() )
	{
		char l_currentChar = m_trollCompiler->_getNextChar();

		if ( _checkSimpleQuote( l_currentChar ) )
		{
			if ( m_contents.size() == 1 )
			{
				m_compiledScript = m_trollCompiler->GetFlyweight( m_contents[0] );
			}
			else if ( m_contents.size() > 1 )
			{
				m_compiledScript = m_trollCompiler->GetFlyweight( m_contents );
			}
			else
			{
				COMPILE_ERROR( "Empty simple quotes ( '' )are not allowed." );
			}

			if ( m_trollCompiler->IsHighlighting() )
			{
				unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
				//if (m_trollCompiler->GetTextCtrl() != NULL)
				//{
				//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin-1, l_end, wxTextAttr(wxColour(163, 21, 21)));
				//}
			}

			return true;
		}
		else
		{
			m_contents.push_back( l_currentChar );
		}
	}

	COMPILE_ERROR( "Mismatched single quotes '' within file" );
	return false;
}

bool TrollScriptBlock :: _parseDoubleQuote()
{
	VERBOSE_COMPILATOR( "TrollScriptBlock _parseDoubleQuote" );
	unsigned int l_begin = m_trollCompiler->GetCurrentCharIndex();

//	m_compiledScript = m_trollCompiler->CreateScriptNode();
	while ( ! m_trollCompiler->_eof() )
	{
		char l_currentChar = m_trollCompiler->_getNextChar();

		if ( _checkDoubleQuote( l_currentChar ) )
		{
			m_compiledScript = m_trollCompiler->GetFlyweight( m_contents );

			if ( m_trollCompiler->IsHighlighting() )
			{
				unsigned int l_end = m_trollCompiler->GetCurrentCharIndex();
				//if (m_trollCompiler->GetTextCtrl() != NULL)
				//{
				//	m_trollCompiler->GetTextCtrl()->SetStyle(l_begin-1, l_end, wxTextAttr(wxColour(163, 21, 21)));
				//}
			}

			return true;
		}
		else
		{
			m_contents.push_back( l_currentChar );
		}
	}

	COMPILE_ERROR( "Mismatched double quotes \"\" within file" );
//	delete m_compiledScript;
//	m_compiledScript = NULL;
	return false;
}
