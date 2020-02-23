/*
See LICENSE file in root folder
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
