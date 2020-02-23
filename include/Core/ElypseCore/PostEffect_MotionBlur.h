/*
See LICENSE file in root folder
*/
#ifndef ___POST_EFFECT_MOTION_BLUR_H___
#define ___POST_EFFECT_MOTION_BLUR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class PostEffect_MotionBlur : public PostEffect
		{
		private:
			static bool sm_initialised;
		public:
			static bool IsInitialised()
			{
				return sm_initialised;
			}
			static void Initialise();
		public:
			PostEffect_MotionBlur( String const & p_name, Viewport * p_viewport );
			virtual ~PostEffect_MotionBlur();
		};
	}
}

#endif
