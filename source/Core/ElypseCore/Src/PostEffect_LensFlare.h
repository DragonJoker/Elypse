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
#ifndef ___POST_EFFECT_LENS_FLARE_H___
#define ___POST_EFFECT_LENS_FLARE_H___

#include "Module_SpecialEffects.h"

#include "PostEffect.h"

#include <OgreFrameListener.h>
#include <OgreVector3.h>

namespace Elypse
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
