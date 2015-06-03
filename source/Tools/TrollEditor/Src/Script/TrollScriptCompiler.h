#ifndef ___TROLL_SCRIPT_COMPILER_H___
#define ___TROLL_SCRIPT_COMPILER_H___

#include <Module_Script.h>
#include <ScriptCompiler.h>
#include <Module_Data.h>

#include <SlottedObjectPool.h>

#include "TrollScriptBlock.h"
#include "GUI/TextCtrl.h"
#include "GUI/StcTextEditor.hpp"
#include "GUI/FunctionsTree.h"

using std::ifstream;

namespace Troll
{
	namespace Script
	{
		class TrollScriptCompiler : public EMuse::Script::ScriptCompiler
		{
			friend TrollScriptBlock;
		private:
			unsigned int m_currentCharIndex;
			Troll::GUI::wxStcTextEditor * m_textCtrl;
			bool m_highlighting;
			bool m_initTree;


		protected:
			SlottedObjectPool <TrollScriptBlock> m_trollBlockPool;


		public:
			TrollScriptCompiler( const String & p_path, bool p_initTree );
			virtual ~TrollScriptCompiler();

			void Initialise( Troll::GUI::wxStcTextEditor * p_textCtrl );

			Troll::GUI::FunctionsTree * tree;

			virtual void _log( const String & p_message );

			virtual ScriptNode *	CompileScript( const String & p_script );
			virtual ScriptNode *	CompileScriptFile( ConfigFile * p_scriptFile );

		protected:
			virtual TrollScriptBlock 	*	_getBlock();
			virtual void					_releaseBlock( ScriptBlock * p_block );
			virtual char					_getNextChar();
			virtual void					_putBack( char p_char );
			virtual void					_creaFunc( const String & id, RawFunction * p_function, VariableBaseType param1 = EMVT_NULL, ... );
			virtual void					_classFunc( const String & id, RawFunction * p_function, VariableBaseType param1 = EMVT_NULL, ... );
			virtual void					_constantGroup( const String & p_name );
			virtual ScriptNode 	*		_createConstant( VariableBaseType p_type, const String & p_name );
			virtual void					_functionGroup( const String & p_name );
			virtual void					_initialiseFunctionMap();

		public:
			inline unsigned int					GetCurrentCharIndex()const
			{
				return m_currentCharIndex;
			}
			inline Troll::GUI::wxStcTextEditor	* GetTextCtrl()const
			{
				return m_textCtrl;
			}
			inline bool							IsHighlighting()const
			{
				return m_highlighting;
			}
			inline unsigned int					GetNumWarnings()const
			{
				return m_numWarnings;
			}
			inline unsigned int					GetNumErrors()const
			{
				return m_numErrors;
			}

		};
	}
}

using namespace Troll::Script;

#endif

