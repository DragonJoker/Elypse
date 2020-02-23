/*
See LICENSE file in root folder
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