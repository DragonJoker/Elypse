/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___EMUSE_SCRIPT_ENGINE_H___
#define ___EMUSE_SCRIPT_ENGINE_H___

#include "Module_Script.h"

#include "Module_Main.h"
#include "Module_Gui.h"
#include "Module_Zone.h"
#include "Module_Data.h"

namespace EMuse
{
	namespace Script
	{
		class d_dll_export ScriptEngine d_debug_check_noncopyable
		{
		protected:
			static Context * sm_context;

		protected:
			Path m_path;
			EMuseLoadingBar * m_loadingBar;

		public:
			ScriptCompiler * m_compiler;
			EMGui * m_gui;
			Context * m_context;
			StringArray m_untrimmedCode;

			ScriptTimerManager * m_timerManager;

		public:
			ScriptEngine( const Path & p_basePath, EMuseLoadingBar * p_loadingBar );
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