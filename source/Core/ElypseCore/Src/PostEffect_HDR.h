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
#ifndef ___POST_EFFECT_HDR_H___
#define ___POST_EFFECT_HDR_H___

#include "PostEffect.h"

#include <OgreCompositorInstance.h>

namespace Elypse
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
			PostEffect_HDR( String const & p_name, Viewport * p_viewport );
			virtual ~PostEffect_HDR();
		};
	}
}

#endif
