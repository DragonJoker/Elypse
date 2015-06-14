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
#include "TrollEditorPch.h"

#include "Elypse/TrollScriptCompiler.h"

#include "Elypse/TrollScriptBlock.h"

#include "GUI/MainFrame.h"

#include <VariableType.h>

#define COMPILE_ERROR( p_desc )	m_trollCompiler->_error();\
								LogMessage( "Compiler Error : ["\
										+ String(m_trollCompiler->_getScriptFileName())\
										+ " @ L# " + StringConverter::toString( m_lineNumBegin)\
										+ " ] -> " + p_desc );

#define COMPILE_WARNING( p_desc )	m_trollCompiler->_warning();\
									LogMessage( "Compiler Warning : "\
									+ m_trollCompiler->_getScriptFileName() + " @ L# "\
									+ wxString(StringConverter::toString( m_lineNumBegin ))\
									+ " ] -> " + p_desc );

#define COMPILE_ERROR_IN_BLOCK( p_desc , p_block ) m_trollCompiler->_error();\
												   LogMessage( "Compiler Error : ["\
												   + String(m_trollCompiler->_getScriptFileName())\
												   + " @ L# "+ StringConverter::toString( p_block->m_lineNumBegin )\
												   + " ] -> " + p_desc );

#define COMPILE_WARNING_IN_BLOCK( p_desc , p_block ) m_trollCompiler->_warning();\
													 LogMessage( "Compiler Warning [ "\
													 + m_trollCompiler->_getScriptFileName()\
													 + " @ L# "+ StringConverter::toString( p_block->m_lineNumBegin)\
													 + " ] -> " + p_desc );

BEGIN_TROLL_ELYPSE_NAMESPACE
{
	TrollScriptBlock::TrollScriptBlock()
		:	ScriptBlock(),
			m_trollCompiler( NULL )
	{
	}

	TrollScriptBlock::~TrollScriptBlock()
	{
	}

	ScriptBlock * TrollScriptBlock::_initialise( ScriptCompiler * p_compiler, BlockType p_type,
			unsigned int p_lineNum , unsigned int p_depth,
			ScriptBlock * p_parent )
	{
		m_trollCompiler = static_cast<TrollScriptCompiler *>( p_compiler );
		m_compiler = p_compiler;
		return ScriptBlock::_initialise( p_compiler, p_type, p_lineNum, p_depth, p_parent );
	}

	bool TrollScriptBlock::_parseString()
	{
		return ScriptBlock::_parseString();
	}

	bool TrollScriptBlock::_parseNumeral()
	{
		return ScriptBlock::_parseNumeral();
	}

	bool TrollScriptBlock::_parseOperator()
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
							return false;
						}
						else
						{
							_compileOperatorBlock();
							return false;
						}
					}
				}
				else if ( !l_cStyleComment && l_currentChar == '/' && l_previousChar == '*' )
				{
					l_commented = false;
				}
			}

			l_previousChar = l_currentChar;
		}

		COMPILE_ERROR( "Unexpected end of file while parsing an operator block" );
		return false;
	}

	bool TrollScriptBlock::_parseSimpleQuote()
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

	bool TrollScriptBlock::_parseDoubleQuote()
	{
		VERBOSE_COMPILATOR( "TrollScriptBlock _parseDoubleQuote" );
		unsigned int l_begin = m_trollCompiler->GetCurrentCharIndex();

		while ( ! m_trollCompiler->_eof() )
		{
			char l_currentChar = m_trollCompiler->_getNextChar();

			if ( _checkDoubleQuote( l_currentChar ) )
			{
				m_compiledScript = m_trollCompiler->GetFlyweight( m_contents );
				return true;
			}
			else
			{
				m_contents.push_back( l_currentChar );
			}
		}

		COMPILE_ERROR( "Mismatched double quotes \"\" within file" );
		return false;
	}
}
END_TROLL_ELYPSE_NAMESPACE
