#ifndef ___POST_EFFECT_GAUSSIAN_BLUR_H___
#define ___POST_EFFECT_GAUSSIAN_BLUR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class GaussianListener : public Ogre::CompositorInstance::Listener
		{
		protected:
			int mVpWidth, mVpHeight;
			// Array params - have to pack in groups of 4 since this is how Cg generates them
			// also prevents dependent texture read problems if ops don't require swizzle
			float mBloomTexWeights[15][4];
			float mBloomTexOffsetsHorz[15][4];
			float mBloomTexOffsetsVert[15][4];
		public:
			GaussianListener();
			virtual ~GaussianListener();
			void notifyViewportSize( int width, int height );
			virtual void notifyMaterialSetup( Ogre::uint32 pass_id, Ogre::MaterialPtr & mat );
			virtual void notifyMaterialRender( Ogre::uint32 pass_id, Ogre::MaterialPtr & mat );
		};

		class PostEffect_GaussianBlur : public PostEffect
		{
		private:
			GaussianListener * m_listener;
		public:
			PostEffect_GaussianBlur( const String & p_name, Viewport * p_viewport );
			virtual ~PostEffect_GaussianBlur();
		};
	}
}

#endif
