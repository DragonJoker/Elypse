/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
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
