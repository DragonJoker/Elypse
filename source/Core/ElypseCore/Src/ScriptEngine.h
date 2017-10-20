/*
See LICENSE file in root folder
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
			ScriptNode * GetKeyboardBind( uint32_t p_keyCode, uint32_t p_bindType )const;
			void AddScriptZone( String & p_scriptString );
			ScriptNode * CompileScriptFile( ConfigFile * p_scriptFile );
			ScriptNode * CompileScript( String & p_scriptString );
			ScriptNode * GetVariable( String const & p_variableName )const;
			ScriptNode * GetFunction( String const & p_functionName )const;
			void ExecuteScriptFile( String const & p_filename );


			virtual void LoadZone( Zone * p_zone );
			virtual void UnloadZone( Zone * p_zone );

		public:
			static void ScriptError( ScriptNode * p_node, String const & p_string );
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
