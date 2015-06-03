#ifndef ___SCRIPT_COMPILER_H___
#define ___SCRIPT_COMPILER_H___

#include "Module_Script.h"
#include "Data/Module_Data.h"

#include <General/SlottedObjectPool.h>
#include "ScriptBlock.h"

#include <fstream>
using std::ifstream;

namespace EMuse
{
	namespace Script
	{
		class ScriptFunction
		{
		public:
			NodeFunction * m_function;
			VariableType m_return;
			VariableTypeArray m_params;
			ScriptNode * m_node;
			ScriptNode * m_returnNode;
			ScriptNode * m_finalNode;
			ScriptNodeArray m_paramNodes;
			String m_name;
			bool m_unary;
			bool m_pre;
		public:
			ScriptFunction()
				:	m_function( NULL ),
					m_node( NULL ),
					m_returnNode( NULL ),
					m_finalNode( NULL ),
					m_return( EMVT_NULL ),
					m_unary( false ),
					m_pre( false )
			{}
		};

		class ScriptCompiler
		{
		protected:
			friend class ScriptBlock;
		protected:
			SlottedObjectPool <ScriptBlock> m_blockPool;
		protected:
			unsigned int m_currentLine;

			ScriptNodeMap m_constants;
			ScriptNodeMap m_userVariables;
			ScriptFunctionMap m_functions;
			ScriptFunctionMultiMap m_operators;
			ScriptClassFunctionMap m_classFunctions;
			ScriptFunctionMap m_userFunctions;
			ConfigFile * m_currentScriptFile;
			Log * m_log;

			String m_path;

			bool m_withinUserFunction;

			unsigned int m_numWarnings;
			unsigned int m_numErrors;

			char * m_buffer;
			const String * m_stringBuffer;
			ifstream * m_currentFileStream;
			unsigned int m_currentBufferIndex;
			unsigned int m_currentMaxIndex;

		public:
			ScriptNode ** m_keyboardBinds[4];

		public:
			ScriptCompiler( Log * p_log, const String & p_path );
			virtual ~ScriptCompiler();

		protected:
			void 					_initialiseVariableMap();
			void 					_initialiseFunctionMap();
			void 					_initialiseOperatorMap();

			void 					_creaFunc(	const String & p_functionName,
												NodeFunction * p_function, VariableType p_returnValue,
												VariableType p_param1 = EMVT_NULL, VariableType p_param2 = EMVT_NULL,
												VariableType p_param3 = EMVT_NULL, VariableType p_param4 = EMVT_NULL );

			void 					_classFunc(	const String & p_functionName,
												NodeFunction * p_function, VariableType p_returnValue,
												VariableType p_param1 = EMVT_NULL, VariableType p_param2 = EMVT_NULL,
												VariableType p_param3 = EMVT_NULL, VariableType p_param4 = EMVT_NULL );

			void 					_createOperator(	const String & p_name,
					NodeFunction * p_func, VariableType p_returnType,
					VariableType p_left, VariableType p_right );


			ScriptNode 	*		_createConstant( VariableType type, const String & p_constantName );

		protected:
			ScriptBlock 	*		_getBlock();
			void					_releaseBlock( ScriptBlock * p_block );

		protected:

			ScriptNode  	*		_compileSentence(	const ScriptBlockArray & p_blockArray );

			ScriptNode  	*		_compileFunctionUse(	const ScriptBlockArray & p_blockArray );
			ScriptNode  	*		_compileOperatedSentence(	const ScriptBlockArray & p_blockArray );
			ScriptNode  	*		_compileBrakets(	const ScriptBlockArray & p_blockArray );
			ScriptNode  	*		_compileIfThenElse(	const ScriptBlockArray & p_blockArray );

			ScriptNode  	*		_compileFuncParams(	const ScriptBlockArray & p_blockArray, ScriptFunction * p_functionNode, ScriptNode * p_node = NULL );

			ScriptNode 	*		_compileUserFunctionUse(	const ScriptBlockArray & p_blockArray, ScriptFunction * p_userFunction );

			ScriptBlock 	*		_getHighestOperator(	const ScriptBlockArray & p_blockArray );
			ScriptFunction 	*	_compileUserFunction(	const ScriptBlockArray & p_blockArray, bool p_predeclareOnly );

			void					_declareVariable(	const ScriptBlockArray & p_blockArray );
			unsigned int			_getTypeList(	const ScriptBlockArray & p_blockArray )const;

		protected:

			ScriptNode 	*		_createUserVariable( const String & p_variableName, VariableType p_variableType, bool p_functionParam = false );
			ScriptNode 	*		_getUserVariable( const String & p_variableName )const
			{
				return map_findOrNull( m_userVariables, p_variableName );
			}

			ScriptFunction 	*	_createUserFunction( const String & p_functionName , VariableType p_functionReturnType );
			ScriptFunction 	*	_getFunction( const String & p_functionName )const
			{
				return map_findOrNull( m_functions, p_functionName );
			}
			ScriptFunction 	*	_getClassFunction( VariableType p_class, const String & p_functionName )const;

			ScriptNode 	*		_getOperator( ScriptBlock * p_operator, ScriptNode * p_leftOperand, ScriptNode * p_rightOperand );

		public:
			ScriptNode 	*		GetProgramConstant( const String & p_variableName )const;
			virtual ScriptNode *	CompileScriptFile( ConfigFile * p_scriptFile );
			virtual ScriptNode *	CompileScript( const String & p_script );
			ScriptNode 	*		GetMouseBind( const String & p_functionName );
			ScriptFunction 	*	GetUserFunction( const String & p_functionName )const
			{
				return map_findOrNull( m_userFunctions, p_functionName );
			}

			ScriptNode 	*		CreateScriptNode();
			ScriptNode 	*		CreateScriptNode( unsigned p_lineNum );

		protected:
			char					_getNextChar();
			bool					_eof();
			void					_readFile();
			void					_putBack( char p_char );

			const String 	&		_getScriptFileName()const;

			virtual void			_log( const String & p_message );

		protected:
			inline unsigned int		_getCurrentLine()const
			{
				return m_currentLine;
			}
			inline Log 	*		_getLog()const
			{
				return m_log;
			}
			inline bool				_isInUserFunction()const
			{
				return m_withinUserFunction;
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

			inline void				_enterUserFunction()
			{
				m_withinUserFunction = true;
			}
			inline void				_leaveUserFunction()
			{
				m_withinUserFunction = false;
			}
			inline ConfigFile 	*	_getCurrentConfigFile()const
			{
				return m_currentScriptFile;
			}
		};
	}
}

#endif
