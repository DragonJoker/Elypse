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
