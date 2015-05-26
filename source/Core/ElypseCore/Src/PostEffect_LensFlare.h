#ifndef ___POST_EFFECT_LENS_FLARE_H___
#define ___POST_EFFECT_LENS_FLARE_H___

#include "Module_SpecialEffects.h"

#include "PostEffect.h"

#include <OgreFrameListener.h>
#include <OgreVector3.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class PostEffect_LensFlare;

		class LensFlareListener : public FrameListener
		{
		private:
			PostEffect_LensFlare * m_owner;

		public:
			LensFlareListener( PostEffect_LensFlare * p_owner );
			virtual ~LensFlareListener();

		public:
			virtual bool frameStarted( const FrameEvent & evt );
		};

		class PostEffect_LensFlare : public PostEffect
		{
		private:
			OverlayElement * m_lensflareElements[4];
			Vector3 m_direction;
			Vector3 m_lastDirection;
			Camera * m_camera;

			LensFlareListener * m_listener;

		public:
			PostEffect_LensFlare( const String & p_name, Viewport * p_viewport );
			virtual ~PostEffect_LensFlare();

		public:
			virtual void SetEnabled( bool p_enable );

			void Update();
		};
	}
}

#endif
