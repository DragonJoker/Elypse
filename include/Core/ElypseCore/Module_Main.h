/*
See LICENSE file in root folder
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
