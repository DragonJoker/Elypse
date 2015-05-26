#ifndef ___POST_EFFECT_HDR_H___
#define ___POST_EFFECT_HDR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace EMuse
{
	namespace SpecialEffects
	{
		class HDR_Listener: public CompositorInstance::Listener
		{
		protected:
			int mVpWidth, mVpHeight;
			int mBloomSize;
			// Array params - have to pack in groups of 4 since this is how Cg generates them
			// also prevents dependent texture read problems if ops don't require swizzle
			float mBloomTexWeights[15][4];
			float mBloomTexOffsetsHorz[15][4];
			float mBloomTexOffsetsVert[15][4];
		public:
			HDR_Listener();
			virtual ~HDR_Listener();
			void notifyViewportSize( int width, int height );
			void notifyCompositor( CompositorInstance * instance );
			virtual void notifyMaterialSetup( Ogre::uint32 pass_id, MaterialPtr & mat );
			virtual void notifyMaterialRender( Ogre::uint32 pass_id, MaterialPtr & mat );
		};

		class PostEffect_HDR : public PostEffect
		{
		private:
			HDR_Listener * m_listener;
		public:
			PostEffect_HDR( const String & p_name, Viewport * p_viewport );
			virtual ~PostEffect_HDR();
		};
	}
}

#endif
