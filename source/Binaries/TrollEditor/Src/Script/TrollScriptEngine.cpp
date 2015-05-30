#include "PrecompiledHeader.h"

#include "Script/TrollScriptEngine.h"
#include "Script/TrollScriptCompiler.h"
#include "SceneFile/TrollSceneFileParser.h"
/*
#include <ScriptNode.h>
#include <ScriptCompiler.h>
#include <ScriptTimerManager.h>
#include <Zone/Zone.h>
#include <Zone/Universe.h>
#include <Main/EMuseLoadingBar.h>
#include <Main/Context.h>
#include <Data/ConfigFile.h>
#include <EMuseLogs.h>
*/
using namespace Troll::Script;

TrollScriptEngine::TrollScriptEngine( const Path & p_path, EMuseLoadingBar * p_loadingBar )
	:	ScriptEngine( p_path, p_loadingBar )
{
	delete m_compiler;
	m_compiler = new TrollScriptCompiler( p_path, false );
}

TrollScriptEngine::~TrollScriptEngine()
{
}
