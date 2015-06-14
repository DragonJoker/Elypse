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
#ifndef ___POST_EFFECT_GAUSSIAN_BLUR_H___
#define ___POST_EFFECT_GAUSSIAN_BLUR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace Elypse
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
