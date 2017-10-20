/*
See LICENSE file in root folder
*/
#ifndef ___ELYPSEPLUGIN_H___
#define ___ELYPSEPLUGIN_H___

#include "ElypseService.h"

#include <functional>

namespace Elypse
{
	namespace Server
	{
		class ElypsePlugin
			: public General::Theory::named
		{
		protected:
			ElypsePlugin( std::string const & p_name );
			virtual ~ElypsePlugin();

		public:
			virtual ElypseServiceArray const & GetServices()const = 0;
			virtual uint16_t GetVersionNo() = 0;

		public:
			static uint16_t sm_versionNo;
		};

		typedef std::function< ElypsePlugin * ( char const * const ) > PluginFactoryFct;
		typedef std::function< void( ElypsePlugin *& ) > PluginDestroyerFct;
	}
}

#endif
