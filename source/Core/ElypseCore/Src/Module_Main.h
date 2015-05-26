#ifndef ___MODULE_MAIN_H___
#define ___MODULE_MAIN_H___

#include "EMuse.h"

#include <Path.h>

namespace EMuse
{
	namespace Main
	{
		class EMuseInstance;
		class EMuseFrameListener;
		class EMuseLoadingBar;
		class EMuseController;
		class EMusePlugin;

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

		typedef std::map < String , EMuseInstance *> InstanceMap;
		typedef std::multimap < String , EMuseInstance *> InstanceMultiMap;
		typedef std::set < EMuseInstance * >InstanceSet;
	}
}

using namespace EMuse::Main;

#endif
