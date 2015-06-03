#ifndef ___ELYPSEPLUGIN_H___
#define ___ELYPSEPLUGIN_H___

#include "ElypseService.h"

#include <functional>

namespace Elypse
{
	namespace ServerPlugin
	{
		class ElypsePlugin
			: public General::Theory::named
		{
		protected:
			ElypsePlugin( const std::string & p_name );
			virtual ~ElypsePlugin();

		public:
			virtual ElypseServiceArray GetServices() = 0;
			virtual unsigned short GetVersionNo() = 0;

		public:
			static unsigned short sm_versionNo;
		};

		typedef std::function< ElypsePlugin * ( const char * ) > PluginFactoryFct;
		typedef std::function< void( ElypsePlugin *& ) > PluginDestroyerFct;

	}
}

#endif
