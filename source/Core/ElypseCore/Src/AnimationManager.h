/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___ANIMATION_MANAGER_H___
#define ___ANIMATION_MANAGER_H___

#include "Module_Animation.h"

#include <Manager.h>

namespace EMuse
{
	namespace Animation
	{
		class d_dll_export AnimationManager : public General::Templates::Manager<AnimatedObjectGroup>, d_noncopyable
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
