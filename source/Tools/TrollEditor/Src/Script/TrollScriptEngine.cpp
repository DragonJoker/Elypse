#include "PrecompiledHeader.h"

#include "TrollScriptEngine.h"

#include "TrollScriptCompiler.h"
#include "SceneFile/TrollSceneFileParser.h"

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
