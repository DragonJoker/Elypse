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
