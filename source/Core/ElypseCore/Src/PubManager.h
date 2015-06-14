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
#ifndef ___PUB_MANAGER_H___
#define ___PUB_MANAGER_H___

#include "Module_Pub.h"
#include "Module_Physics.h"
#include "Module_Main.h"

#include <Singleton.h>

namespace Elypse
{
	namespace Pub
	{
		class d_dll_export PubManager : public General::Theory::Singleton<PubManager>
		{
		private:
			PubObjectMap m_objectList;

		public:
			PubManager();
			~PubManager();

		public:
			PubObject * CreatePubObject( const String & p_name, ElypsePlugin * p_plugin );

			void RemovePubObject( const String & p_name );

			void OnClick( Real p_x, Real p_y, SceneManager * p_sceneManager, Camera * p_camera, PhysicsSimulation * p_physics );

		public:
			inline PubObject * GetPubObject( const String & p_name )
			{
				return General::Utils::map::findOrNull( m_objectList, p_name );
			}
		};
	}
}

#endif
