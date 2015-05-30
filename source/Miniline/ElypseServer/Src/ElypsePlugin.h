#ifndef ___ELYPSEPLUGIN_H___
#define ___ELYPSEPLUGIN_H___

#include "ElypseService.h"

namespace Elypse
{
	namespace ServerPlugin
	{
		class ElypsePlugin : public named
		{
		protected:
			ElypsePlugin( const std::string p_name );
			virtual ~ElypsePlugin();

		public:
			virtual ElypseServiceArray GetServices() = 0;
			virtual unsigned short GetVersionNo() = 0;

		public:
			static unsigned short sm_versionNo;
		};

		typedef ElypsePlugin * ( *PluginFactoryFct )( const char * );
		typedef void( *PluginDestroyerFct )( ElypsePlugin *& );

	}
}

#endif
