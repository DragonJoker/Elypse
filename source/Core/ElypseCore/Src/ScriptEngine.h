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
#ifndef ___EMUSE_SCRIPT_ENGINE_H___
#define ___EMUSE_SCRIPT_ENGINE_H___

#include "Module_Script.h"

#include "Module_Main.h"
#include "Module_Gui.h"
#include "Module_Zone.h"
#include "Module_Data.h"

namespace Elypse
{
	namespace Script
	{
		class d_dll_export ScriptEngine d_debug_check_noncopyable
		{
		protected:
			static Context * sm_context;

		protected:
			Path m_path;
			ElypseLoadingBar * m_loadingBar;

		public:
			ScriptCompiler * m_compiler;
			EMGui * m_gui;
			Context * m_context;
			StringArray m_untrimmedCode;

			ScriptTimerManager * m_timerManager;

		protected:
			ScriptEngine( ScriptCompiler * p_compiler, const Path & p_basePath, ElypseLoadingBar * p_loadingBar );

		public:
			ScriptEngine( const Path & p_basePath, ElypseLoadingBar * p_loadingBar );
			virtual ~ScriptEngine();

		public:
			virtual void Initialise();

			void Execute( ScriptNode * p_node );
			ScriptNode * GetKeyboardBind( unsigned int p_keyCode, unsigned int p_bindType )const;
			void AddScriptZone( String & p_scriptString );
			ScriptNode * CompileScriptFile( ConfigFile * p_scriptFile );
			ScriptNode * CompileScript( String & p_scriptString );
			ScriptNode * GetVariable( const String & p_variableName )const;
			ScriptNode * GetFunction( const String & p_functionName )const;
			void ExecuteScriptFile( const String & p_filename );


			virtual void LoadZone( Zone * p_zone );
			virtual void UnloadZone( Zone * p_zone );

		public:
			static void ScriptError( ScriptNode * p_node, const String & p_string );
			static inline Context * GetContext()
			{
				return sm_context;
			}
			void UseContext()const;
			void SetContext( Context * p_context );
			static inline void SetGlobalContext( Context * p_context )
			{
				sm_context = p_context;
			}
		};
	}
}

#endif
