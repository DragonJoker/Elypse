/*
See LICENSE file in root folder
*/
#ifndef ___ANIMATION_MANAGER_H___
#define ___ANIMATION_MANAGER_H___

#include "Module_Animation.h"

#include <Manager.h>

namespace Elypse
{
	namespace Animation
	{
		class d_dll_export AnimationManager
			: public General::Templates::Manager< AnimatedObjectGroup >
		{
		public:
			AnimationManager();
			~AnimationManager();

		public:
			void Update( Real p_timeStep )const;

		};
	}
}

#endif
