/*****************************************************************************

	Author : Marc BILLON
	Compagny : Fordev Studio - Copyright 2007

*****************************************************************************/

#ifndef ___TROLL_SCRIPT_ENGINE_H___
#define ___TROLL_SCRIPT_ENGINE_H___

#include <Module_Script.h>
#include <ScriptEngine.h>

namespace Troll
{
	namespace Script
	{
		class TrollScriptEngine
			: public ScriptEngine
		{
		public:
			TrollScriptEngine( const Path & p_basePath, EMuseLoadingBar * p_loadingBar );
			virtual ~TrollScriptEngine();
		};
	}
}

#endif
