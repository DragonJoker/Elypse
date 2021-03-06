/*
See LICENSE file in root folder
*/
#ifndef ___SCRIPT_BLOCK_H___
#define ___SCRIPT_BLOCK_H___

#include "Module_Script.h"

namespace Elypse
{
	namespace Script
	{
		class d_dll_export ScriptBlock
		{
			friend class ScriptCompiler;

		protected:
			ScriptBlock * m_parent;
			ScriptNode * m_compiledScript;
			ScriptCompiler * m_compiler;

			String m_contents;
			BlockType m_type;
			BlockSubType m_subType;
			ScriptBlockArray m_childs;
			VariableType * m_variableType;
			OperatorLevel m_operatorLevel;

			uint32_t m_lineNumBegin;
			uint32_t m_depth;

			bool m_operator_rightToLeft;

		public:
			ScriptBlock();
			virtual ~ScriptBlock();

		public:
			bool Parse();
			void Clear() noexcept;
			virtual ScriptBlock * _initialise(	ScriptCompiler * p_compiler, BlockType p_type,
												uint32_t p_lineNum, uint32_t p_depth,
												ScriptBlock * p_parent );
			ScriptNode * Compile();

		protected:
			virtual bool _parseSeparator();
			virtual bool _parseString();
			virtual bool _parseNumeral();
			virtual bool _parseOperator();
			virtual bool _parseParenthesis();
			virtual bool _parseBraces();
			virtual bool _parseSimpleQuote();
			virtual bool _parseDoubleQuote();
			virtual bool _parseBrackets();
			virtual bool _parseInitial();

			virtual bool _preCompiledCurrent( ScriptBlockArray & p_childs, ScriptNodeArray & p_nodeArray, bool p_clear = false );

			ScriptBlock * _delegate( char p_currentChar );
			ScriptNode * _compileInitial();
			ScriptNode * _compileString();

			void _compileOperatorBlock();
			void _checkKeywords();

		protected:
			static bool _checkSeparator( char p_charToCheck );
			static bool _checkString( char p_charToCheck );
			static bool _checkNumeral( char p_charToCheck );
			static bool _checkNumeralStart( char p_charToCheck );
			static bool _checkOperator( char p_charToCheck );
			static bool _checkParenthesis( char p_charToCheck );
			static bool _checkAccolades( char p_charToCheck );
			static bool _checkSimpleQuote( char p_charToCheck );
			static bool _checkDoubleQuote( char p_charToCheck );
			static bool _checkArray( char p_charToCheck );

		public:
			inline void SetCompiler( ScriptCompiler * p_compiler )
			{
				m_compiler = p_compiler;
			}
			inline ScriptNode * GetScriptNode()const
			{
				return m_compiledScript;
			}
		};
	}
}

#endif
