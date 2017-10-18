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
#ifndef ___SCRIPT_COMPILER_H___
#define ___SCRIPT_COMPILER_H___

#include "Module_Script.h"
#include "Module_Data.h"

#include <SlottedObjectPool.h>
#include <Singleton.h>
#include "ScriptBlock.h"
#include "Function.h"

#include <fstream>
using std::ifstream;

#include <Path.h>

namespace Elypse
{
	namespace Script
	{
		class d_dll_export ScriptCompiler : public General::Theory::Singleton<ScriptCompiler>
		{
		protected:
			friend class ScriptBlock;

		protected:
			SlottedObjectPool <ScriptBlock> m_blockPool;
			SlottedObjectPool <ScriptNode> m_nodePool;

		protected:
			uint32_t m_currentLine;

			ScriptNodeMap m_constants;
			ScriptNodeMap m_userVariables;

			FunctionMap m_functions;
			OperatorFunctionMultiMap m_operators;
			ClassFunctionMap m_classFunctions;
			UserFunctionMap m_userFunctions;
			StructureMap m_structures;
			VariableTypeMap m_typedefs;

			ConfigFile * m_currentScriptFile;

			UserFunction * m_currentUserFunction;

			Path m_path;

			Structure  * m_currentStructure;

			uint32_t m_numWarnings;
			uint32_t m_numErrors;

			char * m_buffer;
			const String * m_stringBuffer;
			ifstream * m_currentFileStream;
			uint32_t m_currentBufferIndex;
			uint32_t m_currentMaxIndex;

			std::map< char, ScriptNode *> m_charFlyweight;
			std::map< String, ScriptNode *> m_stringFlyweight;
			std::map< int, ScriptNode *> m_intFlyweight;
			std::map< Real, ScriptNode *> m_realFlyweight;

		public:
			ScriptNode ** m_keyboardBinds[4];

		public:
			ScriptCompiler()
			{
				genlib_assert( false );
			}
			ScriptCompiler( const Path & p_path );
			virtual ~ScriptCompiler();

		public:
			ScriptNode * GetUsableFunctionNode( String const & p_functionName )const;

		protected:
			void 					_initialiseVariableMap();
			void 					_initialiseFunctionMap();
			void 					_initialiseOperatorMap();

			virtual void			_creaFunc( String const & p_functionName,
											   RawFunction * p_function, VariableBaseType p_returnValue, ... );
			virtual void			_classFunc( String const & p_functionName,
												RawFunction * p_function, VariableBaseType p_returnValue, ... );
			virtual void			_createOperator( String const & p_name,
					RawFunction * p_func, VariableBaseType p_returnType,
					VariableBaseType p_left, VariableBaseType p_right );

			virtual void			_functionGroup( String const & p_name ) {}

			virtual void			_constantGroup( String const & p_name ) {}
			virtual ScriptNode *	_createConstant( VariableBaseType type, String const & p_constantName );

		protected:
			virtual ScriptBlock *	_getBlock();
			virtual void			_releaseBlock( ScriptBlock * p_block );

		protected:

			ScriptNode  	*		_compileSentence( ScriptBlockArray & p_blockArray );

			ScriptNode  	*		_compileFunctionUse( ScriptBlockArray & p_blockArray );
			ScriptNode  	*		_compileOperatedSentence( ScriptBlockArray & p_blockArray );
			ScriptNode  	*		_compileBrakets( ScriptBlockArray & p_blockArray );
			void					_compileStructDeclatation( ScriptBlockArray & p_blockArray );
			void					_addStructMember( Structure * p_struct, ScriptBlockArray & p_blockArray );
			void					_declareStruct( ScriptBlockArray & p_blockArray );

			ScriptNode  	*		_compileIfThenElse( ScriptBlockArray & p_blockArray );

			ScriptNode 	*		_compileReturn( ScriptBlockArray & p_childs );

			ScriptNode  	*		_compileFuncParams( ScriptBlockArray & p_blockArray, Function * p_functionNode, ScriptNode * p_node = NULL );

			ScriptNode 	*		_compileUserFunctionUse( ScriptBlockArray & p_blockArray, UserFunction * p_userFunction );

