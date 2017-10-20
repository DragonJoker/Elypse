/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_ELYPSE_PREREQUISITES_H___
#define ___TROLL_ELYPSE_PREREQUISITES_H___

#include <queue>

#include <Elypse.h>
#include <Module_Script.h>

#include "TrollEditorPrerequisites.h"

namespace Troll
{
	namespace ElypseRW
	{
		class TrollScriptBlock;
		class TrollScriptEngine;
		class TrollScriptCompiler;
		class TrollScriptFunction;
		class TrollMuseFile;
		class TrollSceneFileParser;
		class TrollInstance;
		class TrollFrameListener;

		String ValueTypeToString( uint32_t p_type );
	}
}

#endif
