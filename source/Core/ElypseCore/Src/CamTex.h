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
#ifndef ___CAMERA_TEXTURE_H___
#define ___CAMERA_TEXTURE_H___

#include "Module_SpecialEffects.h"

#include <OgreTexture.h>

namespace Elypse
{
	namespace SpecialEffects
	{
		class d_dll_export CamTex : public named, noncopyable
		{
		private:
			Camera * m_camera;
			SceneManager * m_sceneManager;
			Ogre::TexturePtr m_texture;
			Viewport * m_viewport;
			RenderTarget * m_renderTarget;
			TextureUnitState * m_textureUnitState;

			bool m_ownsMaterial;

		public:
			CamTex( String const & p_name, SceneManager * p_sceneManager, Camera * p_camera, uint32_t p_textureWidth, uint32_t p_textureHeight );
			~CamTex();

		public:
			void SetCamera( Camera * p_camera );
			void Update()const;

		public:
			inline RenderTarget *	GetTarget()const
			{
				return m_renderTarget;
			}
			inline Camera 	*		GetCamera()const
			{
				return m_camera;
			}
			inline Viewport 	*	GetViewport()const
			{
				return m_viewport;
			}
		};
	}
}

#endif
