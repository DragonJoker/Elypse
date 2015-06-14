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
#ifndef ___MODULE_MAIN_H___
#define ___MODULE_MAIN_H___

#include "Elypse.h"

#include <Path.h>

namespace Elypse
{
	namespace Main
	{
		class ElypseInstance;
		class ElypseFrameListener;
		class ElypseLoadingBar;
		class ElypseController;
		class ElypsePlugin;

		struct Context;

		enum ControllerStatus
		{
			CS_NULL,
			CS_PREINITIALISED,
			CS_STARTED,
			CS_INITIALISED,
			CS_RENDERING,
			CS_DESTROYING,
			CS_DESTROYED
		};

		enum CallbackType	//callbacks
		{
			MOUSE_R_DOWN,
			MOUSE_M_DOWN,
			MOUSE_L_DOWN,
			MOUSE_R_UP,
			MOUSE_M_UP,
			MOUSE_L_UP,
			MOUSE_WHEEL_UP,
			MOUSE_WHEEL_DOWN,
			KEY_CHAR_DOWN,
			KEY_CHAR_REPEAT,
			NETWORK_MESSAGE,
			NETWORK_QUIT,
			NUM_SCRIPT_BINDS
		};

		typedef std::map < String , ElypseInstance *> InstanceMap;
		typedef std::multimap < String , ElypseInstance *> InstanceMultiMap;
		typedef std::set < ElypseInstance * >InstanceSet;
	}
}

using namespace Elypse::Main;

#endif
