#ifndef ___TROLL_SCRIPT_BLOCK_H___
#define ___TROLL_SCRIPT_BLOCK_H___

#include <Module_Script.h>

#include <fstream>
#include <ScriptBlock.h>

#include "TrollScriptCompiler.h"

using std::ifstream;

namespace Troll
{
	namespace Script
	{
		class TrollScriptCompiler;

		class TrollScriptBlock
			: public ScriptBlock
		{
			friend TrollScriptCompiler;
			TrollScriptCompiler * m_trollCompiler;
		public:
			TrollScriptBlock();
			virtual ~TrollScriptBlock();
			virtual ScriptBlock * _initialise(	EMuse::Script::ScriptCompiler * p_compiler, BlockType p_type,
												unsigned int p_lineNum, unsigned int p_depth,
												ScriptBlock * p_parent );

		private:
			virtual bool _parseNumeral();
			virtual bool _parseString();

			virtual bool _parseOperator();
			virtual bool _parseSimpleQuote();
			virtual bool _parseDoubleQuote();
		};
	}
}

#endif