			ScriptBlock 	*		_getHighestOperator( ScriptBlockArray & p_blockArray );
			UserFunction 	*		_compileUserFunction( ScriptBlockArray & p_blockArray, bool p_predeclareOnly );

			void					_declareVariable( ScriptBlockArray & p_blockArray );
			void					_declareVariableDetail( VariableType * p_variableType, ScriptBlockArray & p_blockArray );
			VariableType 	*		_getVariableType( ScriptBlockArray & p_blockArray );
			VariableType 	*		_getVariableTypeRecus( ScriptBlockArray & p_blockArray, uint32_t & p_recusIndex );
			VariableType 	*		_substituteVarType( VariableType * p_base, VariableType * p_type );
			uint32_t			_getTypeList( ScriptBlockArray & p_blockArray )const;

		protected:
			void					_typedef( ScriptBlockArray & p_blockArray );
			VariableType 	*		FindType( String const & p_name )const;
			ScriptNode 	*		_compileStructMember( ScriptBlockArray & p_left, ScriptBlockArray & p_right, ScriptBlock * p_block );
			void					_compileFuncParamsWithinParenthesis( const ScriptBlockArray & p_blockArray, ScriptNodeArray & p_compiledNodes );

			void					_printBlockArray( String const & p_where, const ScriptBlockArray & p_childs );

		protected:

			ScriptNode 	*		_createUserVariable( String const & p_variableName, VariableType * p_variableType, bool p_functionParam = false );
			ScriptNode 	*		_getUserVariable( String const & p_variableName );

			UserFunction 	*		_createUserFunction( String const & p_functionName, VariableType * p_functionReturnType );
			Function 		*		_getFunction( String const & p_functionName )const
			{
				return General::Utils::map::findOrNull( m_functions, p_functionName );
			}
			Function 		*		_getClassFunction( VariableType * p_class, String const & p_functionName )const;

			ScriptNode 	*		_getOperator( ScriptBlock * p_operator, ScriptNode * p_leftOperand, ScriptNode * p_rightOperand );

		public:
			ScriptNode 	*		GetProgramConstant( String const & p_variableName )const;
			virtual ScriptNode *	CompileScriptFile( ConfigFile * p_scriptFile );
			virtual ScriptNode *	CompileScript( String const & p_script );
			UserFunction 	*		GetUserFunction( String const & p_functionName )const; // { return General::Utils::map::findOrNull( m_userFunctions, p_functionName); }

			ScriptNode 	*		CreateScriptNode();
			ScriptNode 	*		CreateScriptNode( unsigned p_lineNum );

			virtual void			Initialise();

			static void ReleaseScriptNode( ScriptNode * p_node );

			void _releaseNode( ScriptNode * p_node );

			ScriptNode * GetFlyweight( char p_value );
			ScriptNode * GetFlyweight( String const & p_value );
			ScriptNode * GetFlyweight( int p_value );
			ScriptNode * GetFlyweight( Real p_value );

		protected:
			virtual char			_getNextChar();
			bool					_eof();
			void					_readFile();
			virtual void			_putBack( char p_char );

			const String 	&		_getScriptFileName()const;

			virtual void			_log( String const & p_message );

		protected:
			inline uint32_t		_getCurrentLine()const
			{
				return m_currentLine;
			}
			inline bool				_isInUserFunction()const
			{
				return m_currentUserFunction != NULL;
			}
			inline bool				_isInStructDecla()const
			{
				return m_currentStructure != NULL;
			}

			inline void				_leaveStructDecla()
			{
				m_currentStructure = NULL;
			}
//		inline void				_setInStructDecla	( bool p_in) { m_withinStructDecla = p_in; }


			inline uint32_t GetNumErrors()const
			{
				return m_numErrors;
			}
			inline uint32_t GetNumWarnings()const
			{
				return m_numWarnings;
			}

			inline void				_newLine()
			{
				m_currentLine ++;
			}
			inline void				_warning()
			{
				m_numWarnings ++;
			}
			inline void				_error()
			{
				m_numErrors ++;
			}

//		inline void				_enterUserFunction()	{ m_withinUserFunction = true; m_userFuncVariables.push_back( ScriptNodeMap()); }
			inline ConfigFile 	*	_getCurrentConfigFile()const
			{
				return m_currentScriptFile;
			}
			void _leaveUserFunction();


		};
	}
}

#endif
