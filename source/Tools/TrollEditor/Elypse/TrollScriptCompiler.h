/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_SCRIPT_COMPILER_H___
#define ___TROLL_SCRIPT_COMPILER_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include "Elypse/TrollScriptBlock.h"

#include "GUI/StcTextEditor.h"
#include "GUI/FunctionsTree.h"

#include <ScriptCompiler.h>
#include <Module_Data.h>
#include <SlottedObjectPool.h>

using std::ifstream;

namespace Troll
{
	namespace ElypseRW
	{
		class TrollScriptCompiler
			: public Elypse::Script::ScriptCompiler
		{
			friend TrollScriptBlock;

		public:
			TrollScriptCompiler( Troll::GUI::StcContext * p_context, String const & p_path, bool p_initTree );
			virtual ~TrollScriptCompiler();

			void Initialise( Troll::GUI::wxStcTextEditor * p_textCtrl );

			Troll::GUI::FunctionsTree * tree;

			virtual void _log( String const & p_message );

			virtual ScriptNode * CompileScript( String const & p_script );
			virtual ScriptNode * CompileScriptFile( ConfigFile * p_scriptFile );

		protected:
			virtual TrollScriptBlock * _getBlock();
			virtual void _releaseBlock( ScriptBlock * p_block );
			virtual char _getNextChar();
			virtual void _putBack( char p_char );
			virtual void _creaFunc( String const & id, RawFunction * p_function, VariableBaseType param1 = EMVT_NULL, ... );
			virtual void _classFunc( String const & id, RawFunction * p_function, VariableBaseType param1 = EMVT_NULL, ... );
			virtual void _constantGroup( String const & p_name );
			virtual ScriptNode * _createConstant( VariableBaseType p_type, String const & p_name );
			virtual void _functionGroup( String const & p_name );
			virtual void _initialiseFunctionMap();

		public:
			inline uint32_t GetCurrentCharIndex()const
			{
				return m_currentCharIndex;
			}
			inline Troll::GUI::wxStcTextEditor * GetTextCtrl()const
			{
				return m_textCtrl;
			}
			inline bool IsHighlighting()const
			{
				return m_highlighting;
			}
			inline uint32_t GetNumWarnings()const
			{
				return m_numWarnings;
			}
			inline uint32_t GetNumErrors()const
			{
				return m_numErrors;
			}

		protected:
			SlottedObjectPool< TrollScriptBlock > m_trollBlockPool;

		private:
			uint32_t m_currentCharIndex;
			Troll::GUI::wxStcTextEditor * m_textCtrl;
			Troll::GUI::StcContext * m_context;
			bool m_highlighting;
			bool m_initTree;
		};
	}
}

#endif
