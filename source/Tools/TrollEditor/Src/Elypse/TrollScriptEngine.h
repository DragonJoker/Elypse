/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_SCRIPT_ENGINE_H___
#define ___TROLL_SCRIPT_ENGINE_H___

#include "Elypse/TrollEditorElypsePrerequisites.h"

#include <ScriptEngine.h>

namespace Troll
{
	namespace ElypseRW
	{
		class TrollScriptEngine
			: public Elypse::Script::ScriptEngine
		{
		public:
			TrollScriptEngine( const Path & p_basePath, ElypseLoadingBar * p_loadingBar );
			virtual ~TrollScriptEngine();
		};
	}
}

#endif
