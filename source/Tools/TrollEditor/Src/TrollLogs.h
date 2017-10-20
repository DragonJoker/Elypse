/*
See LICENSE file in root folder
*/
#ifndef ___TROLL_LOGS___
#define ___TROLL_LOGS___

#include "TrollEditorPrerequisites.h"

#include <ElypseLogs.h>

namespace Troll
{
	class TrollLogs
		: public Elypse::Debug::ElypseLogs
	{
	public:
		TrollLogs();

	protected:
		virtual void DoLogMessage( String const & p_comment );
		virtual void DoConsoleMessage( String const & p_comment );
	};
}

#endif
