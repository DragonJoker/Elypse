/*****************************************************************************

	Author : Marc BILLON
	Compagny : FDSSoftMedia - Copyright 2007

*****************************************************************************/

#ifndef ___CAMERA_TEXTURE_H___
#define ___CAMERA_TEXTURE_H___

#include "Module_SpecialEffects.h"

#include <OgreTexture.h>

namespace EMuse
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
			CamTex( const String & p_name, SceneManager * p_sceneManager, Camera * p_camera, unsigned int p_textureWidth, unsigned int p_textureHeight );
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
