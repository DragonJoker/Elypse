#ifndef ___POST_EFFECT_MOTION_BLUR_H___
#define ___POST_EFFECT_MOTION_BLUR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace EMuse
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
			PostEffect_MotionBlur( const String & p_name, Viewport * p_viewport );
			virtual ~PostEffect_MotionBlur();
		};
	}
}

#endif
