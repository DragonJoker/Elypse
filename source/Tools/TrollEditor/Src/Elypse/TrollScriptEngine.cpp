/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "Elypse/TrollScriptEngine.h"

#include "Elypse/TrollScriptCompiler.h"
#include "Elypse/TrollSceneFileParser.h"

namespace Troll
{
	namespace ElypseRW
	{
		TrollScriptEngine::TrollScriptEngine( const Path & p_path, ElypseLoadingBar * p_loadingBar )
			: ScriptEngine( new TrollScriptCompiler( NULL, p_path, false ), p_path, p_loadingBar )
		{
		}

		TrollScriptEngine::~TrollScriptEngine()
		{
		}
	}
}
