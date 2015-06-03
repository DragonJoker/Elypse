#ifndef ___TROLL_LOGS___
#define ___TROLL_LOGS___

//#define ____MUSE_DEBUG____

#include <EMuseLogs.h>

namespace Troll
{
	class TrollLogs : public EMuse::Debug::EMuseLogs
	{
	public:
		TrollLogs();

	protected:
		virtual void _logMessage( const String & p_comment );
		virtual void _consoleMessage( const String & p_comment );
	};
}

#endif
