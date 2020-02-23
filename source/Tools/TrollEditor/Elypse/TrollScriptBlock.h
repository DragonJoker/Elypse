/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_SCRIPT_BLOCK_H___
#define ___TROLL_SCRIPT_BLOCK_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <fstream>
#include <ScriptBlock.h>

using std::ifstream;

namespace Troll
{
	namespace ElypseRW
	{
		class TrollScriptBlock
			: public Elypse::Script::ScriptBlock
		{
			friend TrollScriptCompiler;
			TrollScriptCompiler * m_trollCompiler;

		public:
			TrollScriptBlock();
			virtual ~TrollScriptBlock();
			virtual ScriptBlock * _initialise( Elypse::Script::ScriptCompiler * p_compiler, BlockType p_type, uint32_t p_lineNum, uint32_t p_depth, ScriptBlock * p_parent );

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
