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
#include "ElypsePlugin.h"

namespace Elypse
{
	namespace Server
	{
		/*
		 *	Ici on définit la version du serveur.
		 *	Les plugins devront avoir été compilés avec la même version de ce fichier
		 *	que le serveur qui les chargera pour être valables.
		 */
		uint16_t ElypsePlugin::sm_versionNo = 0;

		ElypsePlugin::ElypsePlugin( std::string const & p_name )
			: General::Theory::named( p_name )
		{
			std::clog << "ElypsePlugin(" << p_name << ")" << std::endl;
		}

		ElypsePlugin::~ElypsePlugin()
		{
			std::clog << "~ElypsePlugin(" << m_name << ")" << std::endl;
		}
	}
}